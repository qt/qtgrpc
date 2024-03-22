// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCHANNELOPTIONS_H
#define QGRPCHANNELOPTIONS_H

#include <QtCore/qurl.h>
#include <QtGrpc/qgrpcmetadata.h>
#include <QtGrpc/qtgrpcglobal.h>

#if QT_CONFIG(ssl)
#  include <QtNetwork/qsslconfiguration.h>
#endif

#include <chrono>
#include <memory>
#include <optional>

QT_BEGIN_NAMESPACE

class QSslConfiguration;
class QGrpcSerializationFormat;

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
    QGrpcChannelOptions &withMetadata(const QGrpcMetadata &metadata);
    QGrpcChannelOptions &withSerializationFormat(const QGrpcSerializationFormat &format);

    QUrl host() const;
    std::optional<std::chrono::milliseconds> deadline() const;
    const QGrpcMetadata &metadata() const;
    const QGrpcSerializationFormat &serializationFormat() const;

#if QT_CONFIG(ssl)
    QGrpcChannelOptions &withSslConfiguration(const QSslConfiguration &sslConfiguration);
    std::optional<QSslConfiguration> sslConfiguration() const;
#endif

private:
    std::unique_ptr<QGrpcChannelOptionsPrivate> dPtr;
};
QT_END_NAMESPACE

#endif // QGRPCHANNELOPTIONS_H
