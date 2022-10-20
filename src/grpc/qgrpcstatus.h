// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCSTATUS_H
#define QGRPCSTATUS_H

#include <QtCore/QMetaType>
#include <QtCore/QString>
#include <QtCore/qobjectdefs.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QGrpcStatusPrivate;

class Q_GRPC_EXPORT QGrpcStatus final
{
    Q_GADGET
    Q_PROPERTY(StatusCode code READ code CONSTANT)
    Q_PROPERTY(QString message READ message CONSTANT)
public:
    enum StatusCode {
        // No error
        Ok = 0,
        // The operation was cancelled, typically by the caller
        Cancelled = 1,
        // Unknown error
        Unknown = 2,
        // The client specified an invalid argument
        InvalidArgument = 3,
        // The deadline expired before the operation could complete
        DeadlineExceeded = 4,
        // Some requested entity (e.g., file or directory) was not found
        NotFound = 5,
        // The entity that a client attempted to create (e.g., file or directory) already exists.
        // 409 Conflict
        AlreadyExists = 6,
        // The caller does not have permission to execute the specified operation.
        // PERMISSION_DENIED must not be used for rejections caused by exhausting some resource
        // (use RESOURCE_EXHAUSTED instead for those errors).
        // PERMISSION_DENIED must not be used if the caller can not be identified
        // (use UNAUTHENTICATED instead for those errors).
        // This error code does not imply the request is valid or the requested entity exists
        // or satisfies other pre-conditions. 403 Forbidden
        PermissionDenied = 7,
        // The request does not have valid authentication credentials for the operation
        Unauthenticated = 16,
        // Some resource has been exhausted, perhaps a per-user quota, or
        // perhaps the entire file system is out of space
        ResourceExhausted = 8,
        // The operation was rejected because the system is not in a state
        // required for the operation's execution
        FailedPrecondition = 9,
        // The operation was aborted, typically due to a concurrency issue such as a
        // sequencer check failure or transaction abort
        Aborted = 10,
        // The operation was attempted past the valid range
        OutOfRange = 11,
        // The operation is not implemented or is not supported/enabled in
        // this service
        Unimplemented = 12,
        // Internal errors. This means that some invariants expected by the
        // underlying system have been broken.
        Internal = 13,
        // The service is currently unavailable
        Unavailable = 14,
        // Unrecoverable data loss or corruption
        DataLoss = 15,
    };

    Q_ENUM(StatusCode)

    QGrpcStatus(StatusCode code = StatusCode::Ok, const QString &message = QString());
    ~QGrpcStatus();

    StatusCode code() const;
    QString message() const;

    QGrpcStatus(const QGrpcStatus &other);
    QGrpcStatus &operator=(const QGrpcStatus &other);

    QGrpcStatus(QGrpcStatus &&other);
    QGrpcStatus &operator=(QGrpcStatus &&other);

private:
    friend bool operator==(const QGrpcStatus &lhs, QGrpcStatus::StatusCode code)
    {
        return lhs.code() == code;
    }
    friend bool operator!=(const QGrpcStatus &lhs, QGrpcStatus::StatusCode code)
    {
        return lhs.code() != code;
    }
    friend bool operator==(const QGrpcStatus &lhs, const QGrpcStatus &rhs)
    {
        return lhs.code() == rhs.code();
    }
    friend bool operator!=(const QGrpcStatus &lhs, const QGrpcStatus &rhs)
    {
        return lhs.code() == rhs.code();
    }

    std::unique_ptr<QGrpcStatusPrivate> dPtr;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QGrpcStatus)

#endif // QGRPCSTATUS_H
