// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/private/qgrpccommonoptions_p.h>

QT_BEGIN_NAMESPACE

#if QT_DEPRECATED_SINCE(6, 13)

QHash<QByteArray, QByteArray>
QtGrpcPrivate::toHash(const QMultiHash<QByteArray, QByteArray> &multiHash)
{
    // Transform a QMultiHash into a QHash by keeping only the first value for each key.
    // The first value will be the newest one when iterating the multi-hash.
    QHash<QByteArray, QByteArray> out;
    out.reserve(multiHash.size());
    for (const auto &[k, v] : multiHash.asKeyValueRange())
        out.try_emplace(k, v);
    return out;
}

/*!
//! [metadata]
    Returns the metadata. If this field is unset, returns empty
    metadata.

    \include qtgrpc-shared.qdocinc metadata-note
//! [metadata]
*/
const QHash<QByteArray, QByteArray> &QGrpcCommonOptions::metadata() const & noexcept
{
    return m_deprecatedMetadata;
}
QHash<QByteArray, QByteArray> QGrpcCommonOptions::metadata() &&
{
    return std::move(m_deprecatedMetadata);
}

/*!
    \internal
    \class QGrpcCommonOptions
    \inmodule QtGrpc
    \brief Shared implementation of QGrpcCallOptions and QGrpcChannelOptions
    functionality.
*/

/*!
//! [set-metadata]
    Sets the \a metadata and returns a reference to the updated object.

    \include qtgrpc-shared.qdocinc metadata-note
//! [set-metadata]
*/
void QGrpcCommonOptions::setMetadata(const QHash<QByteArray, QByteArray> &md)
{
    m_deprecatedMetadata = md;
    m_metadata = QMultiHash<QByteArray, QByteArray>(md);
}
void QGrpcCommonOptions::setMetadata(QHash<QByteArray, QByteArray> &&md)
{
    m_deprecatedMetadata = std::move(md);
    m_metadata = QMultiHash<QByteArray, QByteArray>(m_deprecatedMetadata);
}

#endif // QT_DEPRECATED_SINCE(6, 13)

/*!
//! [deadline-timeout]
    Returns the timeout duration that is used to calculate the deadline for RPCs.

    If this field is unset, returns an empty \c {std::optional}.
//! [deadline-timeout]
*/
std::optional<std::chrono::milliseconds> QGrpcCommonOptions::deadlineTimeout() const noexcept
{
    return m_timeout;
}

/*!
//! [set-deadline-timeout]
    Sets the \a timeout and returns a reference to the updated object.

    A deadline sets the limit for how long a client is willing to wait for a
    response from a server. The actual deadline is computed by adding the \a
    timeout to the start time of the RPC.

    The deadline applies to the entire lifetime of an RPC, which includes
    receiving the final QGrpcStatus for a previously started call and can thus
    be unwanted for (long-lived) streams.
//! [set-deadline-timeout]
*/
void QGrpcCommonOptions::setDeadlineTimeout(std::chrono::milliseconds t)
{
    m_timeout = t;
}

/*!
//! [metadata-multi]
    \include qgrpccommonoptions.cpp metadata
    Multiple values per key are supported.

    \code
        const auto &md = opts.metadata(QtGrpc::MultiValue);
    \endcode
//! [metadata-multi]
*/
const QMultiHash<QByteArray, QByteArray> &
QGrpcCommonOptions::metadata(QtGrpc::MultiValue_t /*tag*/) const & noexcept
{
    return m_metadata;
}
QMultiHash<QByteArray, QByteArray> QGrpcCommonOptions::metadata(QtGrpc::MultiValue_t /*tag*/) &&
{
    return std::move(m_metadata);
}

/*!
//! [set-metadata-multi]
    \include qgrpccommonoptions.cpp set-metadata
    Multiple values per key are supported.
//! [set-metadata-multi]
*/
void QGrpcCommonOptions::setMetadata(const QMultiHash<QByteArray, QByteArray> &md)
{
    m_metadata = md;
#if QT_DEPRECATED_SINCE(6, 13)
    m_deprecatedMetadata = QtGrpcPrivate::toHash(m_metadata);
#endif
}

void QGrpcCommonOptions::setMetadata(QMultiHash<QByteArray, QByteArray> &&md)
{
    m_metadata = std::move(md);
#if QT_DEPRECATED_SINCE(6, 13)
    m_deprecatedMetadata = QtGrpcPrivate::toHash(m_metadata);
#endif
}

/*!
//! [add-metadata]
    \since 6.10

    Adds \a key and \a value to the \l metadata if the exact pair is not already contained.
    The same key may appear multiple times with different values.

    \include qtgrpc-shared.qdocinc metadata-note

    \sa {metadata(QtGrpc::MultiValue_t)}{metadata()} {setMetadata(const
    QMultiHash<QByteArray, QByteArray>&)}{setMetadata()}
//! [add-metadata]
*/
void QGrpcCommonOptions::addMetadata(QByteArray &&key, QByteArray &&value)
{
#if QT_DEPRECATED_SINCE(6, 13)
    m_deprecatedMetadata.insertOrAssign(key, value);
#endif
    m_metadata.emplace(std::move(key), std::move(value));
}

bool QGrpcCommonOptions::containsMetadata(QByteArrayView key, QByteArrayView value) const
{
    const auto &md = metadata(QtGrpc::MultiValue);
    auto [f, l] = md.equal_range(key);
    return std::find(f, l, value) != l;
}

/*!
//! [filterServerMetadata]
    \since 6.11

    Returns \c true if protocol-related server metadata is excluded from the
    result. Returns \c false to include all metadata received from the server.

    If this field is unset, returns an empty \c {std::optional}.

    \sa setFilterServerMetadata()
//! [filterServerMetadata]
*/
std::optional<bool> QGrpcCommonOptions::filterServerMetadata() const noexcept
{
    return m_filterServerMetadata;
}

/*!
//! [setFilterServerMetadata]
    \since 6.11

    If \a value is \c true, protocol-related metadata defined by the gRPC
    transport (such as reserved \c{grpc-} keys) will be excluded. If \a value
    is \c false, all metadata received from the server will be included.

    \sa filterServerMetadata()
//! [setFilterServerMetadata]
*/
void QGrpcCommonOptions::setFilterServerMetadata(bool value)
{
    m_filterServerMetadata = value;
}

/*!
//! [maximumReceiveMessageSize]
    \since 6.12

    Returns the maximum allowed size in bytes of an incoming gRPC message. The
    limit applies to the decoded gRPC frame payload, after transport-level
    decryption and any per-message decompression.

    If the value is not specified, returns an empty \c {std::optional}.

    By default, the value is not specified.

    \h2EnvFallback {QT_GRPC_MAXIMUM_RECEIVE_MESSAGE_SIZE} {4 MiB}

    \sa setMaximumReceiveMessageSize()
//! [maximumReceiveMessageSize]
*/
std::optional<quint64> QGrpcCommonOptions::maximumReceiveMessageSize() const noexcept
{
    return m_maximumReceiveMessageSize;
}

/*!
//! [setMaximumReceiveMessageSize]
    \since 6.12

    Sets the maximum incoming message \a size in bytes and returns a reference
    to the updated object.

    Messages exceeding this limit are rejected with
    \l{QtGrpc::}{StatusCode::ResourceExhausted} before their payload is
    buffered. The value is used verbatim, so a \a size of \c 0 accepts only
    empty messages.

    \note Concrete channel implementations may impose a hard upper bound on the
    effective limit. The \l QGrpcHttp2Channel transport caps the value at the
    maximum gRPC-over-HTTP/2 frame payload (\c{2^32 - 1}) bytes, or \c
    qsizetype's maximum minus the 5-byte gRPC frame header on 32-bit platforms.
    A larger configured value is clamped to that cap.

    \sa maximumReceiveMessageSize()
//! [setMaximumReceiveMessageSize]
*/
void QGrpcCommonOptions::setMaximumReceiveMessageSize(quint64 size)
{
    m_maximumReceiveMessageSize = size;
}

/*!
//! [requestCompression]
    \since 6.12

    Returns the compression algorithm used to compress outbound messages on
    this channel or call.

    If this field is unset, returns an empty \c {std::optional}.

    \sa setRequestCompression()
//! [requestCompression]
*/
std::optional<QtGrpc::CompressionAlgorithm> QGrpcCommonOptions::requestCompression() const noexcept
{
    return m_requestCompression;
}

/*!
//! [setRequestCompression]
    \since 6.12

    Sets the compression \a algorithm used to compress messages sent by the
    client and returns a reference to the updated object.

    When set, the algorithm is announced in the \c{grpc-encoding} request
    header. Each message is compressed independently; if compression does not
    reduce the payload size, the message is sent uncompressed with the
    \c{Compressed-Flag} set to \c 0 regardless.

    \note Compressing messages that contain both sensitive data and data that
    could potentially be influenced by an attacker may expose the connection to
    compression side-channel attacks such as CRIME or BREACH. If possible,
    avoid compressing such messages, or separate sensitive and untrusted data
    into different requests.

    \sa requestCompression()
//! [setRequestCompression]
*/
void QGrpcCommonOptions::setRequestCompression(QtGrpc::CompressionAlgorithm algorithm)
{
    m_requestCompression = algorithm;
}

QT_END_NAMESPACE
