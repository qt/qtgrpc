// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpcchanneloptions.h>
#include <QtGrpc/qgrpcserializationformat.h>
#include <QtGrpc/qtgrpcnamespace.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qdebug.h>
#include <QtCore/qvariant.h>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;
using namespace QtGrpc;

/*!
    \class QGrpcChannelOptions
    \inmodule QtGrpc
    \since 6.6
    \brief The QGrpcChannelOptions is an storage class used to set additional channel options.

    QGrpcChannelOptions provides a set of functions to set and access the channel and default call
    options that are used by gRPC channels to communicate with the services.
*/

class QGrpcChannelOptionsPrivate : public QSharedData
{
public:
    std::optional<std::chrono::milliseconds> timeout;
    QHash<QByteArray, QByteArray> metadata;
    QGrpcSerializationFormat serializationFormat;
#if QT_CONFIG(ssl)
    std::optional<QSslConfiguration> sslConfiguration;
#endif
};

QT_DEFINE_QESDP_SPECIALIZATION_DTOR(QGrpcChannelOptionsPrivate)

/*!
    Constructs a QGrpcChannelOptions.
*/
QGrpcChannelOptions::QGrpcChannelOptions() : d_ptr(new QGrpcChannelOptionsPrivate())
{
}

/*!
    Construct a copy of QGrpcChannelOptions with \a other object.
*/
QGrpcChannelOptions::QGrpcChannelOptions(const QGrpcChannelOptions &other) = default;

/*!
    Assigns \a other to this QGrpcChannelOptions and returns a reference to this
    QGrpcChannelOptions.
*/
QGrpcChannelOptions &QGrpcChannelOptions::operator=(const QGrpcChannelOptions &other) = default;

/*!
    \fn QGrpcChannelOptions::QGrpcChannelOptions(QGrpcChannelOptions &&other) noexcept
    Move-constructs a new QGrpcChannelOptions from \a other.

    \note The moved-from object \a other is placed in a partially-formed state,
    in which the only valid operations are destruction and assignment of a new
    value.
*/

/*!
    \fn QGrpcChannelOptions &QGrpcChannelOptions::operator=(QGrpcChannelOptions &&other) noexcept
    Move-assigns \a other to this QGrpcChannelOptions instance and returns a
    reference to it.

    \note The moved-from object \a other is placed in a partially-formed state,
    in which the only valid operations are destruction and assignment of a new
    value.
*/

/*!
    \since 6.8
    \fn void QGrpcChannelOptions::swap(QGrpcChannelOptions &other) noexcept
    Swaps this instance with \a other. This operation is very fast and never fails.
*/

/*!
    Destroys the QGrpcChannelOptions object.
*/
QGrpcChannelOptions::~QGrpcChannelOptions() = default;

/*!
    \since 6.8
    Constructs a new QVariant object from this QGrpcChannelOptions.
*/
QGrpcChannelOptions::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

/*!
    Sets deadline value with \a timeout and returns updated QGrpcChannelOptions object.
*/
QGrpcChannelOptions &QGrpcChannelOptions::setDeadlineTimeout(std::chrono::milliseconds timeout)
{
    if (d_ptr->timeout == timeout)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->timeout = timeout;
    return *this;
}

/*!
    \fn QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(const QHash<QByteArray, QByteArray> &metadata)
    \fn QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(QHash<QByteArray, QByteArray> &&metadata)

    Sets \a metadata for all calls and returns updated QGrpcChannelOptions object.

    For HTTP2-based channels, \a metadata is converted into HTTP/2 headers, that
    added to each HTTP/2 request.
*/
QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(const QHash<QByteArray, QByteArray> &metadata)
{
    if (d_ptr->metadata == metadata)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->metadata = metadata;
    return *this;
}

QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(QHash<QByteArray, QByteArray> &&metadata)
{
    if (d_ptr->metadata == metadata)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->metadata = std::move(metadata);
    return *this;
}

/*!
    \since 6.8
    Sets the serialization \a format in the channel and returns updated
    QGrpcChannelOptions object.

    The serialization \a format should be considered in \l QAbstractGrpcChannel
    implementations.
*/
QGrpcChannelOptions &
QGrpcChannelOptions::setSerializationFormat(const QGrpcSerializationFormat &format)
{
    if (d_ptr->serializationFormat == format)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->serializationFormat = format;
    return *this;
}

/*!
    Returns deadline value for setting up the channel.

    Deadline value controls the maximum execution time of any call or stream
    executed on the channel.

    If value was not set returns empty std::optional.
*/
std::optional<std::chrono::milliseconds> QGrpcChannelOptions::deadlineTimeout() const noexcept
{
    Q_D(const QGrpcChannelOptions);
    return d->timeout;
}

/*!
    \fn const QHash<QByteArray, QByteArray> &QGrpcChannelOptions::metadata() const & noexcept
    \fn QHash<QByteArray, QByteArray> QGrpcChannelOptions::metadata() && noexcept

    Returns metadata used for every call on the channel.

    If value was not set returns empty QHash<QByteArray, QByteArray>.
*/
const QHash<QByteArray, QByteArray> &QGrpcChannelOptions::metadata() const & noexcept
{
    Q_D(const QGrpcChannelOptions);
    return d->metadata;
}

QHash<QByteArray, QByteArray> QGrpcChannelOptions::metadata() &&
{
    Q_D(QGrpcChannelOptions);
    if (d->ref.loadRelaxed() != 1) // return copy if shared
        return { d->metadata };
    return std::move(d->metadata);
}

/*!
    \since 6.8
    Returns the serialization format used in \l QAbstractGrpcChannel implementations.
 */
QGrpcSerializationFormat QGrpcChannelOptions::serializationFormat() const
{
    Q_D(const QGrpcChannelOptions);
    return d->serializationFormat;
}

#if QT_CONFIG(ssl)
/*!
    Sets SSL configuration with \a sslConfiguration and returns updated QGrpcChannelOptions object.
*/
QGrpcChannelOptions &
QGrpcChannelOptions::setSslConfiguration(const QSslConfiguration &sslConfiguration)
{
    if (d_ptr->sslConfiguration == sslConfiguration)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->sslConfiguration = sslConfiguration;
    return *this;
}

/*!
    Returns SSL configuration for the channel.

    If value was not set returns empty std::optional.
*/
std::optional<QSslConfiguration> QGrpcChannelOptions::sslConfiguration() const
{
    Q_D(const QGrpcChannelOptions);
    return d->sslConfiguration;
}
#endif

#ifndef QT_NO_DEBUG_STREAM
/*!
    \since 6.8
    \fn QDebug QGrpcChannelOptions::operator<<(QDebug debug, const QGrpcChannelOptions &chOpts)
    Writes \a chOpts to the specified stream \a debug.
*/
QDebug operator<<(QDebug debug, const QGrpcChannelOptions &chOpts)
{
    const QDebugStateSaver save(debug);
    debug.nospace().noquote();
    debug << "QGrpcChannelOptions(deadline: " << chOpts.deadlineTimeout()
          << ", metadata: " << chOpts.metadata()
          << ", serializationFormat: " << chOpts.serializationFormat().suffix()
          << ", sslConfiguration: ";
#  if QT_CONFIG(ssl)
    if (chOpts.sslConfiguration())
        debug << "available";
    else
        debug << std::nullopt;
#  else
    debug << "unsupported";
#  endif
    debug << ')';
    return debug;
}
#endif

QT_END_NAMESPACE
