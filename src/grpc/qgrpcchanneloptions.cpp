// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <qtgrpcglobal_p.h>

#include "qgrpcchanneloptions.h"
#include "qgrpcserializationformat.h"

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

/*!
    \class QGrpcChannelOptions
    \inmodule QtGrpc
    \since 6.6
    \brief The QGrpcChannelOptions is an storage class used to set additional channel options.

    QGrpcChannelOptions provides a set of functions to set and access the channel and default call
    options that are used by gRPC channels to communicate with the services.
*/

struct QGrpcChannelOptionsPrivate
{
public:
    QGrpcChannelOptionsPrivate(const QUrl &_host)
        : host(_host), serializationFormat(QGrpcSerializationFormat::Format::Default)
    {
    }

    QUrl host;
    std::optional<std::chrono::milliseconds> deadline;
    QGrpcMetadata metadata;
    std::optional<QStringList> credentialList;
    QGrpcSerializationFormat serializationFormat;
#if QT_CONFIG(ssl)
    std::optional<QSslConfiguration> sslConfiguration;
#endif
};

/*!
    Constructs an QGrpcChannelOptions object with \a host value.
*/
QGrpcChannelOptions::QGrpcChannelOptions(const QUrl &host)
    : dPtr(std::make_unique<QGrpcChannelOptionsPrivate>(host))
{
}

/*!
    Construct a copy of QGrpcChannelOptions with \a other object.
*/
QGrpcChannelOptions::QGrpcChannelOptions(const QGrpcChannelOptions &other)
    : dPtr(std::make_unique<QGrpcChannelOptionsPrivate>(*other.dPtr))
{
}

/*!
    Assigns \a other to this QGrpcChannelOptions and returns a reference to this
    QGrpcChannelOptions.
*/
QGrpcChannelOptions &QGrpcChannelOptions::operator=(const QGrpcChannelOptions &other)
{
    *dPtr = *other.dPtr;
    return *this;
}

/*!
    Destroys the QGrpcChannelOptions object.
*/
QGrpcChannelOptions::~QGrpcChannelOptions() = default;

/*!
    Sets host value with \a host and returns updated QGrpcChannelOptions object.
*/
QGrpcChannelOptions &QGrpcChannelOptions::withHost(const QUrl &host)
{
    dPtr->host = host;
    return *this;
}

/*!
    Sets deadline value with \a deadline and returns updated QGrpcChannelOptions object.
*/
QGrpcChannelOptions &QGrpcChannelOptions::withDeadline(std::chrono::milliseconds deadline)
{
    dPtr->deadline = deadline;
    return *this;
}

/*!
    Sets \a metadata for all calls and returns updated QGrpcChannelOptions object.

    For HTTP2-based channels, \a metadata is converted into HTTP/2 headers, that
    added to each HTTP/2 request.
*/
QGrpcChannelOptions &QGrpcChannelOptions::withMetadata(const QGrpcMetadata &metadata)
{
    dPtr->metadata = metadata;
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
QGrpcChannelOptions::withSerializationFormat(const QGrpcSerializationFormat &format)
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
std::optional<std::chrono::milliseconds> QGrpcChannelOptions::deadline() const
{
    return dPtr->deadline;
}

/*!
    Returns host value for the channel.
*/
QUrl QGrpcChannelOptions::host() const
{
    return dPtr->host;
}

/*!
    Returns metadata used for every call on the channel.

    If value was not set returns empty QGrpcMetadata.
*/
const QGrpcMetadata &QGrpcChannelOptions::metadata() const
{
    return dPtr->metadata;
}

/*!
    \since 6.8
    Returns the serialization format used in \l QAbstractGrpcChannel implementations.
 */
const QGrpcSerializationFormat &QGrpcChannelOptions::serializationFormat() const
{
    return dPtr->serializationFormat;
}

#if QT_CONFIG(ssl)
/*!
    Sets SSL configuration with \a sslConfiguration and returns updated QGrpcChannelOptions object.
*/
QGrpcChannelOptions &QGrpcChannelOptions::withSslConfiguration(
        const QSslConfiguration &sslConfiguration)
{
    dPtr->sslConfiguration = sslConfiguration;
    return *this;
}

/*!
    Returns SSL configuration for the channel.

    If value was not set returns empty std::optional.
*/
std::optional<QSslConfiguration> QGrpcChannelOptions::sslConfiguration() const
{
    return dPtr->sslConfiguration;
}
#endif

QT_END_NAMESPACE
