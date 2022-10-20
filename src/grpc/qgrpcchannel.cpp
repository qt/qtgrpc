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

/*!
    \class QGrpcChannel
    \inmodule QtGrpc

    \brief The QGrpcChannel class is a gRPC-cpp native api implementation of
   QAbstractGrpcChannel interface.

    QGrpcChannel accepts the same grpc::ChannelCredentials type that is required
    by native-api grpc::CreateChannel.
    See \l{gRPC Channel
   Credentials}[https://grpc.github.io/grpc/cpp/classgrpc__impl_1_1_channel_credentials.html].
 */

static inline grpc::Status parseByteBuffer(const grpc::ByteBuffer &buffer, QByteArray &data)
{
    std::vector<grpc::Slice> slices;
    auto status = buffer.Dump(&slices);

    if (!status.ok())
        return status;

    for (const auto &slice : slices) {
        data.append(QByteArray((const char *)slice.begin(), slice.size()));
    }

    return grpc::Status::OK;
}

static inline grpc::ByteBuffer parseQByteArray(const QByteArray &bytearray)
{
    grpc::ByteBuffer buffer;
    grpc::Slice slice(bytearray.data(), bytearray.size());
    grpc::ByteBuffer tmp(&slice, 1);
    buffer.Swap(&tmp);
    return buffer;
}

QGrpcChannelStream::QGrpcChannelStream(grpc::Channel *channel, const QString &method,
                                       const QByteArray &data, QObject *parent)
    : QObject(parent)
{
    grpc::ByteBuffer request = parseQByteArray(data);

    reader = grpc::internal::ClientReaderFactory<grpc::ByteBuffer>::Create(
            channel,
            grpc::internal::RpcMethod(method.toStdString().c_str(),
                                      grpc::internal::RpcMethod::SERVER_STREAMING),
            &context, request);

    thread = QThread::create([this]() {
        grpc::ByteBuffer response;
        grpc::Status status;

        while (reader->Read(&response)) {
            QByteArray data;
            status = parseByteBuffer(response, data);
            if (!status.ok()) {
                this->status = { static_cast<QGrpcStatus::StatusCode>(status.error_code()),
                                 QString::fromStdString(status.error_message()) };
                return; // exit thread
            }

            emit this->dataReady(data);
        }

        status = reader->Finish();
        this->status = { static_cast<QGrpcStatus::StatusCode>(status.error_code()),
                         QString::fromStdString(status.error_message()) };
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

QGrpcChannelCall::QGrpcChannelCall(grpc::Channel *channel, const QString &method,
                                   const QByteArray &data, QObject *parent)
    : QObject(parent)
{
    grpc::ByteBuffer request = parseQByteArray(data);

    thread = QThread::create([this, request, channel, method]() {
        grpc::ByteBuffer response;
        QByteArray data;
        grpc::Status status;

        status = grpc::internal::BlockingUnaryCall(
                channel,
                grpc::internal::RpcMethod(method.toStdString().c_str(),
                                          grpc::internal::RpcMethod::NORMAL_RPC),
                &context, request, &response);
        if (!status.ok()) {
            this->status = { static_cast<QGrpcStatus::StatusCode>(status.error_code()),
                             QString::fromStdString(status.error_message()) };
            return; // exit thread
        }

        status = parseByteBuffer(response, this->response);
        this->status = { static_cast<QGrpcStatus::StatusCode>(status.error_code()),
                         QString::fromStdString(status.error_message()) };
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
    case QGrpcChannel::SslDefaultCredentials: {
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
    } break;
    }
}

QGrpcChannelPrivate::~QGrpcChannelPrivate()
{
}

void QGrpcChannelPrivate::call(const QString &method, const QString &service,
                               const QByteArray &args, QGrpcCallReply *reply)
{
    const QString rpcName = QLatin1StringView("/%1/%2").arg(service, method);

    QSharedPointer<QGrpcChannelCall> call(new QGrpcChannelCall(m_channel.get(), rpcName, args,
                                                               reply),
                                          &QGrpcChannelCall::deleteLater);
    auto connection = QSharedPointer<QMetaObject::Connection>::create();
    auto abortConnection = QSharedPointer<QMetaObject::Connection>::create();

    *connection = QObject::connect(call.get(), &QGrpcChannelCall::finished, reply,
                                   [call, reply, connection, abortConnection]() {
                                       if (call->status.code() == QGrpcStatus::Ok) {
                                           reply->setData(call->response);
                                           reply->finished();
                                       } else {
                                           reply->setData({});
                                           reply->errorOccurred(call->status);
                                       }

                                       QObject::disconnect(*connection);
                                       QObject::disconnect(*abortConnection);
                                   });

    *abortConnection = QObject::connect(reply, &QGrpcCallReply::errorOccurred, call.get(),
                                        [call, connection,
                                         abortConnection](const QGrpcStatus &status) {
                                            if (status.code() == QGrpcStatus::Aborted) {
                                                QObject::disconnect(*connection);
                                                QObject::disconnect(*abortConnection);
                                            }
                                        });

    call->start();
}

QGrpcStatus QGrpcChannelPrivate::call(const QString &method, const QString &service,
                                      const QByteArray &args, QByteArray &ret)
{
    const QString rpcName = QLatin1StringView("/%1/%2").arg(service, method);
    QGrpcChannelCall call(m_channel.get(), rpcName, args);

    call.start();
    call.waitForFinished();

    ret = call.response;
    return call.status;
}

void QGrpcChannelPrivate::stream(QGrpcStream *stream, const QString &service,
                                 QAbstractGrpcClient *client)
{
    Q_ASSERT(stream != nullptr);

    const QString rpcName = QLatin1StringView("/%1/%2").arg(service, stream->method());

    QSharedPointer<QGrpcChannelStream> sub(new QGrpcChannelStream(m_channel.get(), rpcName,
                                                                  stream->arg(), stream),
                                           &QGrpcChannelStream::deleteLater);

    auto abortConnection = QSharedPointer<QMetaObject::Connection>::create();
    auto readConnection = QSharedPointer<QMetaObject::Connection>::create();
    auto clientConnection = QSharedPointer<QMetaObject::Connection>::create();
    auto connection = QSharedPointer<QMetaObject::Connection>::create();

    *readConnection = QObject::connect(sub.get(), &QGrpcChannelStream::dataReady, stream,
                                       [stream](const QByteArray &data) { stream->handler(data); });

    *connection = QObject::connect(sub.get(), &QGrpcChannelStream::finished, stream,
                                   [sub, stream, readConnection, abortConnection, service,
                                    connection, clientConnection]() {
                                       qGrpcDebug()
                                               << "Stream ended with server closing connection";

                                       QObject::disconnect(*connection);
                                       QObject::disconnect(*readConnection);
                                       QObject::disconnect(*abortConnection);
                                       QObject::disconnect(*clientConnection);

                                       if (sub->status.code() != QGrpcStatus::Ok) {
                                           stream->errorOccurred(sub->status);
                                       }
                                   });

    *abortConnection = QObject::connect(stream, &QGrpcStream::finished, sub.get(),
                                        [connection, abortConnection, readConnection, sub,
                                         clientConnection] {
                                            qGrpcDebug() << "Stream client was finished";

                                            QObject::disconnect(*connection);
                                            QObject::disconnect(*readConnection);
                                            QObject::disconnect(*abortConnection);
                                            QObject::disconnect(*clientConnection);

                                            sub->cancel();
                                        });

    *clientConnection = QObject::connect(client, &QAbstractGrpcClient::destroyed, sub.get(),
                                         [readConnection, connection, abortConnection, sub,
                                          clientConnection]() {
                                             qGrpcDebug() << "Grpc client was destroyed";

                                             QObject::disconnect(*connection);
                                             QObject::disconnect(*readConnection);
                                             QObject::disconnect(*abortConnection);
                                             QObject::disconnect(*clientConnection);

                                             sub->cancel();
                                         });

    sub->start();
}

/*!
    QGrpcChannel constructs QGrpcChannel with \a url, \a credentialsType and \a credentialsList
    object.
 */
QGrpcChannel::QGrpcChannel(const QUrl &url, NativeGrpcChannelCredentials credentialsType,
                           const QStringList &credentialsList)
    : QAbstractGrpcChannel(),
      dPtr(std::make_unique<QGrpcChannelPrivate>(url, credentialsType, credentialsList))
{
}

QGrpcChannel::~QGrpcChannel() = default;

QGrpcStatus QGrpcChannel::call(const QString &method, const QString &service,
                               const QByteArray &args, QByteArray &ret)
{
    return dPtr->call(method, service, args, ret);
}

void QGrpcChannel::call(const QString &method, const QString &service, const QByteArray &args,
                        QGrpcCallReply *reply)
{
    dPtr->call(method, service, args, reply);
}

void QGrpcChannel::stream(QGrpcStream *stream, const QString &service, QAbstractGrpcClient *client)
{
    dPtr->stream(stream, service, client);
}

QSharedPointer<QAbstractProtobufSerializer> QGrpcChannel::serializer() const
{
    // TODO: make selection based on credentials or channel settings
    return QSharedPointer<QProtobufSerializer>::create();
}

QT_END_NAMESPACE
