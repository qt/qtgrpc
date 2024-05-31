// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCHANNELOPTIONS_H
#define QGRPCHANNELOPTIONS_H

#include <QtGrpc/qgrpcdefs.h>
#include <QtGrpc/qtgrpcglobal.h>
#if QT_CONFIG(ssl)
#  include <QtNetwork/qsslconfiguration.h>
#endif
#include <QtCore/qurl.h>

#include <memory>
#include <optional>

QT_BEGIN_NAMESPACE

class QSslConfiguration;
class QGrpcSerializationFormat;
class QGrpcChannelOptionsPrivate;

class QGrpcChannelOptions final
{
public:
    Q_GRPC_EXPORT QGrpcChannelOptions();
    Q_GRPC_EXPORT ~QGrpcChannelOptions();

    Q_GRPC_EXPORT QGrpcChannelOptions(const QGrpcChannelOptions &other);
    Q_GRPC_EXPORT QGrpcChannelOptions &operator=(const QGrpcChannelOptions &other);
    Q_GRPC_EXPORT QGrpcChannelOptions(QGrpcChannelOptions &&other) noexcept;
    Q_GRPC_EXPORT QGrpcChannelOptions &operator=(QGrpcChannelOptions &&other) noexcept;

    Q_GRPC_EXPORT QGrpcChannelOptions &setDeadline(QGrpcDuration deadline);
    Q_GRPC_EXPORT QGrpcChannelOptions &setMetadata(const QGrpcMetadata &metadata);
    Q_GRPC_EXPORT QGrpcChannelOptions &setMetadata(QGrpcMetadata &&metadata);
    Q_GRPC_EXPORT QGrpcChannelOptions &
    setSerializationFormat(const QGrpcSerializationFormat &format);

    [[nodiscard]] Q_GRPC_EXPORT std::optional<QGrpcDuration> deadline() const noexcept;
    [[nodiscard]] Q_GRPC_EXPORT const QGrpcMetadata &metadata() const & noexcept;
    [[nodiscard]] Q_GRPC_EXPORT QGrpcMetadata metadata() && noexcept;
    [[nodiscard]] Q_GRPC_EXPORT const QGrpcSerializationFormat &
    serializationFormat() const & noexcept;
    [[nodiscard]] Q_GRPC_EXPORT QGrpcSerializationFormat serializationFormat() && noexcept;

#if QT_CONFIG(ssl)
    Q_GRPC_EXPORT QGrpcChannelOptions &
    withSslConfiguration(const QSslConfiguration &sslConfiguration);
    [[nodiscard]] Q_GRPC_EXPORT std::optional<QSslConfiguration> sslConfiguration() const noexcept;
#endif

private:
    std::unique_ptr<QGrpcChannelOptionsPrivate> dPtr;

#ifndef QT_NO_DEBUG_STREAM
    friend Q_GRPC_EXPORT QDebug operator<<(QDebug debug, const QGrpcChannelOptions &chOpts);
#endif
};
QT_END_NAMESPACE

#endif // QGRPCHANNELOPTIONS_H
