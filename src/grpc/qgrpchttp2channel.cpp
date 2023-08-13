// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>, Viktor Kopp <vifactor@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpchttp2channel.h"

#include "qabstractgrpcclient.h"
#include "qgrpcchanneloperation.h"

#include <QtCore/QMetaObject>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtProtobuf/qprotobufserializer.h>
#include <qtgrpcglobal_p.h>

#include <unordered_map>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

/*!
    \class QGrpcHttp2Channel
    \inmodule QtGrpc

    \brief The QGrpcHttp2Channel class is an HTTP/2 implementation of
    QAbstractGrpcChannel, based on \l QNetworkAccessManager.

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

static void addMetadataToRequest(QNetworkRequest *request, const QGrpcMetadata &channelMetadata,
                                 const QGrpcMetadata &callMetadata)
{
    auto iterateMetadata = [&request](const auto &metadata) {
        for (const auto &[key, value] : std::as_const(metadata)) {
            request->setRawHeader(key, value);
        }
    };

    iterateMetadata(channelMetadata);
    iterateMetadata(callMetadata);
}

static QGrpcMetadata collectMetadata(QNetworkReply *networkReply)
{
    return QGrpcMetadata(networkReply->rawHeaderPairs().begin(),
                         networkReply->rawHeaderPairs().end());
}

static std::optional<std::chrono::milliseconds> deadlineForCall(
        const QGrpcChannelOptions &channelOptions, const QGrpcCallOptions &callOptions)
{
    if (callOptions.deadline())
        return *callOptions.deadline();
    if (channelOptions.deadline())
        return *channelOptions.deadline();
    return std::nullopt;
}

struct QGrpcHttp2ChannelPrivate
{
    struct ExpectedData
    {
        qsizetype expectedSize;
        QByteArray container;
    };

    QNetworkAccessManager nm;
    QGrpcChannelOptions channelOptions;
#if QT_CONFIG(ssl)
    QSslConfiguration sslConfig;
#endif
    std::unordered_map<QNetworkReply *, ExpectedData> activeStreamReplies;
    QObject lambdaContext;

    QNetworkReply *post(QLatin1StringView method, QLatin1StringView service, QByteArrayView arg,
                        const QGrpcCallOptions &callOptions)
    {
        QUrl callUrl = channelOptions.host();
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

        addMetadataToRequest(&request, channelOptions.metadata(), callOptions.metadata());

        request.setAttribute(QNetworkRequest::Http2DirectAttribute, true);

        QByteArray msg(GrpcMessageSizeHeaderSize, '\0');
        // Args must be 4-byte unsigned int to fit into 4-byte big endian
        qToBigEndian(static_cast<quint32>(arg.size()), msg.data() + 1);
        msg += arg;
        qGrpcDebug() << "SEND msg with size:" << msg.size();

        QNetworkReply *networkReply = nm.post(request, msg);
#if QT_CONFIG(ssl)
        QObject::connect(networkReply, &QNetworkReply::sslErrors, networkReply,
                         [networkReply](const QList<QSslError> &errors) {
                             qGrpcCritical() << errors;
                             // TODO: filter out noncritical SSL handshake errors
                             // FIXME: error due to ssl failure is not transferred to the client:
                             // last error will be Operation cancelled
                             QGrpcHttp2ChannelPrivate::abortNetworkReply(networkReply);
                         });
#endif
        if (auto deadline = deadlineForCall(channelOptions, callOptions)) {
            QTimer::singleShot(*deadline, networkReply, [networkReply] {
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

    QGrpcHttp2ChannelPrivate(const QGrpcChannelOptions &options) : channelOptions(options)
    {
#if QT_CONFIG(ssl)
        if (channelOptions.host().scheme() == "https"_L1) {
            // HTTPS connection requested but not ssl configuration provided.
            Q_ASSERT(channelOptions.sslConfiguration());
            sslConfig = *channelOptions.sslConfiguration();
        } else if (channelOptions.host().scheme().isEmpty()) {
            auto tmpHost = channelOptions.host();
            tmpHost.setScheme("http"_L1);
            channelOptions.withHost(tmpHost);
        }
#else
        auto tmpHost = channelOptions.host();
        tmpHost.setScheme("http"_L1);
        channelOptions.withHost(tmpHost);
#endif
    }

    static int getExpectedDataSize(QByteArrayView container)
    {
        return qFromBigEndian(*reinterpret_cast<const quint32 *>(container.data() + 1))
                + GrpcMessageSizeHeaderSize;
    }
};

/*!
    QGrpcHttp2Channel constructs QGrpcHttp2Channel with \a options.
*/
QGrpcHttp2Channel::QGrpcHttp2Channel(const QGrpcChannelOptions &options)
    : QAbstractGrpcChannel(), dPtr(std::make_unique<QGrpcHttp2ChannelPrivate>(options))
{
}

/*!
    Destroys the QGrpcHttp2Channel object.
*/
QGrpcHttp2Channel::~QGrpcHttp2Channel() = default;

/*!
    \internal
    Implementation of unary gRPC call based on \l QNetworkAccessManager.
*/
void QGrpcHttp2Channel::call(std::shared_ptr<QGrpcChannelOperation> channelOperation)
{
    QNetworkReply *networkReply =
            dPtr->post(channelOperation->method(), channelOperation->service(),
                       channelOperation->arg(), channelOperation->options());

    auto connection = std::make_shared<QMetaObject::Connection>();
    auto abortConnection = std::make_shared<QMetaObject::Connection>();

    *connection = QObject::connect(
            networkReply, &QNetworkReply::finished, channelOperation.get(),
            [channelOperation, networkReply, connection, abortConnection] {
                QObject::disconnect(*connection);
                QObject::disconnect(*abortConnection);

                QGrpcStatus::StatusCode code = QGrpcStatus::StatusCode::Unknown;
                QByteArray data = QGrpcHttp2ChannelPrivate::processReply(networkReply, code);
                channelOperation->setServerMetadata(collectMetadata(networkReply));
                qGrpcDebug() << "RECV:" << data;
                if (QGrpcStatus::StatusCode::Ok == code) {
                    emit channelOperation->dataReady(data);
                    emit channelOperation->finished();
                } else {
                    emit channelOperation->errorOccurred(
                            { code,
                              QLatin1StringView(
                                      networkReply->rawHeader(GrpcStatusMessageHeader)) });
                }
                networkReply->deleteLater();
            });

    *abortConnection =
            QObject::connect(channelOperation.get(), &QGrpcChannelOperation::cancelled,
                             networkReply, [networkReply, connection, abortConnection]() {
                                 QObject::disconnect(*connection);
                                 QObject::disconnect(*abortConnection);

                                 networkReply->deleteLater();
                             });
}

/*!
    \internal
    Implementation of server-side gRPC stream based on \l QNetworkAccessManager.
*/
void QGrpcHttp2Channel::startServerStream(std::shared_ptr<QGrpcChannelOperation> channelOperation)
{
    QNetworkReply *networkReply =
            dPtr->post(channelOperation->method(), channelOperation->service(),
                       channelOperation->arg(), channelOperation->options());

    auto finishConnection = std::make_shared<QMetaObject::Connection>();
    auto abortConnection = std::make_shared<QMetaObject::Connection>();
    auto readConnection = std::make_shared<QMetaObject::Connection>();

    *readConnection = QObject::connect(
            networkReply, &QNetworkReply::readyRead, channelOperation.get(),
            [networkReply, channelOperation, this] {
                auto replyIt = dPtr->activeStreamReplies.find(networkReply);

                const QByteArray data = networkReply->readAll();
                qGrpcDebug() << "RECV data size:" << data.size();

                if (replyIt == dPtr->activeStreamReplies.end()) {
                    qGrpcDebug() << data.toHex();
                    int expectedDataSize = QGrpcHttp2ChannelPrivate::getExpectedDataSize(data);
                    qGrpcDebug() << "First chunk received:" << data.size()
                                 << "expectedDataSize:" << expectedDataSize;

                    if (expectedDataSize == 0) {
                        channelOperation->dataReady({});
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
                    channelOperation->setServerMetadata(collectMetadata(networkReply));
                    channelOperation->dataReady(dataContainer.container.mid(
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

    std::weak_ptr<QGrpcChannelOperation> weakChannelOperation(channelOperation);
    *finishConnection = QObject::connect(
            networkReply, &QNetworkReply::finished, channelOperation.get(),
            [weakChannelOperation, networkReply, abortConnection, readConnection, finishConnection,
             this]() {
                QObject::disconnect(*readConnection);
                QObject::disconnect(*abortConnection);

                dPtr->activeStreamReplies.erase(networkReply);
                QGrpcHttp2ChannelPrivate::abortNetworkReply(networkReply);
                networkReply->deleteLater();

                auto channelOperation = weakChannelOperation.lock();
                if (!channelOperation) {
                    qGrpcWarning() << "Could not lock gRPC stream pointer.";
                    return;
                }

                const QString errorString = networkReply->errorString();
                qGrpcWarning() << channelOperation->method() << "call"
                               << channelOperation->service() << "stream finished:" << errorString;
                channelOperation->setServerMetadata(collectMetadata(networkReply));

                QGrpcStatus::StatusCode code;
                QGrpcHttp2ChannelPrivate::processReply(networkReply, code);
                QGrpcStatus status{
                    code,
                    networkReply->error() != QNetworkReply::NoError
                            ? QLatin1StringView(networkReply->rawHeader(GrpcStatusMessageHeader))
                            : "%1 call %2 stream failed: %3"_L1.arg(channelOperation->service(),
                                                                    channelOperation->method(),
                                                                    errorString)
                };

                if (status != QGrpcStatus::Ok)
                    emit channelOperation->errorOccurred(status);
                emit channelOperation->finished();
            });

    *abortConnection = QObject::connect(
            channelOperation.get(), &QGrpcChannelOperation::cancelled, networkReply,
            [networkReply, finishConnection, abortConnection, readConnection, channelOperation] {
                QObject::disconnect(*finishConnection);
                QObject::disconnect(*readConnection);
                QObject::disconnect(*abortConnection);

                QGrpcHttp2ChannelPrivate::abortNetworkReply(networkReply);
                networkReply->deleteLater();
            });
}

/*!
    \internal
    Implementation of client-side gRPC stream based on \l QNetworkAccessManager.
*/
void QGrpcHttp2Channel::startClientStream(std::shared_ptr<QGrpcChannelOperation> channelOperation)
{
    QTimer::singleShot(0, channelOperation.get(), [channelOperation] {
        emit channelOperation->errorOccurred(
                { QGrpcStatus::Unknown,
                  "Client-side streaming support is not implemented in QGrpcChannel"_L1 });
    });
}

/*!
    \internal
    Implementation of bidirectional gRPC stream based on \l QNetworkAccessManager.
*/
void QGrpcHttp2Channel::startBidirStream(std::shared_ptr<QGrpcChannelOperation> channelOperation)
{
    QTimer::singleShot(0, channelOperation.get(), [channelOperation] {
        emit channelOperation->errorOccurred(
                { QGrpcStatus::Unknown,
                  "Bidirectional streaming support is not implemented in QGrpcChannel"_L1 });
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
