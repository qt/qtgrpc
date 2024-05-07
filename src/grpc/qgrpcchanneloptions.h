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

class Q_GRPC_EXPORT QGrpcChannelOptions final
{
public:
    QGrpcChannelOptions();
    ~QGrpcChannelOptions();

    QGrpcChannelOptions(const QGrpcChannelOptions &other);
    QGrpcChannelOptions &operator=(const QGrpcChannelOptions &other);
    QGrpcChannelOptions(QGrpcChannelOptions &&other) noexcept;
    QGrpcChannelOptions &operator=(QGrpcChannelOptions &&other) noexcept;

    QGrpcChannelOptions &setDeadline(QGrpcDuration deadline);
    QGrpcChannelOptions &setMetadata(const QGrpcMetadata &metadata);
    QGrpcChannelOptions &setMetadata(QGrpcMetadata &&metadata);
    QGrpcChannelOptions &setSerializationFormat(const QGrpcSerializationFormat &format);

    [[nodiscard]] std::optional<QGrpcDuration> deadline() const noexcept;
    [[nodiscard]] const QGrpcMetadata &metadata() const & noexcept;
    [[nodiscard]] QGrpcMetadata metadata() && noexcept;
    [[nodiscard]] const QGrpcSerializationFormat &serializationFormat() const & noexcept;
    [[nodiscard]] QGrpcSerializationFormat serializationFormat() && noexcept;

#if QT_CONFIG(ssl)
    QGrpcChannelOptions &withSslConfiguration(const QSslConfiguration &sslConfiguration);
    [[nodiscard]] std::optional<QSslConfiguration> sslConfiguration() const noexcept;
#endif

private:
    std::unique_ptr<QGrpcChannelOptionsPrivate> dPtr;

#ifndef QT_NO_DEBUG_STREAM
    friend Q_GRPC_EXPORT QDebug operator<<(QDebug debug, const QGrpcChannelOptions &chOpts);
#endif
};
QT_END_NAMESPACE

#endif // QGRPCHANNELOPTIONS_H
