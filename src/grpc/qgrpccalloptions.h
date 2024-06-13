// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCALLOPTIONS_H
#define QGRPCALLOPTIONS_H

#include <QtGrpc/qgrpcdefs.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <QtCore/qtclasshelpermacros.h>

#include <memory>
#include <optional>

QT_BEGIN_NAMESPACE

class QDebug;
class QGrpcCallOptionsPrivate;

class QGrpcCallOptions final
{
public:
    Q_GRPC_EXPORT QGrpcCallOptions();
    Q_GRPC_EXPORT ~QGrpcCallOptions();

    Q_GRPC_EXPORT QGrpcCallOptions(const QGrpcCallOptions &other);
    Q_GRPC_EXPORT QGrpcCallOptions &operator=(const QGrpcCallOptions &other);

    QGrpcCallOptions(QGrpcCallOptions &&other) noexcept = default;
    QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QGrpcCallOptions)

    void swap(QGrpcCallOptions &other) noexcept { dPtr.swap(other.dPtr); }

    Q_GRPC_EXPORT QGrpcCallOptions &setDeadline(QGrpcDuration deadline);
    Q_GRPC_EXPORT QGrpcCallOptions &setMetadata(const QGrpcMetadata &metadata);
    Q_GRPC_EXPORT QGrpcCallOptions &setMetadata(QGrpcMetadata &&metadata) noexcept;

    [[nodiscard]] Q_GRPC_EXPORT std::optional<QGrpcDuration> deadline() const noexcept;
    [[nodiscard]] Q_GRPC_EXPORT const QGrpcMetadata &metadata() const & noexcept;
    [[nodiscard]] Q_GRPC_EXPORT QGrpcMetadata metadata() && noexcept;

private:
    std::unique_ptr<QGrpcCallOptionsPrivate, void (*)(QGrpcCallOptionsPrivate *)> dPtr;

#ifndef QT_NO_DEBUG_STREAM
    friend Q_GRPC_EXPORT QDebug operator<<(QDebug debug, const QGrpcCallOptions &callOpts);
#endif
};

QT_END_NAMESPACE

#endif // QGRPCALLOPTIONS_H
