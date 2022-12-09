// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <qtgrpcglobal_p.h>

#include "qgrpcoperation.h"

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcOperation
    \inmodule QtGrpc
    \brief The QGrpcOperation class implements common logic to
           handle communication in Grpc channel.
*/

/*!
    \fn template <typename T> T QGrpcOperation::read() const;

    Reads message from raw byte array stored in QGrpcCallReply.

    Returns a copy of the deserialized message or, on failure,
    a default-constructed message.
*/

/*!
    \fn void QGrpcOperation::finished();

    This signal indicates the end of communication for this call.

    If signal emitted by stream this means that stream is successfully
    closed either by client or server.
*/

/*!
    \fn void QGrpcOperation::errorOccurred(const QGrpcStatus &status);

    This signal indicates the error occurred during serialization.

    This signal is emitted when error with \a status occurs in channel
    or during serialization.
*/

QGrpcOperation::QGrpcOperation(QAbstractGrpcClient *parent) : QObject(parent)
{
}

QGrpcOperation::~QGrpcOperation() = default;

/*!
    Interface for implementation of QAbstractGrpcChannel.

    Should be used to write raw data from channel to reply \a data raw data
    received from channel.
 */
void QGrpcOperation::setData(const QByteArray &data)
{
    m_data = data;
}

/*!
    Interface for implementation of QAbstractGrpcChannel.

    Should be used to write raw data from channel to reply \a data raw data
    received from channel.
*/
void QGrpcOperation::setData(QByteArray &&data)
{
    m_data = std::move(data);
}

QT_END_NAMESPACE

#include "moc_qgrpcoperation.cpp"
