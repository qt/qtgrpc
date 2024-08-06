// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpccalloptions.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qdebug.h>
#include <QtCore/qvariant.h>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

/*!
    \class QGrpcCallOptions
    \inmodule QtGrpc
    \brief The QGrpcCallOptions is an storage class used to set additional call options.
    \since 6.6

    QGrpcCallOptions provides a set of functions to access the call options
    that are used by gRPC channels to communicate with the services.
*/

class QGrpcCallOptionsPrivate : public QSharedData
{
public:
    std::optional<std::chrono::milliseconds> timeout;
    QHash<QByteArray, QByteArray> metadata;
};

QT_DEFINE_QESDP_SPECIALIZATION_DTOR(QGrpcCallOptionsPrivate)

/*!
    Constructs an empty QGrpcCallOptions object.
*/
QGrpcCallOptions::QGrpcCallOptions() : d_ptr(new QGrpcCallOptionsPrivate())
{
}

/*!
    Destroys the QGrpcCallOptions object.
*/
QGrpcCallOptions::~QGrpcCallOptions() = default;

/*!
    Construct a copy of QGrpcCallOptions with \a other object.
*/
QGrpcCallOptions::QGrpcCallOptions(const QGrpcCallOptions &other) = default;

/*!
    Assigns \a other to this QGrpcCallOptions and returns a reference to this
    QGrpcCallOptions.
*/
QGrpcCallOptions &QGrpcCallOptions::operator=(const QGrpcCallOptions &other) = default;

/*!
    \fn QGrpcCallOptions::QGrpcCallOptions(QGrpcCallOptions &&other) noexcept
    Move-constructs a new QGrpcCallOptions from \a other.

    \note The moved-from object \a other is placed in a partially-formed state,
    in which the only valid operations are destruction and assignment of a new
    value.
*/

/*!
    \fn QGrpcCallOptions &QGrpcCallOptions::operator=(QGrpcCallOptions &&other) noexcept
    Move-assigns \a other to this QGrpcCallOptions instance and returns a
    reference to it.

    \note The moved-from object \a other is placed in a partially-formed state,
    in which the only valid operations are destruction and assignment of a new
    value.
*/

/*!
    \since 6.8
    Constructs a new QVariant object from this QGrpcCallOptions.
*/
QGrpcCallOptions::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

/*!
    \since 6.8
    \fn void QGrpcCallOptions::swap(QGrpcCallOptions &other) noexcept
    Swaps this instance with \a other. This operation is very fast and never fails.
*/

/*!
    Sets deadline value with \a timeout and returns updated QGrpcCallOptions object.
*/
QGrpcCallOptions &QGrpcCallOptions::setDeadlineTimeout(std::chrono::milliseconds timeout)
{
    if (d_ptr->timeout == timeout)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcCallOptions);
    d->timeout = timeout;
    return *this;
}

/*!
    Sets \a metadata for a call and returns updated QGrpcCallOptions object.

    For HTTP2-based channels, \a metadata is converted into HTTP/2 headers, that
    added to the corresponding HTTP/2 request.
*/
QGrpcCallOptions &QGrpcCallOptions::setMetadata(const QHash<QByteArray, QByteArray> &metadata)
{
    if (d_ptr->metadata == metadata)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcCallOptions);
    d->metadata = metadata;
    return *this;
}

/*!
    Sets \a metadata for a call and returns updated QGrpcCallOptions object.

    \sa setMetadata()
*/
QGrpcCallOptions &QGrpcCallOptions::setMetadata(QHash<QByteArray, QByteArray> &&metadata)
{
    if (d_ptr->metadata == metadata)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcCallOptions);
    d->metadata = std::move(metadata);
    return *this;
}

/*!
    Returns deadline value for a call.

    Deadline value controls the maximum execution time of an call or a stream.
    This value overrides value set by QGrpcChannelOptions::deadline()
    for a specific call or stream.

    If value was not set returns empty std::optional.
*/
std::optional<std::chrono::milliseconds> QGrpcCallOptions::deadlineTimeout() const noexcept
{
    Q_D(const QGrpcCallOptions);
    return d->timeout;
}

/*!
    \fn const QHash<QByteArray, QByteArray> &QGrpcCallOptions::metadata() const & noexcept
    \fn QHash<QByteArray, QByteArray> QGrpcCallOptions::metadata() && noexcept

    Returns metadata used for a call.

    If the value was not set returns an empty QHash<QByteArray, QByteArray>.
*/
const QHash<QByteArray, QByteArray> &QGrpcCallOptions::metadata() const & noexcept
{
    Q_D(const QGrpcCallOptions);
    return d->metadata;
}

QHash<QByteArray, QByteArray> QGrpcCallOptions::metadata() &&
{
    Q_D(QGrpcCallOptions);
    if (d->ref.loadRelaxed() != 1) // return copy if shared
        return { d->metadata };
    return std::move(d->metadata);
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \since 6.8
    \fn QDebug QGrpcCallOptions::operator<<(QDebug debug, const QGrpcCallOptions &callOpts)
    Writes \a callOpts to the specified stream \a debug.
*/
QDebug operator<<(QDebug debug, const QGrpcCallOptions &callOpts)
{
    const QDebugStateSaver save(debug);
    debug.nospace().noquote();
    debug << "QGrpcCallOptions(deadline: " << callOpts.deadlineTimeout()
          << ", metadata: " << callOpts.metadata() << ')';
    return debug;
}
#endif

QT_END_NAMESPACE
