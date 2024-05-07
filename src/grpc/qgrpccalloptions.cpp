// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/private/qtgrpcglobal_p.h>
#include <QtGrpc/qgrpccalloptions.h>

#include <QtCore/qdebug.h>

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

class QGrpcCallOptionsPrivate
{
public:
    std::optional<QGrpcDuration> deadline;
    QGrpcMetadata metadata;
};

/*!
    Constructs an empty QGrpcCallOptions object.
*/
QGrpcCallOptions::QGrpcCallOptions() : dPtr(std::make_unique<QGrpcCallOptionsPrivate>())
{
}

/*!
    Destroys the QGrpcCallOptions object.
*/
QGrpcCallOptions::~QGrpcCallOptions() = default;

/*!
    Construct a copy of QGrpcCallOptions with \a other object.
*/
QGrpcCallOptions::QGrpcCallOptions(const QGrpcCallOptions &other)
    : dPtr(std::make_unique<QGrpcCallOptionsPrivate>(*other.dPtr))
{
}

/*!
    Assigns \a other to this QGrpcCallOptions and returns a reference to this
    QGrpcCallOptions.
*/
QGrpcCallOptions &QGrpcCallOptions::operator=(const QGrpcCallOptions &other)
{
    if (this != &other)
        *dPtr = *other.dPtr;
    return *this;
}

QGrpcCallOptions::QGrpcCallOptions(QGrpcCallOptions &&other) noexcept = default;

QGrpcCallOptions &QGrpcCallOptions::operator=(QGrpcCallOptions &&other) noexcept = default;

/*!
    Sets deadline value with \a deadline and returns updated QGrpcCallOptions object.
*/
QGrpcCallOptions &QGrpcCallOptions::withDeadline(QGrpcDuration deadline)
{
    dPtr->deadline = deadline;
    return *this;
}

/*!
    Sets \a metadata for a call and returns updated QGrpcCallOptions object.

    For HTTP2-based channels, \a metadata is converted into HTTP/2 headers, that
    added to the corresponding HTTP/2 request.
*/
QGrpcCallOptions &QGrpcCallOptions::withMetadata(const QGrpcMetadata &metadata)
{
    dPtr->metadata = metadata;
    return *this;
}

/*!
    Sets \a metadata for a call and returns updated QGrpcCallOptions object.

    \sa withMetadata()
*/
QGrpcCallOptions &QGrpcCallOptions::withMetadata(QGrpcMetadata &&metadata) noexcept
{
    dPtr->metadata = std::move(metadata);
    return *this;
}

/*!
    Returns deadline value for a call.

    Deadline value controls the maximum execution time of an call or a stream.
    This value overrides value set by QGrpcChannelOptions::deadline()
    for a specific call or stream.

    If value was not set returns empty std::optional.
*/
std::optional<QGrpcDuration> QGrpcCallOptions::deadline() const noexcept
{
    return dPtr->deadline;
}

/*!
    \fn const QGrpcMetadata &QGrpcCallOptions::metadata() const & noexcept
    \fn QGrpcMetadata QGrpcCallOptions::metadata() && noexcept

    Returns metadata used for a call.

    If value was not set returns empty QGrpcMetadata.
*/
const QGrpcMetadata &QGrpcCallOptions::metadata() const & noexcept
{
    return dPtr->metadata;
}

QGrpcMetadata QGrpcCallOptions::metadata() && noexcept
{
    return std::move(dPtr->metadata);
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \since 6.8
    \fn QDebug QGrpcCallOptions::operator<<(QDebug debug, const QGrpcCallOptions &callOpts)
    Writes \a callOpts to the specified stream \a debug.
*/
QDebug operator<<(QDebug debug, const QGrpcCallOptions &callOpts)
{
    QDebugStateSaver save(debug);
    debug.nospace();
    debug.noquote();
    debug << "QGrpcCallOptions(deadline: " << callOpts.deadline()
          << ", metadata: " << callOpts.metadata() << ')';
    return debug;
}
#endif

QT_END_NAMESPACE
