// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCHANNELOPTIONS_H
#define QGRPCHANNELOPTIONS_H

#include <QtGrpc/qgrpcdefs.h>
#include <QtGrpc/qtgrpcglobal.h>

#if QT_CONFIG(ssl)
#  include <QtNetwork/qsslconfiguration.h>
#endif

#include <QtCore/qshareddata.h>
#include <QtCore/qstringfwd.h>
#include <QtCore/qtclasshelpermacros.h>
#include <QtCore/qurl.h>

#include <optional>

QT_BEGIN_NAMESPACE

class QDebug;
class QVariant;
class QGrpcSerializationFormat;

class QGrpcChannelOptionsPrivate;
QT_DECLARE_QESDP_SPECIALIZATION_DTOR_WITH_EXPORT(QGrpcChannelOptionsPrivate, Q_GRPC_EXPORT)

class QGrpcChannelOptions final
{
public:
    Q_GRPC_EXPORT QGrpcChannelOptions();
    Q_GRPC_EXPORT ~QGrpcChannelOptions();

    Q_GRPC_EXPORT QGrpcChannelOptions(const QGrpcChannelOptions &other);
    Q_GRPC_EXPORT QGrpcChannelOptions &operator=(const QGrpcChannelOptions &other);

    QGrpcChannelOptions(QGrpcChannelOptions &&other) noexcept = default;
    QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QGrpcChannelOptions)

    Q_GRPC_EXPORT Q_IMPLICIT operator QVariant() const;

    void swap(QGrpcChannelOptions &other) noexcept { d_ptr.swap(other.d_ptr); }

    Q_GRPC_EXPORT QGrpcChannelOptions &setDeadline(QGrpcDuration deadline);
    Q_GRPC_EXPORT QGrpcChannelOptions &setMetadata(const QGrpcMetadata &metadata);
    Q_GRPC_EXPORT QGrpcChannelOptions &setMetadata(QGrpcMetadata &&metadata);
    Q_GRPC_EXPORT QGrpcChannelOptions &
    setSerializationFormat(const QGrpcSerializationFormat &format);

    [[nodiscard]] Q_GRPC_EXPORT std::optional<QGrpcDuration> deadline() const noexcept;
    [[nodiscard]] Q_GRPC_EXPORT const QGrpcMetadata &metadata() const & noexcept;
    [[nodiscard]] Q_GRPC_EXPORT QGrpcMetadata metadata() &&;
    [[nodiscard]] Q_GRPC_EXPORT QGrpcSerializationFormat serializationFormat() const;

#if QT_CONFIG(ssl)
    Q_GRPC_EXPORT QGrpcChannelOptions &
    setSslConfiguration(const QSslConfiguration &sslConfiguration);
    [[nodiscard]] Q_GRPC_EXPORT std::optional<QSslConfiguration> sslConfiguration() const noexcept;
#endif

private:
    QExplicitlySharedDataPointer<QGrpcChannelOptionsPrivate> d_ptr;

#ifndef QT_NO_DEBUG_STREAM
    friend Q_GRPC_EXPORT QDebug operator<<(QDebug debug, const QGrpcChannelOptions &chOpts);
#endif

    Q_DECLARE_PRIVATE(QGrpcChannelOptions)
};

Q_DECLARE_SHARED(QGrpcChannelOptions)

QT_END_NAMESPACE

#endif // QGRPCHANNELOPTIONS_H
