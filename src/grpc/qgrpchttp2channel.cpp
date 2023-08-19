// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>, Viktor Kopp <vifactor@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QEventLoop>
#include <QtCore/QMetaObject>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtGrpc/qabstractgrpcclient.h>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpccredentials.h>
#include <QtGrpc/qgrpcstream.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtProtobuf/qprotobufserializer.h>
#include <qtgrpcglobal_p.h>

#include <unordered_map>

#include "qgrpchttp2channel.h"

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

/*!
    \class QGrpcHttp2Channel
    \inmodule QtGrpc

    \brief The QGrpcHttp2Channel class is an HTTP/2 implementation of
    QAbstractGrpcChannel interface.

    QGrpcHttp2Channel utilizes channel and call credentials.
    Channel credential QGrpcHttp2Channel supports SslConfigCredential key.
    When HTTPS is used, this key has to be explicitly specified and provide
    QSslConfiguration and value. The QSslConfiguration provided will be used
    to establish HTTP/2 secured connection. All keys passed as
    QGrpcCallCredentials will be used as HTTP/2 headers with related
    values assigned.
*/

// This QNetworkReply::NetworkError -> QGrpcStatus::StatusCode mapping should be kept in sync
// with original https://github.com/grpc/grpc/blob/master/doc/statuscodes.md
const static std::unordered_map<QNetworkReply::NetworkError, QGrpcStatus::StatusCode>
        StatusCodeMap = {
            { QNetworkReply::ConnectionRefusedError, QGrpcStatus::Unavailable },
            { QNetworkReply::RemoteHostClosedError, QGrpcStatus::Unavailable },
            { QNetworkReply::HostNotFoundError, QGrpcStatus::Unavailable },
            { QNetworkReply::TimeoutError, QGrpcStatus::DeadlineExceeded },
            { QNetworkReply::OperationCanceledError, QGrpcStatus::Unavailable },
            { QNetworkReply::SslHandshakeFailedError, QGrpcStatus::PermissionDenied },
            { QNetworkReply::TemporaryNetworkFailureError, QGrpcStatus::Unknown },
            { QNetworkReply::NetworkSessionFailedError, QGrpcStatus::Unavailable },
            { QNetworkReply::BackgroundRequestNotAllowedError, QGrpcStatus::Unknown },
            { QNetworkReply::TooManyRedirectsError, QGrpcStatus::Unavailable },
            { QNetworkReply::InsecureRedirectError, QGrpcStatus::PermissionDenied },
            { QNetworkReply::UnknownNetworkError, QGrpcStatus::Unknown },
            { QNetworkReply::ProxyConnectionRefusedError, QGrpcStatus::Unavailable },
            { QNetworkReply::ProxyConnectionClosedError, QGrpcStatus::Unavailable },
            { QNetworkReply::ProxyNotFoundError, QGrpcStatus::Unavailable },
            { QNetworkReply::ProxyTimeoutError, QGrpcStatus::DeadlineExceeded },
            { QNetworkReply::ProxyAuthenticationRequiredError, QGrpcStatus::Unauthenticated },
            { QNetworkReply::UnknownProxyError, QGrpcStatus::Unknown },
            { QNetworkReply::ContentAccessDenied, QGrpcStatus::PermissionDenied },
            { QNetworkReply::ContentOperationNotPermittedError, QGrpcStatus::PermissionDenied },
            { QNetworkReply::ContentNotFoundError, QGrpcStatus::NotFound },
            { QNetworkReply::AuthenticationRequiredError, QGrpcStatus::PermissionDenied },
            { QNetworkReply::ContentReSendError, QGrpcStatus::DataLoss },
            { QNetworkReply::ContentConflictError, QGrpcStatus::InvalidArgument },
            { QNetworkReply::ContentGoneError, QGrpcStatus::DataLoss },
            { QNetworkReply::UnknownContentError, QGrpcStatus::Unknown },
            { QNetworkReply::ProtocolUnknownError, QGrpcStatus::Unknown },
            { QNetworkReply::ProtocolInvalidOperationError, QGrpcStatus::Unimplemented },
            { QNetworkReply::ProtocolFailure, QGrpcStatus::Unknown },
            { QNetworkReply::InternalServerError, QGrpcStatus::Internal },
            { QNetworkReply::OperationNotImplementedError, QGrpcStatus::Unimplemented },
            { QNetworkReply::ServiceUnavailableError, QGrpcStatus::Unavailable },
            { QNetworkReply::UnknownServerError, QGrpcStatus::Unknown }
        };

constexpr char GrpcAcceptEncodingHeader[] = "grpc-accept-encoding";
constexpr char AcceptEncodingHeader[] = "accept-encoding";
constexpr char TEHeader[] = "te";
constexpr char GrpcStatusHeader[] = "grpc-status";
constexpr char GrpcStatusMessageHeader[] = "grpc-message";
constexpr qsizetype GrpcMessageSizeHeaderSize = 5;

struct QGrpcHttp2ChannelPrivate
{
    struct ExpectedData
    {
        qsizetype expectedSize;
        QByteArray container;
    };

    QUrl url;
    QNetworkAccessManager nm;
    std::unique_ptr<QAbstractGrpcCredentials> credentials;
#if QT_CONFIG(ssl)
    QSslConfiguration sslConfig;
#endif
    std::unordered_map<QNetworkReply *, ExpectedData> activeStreamReplies;
    QObject lambdaContext;

    QNetworkReply *post(QLatin1StringView method, QLatin1StringView service, QByteArrayView args,
                        bool stream = false)
    {
        QUrl callUrl = url;
        callUrl.setPath("/%1/%2"_L1.arg(service, method));

        qGrpcDebug() << "Service call url:" << callUrl;
        QNetworkRequest request(callUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/grpc"_L1));
        request.setRawHeader(GrpcAcceptEncodingHeader, "identity,deflate,gzip");
        request.setRawHeader(AcceptEncodingHeader, "identity,gzip");
        request.setRawHeader(TEHeader, "trailers");
#if QT_CONFIG(ssl)
        request.setSslConfiguration(sslConfig);
#endif
        for (const auto &[key, value] : credentials->callCredentials().toStdMap())
            request.setRawHeader(key, value.toString().toUtf8());

        request.setAttribute(QNetworkRequest::Http2DirectAttribute, true);

        QByteArray msg(GrpcMessageSizeHeaderSize, '\0');
        // Args must be 4-byte unsigned int to fit into 4-byte big endian
        qToBigEndian(static_cast<quint32>(args.size()), msg.data() + 1);
        msg += args;
        qGrpcDebug() << "SEND msg with size:" << msg.size();

        QNetworkReply *networkReply = nm.post(request, msg);
#if QT_CONFIG(ssl)
        QObject::connect(networkReply, &QNetworkReply::sslErrors, networkReply,
                         [networkReply](const QList<QSslError> &errors) {
                             qGrpcCritical() << errors;
                             // TODO: filter out noncritical SSL handshake errors
                             // FIXME: error due to ssl failure is not transferred to the client:
                             // last error will be Operation canceled
                             QGrpcHttp2ChannelPrivate::abortNetworkReply(networkReply);
                         });
#endif
        if (!stream) {
            // TODO: Add configurable timeout logic
            QTimer::singleShot(6000, networkReply, [networkReply] {
                QGrpcHttp2ChannelPrivate::abortNetworkReply(networkReply);
            });
        }
        return networkReply;
    }

    static void abortNetworkReply(QNetworkReply *networkReply)
    {
        if (networkReply->isRunning())
            networkReply->abort();
        else
            networkReply->deleteLater();
    }

    static QByteArray processReply(QNetworkReply *networkReply, QGrpcStatus::StatusCode &statusCode)
    {
        // Check if no network error occurred
        if (networkReply->error() != QNetworkReply::NoError) {
            statusCode = StatusCodeMap.at(networkReply->error());
            return {};
        }

        // Check if server answer with error
        statusCode = static_cast<QGrpcStatus::StatusCode>(
                networkReply->rawHeader(GrpcStatusHeader).toInt());
        if (statusCode != QGrpcStatus::StatusCode::Ok)
            return {};

        // Message size doesn't matter for now
        return networkReply->readAll().mid(GrpcMessageSizeHeaderSize);
    }

    QGrpcHttp2ChannelPrivate(const QUrl &_url,
                             std::unique_ptr<QAbstractGrpcCredentials> _credentials)
        : url(_url), credentials(std::move(_credentials))
    {
#if QT_CONFIG(ssl)
        if (url.scheme() == "https"_L1) {
            // HTTPS connection requested but not ssl configuration provided.
            Q_ASSERT(credentials->channelCredentials().contains(SslConfigCredential));
            sslConfig = credentials->channelCredentials()
                                .value(SslConfigCredential)
                                .value<QSslConfiguration>();
        } else if (url.scheme().isEmpty()) {
            url.setScheme("http"_L1);
        }
#else
        url.setScheme("http"_L1);
#endif
    }

    static int getExpectedDataSize(QByteArrayView container)
    {
        return qFromBigEndian(*reinterpret_cast<const quint32 *>(container.data() + 1))
                + GrpcMessageSizeHeaderSize;
    }
};

/*!
    QGrpcHttp2Channel constructs QGrpcHttp2Channel with \a url used to
    establish channel connections and \a credentials.
*/
QGrpcHttp2Channel::QGrpcHttp2Channel(const QUrl &url,
                                     std::unique_ptr<QAbstractGrpcCredentials> credentials)
    : QAbstractGrpcChannel(),
      dPtr(std::make_unique<QGrpcHttp2ChannelPrivate>(url, std::move(credentials)))
{
}

/*!
    Destroys the QGrpcHttp2Channel object.
*/
QGrpcHttp2Channel::~QGrpcHttp2Channel() = default;

/*!
    Synchronously calls the RPC method and writes the result to the output parameter \a ret.

    The RPC method name is concatenated from the \a method and \a service parameters
    with the given \a args.
*/
QGrpcStatus QGrpcHttp2Channel::call(QLatin1StringView method, QLatin1StringView service,
                                    QByteArrayView args, QByteArray &ret)
{
    QEventLoop loop;

    QNetworkReply *networkReply = dPtr->post(method, service, args);
    QObject::connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    // If reply was finished in same stack it doesn't make sense to start event loop
    if (!networkReply->isFinished())
        loop.exec();

    QGrpcStatus::StatusCode grpcStatus = QGrpcStatus::StatusCode::Unknown;
    ret = dPtr->processReply(networkReply, grpcStatus);

    networkReply->deleteLater();
    qGrpcDebug() << __func__ << "RECV:" << ret.toHex() << "grpcStatus" << grpcStatus;
    return { grpcStatus, QString::fromUtf8(networkReply->rawHeader(GrpcStatusMessageHeader)) };
}

/*!
    Asynchronously calls the RPC method.

    The RPC method name is concatenated from the \a method and \a service parameters
    with the given \a args.
    The method can emit QGrpcCallReply::finished() and QGrpcCallReply::errorOccurred()
    signals on a QGrpcCallReply returned object.
*/
std::shared_ptr<QGrpcCallReply> QGrpcHttp2Channel::call(QAbstractGrpcClient *client,
                                                        QLatin1StringView method,
                                                        QLatin1StringView service,
                                                        QByteArrayView args)
{
    std::shared_ptr<QGrpcCallReply> reply(new QGrpcCallReply(client),
                                          [](QGrpcCallReply *reply) { reply->deleteLater(); });

    QNetworkReply *networkReply = dPtr->post(method, service, args);

    auto connection = std::make_shared<QMetaObject::Connection>();
    auto abortConnection = std::make_shared<QMetaObject::Connection>();

    *connection = QObject::connect(
            networkReply, &QNetworkReply::finished, reply.get(),
            [reply, networkReply, connection, abortConnection] {
                QGrpcStatus::StatusCode grpcStatus = QGrpcStatus::StatusCode::Unknown;
                QByteArray data = QGrpcHttp2ChannelPrivate::processReply(networkReply, grpcStatus);
                QObject::disconnect(*connection);
                QObject::disconnect(*abortConnection);

                qGrpcDebug() << "RECV:" << data;
                if (QGrpcStatus::StatusCode::Ok == grpcStatus) {
                    reply->setData(data);
                    emit reply->finished();
                } else {
                    reply->setData({});
                    emit reply->errorOccurred({ grpcStatus,
                                                QLatin1StringView(networkReply->rawHeader(
                                                        GrpcStatusMessageHeader)) });
                }
                networkReply->deleteLater();
            });

    *abortConnection = QObject::connect(reply.get(), &QGrpcCallReply::errorOccurred, networkReply,
                                        [networkReply, connection,
                                         abortConnection](const QGrpcStatus &status) {
                                            if (status.code() == QGrpcStatus::Aborted) {
                                                QObject::disconnect(*connection);
                                                QObject::disconnect(*abortConnection);

                                                networkReply->deleteLater();
                                            }
                                        });
    return reply;
}

/*!
    Starts a stream on \a grpcStream using QGrpcStream::method() and
    \a service to get the name of the RPC method.

    Calls QGrpcStream::handler() when the stream receives data from the server.
    The method may emit QGrpcStream::errorOccurred() when the stream has terminated with an error.
*/
void QGrpcHttp2Channel::startStream(QGrpcStream *grpcStream, QLatin1StringView service)
{
    Q_ASSERT(grpcStream != nullptr);
    QNetworkReply *networkReply = dPtr->post(grpcStream->method(), service, grpcStream->arg(),
                                             true);

    auto finishConnection = std::make_shared<QMetaObject::Connection>();
    auto abortConnection = std::make_shared<QMetaObject::Connection>();
    auto readConnection = std::make_shared<QMetaObject::Connection>();

    *readConnection = QObject::connect(
            networkReply, &QNetworkReply::readyRead, grpcStream, [networkReply, grpcStream, this] {
                auto replyIt = dPtr->activeStreamReplies.find(networkReply);

                const QByteArray data = networkReply->readAll();
                qGrpcDebug() << "RECV data size:" << data.size();

                if (replyIt == dPtr->activeStreamReplies.end()) {
                    qGrpcDebug() << data.toHex();
                    int expectedDataSize = QGrpcHttp2ChannelPrivate::getExpectedDataSize(data);
                    qGrpcDebug() << "First chunk received:" << data.size()
                                 << "expectedDataSize:" << expectedDataSize;

                    if (expectedDataSize == 0) {
                        grpcStream->handler(QByteArray());
                        return;
                    }

                    QGrpcHttp2ChannelPrivate::ExpectedData dataContainer{ expectedDataSize,
                                                                          QByteArray{} };
                    replyIt = dPtr->activeStreamReplies.insert({ networkReply, dataContainer })
                                      .first;
                }

                QGrpcHttp2ChannelPrivate::ExpectedData &dataContainer = replyIt->second;
                dataContainer.container.append(data);

                qGrpcDebug() << "Processeded chunk:" << data.size()
                             << "dataContainer:" << dataContainer.container.size()
                             << "capacity:" << dataContainer.expectedSize;
                while (dataContainer.container.size() >= dataContainer.expectedSize
                       && !networkReply->isFinished()) {
                    qGrpcDebug() << "Full data received:" << data.size()
                                 << "dataContainer:" << dataContainer.container.size()
                                 << "capacity:" << dataContainer.expectedSize;
                    grpcStream->handler(dataContainer.container.mid(
                            GrpcMessageSizeHeaderSize,
                            dataContainer.expectedSize - GrpcMessageSizeHeaderSize));
                    dataContainer.container.remove(0, dataContainer.expectedSize);
                    if (dataContainer.container.size() > GrpcMessageSizeHeaderSize) {
                        dataContainer.expectedSize = QGrpcHttp2ChannelPrivate::getExpectedDataSize(
                                dataContainer.container);
                    } else if (dataContainer.container.size() > 0) {
                        qGrpcWarning("Invalid container size received, size header is less than 5 "
                                     "bytes");
                    }
                }

                if (dataContainer.container.size() < GrpcMessageSizeHeaderSize
                    || networkReply->isFinished()) {
                    dPtr->activeStreamReplies.erase(replyIt);
                }
            });

    *finishConnection = QObject::connect(
            networkReply, &QNetworkReply::finished, grpcStream,
            [grpcStream, service, networkReply, abortConnection, readConnection, finishConnection,
             this]() {
                const QString errorString = networkReply->errorString();
                const QNetworkReply::NetworkError networkError = networkReply->error();
                QObject::disconnect(*readConnection);
                QObject::disconnect(*abortConnection);

                dPtr->activeStreamReplies.erase(networkReply);
                QGrpcHttp2ChannelPrivate::abortNetworkReply(networkReply);
                networkReply->deleteLater();
                qGrpcWarning() << grpcStream->method() << "call" << service
                               << "stream finished:" << errorString;
                switch (networkError) {
                case QNetworkReply::NoError: {
                    // Reply is closed without network error, but may contain an unhandled data
                    // TODO: processReply returns the data, that might need the processing. It's
                    // should be taken into account in new HTTP/2 channel implementation.
                    QGrpcStatus::StatusCode grpcStatus;
                    QGrpcHttp2ChannelPrivate::processReply(networkReply, grpcStatus);
                    if (grpcStatus != QGrpcStatus::StatusCode::Ok) {
                        emit grpcStream->errorOccurred(
                                QGrpcStatus{ grpcStatus,
                                             QLatin1StringView(networkReply->rawHeader(
                                                     GrpcStatusMessageHeader)) });
                    }
                    break;
                }
                default:
                    emit grpcStream->errorOccurred(
                            QGrpcStatus{ StatusCodeMap.at(networkError),
                                         "%1 call %2 stream failed: %3"_L1.arg(
                                                 service, grpcStream->method(), errorString) });
                    break;
                }
                emit grpcStream->finished();
            });

    *abortConnection = QObject::connect(
            grpcStream, &QGrpcStream::finished, networkReply,
            [networkReply, finishConnection, abortConnection, readConnection] {
                QObject::disconnect(*finishConnection);
                QObject::disconnect(*readConnection);
                QObject::disconnect(*abortConnection);

                QGrpcHttp2ChannelPrivate::abortNetworkReply(networkReply);
                networkReply->deleteLater();
            });
}

/*!
    Returns the newly created QProtobufSerializer shared pointer.
*/
std::shared_ptr<QAbstractProtobufSerializer> QGrpcHttp2Channel::serializer() const
{
    // TODO: make selection based on credentials or channel settings
    return std::make_shared<QProtobufSerializer>();
}

QT_END_NAMESPACE
