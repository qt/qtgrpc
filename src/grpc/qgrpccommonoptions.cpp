// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/private/qgrpccommonoptions_p.h>

QT_BEGIN_NAMESPACE

#if QT_DEPRECATED_SINCE(6, 13)

namespace
{

inline QHash<QByteArray, QByteArray> mergeHash(const QMultiHash<QByteArray, QByteArray> &multiHash)
{
    QHash<QByteArray, QByteArray> out;
    for (const auto &key : multiHash.uniqueKeys())
        out.insert(key, multiHash.value(key));
    return out;
}

} // namespace

/*!
//! [metadata]
    Returns the metadata. If this field is unset, returns empty
    metadata.
//! [metadata]
*/
const QHash<QByteArray, QByteArray> &QGrpcCommonOptions::metadata() const &
{
    m_deprecatedQHashRefUsed = true;
    if (m_metadataMulti != m_metadata)
        m_metadata = mergeHash(m_metadataMulti);
    return m_metadata;
}
QHash<QByteArray, QByteArray> QGrpcCommonOptions::metadata() &&
{
    if (m_metadataMulti != m_metadata)
        m_metadata = mergeHash(m_metadataMulti);
    return std::move(m_metadata);
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

    When using QGrpcHttp2Channel, the metadata is converted to HTTP/2 headers
    and added to the gRPC request.
//! [set-metadata]
*/
void QGrpcCommonOptions::setMetadata(const QHash<QByteArray, QByteArray> &md)
{
    if (m_deprecatedQHashRefUsed)
        m_metadata = md;
    m_metadataMulti = QMultiHash<QByteArray, QByteArray>(md);
}
void QGrpcCommonOptions::setMetadata(QHash<QByteArray, QByteArray> &&md)
{
    if (m_deprecatedQHashRefUsed)
        m_metadata = md;
    m_metadataMulti = QMultiHash<QByteArray, QByteArray>(std::move(md));
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
QGrpcCommonOptions::metadata(QtGrpc::MultiValue_t /*tag*/) const &
{
    return m_metadataMulti;
}
QMultiHash<QByteArray, QByteArray> QGrpcCommonOptions::metadata(QtGrpc::MultiValue_t /*tag*/) &&
{
    return std::move(m_metadataMulti);
}

/*!
//! [set-metadata-multi]
    \include qgrpccommonoptions.cpp set-metadata
    Multiple values per key are supported.
//! [set-metadata-multi]
*/
void QGrpcCommonOptions::setMetadata(const QMultiHash<QByteArray, QByteArray> &md)
{
    m_metadataMulti = md;
#if QT_DEPRECATED_SINCE(6, 13)
    if (m_deprecatedQHashRefUsed)
        m_metadata = mergeHash(m_metadataMulti);
#endif
}

void QGrpcCommonOptions::setMetadata(QMultiHash<QByteArray, QByteArray> &&md)
{
    m_metadataMulti = std::move(md);
#if QT_DEPRECATED_SINCE(6, 13)
    if (m_deprecatedQHashRefUsed)
        m_metadata = mergeHash(m_metadataMulti);
#endif
}

/*!
//! [add-metadata]
    \since 6.10

    Adds \a key and \a value to the \l metadata if the exact pair is not already contained.
    The same key may appear multiple times with different values.

    \sa {metadata(QtGrpc::MultiValue_t)}{metadata()} {setMetadata(const
    QMultiHash<QByteArray, QByteArray>&)}{setMetadata()}
//! [add-metadata]
*/
void QGrpcCommonOptions::addMetadata(QByteArray &&key, QByteArray &&value)
{
#if QT_DEPRECATED_SINCE(6, 13)
    if (m_deprecatedQHashRefUsed)
        m_metadata.insertOrAssign(key, value);
#endif
    m_metadataMulti.emplace(std::move(key), std::move(value));
}

QT_END_NAMESPACE
