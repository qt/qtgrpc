// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpcstream.h"

#include "qgrpcchanneloperation.h"

#include <QtCore/QThread>

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcServerStream
    \inmodule QtGrpc

    \brief The QGrpcServerStream class provides the interface to access the
    server-side gRPC stream functionality from gRPC client side.

    The QGrpcServerStream object is owned by the client object that created it.
*/

/*!
    \fn void QGrpcServerStream::messageReceived()

    The signal is emitted when the stream receives an updated value from server.
*/

QGrpcServerStream::QGrpcServerStream(std::shared_ptr<QGrpcChannelOperation> channelOperation,
                                     std::shared_ptr<QAbstractProtobufSerializer> serializer)
    : QGrpcOperation(std::move(channelOperation), std::move(serializer))
{
    QObject::connect(QGrpcOperation::channelOperation(), &QGrpcChannelOperation::dataReady, this,
                     [this] { emit messageReceived(); });
}

/*!
    Destroys the QGrpcServerStream object.
*/
QGrpcServerStream::~QGrpcServerStream() = default;

QT_END_NAMESPACE

#include "moc_qgrpcstream.cpp"
