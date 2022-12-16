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
    \fn template <typename T> T read();

    Reads message from raw byte array stored in QGrpcCallReply. Returns copy of deserialized
    message or non-initialized message in case of exceptional situation.
 */

/*!
    \fn void finished();
    The signal indicates the end of communication for this call.
    If signal emitted by stream this means that stream is successfully closed either by client
    or server.
 */

/*!
    \fn void errorOccurred(const QGrpcStatus &status);

    The signal is emitted when error with \a status occurs in channel or during serialization.
 */

QGrpcOperation::QGrpcOperation(QAbstractGrpcClient *parent) : QObject(parent)
{
}

QGrpcOperation::~QGrpcOperation()
{
}

/*!
    Interface for implementation of QAbstractGrpcChannel. Should be used to write raw data from
   channel to reply \a data Raw data received from channel.
 */
void QGrpcOperation::setData(const QByteArray &data)
{
    m_data = data;
}

QT_END_NAMESPACE
