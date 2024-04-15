// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCHANNELOPTIONS_H
#define QGRPCHANNELOPTIONS_H

#include <QtCore/qurl.h>
#include <QtGrpc/qgrpcdefs.h>
#include <QtGrpc/qtgrpcglobal.h>

#if QT_CONFIG(ssl)
#  include <QtNetwork/qsslconfiguration.h>
#endif

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
    QGrpcChannelOptions(QGrpcChannelOptions &&other) noexcept;
    QGrpcChannelOptions &operator=(QGrpcChannelOptions &&other) noexcept;

    QGrpcChannelOptions &withHost(const QUrl &host);
    QGrpcChannelOptions &withDeadline(QGrpcDuration deadline);
    QGrpcChannelOptions &withMetadata(const QGrpcMetadata &metadata);
    QGrpcChannelOptions &withSerializationFormat(const QGrpcSerializationFormat &format);

    [[nodiscard]] QUrl host() const noexcept;
    [[nodiscard]] std::optional<QGrpcDuration> deadline() const noexcept;
    [[nodiscard]] const QGrpcMetadata &metadata() const noexcept;
    [[nodiscard]] const QGrpcSerializationFormat &serializationFormat() const noexcept;

#if QT_CONFIG(ssl)
    QGrpcChannelOptions &withSslConfiguration(const QSslConfiguration &sslConfiguration);
    [[nodiscard]] std::optional<QSslConfiguration> sslConfiguration() const noexcept;
#endif

private:
    std::unique_ptr<QGrpcChannelOptionsPrivate> dPtr;
};
QT_END_NAMESPACE

#endif // QGRPCHANNELOPTIONS_H
