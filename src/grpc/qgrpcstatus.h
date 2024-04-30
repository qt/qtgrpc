// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCSTATUS_H
#define QGRPCSTATUS_H

#include <QtGrpc/qtgrpcglobal.h>

#include <QtCore/qanystringview.h>
#include <QtCore/qcompare.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qobjectdefs.h>
#include <QtCore/qstring.h>
#include <QtCore/qtclasshelpermacros.h>

QT_BEGIN_NAMESPACE

class QDataStream;
class QDebug;
class QVariant;

class QGrpcStatus final
{
    Q_GADGET_EXPORT(Q_GRPC_EXPORT)
    Q_PROPERTY(StatusCode code READ code CONSTANT)
    Q_PROPERTY(QString message READ message CONSTANT)

public:
    enum StatusCode : uint8_t {
        Ok = 0,
        Cancelled = 1,
        Unknown = 2,
        InvalidArgument = 3,
        DeadlineExceeded = 4,
        NotFound = 5,
        AlreadyExists = 6,
        PermissionDenied = 7,
        ResourceExhausted = 8,
        FailedPrecondition = 9,
        Aborted = 10,
        OutOfRange = 11,
        Unimplemented = 12,
        Internal = 13,
        Unavailable = 14,
        DataLoss = 15,
        Unauthenticated = 16,
    };
    Q_ENUM(StatusCode)

    Q_GRPC_EXPORT Q_IMPLICIT QGrpcStatus(StatusCode code = Ok, QAnyStringView message = {});
    Q_GRPC_EXPORT ~QGrpcStatus();
    Q_GRPC_EXPORT QGrpcStatus(const QGrpcStatus &other);
    Q_GRPC_EXPORT QGrpcStatus &operator=(const QGrpcStatus &other);
    QGrpcStatus(QGrpcStatus &&other) noexcept = default;
    QGrpcStatus &operator=(QGrpcStatus &&other) noexcept = default;

    Q_GRPC_EXPORT Q_IMPLICIT operator QVariant() const;

    void swap(QGrpcStatus &other) noexcept
    {
        std::swap(m_code, other.m_code);
        m_message.swap(other.m_message);
    }

    [[nodiscard]] StatusCode code() const noexcept { return m_code; }
    [[nodiscard]] QString message() const noexcept { return m_message; }

private:
    QGrpcStatus::StatusCode m_code;
    QString m_message;

    friend bool comparesEqual(const QGrpcStatus &lhs, StatusCode rhs) noexcept
    {
        return lhs.code() == rhs;
    }
    friend bool comparesEqual(const QGrpcStatus &lhs, const QGrpcStatus &rhs) noexcept
    {
        return lhs.code() == rhs.code();
    }
    Q_DECLARE_EQUALITY_COMPARABLE(QGrpcStatus, StatusCode)
    Q_DECLARE_EQUALITY_COMPARABLE(QGrpcStatus)

    friend size_t qHash(const QGrpcStatus &key, size_t seed = 0) noexcept
    {
        return qHash(key.code(), seed);
    }

#ifndef QT_NO_DEBUG_STREAM
    friend Q_GRPC_EXPORT QDebug operator<<(QDebug debug, const QGrpcStatus &status);
#endif
#ifndef QT_NO_DATASTREAM
    friend Q_GRPC_EXPORT QDataStream &operator<<(QDataStream &out, const QGrpcStatus &status);
    friend Q_GRPC_EXPORT QDataStream &operator>>(QDataStream &in, QGrpcStatus &status);
#endif
};

Q_DECLARE_SHARED(QGrpcStatus)

QT_END_NAMESPACE

#endif // QGRPCSTATUS_H
