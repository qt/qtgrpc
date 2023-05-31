// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Giulio Girardi <giulio.girardi@protechgroup.it>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QFuture>
#include <QtCore/QList>
#include <QtCore/QThread>
#include <QtCore/qloggingcategory.h>
#include <QtGrpc/qabstractgrpcclient.h>
#include <QtGrpc/qabstractgrpccredentials.h>
#include <QtGrpc/qgrpccredentials.h>
#include <QtProtobuf/QProtobufSerializer>
#include <qtgrpcglobal_p.h>

#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/impl/codegen/byte_buffer.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/slice.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/sync_stream.h>
#include <grpcpp/security/credentials.h>

#include <unordered_map>

#include "qgrpcchannel.h"
#include "qgrpcchannel_p.h"

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

/*!
    \class QGrpcChannel
    \inmodule QtGrpc

    \brief The QGrpcChannel class is a gRPC-cpp native api implementation of
    QAbstractGrpcChannel interface.

    QGrpcChannel accepts the same grpc::ChannelCredentials type that is required
    by native-api grpc::CreateChannel.
    \sa{https://grpc.github.io/grpc/cpp/classgrpc_1_1_channel_credentials.html}{gRPC ChannelCredentials}.
*/

static grpc::Status parseByteBuffer(const grpc::ByteBuffer &buffer, QByteArray &data)
{
    std::vector<grpc::Slice> slices;
    auto status = buffer.Dump(&slices);

    if (!status.ok())
        return status;

    for (const auto &slice : slices)
        data.append((const char *)slice.begin(), slice.size());

    return grpc::Status::OK;
}

static grpc::ByteBuffer parseQByteArray(QByteArrayView bytearray)
{
    grpc::ByteBuffer buffer;
    grpc::Slice slice(bytearray.data(), bytearray.size());
    grpc::ByteBuffer tmp(&slice, 1);
    buffer.Swap(&tmp);
    return buffer;
}

static std::string toStdString(QLatin1StringView view)
{
    return std::string(view.data(), view.size());
}

static QByteArray buildRpcName(QLatin1StringView service, QLatin1StringView method)
{
    return '/' % QByteArrayView(service) % '/' % QByteArrayView(method);
}

QGrpcChannelStream::QGrpcChannelStream(grpc::Channel *channel, QLatin1StringView method,
                                       QByteArrayView data)
{
    grpc::ByteBuffer request = parseQByteArray(data);

    reader = grpc::internal::ClientReaderFactory<grpc::ByteBuffer>::Create(
            channel,
            grpc::internal::RpcMethod(toStdString(method).c_str(),
                                      grpc::internal::RpcMethod::SERVER_STREAMING),
            &context, request);

    thread = QThread::create([this] {
        grpc::ByteBuffer response;
        grpc::Status parseStatus;

        while (reader->Read(&response)) {
            QByteArray data;
            parseStatus = parseByteBuffer(response, data);
            if (!parseStatus.ok()) {
                status = { static_cast<QGrpcStatus::StatusCode>(parseStatus.error_code()),
                           QString::fromStdString(parseStatus.error_message()) };
                return; // exit thread
            }

            emit dataReady(data);
        }

        parseStatus = reader->Finish();
        status = { static_cast<QGrpcStatus::StatusCode>(parseStatus.error_code()),
                   QString::fromStdString(parseStatus.error_message()) };
    });

    connect(thread, &QThread::finished, this, &QGrpcChannelStream::finished);
}

void QGrpcChannelStream::start()
{
    thread->start();
}

QGrpcChannelStream::~QGrpcChannelStream()
{
    cancel();
    thread->wait();
    thread->deleteLater();
    delete reader;
}

void QGrpcChannelStream::cancel()
{
    // TODO: check thread safety
    context.TryCancel();
}

QGrpcChannelCall::QGrpcChannelCall(grpc::Channel *channel, QLatin1StringView method,
                                   QByteArrayView data)
{
    grpc::ByteBuffer request = parseQByteArray(data);
    thread = QThread::create([this, request, channel, method = toStdString(method)] {
        grpc::ByteBuffer callResponse;
        grpc::Status callStatus;

        callStatus = grpc::internal::BlockingUnaryCall(
                channel,
                grpc::internal::RpcMethod(method.c_str(), grpc::internal::RpcMethod::NORMAL_RPC),
                &context, request, &callResponse);
        if (!callStatus.ok()) {
            status = { static_cast<QGrpcStatus::StatusCode>(callStatus.error_code()),
                       QString::fromStdString(callStatus.error_message()) };
            return; // exit thread
        }

        callStatus = parseByteBuffer(callResponse, response);
        status = { static_cast<QGrpcStatus::StatusCode>(callStatus.error_code()),
                   QString::fromStdString(callStatus.error_message()) };
    });

    connect(thread, &QThread::finished, this, &QGrpcChannelCall::finished);
}

void QGrpcChannelCall::start()
{
    thread->start();
}

QGrpcChannelCall::~QGrpcChannelCall()
{
    cancel();
    thread->wait();
    thread->deleteLater();
}

void QGrpcChannelCall::cancel()
{
    // TODO: check thread safety
    context.TryCancel();
}

void QGrpcChannelCall::waitForFinished(const QDeadlineTimer &deadline)
{
    thread->wait(deadline);
}

QGrpcChannelPrivate::QGrpcChannelPrivate(const QUrl &url,
                                         QGrpcChannel::NativeGrpcChannelCredentials credentialsType,
                                         const QStringList &credentialsList)
{
    switch (credentialsType) {
    case QGrpcChannel::InsecureChannelCredentials:
        m_credentials = grpc::InsecureChannelCredentials();
        m_channel = grpc::CreateChannel(url.toString().toStdString(), m_credentials);
        break;
    case QGrpcChannel::GoogleDefaultCredentials:
        m_credentials = grpc::GoogleDefaultCredentials();
        m_channel = grpc::CreateChannel(url.toString().toStdString(), m_credentials);
        break;
    case QGrpcChannel::SslDefaultCredentials:
        if (credentialsList.size() < 3) {
            m_credentials = grpc::SslCredentials(grpc::SslCredentialsOptions());
        } else {
            grpc::SslCredentialsOptions options;
            options.pem_root_certs = credentialsList[0].toStdString();
            options.pem_private_key = credentialsList[1].toStdString();
            options.pem_cert_chain = credentialsList[2].toStdString();
            m_credentials = grpc::SslCredentials(options);
        }
        m_channel = grpc::CreateChannel(url.toString().toStdString(), m_credentials);
        break;
    }
}

QGrpcChannelPrivate::~QGrpcChannelPrivate() = default;

std::shared_ptr<QGrpcCallReply> QGrpcChannelPrivate::call(QAbstractGrpcClient *client,
                                                          QLatin1StringView method,
                                                          QLatin1StringView service,
                                                          QByteArrayView args)
{
    const QByteArray rpcName = buildRpcName(service, method);
    std::shared_ptr<QGrpcCallReply> reply(new QGrpcCallReply(client));
    QSharedPointer<QGrpcChannelCall> call(new QGrpcChannelCall(m_channel.get(),
                                                               QLatin1StringView(rpcName), args));
    auto connection = std::make_shared<QMetaObject::Connection>();
    auto abortConnection = std::make_shared<QMetaObject::Connection>();

    *connection = QObject::connect(call.get(), &QGrpcChannelCall::finished, reply.get(),
                                   [call, reply, connection, abortConnection] {
                                       if (call->status.code() == QGrpcStatus::Ok) {
                                           reply->setData(call->response);
                                           emit reply->finished();
                                       } else {
                                           reply->setData({});
                                           emit reply->errorOccurred(call->status);
                                       }

                                       QObject::disconnect(*connection);
                                       QObject::disconnect(*abortConnection);
                                   });

    *abortConnection = QObject::connect(reply.get(), &QGrpcCallReply::errorOccurred, call.get(),
                                        [call, connection,
                                         abortConnection](const QGrpcStatus &status) {
                                            if (status.code() == QGrpcStatus::Aborted) {
                                                QObject::disconnect(*connection);
                                                QObject::disconnect(*abortConnection);
                                            }
                                        });
    call->start();
    return reply;
}

QGrpcStatus QGrpcChannelPrivate::call(QLatin1StringView method, QLatin1StringView service,
                                      QByteArrayView args, QByteArray &ret)
{
    const QByteArray rpcName = buildRpcName(service, method);
    QGrpcChannelCall call(m_channel.get(), QLatin1StringView(rpcName), args);

    call.start();
    call.waitForFinished();

    ret = call.response;
    return call.status;
}

void QGrpcChannelPrivate::startStream(QGrpcStream *stream, QLatin1StringView service)
{
    Q_ASSERT(stream != nullptr);
    const QByteArray rpcName = buildRpcName(service, stream->method());

    QSharedPointer<QGrpcChannelStream> sub(new QGrpcChannelStream(m_channel.get(),
                                                                  QLatin1StringView(rpcName),
                                                                  stream->arg()));

    auto abortConnection = std::make_shared<QMetaObject::Connection>();
    auto readConnection = std::make_shared<QMetaObject::Connection>();
    auto connection = std::make_shared<QMetaObject::Connection>();

    auto disconnectAllConnections = [abortConnection, readConnection, connection]() {
        QObject::disconnect(*connection);
        QObject::disconnect(*readConnection);
        QObject::disconnect(*abortConnection);
    };

    *readConnection = QObject::connect(sub.get(), &QGrpcChannelStream::dataReady, stream,
                                       [stream](QByteArrayView data) {
                                           stream->handler(data.toByteArray());
                                       });

    *connection = QObject::connect(sub.get(), &QGrpcChannelStream::finished, stream,
                                   [disconnectAllConnections, sub, stream] {
                                       qGrpcDebug()
                                               << "Stream ended with server closing connection";
                                       disconnectAllConnections();

                                       if (sub->status.code() != QGrpcStatus::Ok)
                                           emit stream->errorOccurred(sub->status);
                                       emit stream->finished();
                                   });

    *abortConnection = QObject::connect(stream, &QGrpcStream::finished, sub.get(),
                                        [disconnectAllConnections, sub] {
                                            qGrpcDebug() << "Stream client was finished";
                                            disconnectAllConnections();
                                            sub->cancel();
                                        });

    sub->start();
}

/*!
    Constructs a gRPC channel, with \a url, \a credentialsType,
    and \a credentialsList object.
*/
QGrpcChannel::QGrpcChannel(const QUrl &url, NativeGrpcChannelCredentials credentialsType,
                           const QStringList &credentialsList)
    : QAbstractGrpcChannel(),
      dPtr(std::make_unique<QGrpcChannelPrivate>(url, credentialsType, credentialsList))
{
}

/*!
    Constructs a gRPC channel, with \a url, \a credentialsType and
    an empty credentials list.
*/
QGrpcChannel::QGrpcChannel(const QUrl &url, NativeGrpcChannelCredentials credentialsType)
    : QGrpcChannel(std::move(url), credentialsType, QStringList())
{
}

/*!
    Destroys the QGrpcChannel object.
*/
QGrpcChannel::~QGrpcChannel() = default;

/*!
    Synchronously calls the RPC method and writes the result to the output parameter \a ret.

    The RPC method name is concatenated from the \a method and \a service parameters
    with the given \a args.
*/
QGrpcStatus QGrpcChannel::call(QLatin1StringView method, QLatin1StringView service,
                               QByteArrayView args, QByteArray &ret)
{
    return dPtr->call(method, service, args, ret);
}

/*!
    Asynchronously calls the RPC method.

    The RPC method name is concatenated from the \a method and \a service parameters
    with the given \a args.
    The method can emit QGrpcCallReply::finished() and QGrpcCallReply::errorOccurred()
    signals on a QGrpcCallReply returned object.
*/
std::shared_ptr<QGrpcCallReply> QGrpcChannel::call(QAbstractGrpcClient *client,
                                                   QLatin1StringView method,
                                                   QLatin1StringView service, QByteArrayView args)
{
    return dPtr->call(client, method, service, args);
}

/*!
    Starts a stream on a \a stream using QGrpcStream::method() and the
    \a service to get the name of the RPC method.

    Calls QGrpcStream::handler() when the stream receives data from the server.
    The method may emit QGrpcStream::errorOccurred() when the stream has terminated with an error.
*/
void QGrpcChannel::startStream(QGrpcStream *stream, QLatin1StringView service)
{
    dPtr->startStream(stream, service);
}

/*!
    Returns the newly created QProtobufSerializer shared pointer.
*/
std::shared_ptr<QAbstractProtobufSerializer> QGrpcChannel::serializer() const
{
    // TODO: make selection based on credentials or channel settings
    return std::make_shared<QProtobufSerializer>();
}

QT_END_NAMESPACE
