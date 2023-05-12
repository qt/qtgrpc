// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCHANNELOPTIONS_H
#define QGRPCHANNELOPTIONS_H

#include <QtCore/QUrl>
#include <QtGrpc/QGrpcChannelCredentials>
#include <QtGrpc/qgrpcmetadata.h>
#include <QtGrpc/qtgrpcglobal.h>

#if QT_CONFIG(ssl)
#  include <QtNetwork/QSslConfiguration>
#endif

#include <chrono>
#include <memory>
#include <optional>

QT_BEGIN_NAMESPACE

class QSslConfiguration;
struct QGrpcChannelOptionsPrivate;

class Q_GRPC_EXPORT QGrpcChannelOptions final
{
public:
    explicit QGrpcChannelOptions(const QUrl &host);
    ~QGrpcChannelOptions();

    QGrpcChannelOptions(const QGrpcChannelOptions &other);
    QGrpcChannelOptions &operator=(const QGrpcChannelOptions &other);

    QGrpcChannelOptions &withHost(const QUrl &host);
    QGrpcChannelOptions &withDeadline(std::chrono::milliseconds deadline);
    QGrpcChannelOptions &withCredentials(std::shared_ptr<QGrpcChannelCredentials> credentials);
    QGrpcChannelOptions &withCredentialList(const QStringList &credentialList);
    QGrpcChannelOptions &withMetadata(const QGrpcMetadata &metadata);

    QUrl host() const;
    std::optional<std::chrono::milliseconds> deadline() const;
    std::optional<QGrpcCredentialMap> credentials() const;
    std::optional<QStringList> credentialList() const;
    QGrpcMetadata metadata() const;

#if QT_CONFIG(ssl)
    QGrpcChannelOptions &withSslConfiguration(const QSslConfiguration &sslConfiguration);
    std::optional<QSslConfiguration> sslConfiguration() const;
#endif

private:
    std::unique_ptr<QGrpcChannelOptionsPrivate> dPtr;
};
QT_END_NAMESPACE

#endif // QGRPCHANNELOPTIONS_H
