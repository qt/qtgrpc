// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/private/qtgrpcglobal_p.h>
#include <QtGrpc/qgrpcchanneloptions.h>
#include <QtGrpc/qgrpcserializationformat.h>
#include <QtGrpc/qtgrpcnamespace.h>

#include <QtCore/qdebug.h>

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

class QGrpcChannelOptionsPrivate
{
public:
    QGrpcChannelOptionsPrivate() : serializationFormat(SerializationFormat::Default) { }

    std::optional<QGrpcDuration> deadline;
    QGrpcMetadata metadata;
    QGrpcSerializationFormat serializationFormat;
#if QT_CONFIG(ssl)
    std::optional<QSslConfiguration> sslConfiguration;
#endif
};

static void dPtrDeleter(QGrpcChannelOptionsPrivate *ptr)
{
    delete ptr;
}

/*!
    Constructs a QGrpcChannelOptions.
*/
QGrpcChannelOptions::QGrpcChannelOptions() : dPtr(new QGrpcChannelOptionsPrivate(), dPtrDeleter)
{
}

/*!
    Construct a copy of QGrpcChannelOptions with \a other object.
*/
QGrpcChannelOptions::QGrpcChannelOptions(const QGrpcChannelOptions &other)
    : dPtr(new QGrpcChannelOptionsPrivate(*other.dPtr), dPtrDeleter)
{
}

/*!
    Assigns \a other to this QGrpcChannelOptions and returns a reference to this
    QGrpcChannelOptions.
*/
QGrpcChannelOptions &QGrpcChannelOptions::operator=(const QGrpcChannelOptions &other)
{
    if (this != &other)
        *dPtr = *other.dPtr;
    return *this;
}

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
    Sets deadline value with \a deadline and returns updated QGrpcChannelOptions object.
*/
QGrpcChannelOptions &QGrpcChannelOptions::setDeadline(QGrpcDuration deadline)
{
    dPtr->deadline = deadline;
    return *this;
}

/*!
    \fn QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(const QGrpcMetadata &metadata)
    \fn QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(QGrpcMetadata &&metadata)

    Sets \a metadata for all calls and returns updated QGrpcChannelOptions object.

    For HTTP2-based channels, \a metadata is converted into HTTP/2 headers, that
    added to each HTTP/2 request.
*/
QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(const QGrpcMetadata &metadata)
{
    dPtr->metadata = metadata;
    return *this;
}

QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(QGrpcMetadata &&metadata)
{
    dPtr->metadata = std::move(metadata);
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
    dPtr->serializationFormat = format;
    return *this;
}

/*!
    Returns deadline value for setting up the channel.

    Deadline value controls the maximum execution time of any call or stream
    executed on the channel.

    If value was not set returns empty std::optional.
*/
std::optional<QGrpcDuration> QGrpcChannelOptions::deadline() const noexcept
{
    return dPtr->deadline;
}

/*!
    \fn const QGrpcMetadata &QGrpcChannelOptions::metadata() const & noexcept
    \fn QGrpcMetadata QGrpcChannelOptions::metadata() && noexcept

    Returns metadata used for every call on the channel.

    If value was not set returns empty QGrpcMetadata.
*/
const QGrpcMetadata &QGrpcChannelOptions::metadata() const & noexcept
{
    return dPtr->metadata;
}

QGrpcMetadata QGrpcChannelOptions::metadata() && noexcept
{
    return std::move(dPtr->metadata);
}

/*!
    \since 6.8
    \fn const QGrpcSerializationFormat &QGrpcChannelOptions::serializationFormat() const & noexcept
    \fn QGrpcSerializationFormat QGrpcChannelOptions::serializationFormat() && noexcept

    Returns the serialization format used in \l QAbstractGrpcChannel implementations.
 */
const QGrpcSerializationFormat &QGrpcChannelOptions::serializationFormat() const & noexcept
{
    return dPtr->serializationFormat;
}

QGrpcSerializationFormat QGrpcChannelOptions::serializationFormat() && noexcept
{
    return std::move(dPtr->serializationFormat);
}

#if QT_CONFIG(ssl)
/*!
    Sets SSL configuration with \a sslConfiguration and returns updated QGrpcChannelOptions object.
*/
QGrpcChannelOptions &QGrpcChannelOptions::setSslConfiguration(
        const QSslConfiguration &sslConfiguration)
{
    dPtr->sslConfiguration = sslConfiguration;
    return *this;
}

/*!
    Returns SSL configuration for the channel.

    If value was not set returns empty std::optional.
*/
std::optional<QSslConfiguration> QGrpcChannelOptions::sslConfiguration() const noexcept
{
    return dPtr->sslConfiguration;
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
    QDebugStateSaver save(debug);
    debug.nospace();
    debug.noquote();
    debug << "QGrpcChannelOptions(deadline: " << chOpts.deadline()
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
