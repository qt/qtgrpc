// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstream.h>
#include <QtProtobuf/qprotobufserializer.h>
#include <qtgrpcglobal_p.h>

#include "qabstractgrpcclient.h"

QT_BEGIN_NAMESPACE

namespace {
static QString threadSafetyWarning(QLatin1StringView methodName)
{
    return QLatin1StringView("%1 is called from different thread.\n"
                             "QtGrpc doesn't guarantee thread safety on channel level.\n"
                             "You have to be confident that channel routines are working in "
                             "the same thread "
                             "as QAbstractGrpcClient")
            .arg(methodName);
}
} // namespace

/*!
    \class QAbstractGrpcClient
    \inmodule QtGrpc
    \brief The QAbstractGrpcClient class is bridge between gRPC clients and channels.

    QAbstractGrpcClient provides set of bridge functions for client classes generated out of
    protobuf services.
    QAbstractGrpcClient provides threads safety for stream and call methods of generated clients.
 */

/*!
    \fn template <typename ParamType> QGrpcStatus call(const QString &method, const QProtobufMessage
   &arg);
   \internal

    Calls \a method with \a arg argument of service client synchronously.
 */

/*!
    \fn template <typename ParamType, typename ReturnType> QGrpcStatus call(const QString
   &method, const QProtobufMessage &arg, QWeakPointer<ReturnType> ret);
   \internal

    Calls \a method with \a arg argument of service client synchronously and fills \a ret
    with gRPC reply.
 */

/*!
    \fn template <typename ParamType> QSharedPointer<QGrpcStream> stream(const QString &method,
   const QProtobufMessage &arg);
   \internal

    Streams to message notifications from server-stream \a method with message argument \a arg.
 */

/*!
    \fn template <typename ParamType, typename ReturnType> QSharedPointer<QGrpcStream> stream(const
   QString &method, const QProtobufMessage &arg, QWeakPointer<ReturnType> ret);
   \internal

    Streams to message notifications from server-stream \a method with message argument \a arg.

    Makes \a ret argument point to allocated return-message structure. The return-message structure
    will be updated each time a message is received from the server-stream.

    \note If \a ret is used as property-fields in other object, property NOTIFY signal won't
   be called in case of updated message received from server-stream.
*/

class QAbstractGrpcClientPrivate final
{
public:
    QAbstractGrpcClientPrivate(const QString &service) : service(service) { }

    std::shared_ptr<QAbstractGrpcChannel> channel;
    const QString service;
    std::vector<std::shared_ptr<QGrpcStream>> activeStreams;
};

QAbstractGrpcClient::QAbstractGrpcClient(const QString &service, QObject *parent)
    : QObject(parent), dPtr(std::make_unique<QAbstractGrpcClientPrivate>(service))
{
}

QAbstractGrpcClient::~QAbstractGrpcClient()
{
}

/*!
    Attaches \a channel to client as transport layer for gRPC.
    Parameters and return values will be serialized to supported by channel format.

    \note \b Warning: QtGrpc doesn't guarantee thread safety on channel level.
    You have to invoke the channel-related functions on the same thread as QAbstractGrpcClient.
*/
void QAbstractGrpcClient::attachChannel(const std::shared_ptr<QAbstractGrpcChannel> &channel)
{
    if (channel->thread() != QThread::currentThread()) {
        const QString status = threadSafetyWarning(
                QLatin1StringView("QAbstractGrpcClient::attachChannel"));
        logError(status);
        errorOccurred({ QGrpcStatus::Unknown, status });
        return;
    }
    for (auto &stream : dPtr->activeStreams) {
        stream->abort();
    }
    dPtr->channel = channel;
    for (auto &stream : dPtr->activeStreams) {
        stream->abort();
    }
}

QGrpcStatus QAbstractGrpcClient::call(const QString &method, const QByteArray &arg, QByteArray &ret)
{
    QGrpcStatus callStatus{ QGrpcStatus::Unknown };
    if (thread() != QThread::currentThread()) {
        const QGrpcStatus status(
                { QGrpcStatus::Unknown,
                  threadSafetyWarning(QLatin1StringView("QAbstractGrpcClient::call")) });
        logError(status.message());
        errorOccurred(status);
        return status;
    }
    callStatus = dPtr->channel
            ? dPtr->channel->call(method, dPtr->service, arg, ret)
            : QGrpcStatus{ QGrpcStatus::Unknown, QLatin1StringView("No channel(s) attached.") };

    if (callStatus != QGrpcStatus::Ok)
        errorOccurred(callStatus);

    return callStatus;
}

std::shared_ptr<QGrpcCallReply> QAbstractGrpcClient::call(const QString &method,
                                                          const QByteArray &arg)
{
    std::shared_ptr<QGrpcCallReply> reply;
    if (thread() != QThread::currentThread()) {
        const QGrpcStatus status(
                { QGrpcStatus::Unknown,
                  threadSafetyWarning(QLatin1StringView("QAbstractGrpcClient::call")) });
        logError(status.message());
        errorOccurred(status);
        return reply;
    }
    if (dPtr->channel) {
        reply.reset(new QGrpcCallReply(this), [](QGrpcCallReply *reply) { reply->deleteLater(); });

        auto errorConnection = std::make_shared<QMetaObject::Connection>();
        auto finishedConnection = std::make_shared<QMetaObject::Connection>();
        *errorConnection = connect(reply.get(), &QGrpcCallReply::errorOccurred, this,
                                   [this, reply, errorConnection,
                                    finishedConnection](const QGrpcStatus &status) mutable {
                                       errorOccurred(status);
                                       QObject::disconnect(*finishedConnection);
                                       QObject::disconnect(*errorConnection);
                                       reply.reset();
                                   });

        *finishedConnection = connect(reply.get(), &QGrpcCallReply::finished,
                                      [this, reply, errorConnection, finishedConnection]() mutable {
                                          // the usage of 'QObject::disconnect' requires the
                                          // compiler to capture 'this' but the current default
                                          // capture mode does not allow it
                                          this->disconnect(*finishedConnection);
                                          this->disconnect(*errorConnection);
                                          reply.reset();
                                      });

        dPtr->channel->call(method, dPtr->service, arg, reply.get());
    } else {
        errorOccurred({ QGrpcStatus::Unknown, QLatin1StringView("No channel(s) attached.") });
    }

    return reply;
}

std::shared_ptr<QGrpcStream> QAbstractGrpcClient::stream(const QString &method,
                                                         const QByteArray &arg,
                                                         const StreamHandler &handler)
{
    std::shared_ptr<QGrpcStream> grpcStream;

    if (thread() != QThread::currentThread()) {
        const QGrpcStatus status(
                { QGrpcStatus::Unknown,
                  threadSafetyWarning(QLatin1StringView("QAbstractGrpcClient::stream")) });
        logError(status.message());
        errorOccurred(status);
        return grpcStream;
    }
    if (dPtr->channel) {
        grpcStream.reset(new QGrpcStream(method, arg, handler, this),
                         [](QGrpcStream *stream) { stream->deleteLater(); });

        auto it = std::find_if(dPtr->activeStreams.begin(), dPtr->activeStreams.end(),
                               [grpcStream](const std::shared_ptr<QGrpcStream> &activeStream) {
                                   return *activeStream == *grpcStream;
                               });

        if (it != dPtr->activeStreams.end()) {
            // TODO:
            // This mechanism is not something that is specified by gRPC standard. From the server
            // perspective each stream request supposed to create a new connection with own scope.
            // Caching and reusing streams lead potential security risks, since we cannot
            // guarantee that the stream sharing is intentional.
            // This feature should have the explicit switch that controls its usage.
            (*it)->addHandler(handler);
            return *it; // If stream already exists return it for handling
        }

        auto errorConnection = std::make_shared<QMetaObject::Connection>();
        *errorConnection = connect(
                grpcStream.get(), &QGrpcStream::errorOccurred, this,
                [this, grpcStream](const QGrpcStatus &status) {
                    qGrpcWarning() << grpcStream->method() << "call" << dPtr->service
                                   << "stream error: " << status.message();
                    errorOccurred(status);
                    std::weak_ptr<QGrpcStream> weakStream(grpcStream);
                    // TODO: Make timeout configurable from channel settings
                    QTimer::singleShot(1000, this,
                                       [this, weakStream, method = grpcStream->method()] {
                                           auto stream = weakStream.lock();
                                           if (stream) {
                                               dPtr->channel->stream(stream.get(), dPtr->service,
                                                                     this);
                                           } else {
                                               qGrpcDebug() << "Stream for " << dPtr->service
                                                            << "method" << method
                                                            << " will not be restored by timeout.";
                                           }
                                       });
                });

        auto finishedConnection = std::make_shared<QMetaObject::Connection>();
        *finishedConnection = connect(
                grpcStream.get(), &QGrpcStream::finished, this,
                [this, grpcStream, errorConnection, finishedConnection]() mutable {
                    qGrpcWarning()
                            << grpcStream->method() << "call" << dPtr->service << "stream finished";
                    auto it = std::find_if(dPtr->activeStreams.begin(), dPtr->activeStreams.end(),
                                           [grpcStream](std::shared_ptr<QGrpcStream> activeStream) {
                                               return *activeStream == *grpcStream;
                                           });

                    if (it != dPtr->activeStreams.end()) {
                        dPtr->activeStreams.erase(it);
                    }
                    QObject::disconnect(*errorConnection);
                    QObject::disconnect(*finishedConnection);
                    grpcStream.reset();
                });

        dPtr->channel->stream(grpcStream.get(), dPtr->service, this);
        dPtr->activeStreams.push_back(grpcStream);
    } else {
        errorOccurred({ QGrpcStatus::Unknown, QLatin1StringView("No channel(s) attached.") });
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
    if (dPtr->channel == nullptr || dPtr->channel->serializer() == nullptr)
        return nullptr;
    return dPtr->channel->serializer();
}

void QAbstractGrpcClient::logError(const QString &str) const
{
    qGrpcCritical() << str;
}

QGrpcStatus QAbstractGrpcClient::handleDeserializationError(
        const QAbstractProtobufSerializer::DeserializationError &err) const
{
    QGrpcStatus status{ QGrpcStatus::Ok };
    switch (err) {
    case QAbstractProtobufSerializer::InvalidHeaderError: {
        const QLatin1StringView errStr("Response deserialization failed invalid field found");
        status = { QGrpcStatus::InvalidArgument, errStr };
        logError(errStr);
        errorOccurred(status);
    } break;
    case QAbstractProtobufSerializer::NoDeserializerError: {
        const QLatin1StringView errStr("No deserializer was found for a given type");
        status = { QGrpcStatus::InvalidArgument, errStr };
        logError(errStr);
        errorOccurred(status);
    } break;
    case QAbstractProtobufSerializer::UnexpectedEndOfStreamError: {
        const QLatin1StringView errStr("Invalid size of received buffer");
        status = { QGrpcStatus::OutOfRange, errStr };
        logError(errStr);
        errorOccurred(status);
    } break;
    case QAbstractProtobufSerializer::NoError:
        Q_FALLTHROUGH();
    default:
        const QLatin1StringView errStr("Deserializing failed, but no error was set.");
        status = { QGrpcStatus::InvalidArgument, errStr };
        logError(errStr);
        errorOccurred(status);
    }
    return status;
}

QT_END_NAMESPACE
