// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <qtgrpcglobal_p.h>

#include "qgrpccalloptions.h"

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

/*!
    \class QGrpcCallOptions
    \inmodule QtGrpc
    \brief The QGrpcCallOptions is an storage class used by set additional call options.

    QGrpcCallOptions provides a set of functions to access the call options
    that are used by gRPC channels to communicate with the services.
*/

struct QGrpcCallOptionsPrivate
{
public:
    QGrpcCallOptionsPrivate() : credentials(nullptr) { }

    std::optional<QUrl> host;
    std::optional<std::chrono::milliseconds> deadline;
    std::shared_ptr<QGrpcCallCredentials> credentials;
    std::optional<qint64> maxRetryAttempts;
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
    *dPtr = *other.dPtr;
    return *this;
}

/*!
    Sets deadline value with \a deadline and returns updated QGrpcCallOptions object.
*/
QGrpcCallOptions &QGrpcCallOptions::withDeadline(std::chrono::milliseconds deadline)
{
    dPtr->deadline = deadline;
    return *this;
}

/*!
    Sets call credentials with \a credentials and returns updated QGrpcCallOptions object.
*/
QGrpcCallOptions &QGrpcCallOptions::withCredentials(
        std::shared_ptr<QGrpcCallCredentials> credentials)
{
    dPtr->credentials = credentials;
    return *this;
}

/*!
    Sets maximum retry attempts value with \a maxRetryAttempts and returns updated QGrpcCallOptions object.
*/
QGrpcCallOptions &QGrpcCallOptions::withMaxRetryAttempts(qint64 maxRetryAttempts)
{
    dPtr->maxRetryAttempts = maxRetryAttempts;
    return *this;
}

/*!
    Returns deadline value for a call.

    If value was not set returns empty std::optional.
*/
std::optional<std::chrono::milliseconds> QGrpcCallOptions::deadline() const
{
    return dPtr->deadline;
}

/*!
    Returns credentials for a call.

    If value was not set returns empty std::optional.
*/
std::optional<QGrpcCredentialMap> QGrpcCallOptions::credentials() const
{
    return dPtr->credentials ? (*dPtr->credentials)() : std::optional<QGrpcCredentialMap>();
}

/*!
    Returns maximum retry attempts value for a call.

    If value was not set returns empty std::optional.
*/
std::optional<qint64> QGrpcCallOptions::maxRetryAttempts() const
{
    return dPtr->maxRetryAttempts;
}

QT_END_NAMESPACE
