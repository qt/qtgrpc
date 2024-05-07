// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCALLOPTIONS_H
#define QGRPCALLOPTIONS_H

#include <QtGrpc/qgrpcdefs.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <memory>
#include <optional>

QT_BEGIN_NAMESPACE

class QGrpcCallOptionsPrivate;

class QGrpcCallOptions final
{
public:
    Q_GRPC_EXPORT QGrpcCallOptions();
    Q_GRPC_EXPORT ~QGrpcCallOptions();

    Q_GRPC_EXPORT QGrpcCallOptions(const QGrpcCallOptions &other);
    Q_GRPC_EXPORT QGrpcCallOptions &operator=(const QGrpcCallOptions &other);
    Q_GRPC_EXPORT QGrpcCallOptions(QGrpcCallOptions &&other) noexcept;
    Q_GRPC_EXPORT QGrpcCallOptions &operator=(QGrpcCallOptions &&other) noexcept;

    Q_GRPC_EXPORT QGrpcCallOptions &withDeadline(QGrpcDuration deadline);
    Q_GRPC_EXPORT QGrpcCallOptions &withMetadata(const QGrpcMetadata &metadata);
    Q_GRPC_EXPORT QGrpcCallOptions &withMetadata(QGrpcMetadata &&metadata) noexcept;

    [[nodiscard]] Q_GRPC_EXPORT std::optional<QGrpcDuration> deadline() const noexcept;
    [[nodiscard]] Q_GRPC_EXPORT const QGrpcMetadata &metadata() const noexcept;

private:
    std::unique_ptr<QGrpcCallOptionsPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QGRPCALLOPTIONS_H
