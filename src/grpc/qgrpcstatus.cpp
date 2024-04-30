// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpcstatus.h>

#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qvariant.h>

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcStatus
    \inmodule QtGrpc
    \compares equality
    \compareswith equality StatusCode
    \endcompareswith

    \brief This class combines a \l StatusCode and a string message.

    The QGrpcStatus class contains information about the last gRPC operation
    returned from the respective channel, or other functions in the QtGrpc
    library.

    If a RPC operation failed, contains a \l StatusCode other than \l {Ok}.
*/

/*!
    \property QGrpcStatus::code
    \brief QGrpcStatus::StatusCode received for prior gRPC call.
*/

/*!
    \property QGrpcStatus::message
    \brief Status message received for prior gRPC call.
*/

/*!
    \enum QGrpcStatus::StatusCode

    \brief Channel's status codes.

    \value Ok No error
    \value Cancelled The operation was cancelled, typically by the caller.
    \omitvalue Unknown
    \value InvalidArgument The client specified an invalid argument,
    \value DeadlineExceeded The deadline expired before the operation
    could complete,
    \value NotFound Some requested entity (e.g., file or directory) was
    not found.
    \value AlreadyExists The entity that a client attempted to create
    (e.g., file or directory) already exists.
    \value PermissionDenied  The caller does not have permission to execute
    the specified operation.
    \c PermissionDenied must not be used for rejections caused by exhausting
    some resource (use \c ResourceExhausted instead for those errors).
    \c PermissionDenied must not be used if the caller can not be identified
    (use \c Unauthenticated instead for those errors).
    This error code does not imply the request is valid or the requested
    entity exists or satisfies other pre-conditions.
    \value ResourceExhausted Some resource has been exhausted, perhaps
    a per-user quota, or perhaps the entire file system is out of space.
    \value FailedPrecondition The operation was rejected because the system
    is not in a state required for the operation's execution.
    \value Aborted The operation was aborted, typically due to
    a concurrency issue such as a sequencer check failure or transaction abort.
    \value OutOfRange The operation was attempted past the valid range.
    \value Unimplemented The operation is not implemented or is
    not supported/enabled in this service.
    \value Internal This means that some invariants expected by
    the underlying system have been broken.
    \value Unavailable The service is currently unavailable.
    This is most likely a transient condition, which can be corrected
    by retrying with a backoff. Note that it is not always safe
    to retry non-idempotent operations.
    \value DataLoss Unrecoverable data loss or corruption.
    \value Unauthenticated The request does not have valid authentication
    credentials for the operation.

    \sa{https://github.com/grpc/grpc/blob/master/doc/statuscodes.md}{gRPC status codes}
*/

/*!
    Constructs a QGrpcStatus with the status code \a code and the string \a
    message.
*/
QGrpcStatus::QGrpcStatus(StatusCode code, QAnyStringView message)
    : m_code(code), m_message(message.toString())
{
}

/*!
    Destroys the status object.
*/
QGrpcStatus::~QGrpcStatus() = default;

/*!
    Copy-constructs a QGrpcStatus from \a other
*/
QGrpcStatus::QGrpcStatus(const QGrpcStatus &other) = default;

/*!
    Assigns the data of the \a other object to this status object and returns
    a reference to it.
*/
QGrpcStatus &QGrpcStatus::operator=(const QGrpcStatus &other) = default;

/*!
    \fn QGrpcStatus::QGrpcStatus(QGrpcStatus &&other) noexcept
    Move-constructs a new QGrpcStatus from \a other.

    \note The moved-from object \a other is placed in a partially-formed state,
    in which the only valid operations are destruction and assignment of a new
    value.
*/

/*!
    \fn QGrpcStatus& QGrpcStatus::operator=(QGrpcStatus &&other) noexcept
    Move-assigns \a other to this QGrpcStatus instance and returns a reference
    to it.

    \note The moved-from object \a other is placed in a partially-formed state,
    in which the only valid operations are destruction and assignment of a new
    value.
*/

/*!
    \since 6.8
    Constructs a new QVariant object from this QGrpcStatus.
*/
QGrpcStatus::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

/*!
    \since 6.8
    \fn void QGrpcStatus::swap(QGrpcStatus &other) noexcept
    Swaps this instance with \a other. This operation is very fast and never fails.
*/

/*!
    \fn QGrpcStatus::StatusCode QGrpcStatus::code() const noexcept
    Returns the contained \l StatusCode.
*/

/*!
    \fn QString QGrpcStatus::message() const noexcept
    Returns the contained status message.
*/

/*!
    \fn bool QGrpcStatus::operator==(const QGrpcStatus &lhs, const StatusCode &rhs) noexcept
    Returns \c true if the status codes in \a lhs and \a rhs are equal.
*/

/*!
    \fn bool QGrpcStatus::operator!=(const QGrpcStatus &lhs, const StatusCode &rhs) noexcept
    Returns \c true if the status codes in \a lhs and \a rhs are not equal.
*/

/*!
    \fn bool QGrpcStatus::operator==(const QGrpcStatus &lhs, const QGrpcStatus &rhs) noexcept
    Returns \c true if the status codes in \a lhs and \a rhs are equal.
*/

/*!
    \fn bool QGrpcStatus::operator!=(const QGrpcStatus &lhs, const QGrpcStatus &rhs) noexcept
    Returns \c true if the status codes in \a lhs and \a rhs are not equal.
*/

/*!
    \since 6.8
    \fn size_t QGrpcStatus::qHash(const QGrpcStatus &key, size_t seed) noexcept
    Returns the hash value of \a key, using \a seed to seed the calculation.
*/

#ifndef QT_NO_DEBUG_STREAM

/*!
    \since 6.8
    \fn QDebug QGrpcStatus::operator<<(QDebug debug, const QGrpcStatus& status)
    Writes \a status to the specified stream \a debug.
*/
QDebug operator<<(QDebug debug, const QGrpcStatus &status)
{
    const QDebugStateSaver save(debug);
    debug.nospace() << "QGrpcStatus( code: " << status.code() << ", message: " << status.message()
                    << " )";
    return debug;
}

#endif // QT_NO_DEBUG_STREAM

#ifndef QT_NO_DATASTREAM

/*!
    \since 6.8
    \fn QDataStream &QGrpcStatus::operator<<(QDataStream &out, const QGrpcStatus &status)
    Writes the given \a status to the specified stream \a out.
*/
QDataStream &operator<<(QDataStream &out, const QGrpcStatus &status)
{
    out << status.m_code << status.m_message;
    return out;
}

/*!
    \since 6.8
    \fn QDataStream &QGrpcStatus::operator>>(QDataStream &in, QGrpcStatus &status)
    Reads a QGrpcStatus from stream \a in into \a status.
*/
QDataStream &operator>>(QDataStream &in, QGrpcStatus &status)
{
    in >> status.m_code;
    in >> status.m_message;
    return in;
}

#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE

#include "moc_qgrpcstatus.cpp"
