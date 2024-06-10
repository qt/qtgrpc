// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>
#include <QtGrpc/private/qabstractgrpcchannel_p.h>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstream.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <qtgrpcglobal_p.h>

#include <private/qobject_p.h>

#include "qgrpcclientbase.h"
#include <QtCore/private/qminimalflatset_p.h>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

namespace {
static QString threadSafetyWarning(QLatin1StringView methodName)
{
    return QGrpcClientBase::tr("%1 is called from a different thread.\n"
                                   "Qt GRPC doesn't guarantee thread safety on the channel level.\n"
                                   "You have to be confident that channel routines are working in "
                                   "the same thread as QGrpcClientBase.")
        .arg(methodName);
}
} // namespace

/*!
    \class QGrpcClientBase
    \inmodule QtGrpc
    \brief The QGrpcClientBase class is bridge between gRPC clients
    and channels.

    QGrpcClientBase provides a set of functions for client classes
    generated out of protobuf services.
    QGrpcClientBase enforces thread safety for startStream() and call() methods
    of generated clients.
    The methods QGrpcClientBase::call() and QGrpcClientBase::startStream()
    should only be called by the generated client classes.
*/

/*!
    \fn template <typename StreamType> std::shared_ptr<StreamType> QGrpcClientBase::startStream(QLatin1StringView method, const QProtobufMessage &arg, const QGrpcCallOptions &options)

    Starts the stream \a method of the \e StreamType type with the message
    argument \a arg to the attached channel.

    Uses \a options argument to set additional parameter in the stream
    communication.

    The implementation is only available for \e StreamType:
    QGrpcServerStream, QGrpcClientStream, and QGrpcBidirStream.
*/

/*!
    \fn void QGrpcClientBase::channelChanged()
    \since 6.7

    Indicates that a new channel is attached to the client.
*/

/*!
    \fn void QGrpcClientBase::errorOccurred(const QGrpcStatus &status);

    Indicates that an error occurred during serialization.

    This signal is emitted when an error with \a status occurs in the channel
    or during serialization.

    \sa QGrpcOperation::errorOccurred
*/

class QGrpcClientBasePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QGrpcClientBase)
public:
    QGrpcClientBasePrivate(QLatin1StringView service) : service(service)
    {
    }

    QGrpcStatus checkThread(QLatin1StringView warningPreamble);
    bool checkChannel();
    void addStream(QGrpcOperation *stream);
    std::shared_ptr<QAbstractProtobufSerializer> serializer() const {
        if (const auto &c = channel)
            return c->serializer();
        return nullptr;
    }

    std::shared_ptr<QAbstractGrpcChannel> channel;
    const QLatin1StringView service;
    QMinimalFlatSet<QGrpcOperation *> activeStreams;
};

QGrpcStatus QGrpcClientBasePrivate::checkThread(QLatin1StringView warningPreamble)
{
    Q_Q(QGrpcClientBase);

    QGrpcStatus status;
    if (q->thread() != QThread::currentThread()) {
        status = QGrpcStatus{ QGrpcStatus::Unknown, threadSafetyWarning(warningPreamble) };
        qGrpcCritical() << status.message();
        emit q->errorOccurred(status);
    }
    return status;
}

bool QGrpcClientBasePrivate::checkChannel()
{
    Q_Q(QGrpcClientBase);

    if (!channel) {
        emit q->errorOccurred(QGrpcStatus{ QGrpcStatus::Unknown,
                                           q->tr("No channel(s) attached.") });
        return false;
    }
    return true;
}

void QGrpcClientBasePrivate::addStream(QGrpcOperation *grpcStream)
{
    Q_ASSERT(grpcStream);

    Q_Q(QGrpcClientBase);
    // Remove the operation pointer upon QObject destruction if it hasn't
    // already been gracefully removed by receiving the signals from below.
    QObject::connect(grpcStream, &QObject::destroyed, q, [this, grpcStream](QObject *obj) {
        Q_ASSERT(obj == grpcStream);
        activeStreams.remove(grpcStream);
    });

    // Transmit the signal from QGrpcOperation. If it emits, the transmission
    // gets disconnected.
    auto finishedConnection = std::make_shared<QMetaObject::Connection>();
    *finishedConnection = QObject::connect(grpcStream, &QGrpcOperation::finished, q,
                                           [this, grpcStream, q,
                                            finishedConnection](const auto &status) mutable {
                                               if (status != QGrpcStatus::StatusCode::Ok)
                                                   Q_EMIT q->errorOccurred(status);
                                               Q_ASSERT(activeStreams.contains(grpcStream));
                                               activeStreams.remove(grpcStream);
                                               QObject::disconnect(*finishedConnection);
                                           });
    const auto it = activeStreams.insert(grpcStream);
    Q_ASSERT(it.second);
}

QGrpcClientBase::QGrpcClientBase(QLatin1StringView service, QObject *parent)
    : QObject(*new QGrpcClientBasePrivate(service), parent)
{
}

QGrpcClientBase::~QGrpcClientBase() = default;

/*!
    Attaches \a channel to client as transport layer for gRPC.

    Parameters and return values will be serialized to the channel
    in a format it supports.

    \note \b Warning: Qt GRPC doesn't guarantee thread safety on the channel level.
    You have to invoke the channel-related functions on the same thread as
    QGrpcClientBase.
*/
void QGrpcClientBase::attachChannel(std::shared_ptr<QAbstractGrpcChannel> channel)
{
    if (channel->dPtr->threadId != QThread::currentThreadId()) {
        const QString status = threadSafetyWarning("QGrpcClientBase::attachChannel"_L1);
        qGrpcCritical() << status;
        emit errorOccurred(QGrpcStatus{ QGrpcStatus::Unknown, status });
        return;
    }
    Q_D(QGrpcClientBase);
    for (auto &stream : d->activeStreams) {
        assert(stream != nullptr);
        stream->cancel();
    }

    d->channel = std::move(channel);
    emit channelChanged();
}

std::shared_ptr<QGrpcCallReply> QGrpcClientBase::call(QLatin1StringView method,
                                                          const QProtobufMessage &arg,
                                                          const QGrpcCallOptions &options)
{
    std::optional<QByteArray> argData = trySerialize(arg);
    if (!argData) {
        Q_EMIT errorOccurred(QGrpcStatus{ QGrpcStatus::Unknown,
                                          tr("Serializing failed. Serializer is not ready.") });
        return {};
    }
    return call(method, *argData, options);
}

/*!
    \since 6.7
    Returns the channel attached to this client.
*/
std::shared_ptr<QAbstractGrpcChannel> QGrpcClientBase::channel() const noexcept
{
    Q_D(const QGrpcClientBase);
    return d->channel;
}

std::shared_ptr<QGrpcCallReply> QGrpcClientBase::call(QLatin1StringView method,
                                                          QByteArrayView arg,
                                                          const QGrpcCallOptions &options)
{
    std::shared_ptr<QGrpcCallReply> reply;
    Q_D(QGrpcClientBase);
    if (d->checkThread("QGrpcClientBase::call"_L1) != QGrpcStatus::Ok)
        return reply;

    if (!d->checkChannel())
        return reply;

    reply = d->channel->call(method, QLatin1StringView(d->service), arg, options);

    auto errorConnection = std::make_shared<QMetaObject::Connection>();
    *errorConnection = connect(reply.get(), &QGrpcCallReply::finished, this,
                               [this, reply, errorConnection](const QGrpcStatus &status) {
                                   if (status != QGrpcStatus::StatusCode::Ok)
                                       emit errorOccurred(status);
                                   QObject::disconnect(*errorConnection);
                               });
    return reply;
}

std::shared_ptr<QGrpcServerStream>
QGrpcClientBase::startServerStream(QLatin1StringView method, QByteArrayView arg,
                                       const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);

    if (d->checkThread("QGrpcClientBase::startStream<QGrpcServerStream>"_L1) != QGrpcStatus::Ok)
        return {};

    if (!d->checkChannel())
        return {};

    auto grpcStream =
            d->channel->startServerStream(method, QLatin1StringView(d->service), arg, options);
    d->addStream(grpcStream.get());
    return grpcStream;
}

std::shared_ptr<QGrpcClientStream>
QGrpcClientBase::startClientStream(QLatin1StringView method, QByteArrayView arg,
                                       const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);

    if (d->checkThread("QGrpcClientBase::startStream<QGrpcClientStream>"_L1) != QGrpcStatus::Ok)
        return {};

    if (!d->checkChannel())
        return {};

    auto grpcStream =
            d->channel->startClientStream(method, QLatin1StringView(d->service), arg, options);
    d->addStream(grpcStream.get());
    return grpcStream;
}

std::shared_ptr<QGrpcBidirStream>
QGrpcClientBase::startBidirStream(QLatin1StringView method, QByteArrayView arg,
                                      const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);

    if (d->checkThread("QGrpcClientBase::startStream<QGrpcBidirStream>"_L1) != QGrpcStatus::Ok)
        return {};

    if (!d->checkChannel())
        return {};

    auto grpcStream =
            d->channel->startBidirStream(method, QLatin1StringView(d->service), arg, options);
    d->addStream(grpcStream.get());
    return grpcStream;
}

std::optional<QByteArray> QGrpcClientBase::trySerialize(const QProtobufMessage &arg) const
{
    Q_D(const QGrpcClientBase);
    using namespace Qt::StringLiterals;
    auto serializer = d->serializer();
    if (serializer == nullptr)
        return std::nullopt;

    return serializer->serialize(&arg);
}

QT_END_NAMESPACE

#include "moc_qgrpcclientbase.cpp"