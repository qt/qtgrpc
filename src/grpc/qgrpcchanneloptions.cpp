// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/private/qgrpcchanneloptions_p.h>
#include <QtGrpc/private/qtgrpclogging_p.h>
#include <QtGrpc/qgrpcchanneloptions.h>
#include <QtGrpc/qgrpcserializationformat.h>
#include <QtGrpc/qtgrpcnamespace.h>

#include <QtNetwork/private/qdecompresshelper_p.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qdebug.h>
#include <QtCore/qvariant.h>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;
using namespace QtGrpc;

namespace {

std::chrono::milliseconds clampNonNegative(const char *setter, std::chrono::milliseconds value)
{
    if (value < std::chrono::milliseconds::zero()) {
        qGrpcWarning("%s: negative intervals aren't allowed (%lldms); clamping to 0.", setter,
                     static_cast<long long>(value.count()));
        return std::chrono::milliseconds::zero();
    }
    return value;
}

} // namespace

/*!
    \class QGrpcChannelOptions
    \inmodule QtGrpc
    \since 6.6
    \compares equality
    \brief The QGrpcChannelOptions class offers various options for fine-tuning
    a gRPC channel.

    QGrpcChannelOptions lets you customize a \gRPC channel. Some options apply
    to all remote procedure calls (RPCs) that operate on the associated
    channel, which is used to communicate with services.

    Override options for specific RPCs with QGrpcCallOptions.

    \code
        QGrpcChannelOptions channelOpts;
        // Apply common metadata to every RPC
        channelOpts.setMetadata({
            { "header" , "value1" },
            { "header" , "value2" },
        });
        const auto &md = channelOpts.metadata(QtGrpc::MultiValue);
        qDebug() << "Channel Metadata: " << md;

        // Apply a 2-second deadline to every RPC
        channelOpts.setDeadlineTimeout(2s);
        qDebug() << "Channel timeout: " << channelOpts.deadlineTimeout();

        // Configure SSL/TLS configuration
        channelOpts.setSslConfiguration(QSslConfiguration());
    \endcode


    \note It is up to the channel's implementation to determine the specifics
    of these options.
*/

QT_DEFINE_QESDP_SPECIALIZATION_DTOR(QGrpcChannelOptionsPrivate)

/*!
    Default-constructs an empty QGrpcChannelOptions.
*/
QGrpcChannelOptions::QGrpcChannelOptions() : d_ptr(new QGrpcChannelOptionsPrivate())
{
}

/*!
    Copy-constructs a QGrpcChannelOptions from \a other.
*/
QGrpcChannelOptions::QGrpcChannelOptions(const QGrpcChannelOptions &other) = default;

/*!
    Assigns \a other to this QGrpcChannelOptions and returns a reference to the
    updated object.
*/
QGrpcChannelOptions &QGrpcChannelOptions::operator=(const QGrpcChannelOptions &other) = default;

/*!
    \fn QGrpcChannelOptions::QGrpcChannelOptions(QGrpcChannelOptions &&other)

    Move-constructs a new QGrpcChannelOptions from \a other.

    \include qtgrpc-shared.qdocinc move-note-desc
*/

/*!
    \fn QGrpcChannelOptions &QGrpcChannelOptions::operator=(QGrpcChannelOptions &&other)

    Move-assigns \a other to this QGrpcChannelOptions and returns a reference
    to the updated object.

    \include qtgrpc-shared.qdocinc move-note-desc
*/

/*!
    \since 6.8
    \fn void QGrpcChannelOptions::swap(QGrpcChannelOptions &other)

    \include qtgrpc-shared.qdocinc swap-desc
*/

/*!
    Destroys the QGrpcChannelOptions.
*/
QGrpcChannelOptions::~QGrpcChannelOptions() = default;

/*!
    \since 6.8
    \include qtgrpc-shared.qdocinc qvariant-desc
*/
QGrpcChannelOptions::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

/*!
    \include qgrpccommonoptions.cpp set-deadline-timeout

//! [channel-note]
    \note Setting this field applies to all RPCs that operate on the channel,
    except those overriden by
//! [channel-note]
    \l{QGrpcCallOptions::setDeadlineTimeout()}

    \sa deadlineTimeout()
*/
QGrpcChannelOptions &QGrpcChannelOptions::setDeadlineTimeout(std::chrono::milliseconds timeout)
{
    if (d_ptr->deadlineTimeout() == timeout)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->setDeadlineTimeout(timeout);
    return *this;
}

#if QT_DEPRECATED_SINCE(6, 13)

/*!
    \fn const QHash<QByteArray, QByteArray> &QGrpcChannelOptions::metadata() const &
    \fn QHash<QByteArray, QByteArray> QGrpcChannelOptions::metadata() &&
    \deprecated [6.13] Use \l{metadata(QtGrpc::MultiValue_t)}{metadata(QtGrpc::MultiValue)} instead.

    \include qgrpccommonoptions.cpp metadata

    \sa metadata(QtGrpc::MultiValue_t), setMetadata()
*/
const QHash<QByteArray, QByteArray> &QGrpcChannelOptions::metadata() const & noexcept
{
    Q_D(const QGrpcChannelOptions);
    return d->metadata();
}
QHash<QByteArray, QByteArray> QGrpcChannelOptions::metadata() &&
{
    Q_D(QGrpcChannelOptions);
    if (d->ref.loadRelaxed() != 1) // return copy if shared
        return d->metadata();
    return std::move(*d_ptr).metadata();
}

/*!
    \fn QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(const QHash<QByteArray, QByteArray> &metadata)
    \fn QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(QHash<QByteArray, QByteArray> &&metadata)
    \deprecated [6.13] Use the QMultiHash overload instead.

    \include qgrpccommonoptions.cpp set-metadata

//! [merge-md-note]
    \note This metadata is included in every RPC made through the channel.
    Channel metadata is \b{merged} with any call-specific metadata when the RPC
    starts — see
//! [merge-md-note]
    \l{QGrpcCallOptions::setMetadata(const QMultiHash<QByteArray,
    QByteArray>&)}{QGrpcCallOptions::setMetadata(QMultiHash)}

    \sa metadata()
*/
QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(const QHash<QByteArray, QByteArray> &metadata)
{
    if (d_ptr->metadata() == metadata)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->setMetadata(metadata);
    return *this;
}
QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(QHash<QByteArray, QByteArray> &&metadata)
{
    if (d_ptr->metadata() == metadata)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->setMetadata(std::move(metadata));
    return *this;
}

#endif // QT_DEPRECATED_SINCE(6, 13)

/*!
    \since 6.10
    \fn const QMultiHash<QByteArray, QByteArray> &QGrpcChannelOptions::metadata(QtGrpc::MultiValue_t) const &
    \fn QMultiHash<QByteArray, QByteArray> QGrpcChannelOptions::metadata(QtGrpc::MultiValue_t) &&

    \include qgrpccommonoptions.cpp metadata-multi

    \sa {setMetadata(const QMultiHash<QByteArray, QByteArray>&)}{setMetadata}
*/
const QMultiHash<QByteArray, QByteArray> &
QGrpcChannelOptions::metadata(QtGrpc::MultiValue_t tag) const & noexcept
{
    Q_D(const QGrpcChannelOptions);
    return d->metadata(tag);
}

QMultiHash<QByteArray, QByteArray>
QGrpcChannelOptions::metadata(QtGrpc::MultiValue_t tag) &&
{
    Q_D(QGrpcChannelOptions);
    if (d->ref.loadRelaxed() != 1)
        return d->metadata(tag);
    return std::move(*d_ptr).metadata(tag);
}

/*!
    \since 6.10
    \fn QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(const QMultiHash<QByteArray, QByteArray> &metadata)
    \fn QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(QMultiHash<QByteArray, QByteArray> &&metadata)
    \fn QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(std::initializer_list<std::pair<QByteArray, QByteArray>> list)

    \include qgrpccommonoptions.cpp set-metadata-multi

    \include qgrpcchanneloptions.cpp merge-md-note
    \l{QGrpcCallOptions::setMetadata(const QMultiHash<QByteArray,
    QByteArray>&)}{QGrpcCallOptions::setMetadata(QMultiHash)}

    \sa metadata(QtGrpc::MultiValue_t)
*/
QGrpcChannelOptions &
QGrpcChannelOptions::setMetadata(const QMultiHash<QByteArray, QByteArray> &metadata)
{
    if (d_ptr->metadata(QtGrpc::MultiValue) == metadata)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->setMetadata(metadata);
    return *this;
}
QGrpcChannelOptions &QGrpcChannelOptions::setMetadata(QMultiHash<QByteArray, QByteArray> &&metadata)
{
    if (d_ptr->metadata(QtGrpc::MultiValue) == metadata)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->setMetadata(std::move(metadata));
    return *this;
}
QGrpcChannelOptions &
QGrpcChannelOptions::setMetadata(std::initializer_list<std::pair<QByteArray, QByteArray>> list)
{
    return setMetadata(QMultiHash<QByteArray, QByteArray>(list));
}

/*!
    \include qgrpccommonoptions.cpp add-metadata

    \include qgrpcchanneloptions.cpp merge-md-note
    \l{QGrpcCallOptions::addMetadata()}
*/
QGrpcChannelOptions &QGrpcChannelOptions::addMetadata(QByteArrayView key, QByteArrayView value)
{
    if (d_ptr->containsMetadata(key, value))
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->addMetadata(key.toByteArray(), value.toByteArray());
    return *this;
}

/*!
    \include qgrpccommonoptions.cpp filterServerMetadata
    \sa QGrpcCallOptions::filterServerMetadata()
*/
std::optional<bool> QGrpcChannelOptions::filterServerMetadata() const noexcept
{
    Q_D(const QGrpcChannelOptions);
    return d->filterServerMetadata();
}

/*!
    \include qgrpccommonoptions.cpp setFilterServerMetadata

    \include qgrpcchanneloptions.cpp channel-note
    \l{QGrpcCallOptions::filterServerMetadata}

    \sa QGrpcCallOptions::setFilterServerMetadata()
*/
QGrpcChannelOptions &QGrpcChannelOptions::setFilterServerMetadata(bool value)
{
    if (filterServerMetadata() == value)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->setFilterServerMetadata(value);
    return *this;
}

/*!
    \include qgrpccommonoptions.cpp maximumReceiveMessageSize
    \sa QGrpcCallOptions::maximumReceiveMessageSize()
*/
std::optional<quint64> QGrpcChannelOptions::maximumReceiveMessageSize() const
{
    Q_D(const QGrpcChannelOptions);
    return d->maximumReceiveMessageSize();
}

/*!
    \include qgrpccommonoptions.cpp setMaximumReceiveMessageSize

    \include qgrpcchanneloptions.cpp channel-note
    \l{QGrpcCallOptions::setMaximumReceiveMessageSize()}

    \sa QGrpcCallOptions::setMaximumReceiveMessageSize()
*/
QGrpcChannelOptions &QGrpcChannelOptions::setMaximumReceiveMessageSize(quint64 size)
{
    if (d_ptr->maximumReceiveMessageSize() == size)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->setMaximumReceiveMessageSize(size);
    return *this;
}

/*!
    \since 6.13

    Returns the initial reconnect backoff delay after a connection failure.

//! [initialReconnectBackoff]
    QtGrpc applies exponential backoff with jitter between reconnection
    attempts to limit the rate of reconnections and prevent resource exhaustion
    caused by repeated connection failures. The initial reconnect delay is
    \l{initialReconnectBackoff}. Subsequent delays grow by a factor of 1.6, up
    to \l{maximumReconnectBackoff}, with jitter applied to desynchronize
    clients.

    The default is 1 second, matching the \gRPC specification.

    \h2EnvFallback {QT_GRPC_INITIAL_RECONNECT_BACKOFF_MS} {1s}
//! [initialReconnectBackoff]

    \sa setInitialReconnectBackoff(), maximumReconnectBackoff()
*/
std::chrono::milliseconds QGrpcChannelOptions::initialReconnectBackoff() const
{
    Q_D(const QGrpcChannelOptions);
    return d->initialReconnectBackoff.value_or(QtGrpcPrivate::DefaultInitialReconnectBackoff);
}

/*!
    \since 6.13

    Sets the initial reconnect backoff to \a delay and returns a reference to
    the updated object.

    \include qgrpcchanneloptions.cpp initialReconnectBackoff

    \note Setting this to zero disables backoff and reconnects immediately.

    Negative values are clamped to zero and log a warning.

    \sa initialReconnectBackoff(), setMaximumReconnectBackoff()
*/
QGrpcChannelOptions &
QGrpcChannelOptions::setInitialReconnectBackoff(std::chrono::milliseconds delay)
{
    delay = clampNonNegative("QGrpcChannelOptions::setInitialReconnectBackoff", delay);
    if (d_ptr->initialReconnectBackoff == delay)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->initialReconnectBackoff = delay;
    return *this;
}

/*!
    \since 6.13

    Returns the maximum reconnect backoff delay.

//! [maximumReconnectBackoff]
    The exponential backoff delay grows up to this maximum. Once reached,
    all subsequent reconnection delays stay at (or near, with jitter)
    this value until the connection succeeds and the backoff resets. If the
    maximum is smaller than \l{initialReconnectBackoff}, the initial delay
    is clamped down to the maximum and a warning is issued.

    The default is 120 seconds, matching the \gRPC specification.

    \h2EnvFallback {QT_GRPC_MAXIMUM_RECONNECT_BACKOFF_MS} {120 s}
//! [maximumReconnectBackoff]

    \sa setMaximumReconnectBackoff(), initialReconnectBackoff()
*/
std::chrono::milliseconds QGrpcChannelOptions::maximumReconnectBackoff() const
{
    Q_D(const QGrpcChannelOptions);
    return d->maximumReconnectBackoff.value_or(QtGrpcPrivate::DefaultMaximumReconnectBackoff);
}

/*!
    \since 6.13

    Sets the maximum reconnect backoff to \a delay and returns a reference to
    the updated object.

    \include qgrpcchanneloptions.cpp maximumReconnectBackoff

    Negative values are clamped to zero and log a warning.

    \sa maximumReconnectBackoff(), setInitialReconnectBackoff()
*/
QGrpcChannelOptions &
QGrpcChannelOptions::setMaximumReconnectBackoff(std::chrono::milliseconds delay)
{
    delay = clampNonNegative("QGrpcChannelOptions::setMaximumReconnectBackoff", delay);
    if (d_ptr->maximumReconnectBackoff == delay)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->maximumReconnectBackoff = delay;
    return *this;
}

/*!
    \since 6.13

    Returns the connection attempt timeout.

//! [connectTimeout]
    If the socket does not reach the connected state within this timeout,
    the attempt is aborted and the channel transitions to the error state.
    The backoff timer then schedules the next attempt.

    The default is 20 seconds, matching the \gRPC specification.

    \h2EnvFallback {QT_GRPC_CONNECT_TIMEOUT_MS} {20 s}
//! [connectTimeout]

    \sa setConnectTimeout(), initialReconnectBackoff()
*/
std::chrono::milliseconds QGrpcChannelOptions::connectTimeout() const
{
    Q_D(const QGrpcChannelOptions);
    return d->connectTimeout.value_or(QtGrpcPrivate::DefaultConnectTimeout);
}

/*!
    \since 6.13

    Sets the connection attempt timeout to \a timeout and returns a reference
    to the updated object.

    \include qgrpcchanneloptions.cpp connectTimeout

    \note Setting this to zero disables the connection attempt timeout.

    Negative values are clamped to zero and log a warning.

    \sa connectTimeout(), setInitialReconnectBackoff()
*/
QGrpcChannelOptions &QGrpcChannelOptions::setConnectTimeout(std::chrono::milliseconds timeout)
{
    timeout = clampNonNegative("QGrpcChannelOptions::setConnectTimeout", timeout);
    if (d_ptr->connectTimeout == timeout)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->connectTimeout = timeout;
    return *this;
}

/*!
    \include qgrpccommonoptions.cpp requestCompression
    \sa QGrpcCallOptions::requestCompression()
*/
std::optional<QtGrpc::CompressionAlgorithm> QGrpcChannelOptions::requestCompression() const
{
    Q_D(const QGrpcChannelOptions);
    return d->requestCompression();
}

/*!
    \include qgrpccommonoptions.cpp setRequestCompression

    \include qgrpcchanneloptions.cpp channel-note
    \l{QGrpcCallOptions::setRequestCompression()}

    \sa QGrpcCallOptions::setRequestCompression()
*/
QGrpcChannelOptions &
QGrpcChannelOptions::setRequestCompression(QtGrpc::CompressionAlgorithm algorithm)
{
    if (d_ptr->requestCompression() == algorithm)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->setRequestCompression(algorithm);
    return *this;
}

/*!
    \since 6.12

    Returns the compression algorithms that the current Qt build can
    negotiate. \l{QtGrpc::CompressionAlgorithm::}{Identity} is always present
    because the \gRPC specification mandates uncompressed messages.
    \l{QtGrpc::CompressionAlgorithm::}{Deflate} and
    \l{QtGrpc::CompressionAlgorithm::}{Gzip} are always supported.

    \sa acceptedCompressionAlgorithms(), setAcceptedCompressionAlgorithms()
*/
CompressionAlgorithms QGrpcChannelOptions::supportedCompressionAlgorithms() noexcept
{
    CompressionAlgorithms supported = CompressionAlgorithm::Identity;
    if (QDecompressHelper::isSupportedEncoding("deflate"))
        supported |= CompressionAlgorithm::Deflate;
    if (QDecompressHelper::isSupportedEncoding("gzip"))
        supported |= CompressionAlgorithm::Gzip;
    return supported;
}

/*!
    \since 6.12

    Returns the compression algorithms that this channel is willing to accept
    from the server. The set always contains
    \l{QtGrpc::CompressionAlgorithm::}{Identity} because the \gRPC specification
    mandates that uncompressed messages are accepted.

    The value is used to build the \c{grpc-accept-encoding} request header.
    If the server responds with a compression algorithm that is not in this
    set, the call is finished with a \l{QtGrpc::StatusCode::}{Unimplemented}
    status code.

    By default, this is \l{supportedCompressionAlgorithms()}: every algorithm
    the current Qt build can negotiate is advertised.

    \sa setAcceptedCompressionAlgorithms(), supportedCompressionAlgorithms()
*/
QtGrpc::CompressionAlgorithms QGrpcChannelOptions::acceptedCompressionAlgorithms() const
{
    Q_D(const QGrpcChannelOptions);
    return d->acceptedCompressionAlgorithms;
}

/*!
    \since 6.12

    Sets the accepted compression \a algorithms and returns a reference to the
    updated object.

    \l{QtGrpc::CompressionAlgorithm::}{Identity} is always added implicitly,
    because the gRPC specification requires clients to accept uncompressed
    messages.

    If \c{{QtGrpc::CompressionAlgorithm::Identity}} is the only accepted
    algorithm, compressed responses are refused. Adding additional flags allows
    the server to respond using those compression algorithms.

    Only algorithms supported by this Qt build and included in \a algorithms
    are advertised to the server as accepted response compression methods.
    Flags that are not present in \l{supportedCompressionAlgorithms()} are
    silently discarded.

    \sa acceptedCompressionAlgorithms(), supportedCompressionAlgorithms()
*/
QGrpcChannelOptions &
QGrpcChannelOptions::setAcceptedCompressionAlgorithms(QtGrpc::CompressionAlgorithms algorithms)
{
    algorithms &= supportedCompressionAlgorithms();
    algorithms.setFlag(CompressionAlgorithm::Identity);
    if (d_ptr->acceptedCompressionAlgorithms == algorithms)
        return *this;
    d_ptr.detach();
    Q_D(QGrpcChannelOptions);
    d->acceptedCompressionAlgorithms = algorithms;
    return *this;
}

/*!
    \since 6.8

    Sets the serialization \a format for the channel and returns a reference to
    the updated object.
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
    Returns the timeout duration that is used to calculate the deadline for the
    channel.

    If this field is unset, returns an empty \c {std::optional}.
*/
std::optional<std::chrono::milliseconds> QGrpcChannelOptions::deadlineTimeout() const noexcept
{
    Q_D(const QGrpcChannelOptions);
    return d->deadlineTimeout();
}

/*!
    \since 6.8

    Returns the serialization format used by the channel.

    If this field is unset, returns a \l {QtGrpc::SerializationFormat::}
    {Default} constructed serialization format.
 */
QGrpcSerializationFormat QGrpcChannelOptions::serializationFormat() const
{
    Q_D(const QGrpcChannelOptions);
    return d->serializationFormat;
}

#if QT_CONFIG(ssl)
/*!
    Sets the \a sslConfiguration for the channel and returns a reference to the
    updated object.
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
    Returns the SSL configuration for the channel.

    If this field is unset, returns an empty \c {std::optional}.
*/
std::optional<QSslConfiguration> QGrpcChannelOptions::sslConfiguration() const
{
    Q_D(const QGrpcChannelOptions);
    return d->sslConfiguration;
}
#endif

/*
//! [compares]
    Returns \c true if the \l{acceptedCompressionAlgorithms},
    \l{deadlineTimeout}, \l{filterServerMetadata},
    \l{maximumReceiveMessageSize}, \l{metadata(QtGrpc::MultiValue_t)},
    \l{requestCompression}, \l{serializationFormat}, \l{initialReconnectBackoff},
    \l{maximumReconnectBackoff}, \l{connectTimeout}
    and \l{sslConfiguration} in \a lhs and \a rhs are
//! [compares]
*/
bool comparesEqual(const QGrpcChannelOptions &lhs, const QGrpcChannelOptions &rhs)
{
    return lhs.acceptedCompressionAlgorithms() == rhs.acceptedCompressionAlgorithms()
        && lhs.deadlineTimeout() == rhs.deadlineTimeout()
        && lhs.filterServerMetadata() == rhs.filterServerMetadata()
        && lhs.maximumReceiveMessageSize() == rhs.maximumReceiveMessageSize()
        && lhs.metadata(QtGrpc::MultiValue) == rhs.metadata(QtGrpc::MultiValue)
        && lhs.requestCompression() == rhs.requestCompression()
        && lhs.serializationFormat() == rhs.serializationFormat()
        && lhs.initialReconnectBackoff() == rhs.initialReconnectBackoff()
        && lhs.maximumReconnectBackoff() == rhs.maximumReconnectBackoff()
        && lhs.connectTimeout() == rhs.connectTimeout()
#if QT_CONFIG(ssl)
        && lhs.sslConfiguration() == rhs.sslConfiguration()
#endif
        ;
}

/*!
    \since 6.11
    \fn bool QGrpcChannelOptions::operator==(const QGrpcChannelOptions &lhs, const QGrpcChannelOptions &rhs)
    \include qgrpcchanneloptions.cpp compares
    equal.
*/

/*!
    \since 6.11
    \fn bool QGrpcChannelOptions::operator!=(const QGrpcChannelOptions &lhs, const QGrpcChannelOptions &rhs)
    \include qgrpcchanneloptions.cpp compares
    not equal.
*/

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
    debug << "QGrpcChannelOptions(acceptedCompressionAlgorithms: "
          << chOpts.acceptedCompressionAlgorithms() << ", deadline: " << chOpts.deadlineTimeout()
          << ", metadata: " << chOpts.metadata(QtGrpc::MultiValue)
          << ", filterServerMetadata: " << chOpts.filterServerMetadata()
          << ", maximumReceiveMessageSize: " << chOpts.maximumReceiveMessageSize()
          << ", requestCompression: " << chOpts.requestCompression()
          << ", serializationFormat: " << chOpts.serializationFormat().suffix()
          << ", initialReconnectBackoff: " << chOpts.initialReconnectBackoff().count() << "ms"
          << ", maximumReconnectBackoff: " << chOpts.maximumReconnectBackoff().count() << "ms"
          << ", connectTimeout: " << chOpts.connectTimeout().count() << "ms"
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
