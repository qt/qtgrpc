// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtGrpc/private/qabstractgrpcchannel_p.h>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstream.h>
#include <QtProtobuf/qprotobufserializer.h>

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
    QAbstractGrpcClient enforces thread safety for startServerStream() and call() methods
    of generated clients.
    The methods QAbstractGrpcClient::call() and QAbstractGrpcClient::startServerStream()
    should only be called by the generated client classes.
*/

/*!
    \fn template <typename ParamType> QGrpcStatus QAbstractGrpcClient::call(QLatin1StringView method,
    const QProtobufMessage &arg, const QGrpcCallOptions &options);

    Synchronously calls the given \a method of this service client,
    with argument \a arg.
    Uses \a options argument to set additional parameter for the call.
*/

/*!
    \fn template <typename ParamType, typename ReturnType> QGrpcStatus QAbstractGrpcClient::call(QLatin1StringView method, const QProtobufMessage &arg, ReturnType *ret, const QGrpcCallOptions &options)

    Synchronously calls the given \a method of this service client,
    with argument \a arg and fills \a ret with gRPC reply.
    Uses \a options argument to set additional parameter for the call.
*/

/*!
    \fn template <typename ParamType> QSharedPointer<QGrpcServerStream>
   QAbstractGrpcClient::startServerStream(QLatin1StringView method, const QProtobufMessage &arg,
   const QGrpcCallOptions &options);

    Streams messages from the server stream \a method with the message
    argument \a arg to the attached channel.
    Uses \a options argument to set additional parameter for the call.
*/

class QAbstractGrpcClientPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAbstractGrpcClient)
public:
    QAbstractGrpcClientPrivate(QLatin1StringView service) : service(service.data(), service.size())
    {
    }

    QGrpcStatus checkThread(QLatin1StringView warningPreamble);

    std::shared_ptr<QAbstractGrpcChannel> channel;
    const std::string service;
    std::vector<std::shared_ptr<QGrpcServerStream>> activeStreams;
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
        qGrpcCritical() << status;
        emit errorOccurred({ QGrpcStatus::Unknown, status });
        return;
    }
    Q_D(QAbstractGrpcClient);
    for (auto &stream : d->activeStreams)
        stream->cancel();

    d->channel = channel;
    emit channelChanged();
}

/*!
 * \brief channel is getter method.
 * \return pointer to currently attached channel.
 */
const std::shared_ptr<QAbstractGrpcChannel> &QAbstractGrpcClient::channel()
{
    Q_D(QAbstractGrpcClient);
    return d->channel;
}

std::shared_ptr<QGrpcCallReply> QAbstractGrpcClient::call(QLatin1StringView method,
                                                          QByteArrayView arg,
                                                          const QGrpcCallOptions &options)
{
    std::shared_ptr<QGrpcCallReply> reply;
    Q_D(QAbstractGrpcClient);
    if (d->checkThread("QAbstractGrpcClient::call"_L1) != QGrpcStatus::Ok)
        return reply;

    if (d->channel) {
        reply = d->channel->call(method, QLatin1StringView(d->service), arg, options);

        auto errorConnection = std::make_shared<QMetaObject::Connection>();
        *errorConnection = connect(reply.get(), &QGrpcCallReply::errorOccurred, this,
                                   [this, reply, errorConnection](const QGrpcStatus &status) {
                                       QObject::disconnect(*errorConnection);
                                       emit errorOccurred(status);
                                   });
    } else {
        emit errorOccurred({ QGrpcStatus::Unknown, "No channel(s) attached."_L1 });
    }

    return reply;
}

std::shared_ptr<QGrpcServerStream>
QAbstractGrpcClient::startServerStream(QLatin1StringView method, QByteArrayView arg,
                                       const QGrpcCallOptions &options)
{
    Q_D(QAbstractGrpcClient);

    std::shared_ptr<QGrpcServerStream> grpcStream;
    if (d->checkThread("QAbstractGrpcClient::startServerStream"_L1) != QGrpcStatus::Ok)
        return grpcStream;

    if (d->channel) {
        grpcStream =
                d->channel->startServerStream(method, QLatin1StringView(d->service), arg, options);

        auto errorConnection = std::make_shared<QMetaObject::Connection>();
        *errorConnection =
                connect(grpcStream.get(), &QGrpcServerStream::errorOccurred, this,
                        [this, grpcStream, method, errorConnection](const QGrpcStatus &status) {
                            QObject::disconnect(*errorConnection);

                            Q_D(QAbstractGrpcClient);
                            qGrpcWarning() << method << "call" << d->service
                                           << "stream error: " << status.message();
                            emit errorOccurred(status);
                        });

        auto finishedConnection = std::make_shared<QMetaObject::Connection>();
        *finishedConnection = connect(
                grpcStream.get(), &QGrpcServerStream::finished, this,
                [this, grpcStream, errorConnection, finishedConnection, method]() mutable {
                    QObject::disconnect(*errorConnection);
                    QObject::disconnect(*finishedConnection);

                    Q_D(QAbstractGrpcClient);
                    qGrpcWarning() << method << "call" << d->service << "stream finished.";

                    auto it =
                            std::find(d->activeStreams.begin(), d->activeStreams.end(), grpcStream);
                    if (it != d->activeStreams.end())
                        d->activeStreams.erase(it);

                    grpcStream.reset();
                });

        d->activeStreams.push_back(grpcStream);
    } else {
        emit errorOccurred({ QGrpcStatus::Unknown, "No channel(s) attached."_L1 });
    }
    return grpcStream;
}

/*!
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

QT_END_NAMESPACE

#include "moc_qabstractgrpcclient.cpp"
