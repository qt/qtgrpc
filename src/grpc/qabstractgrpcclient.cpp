// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstream.h>
#include <QtProtobuf/qprotobufserializer.h>
#include <QtGrpc/private/qabstractgrpcchannel_p.h>

#include <qtgrpcglobal_p.h>

#include <private/qobject_p.h>

#include "qabstractgrpcclient.h"

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

namespace {
static QString threadSafetyWarning(QLatin1StringView methodName)
{
    return "%1 is called from a different thread.\n"
           "Qt GRPC doesn't guarantee thread safety on the channel level.\n"
           "You have to be confident that channel routines are working in "
           "the same thread as QAbstractGrpcClient."_L1.arg(methodName);
}
} // namespace

/*!
    \class QAbstractGrpcClient
    \inmodule QtGrpc
    \brief The QAbstractGrpcClient class is bridge between gRPC clients
    and channels.

    QAbstractGrpcClient provides a set of functions for client classes
    generated out of protobuf services.
    QAbstractGrpcClient enforces thread safety for startStream() and call() methods
    of generated clients.
*/

/*!
    \fn template <typename ParamType> QGrpcStatus QAbstractGrpcClient::call(QLatin1StringView method,
    const QProtobufMessage &arg);
    \internal

    Synchronously calls the given \a method of this service client,
    with argument \a arg.
*/

/*!
    \fn template <typename ParamType, typename ReturnType> QGrpcStatus QAbstractGrpcClient::call(
    QLatin1StringView method, const QProtobufMessage &arg, ReturnType *ret);
    \internal

    Synchronously calls the given \a method of this service client,
    with argument \a arg and fills \a ret with gRPC reply.
*/

/*!
    \fn template <typename ParamType> QSharedPointer<QGrpcStream> QAbstractGrpcClient::startStream(
    QLatin1StringView method, const QProtobufMessage &arg);
    \internal

    Streams messages from the server stream \a method with the message
    argument \a arg to the attached channel.
*/

/*!
    \fn template <typename ParamType, typename ReturnType> QSharedPointer<QGrpcStream> QAbstractGrpcClient::startStream(
    QLatin1StringView method, const QProtobufMessage &arg, const QWeakPointer<ReturnType> ret);
    \internal

    Streams messages from the server stream \a method with the message
    argument \a arg to the attached channel.

    Makes \a ret argument point to allocated return-message structure.
    The return-message structure will be updated each time a message
    is received from the server-stream.

    \note If \a ret is used as property-fields in other object,
    property NOTIFY signal won't be called in case of updated
    message received from server-stream.
*/

class QAbstractGrpcClientPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAbstractGrpcClient)
public:
    QAbstractGrpcClientPrivate(QLatin1StringView service) : service(service)
    {
    }

    QGrpcStatus checkThread(QLatin1StringView warningPreamble);

    std::shared_ptr<QAbstractGrpcChannel> channel;
    const QLatin1StringView service;
    std::vector<std::shared_ptr<QGrpcStream>> activeStreams;
};

QGrpcStatus QAbstractGrpcClientPrivate::checkThread(QLatin1StringView warningPreamble)
{
    Q_Q(QAbstractGrpcClient);

    QGrpcStatus status;
    if (q->thread() != QThread::currentThread()) {
        status = { QGrpcStatus::Unknown, threadSafetyWarning(warningPreamble) };
        qGrpcCritical() << status.message();
        emit q->errorOccurred(status);
    }
    return status;
}

QAbstractGrpcClient::QAbstractGrpcClient(QLatin1StringView service, QObject *parent)
    : QObject(*new QAbstractGrpcClientPrivate(service), parent)
{
}

QAbstractGrpcClient::~QAbstractGrpcClient() = default;

/*!
    Attaches \a channel to client as transport layer for gRPC.

    Parameters and return values will be serialized to the channel
    in a format it supports.

    \note \b Warning: Qt GRPC doesn't guarantee thread safety on the channel level.
    You have to invoke the channel-related functions on the same thread as
    QAbstractGrpcClient.
*/
void QAbstractGrpcClient::attachChannel(const std::shared_ptr<QAbstractGrpcChannel> &channel)
{
    if (channel->dPtr->threadId != QThread::currentThreadId()) {
        const QString status = threadSafetyWarning("QAbstractGrpcClient::attachChannel"_L1);
        logError(status);
        emit errorOccurred({ QGrpcStatus::Unknown, status });
        return;
    }
    Q_D(QAbstractGrpcClient);
    for (auto &stream : d->activeStreams)
        stream->abort();

    d->channel = channel;
}

QGrpcStatus QAbstractGrpcClient::call(QLatin1StringView method, QByteArrayView arg, QByteArray &ret)
{
    Q_D(QAbstractGrpcClient);

    QGrpcStatus callStatus = d->checkThread("QAbstractGrpcClient::call"_L1);
    if (callStatus != QGrpcStatus::Ok)
        return callStatus;

    callStatus = d->channel
            ? d->channel->call(method, QLatin1StringView(d->service), arg, ret)
            : QGrpcStatus{ QGrpcStatus::Unknown, "No channel(s) attached."_L1 };

    if (callStatus != QGrpcStatus::Ok)
        emit errorOccurred(callStatus);

    return callStatus;
}

std::shared_ptr<QGrpcCallReply> QAbstractGrpcClient::call(QLatin1StringView method,
                                                          QByteArrayView arg)
{
    std::shared_ptr<QGrpcCallReply> reply;
    Q_D(QAbstractGrpcClient);
    if (d->checkThread("QAbstractGrpcClient::call"_L1) != QGrpcStatus::Ok)
        return reply;

    if (d->channel) {
        reply = d->channel->call(this, method, QLatin1StringView(d->service), arg);

        auto errorConnection = std::make_shared<QMetaObject::Connection>();
        *errorConnection =
                connect(reply.get(), &QGrpcCallReply::errorOccurred, this,
                        [this](const QGrpcStatus &status) { emit errorOccurred(status); });
    } else {
        emit errorOccurred({ QGrpcStatus::Unknown, "No channel(s) attached."_L1 });
    }

    return reply;
}

std::shared_ptr<QGrpcStream> QAbstractGrpcClient::startStream(QLatin1StringView method,
                                                              QByteArrayView arg,
                                                              const StreamHandler &handler)
{
    Q_D(QAbstractGrpcClient);

    std::shared_ptr<QGrpcStream> grpcStream;
    if (d->checkThread("QAbstractGrpcClient::startStream"_L1) != QGrpcStatus::Ok)
        return grpcStream;

    if (d->channel) {
        grpcStream.reset(new QGrpcStream(method, arg, handler, this),
                         [](QGrpcStream *stream) { stream->deleteLater(); });

        auto it = std::find_if(d->activeStreams.begin(), d->activeStreams.end(),
                               [grpcStream](const std::shared_ptr<QGrpcStream> &activeStream) {
                                   return *activeStream == *grpcStream;
                               });

        if (it != d->activeStreams.end()) {
            // TODO:
            // This mechanism is not something that is specified by gRPC standard. From the server
            // perspective each stream request supposed to create a new connection with own scope.
            // Caching and reusing streams lead potential security risks, since we cannot
            // guarantee that the stream sharing is intentional.
            // This feature should have an explicit switch that controls its usage.
            (*it)->addHandler(handler);
            return *it; // If stream already exists return it for handling
        }

        auto errorConnection = std::make_shared<QMetaObject::Connection>();
        *errorConnection = connect(
                grpcStream.get(), &QGrpcStream::errorOccurred, this,
                [this, grpcStream](const QGrpcStatus &status) {
                    Q_D(QAbstractGrpcClient);
                    qGrpcWarning() << grpcStream->method() << "call" << d->service
                                   << "stream error: " << status.message();
                    emit errorOccurred(status);
                    std::weak_ptr<QGrpcStream> weakStream(grpcStream);
                    // TODO: Make timeout configurable from channel settings
                    QTimer::singleShot(1000, this,
                                       [this, weakStream, method = grpcStream->method()] {
                                           Q_D(QAbstractGrpcClient);
                                           auto stream = weakStream.lock();
                                           if (stream) {
                                               d->channel->startStream(
                                                       stream.get(), QLatin1StringView(d->service));
                                           } else {
                                               qGrpcDebug() << "Stream for" << d->service
                                                            << "method" << method
                                                            << "will not be restored by timeout.";
                                           }
                                       });
                });

        auto finishedConnection = std::make_shared<QMetaObject::Connection>();
        *finishedConnection = connect(
                grpcStream.get(), &QGrpcStream::finished, this,
                [this, grpcStream, errorConnection, finishedConnection]() mutable {
                    Q_D(QAbstractGrpcClient);
                    qGrpcWarning()
                            << grpcStream->method() << "call" << d->service << "stream finished.";
                    auto it = std::find_if(d->activeStreams.begin(), d->activeStreams.end(),
                                           [grpcStream](std::shared_ptr<QGrpcStream> activeStream) {
                                               return *activeStream == *grpcStream;
                                           });

                    if (it != d->activeStreams.end())
                        d->activeStreams.erase(it);

                    QObject::disconnect(*errorConnection);
                    QObject::disconnect(*finishedConnection);
                    grpcStream.reset();
                });

        d->channel->startStream(grpcStream.get(), QLatin1StringView(d->service));
        d->activeStreams.push_back(grpcStream);
    } else {
        emit errorOccurred({ QGrpcStatus::Unknown, "No channel(s) attached."_L1 });
    }
    return grpcStream;
}

/*!
    \internal

    Serializer provides assigned to client serializer.
    Returns pointer to serializerowned by QProtobufSerializerRegistry.
*/
std::shared_ptr<QAbstractProtobufSerializer> QAbstractGrpcClient::serializer() const
{
    Q_D(const QAbstractGrpcClient);
    if (const auto &c = d->channel)
        return c->serializer();
    return nullptr;
}

void QAbstractGrpcClient::logError(const QString &str) const
{
    qGrpcCritical() << str;
}

QGrpcStatus QAbstractGrpcClient::handleDeserializationError(
        const QAbstractProtobufSerializer::DeserializationError &err)
{
    QGrpcStatus status{ QGrpcStatus::Ok };
    switch (err) {
    case QAbstractProtobufSerializer::InvalidHeaderError: {
        const QLatin1StringView errStr("Response deserialization failed: invalid field found.");
        status = { QGrpcStatus::InvalidArgument, errStr };
        logError(errStr);
        emit errorOccurred(status);
    } break;
    case QAbstractProtobufSerializer::NoDeserializerError: {
        const QLatin1StringView errStr("No deserializer was found for a given type.");
        status = { QGrpcStatus::InvalidArgument, errStr };
        logError(errStr);
        emit errorOccurred(status);
    } break;
    case QAbstractProtobufSerializer::UnexpectedEndOfStreamError: {
        const QLatin1StringView errStr("Invalid size of received buffer.");
        status = { QGrpcStatus::OutOfRange, errStr };
        logError(errStr);
        emit errorOccurred(status);
    } break;
    case QAbstractProtobufSerializer::NoError:
        Q_FALLTHROUGH();
    default:
        const QLatin1StringView errStr("Deserializing failed, but no error was set.");
        status = { QGrpcStatus::InvalidArgument, errStr };
        logError(errStr);
        emit errorOccurred(status);
    }
    return status;
}

QT_END_NAMESPACE

#include "moc_qabstractgrpcclient.cpp"
