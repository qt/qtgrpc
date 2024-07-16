// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCALLOPTIONS_H
#define QGRPCALLOPTIONS_H

#include <QtGrpc/qgrpcdefs.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <QtCore/qshareddata.h>
#include <QtCore/qstringfwd.h>
#include <QtCore/qtclasshelpermacros.h>

#include <optional>

QT_BEGIN_NAMESPACE

class QDebug;
class QVariant;

class QGrpcCallOptionsPrivate;
QT_DECLARE_QESDP_SPECIALIZATION_DTOR_WITH_EXPORT(QGrpcCallOptionsPrivate, Q_GRPC_EXPORT)

class QGrpcCallOptions final
{
public:
    Q_GRPC_EXPORT QGrpcCallOptions();
    Q_GRPC_EXPORT ~QGrpcCallOptions();

    Q_GRPC_EXPORT QGrpcCallOptions(const QGrpcCallOptions &other);
    Q_GRPC_EXPORT QGrpcCallOptions &operator=(const QGrpcCallOptions &other);

    QGrpcCallOptions(QGrpcCallOptions &&other) noexcept = default;
    QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QGrpcCallOptions)

    Q_GRPC_EXPORT Q_IMPLICIT operator QVariant() const;

    void swap(QGrpcCallOptions &other) noexcept { d_ptr.swap(other.d_ptr); }

    Q_GRPC_EXPORT QGrpcCallOptions &setDeadline(QGrpcDuration deadline);
    Q_GRPC_EXPORT QGrpcCallOptions &setMetadata(const QGrpcMetadata &metadata);
    Q_GRPC_EXPORT QGrpcCallOptions &setMetadata(QGrpcMetadata &&metadata);

    [[nodiscard]] Q_GRPC_EXPORT std::optional<QGrpcDuration> deadline() const noexcept;
    [[nodiscard]] Q_GRPC_EXPORT const QGrpcMetadata &metadata() const & noexcept;
    [[nodiscard]] Q_GRPC_EXPORT QGrpcMetadata metadata() &&;

private:
    QExplicitlySharedDataPointer<QGrpcCallOptionsPrivate> d_ptr;

#ifndef QT_NO_DEBUG_STREAM
    friend Q_GRPC_EXPORT QDebug operator<<(QDebug debug, const QGrpcCallOptions &callOpts);
#endif

    Q_DECLARE_PRIVATE(QGrpcCallOptions)
};

Q_DECLARE_SHARED(QGrpcCallOptions)

QT_END_NAMESPACE

#endif // QGRPCALLOPTIONS_H
