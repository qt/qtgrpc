// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/private/qabstractgrpcchannel_p.h>
#include <QtGrpc/private/qtgrpcglobal_p.h>
#include <QtGrpc/qgrpcclientbase.h>
#include <QtGrpc/qgrpcoperation.h>

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/private/qminimalflatset_p.h>
#include <QtCore/private/qobject_p.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qlatin1stringview.h>

#include <optional>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

namespace {
QString threadSafetyWarning(const char *methodName)
{
    return QGrpcClientBase::tr("QGrpcClientBase::%1 is called from a different thread.\n"
                               "Qt GRPC doesn't guarantee thread safety on the channel level.\n"
                               "You have to be confident that channel routines are working in "
                               "the same thread as QGrpcClientBase.")
        .arg(QLatin1StringView(methodName));
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
    \fn template <typename StreamType, QGrpcClientBase::if_qtgrpc_stream<StreamType> = true> std::shared_ptr<StreamType> QGrpcClientBase::startStream(QLatin1StringView method, const QProtobufMessage &arg, const QGrpcCallOptions &options)

    Starts the stream \a method of the \e StreamType type with the message
    argument \a arg to the attached channel.

    Uses \a options argument to set additional parameter in the stream
    communication.

    The implementation is only available for \e StreamType:
    QGrpcServerStream, QGrpcClientStream, and QGrpcBidiStream.
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
*/

class QGrpcClientBasePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QGrpcClientBase)
public:
    explicit QGrpcClientBasePrivate(QLatin1StringView service) : service(service)
    {
    }

    bool checkThread(const char *methodName);
    bool checkChannel();
    void addStream(QGrpcOperation *stream);
    std::optional<QByteArray> trySerialize(const QProtobufMessage &arg) const;

    std::shared_ptr<QAbstractProtobufSerializer> serializer() const {
        return channel ? channel->serializer() : nullptr;
    }

    std::shared_ptr<QAbstractGrpcChannel> channel;
    const QLatin1StringView service;
    QMinimalFlatSet<QGrpcOperation *> activeStreams;
};

bool QGrpcClientBasePrivate::checkThread(const char *methodName)
{
    Q_Q(QGrpcClientBase);
    if (q->thread() != QThread::currentThread()) {
        qGrpcWarning() << threadSafetyWarning(methodName);
        return false;
    }
    return true;
}

bool QGrpcClientBasePrivate::checkChannel()
{
    if (!channel) {
        qGrpcWarning("No channel(s) attached");
        return false;
    }
    return true;
}

void QGrpcClientBasePrivate::addStream(QGrpcOperation *grpcStream)
{
    Q_ASSERT(grpcStream);

    Q_Q(QGrpcClientBase);
    // Remove the operation pointer upon QObject destruction if it hasn't
    // already been gracefully removed by receiving finished()
    QObject::connect(grpcStream, &QObject::destroyed, q, [this, grpcStream](QObject *obj) {
        Q_ASSERT(obj == grpcStream);
        activeStreams.remove(grpcStream);
    });

    auto finishedConnection = std::make_shared<QMetaObject::Connection>();
    *finishedConnection = QObject::connect(grpcStream, &QGrpcOperation::finished, q,
                                           [this, grpcStream, finishedConnection] {
                                               Q_ASSERT(activeStreams.contains(grpcStream));
                                               activeStreams.remove(grpcStream);
                                               QObject::disconnect(*finishedConnection);
                                           });
    const auto it = activeStreams.insert(grpcStream);
    Q_ASSERT(it.second);
}

std::optional<QByteArray> QGrpcClientBasePrivate::trySerialize(const QProtobufMessage &arg) const
{
    if (auto s = serializer())
        return s->serialize(&arg);

    qGrpcWarning("Serializing failed. Serializer is not ready");
    return std::nullopt;
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
bool QGrpcClientBase::attachChannel(std::shared_ptr<QAbstractGrpcChannel> channel)
{
    Q_D(QGrpcClientBase);
    // channel is not a QObject so we compare against the threadId set on construction.
    if (channel->dPtr->threadId != QThread::currentThreadId()) {
        qGrpcWarning() << threadSafetyWarning("attachChannel");
        return false;
    }

    for (const auto &stream : d->activeStreams) {
        assert(stream != nullptr);
        stream->cancel();
    }

    d->channel = std::move(channel);
    emit channelChanged();
    return true;
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
                                                      const QProtobufMessage &arg,
                                                      const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);

    if (!d->checkThread("call"))
        return {};

    if (!d->checkChannel())
        return {};

    std::optional<QByteArray> argData = d->trySerialize(arg);
    if (!argData)
        return {};

    return d->channel->call(method, d->service, *argData, options);
}

std::shared_ptr<QGrpcServerStream>
QGrpcClientBase::startServerStream(QLatin1StringView method, const QProtobufMessage &arg,
                                   const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);

    if (!d->checkThread("startStream<QGrpcServerStream>"))
        return {};

    if (!d->checkChannel())
        return {};

    std::optional<QByteArray> argData = d->trySerialize(arg);
    if (!argData)
        return {};

    auto grpcStream = d->channel->startServerStream(method, d->service, *argData, options);
    d->addStream(grpcStream.get());
    return grpcStream;
}

std::shared_ptr<QGrpcClientStream>
QGrpcClientBase::startClientStream(QLatin1StringView method, const QProtobufMessage &arg,
                                   const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);

    if (!d->checkThread("startStream<QGrpcClientStream>"))
        return {};

    if (!d->checkChannel())
        return {};

    std::optional<QByteArray> argData = d->trySerialize(arg);
    if (!argData)
        return {};

    auto grpcStream = d->channel->startClientStream(method, d->service, *argData, options);
    d->addStream(grpcStream.get());
    return grpcStream;
}

std::shared_ptr<QGrpcBidiStream> QGrpcClientBase::startBidiStream(QLatin1StringView method,
                                                                  const QProtobufMessage &arg,
                                                                  const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);

    if (!d->checkThread("startStream<QGrpcBidiStream>"))
        return {};

    if (!d->checkChannel())
        return {};

    std::optional<QByteArray> argData = d->trySerialize(arg);
    if (!argData)
        return {};

    auto grpcStream = d->channel->startBidiStream(method, d->service, *argData, options);
    d->addStream(grpcStream.get());
    return grpcStream;
}

bool QGrpcClientBase::event(QEvent *event)
{
    return QObject::event(event);
}

QT_END_NAMESPACE

#include "moc_qgrpcclientbase.cpp"
