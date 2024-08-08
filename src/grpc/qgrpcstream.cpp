// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpcoperationcontext.h>
#include <QtGrpc/qgrpcstream.h>

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcServerStream
    \inmodule QtGrpc
    \since 6.7

    \brief The QGrpcServerStream class provides the interface to access the
    server-side gRPC stream functionality from gRPC client side.

    The QGrpcServerStream object is owned by the client object that created it.
*/

/*!
    \fn void QGrpcServerStream::messageReceived()

    The signal is emitted when the stream receives an updated value from server.
*/

QGrpcServerStream::QGrpcServerStream(std::shared_ptr<QGrpcOperationContext> operationContext,
                                     QObject *parent)
    : QGrpcOperation(std::move(operationContext), parent)
{
    QObject::connect(&QGrpcOperation::context(), &QGrpcOperationContext::messageReceived, this,
                     &QGrpcServerStream::messageReceived);
}

/*!
    Destroys the QGrpcServerStream object.
*/
QGrpcServerStream::~QGrpcServerStream() = default;

bool QGrpcServerStream::event(QEvent *event)
{
    return QGrpcOperation::event(event);
}

/*!
    \class QGrpcClientStream
    \inmodule QtGrpc
    \since 6.7

    \brief The QGrpcClientStream class provides the interface to access the
    client-side gRPC stream functionality from gRPC client side.
*/

QGrpcClientStream::QGrpcClientStream(std::shared_ptr<QGrpcOperationContext> operationContext,
                                     QObject *parent)
    : QGrpcOperation(std::move(operationContext), parent)
{
}

/*!
    Destroys the QGrpcClientStream object.
*/
QGrpcClientStream::~QGrpcClientStream() = default;

/*!
    Serializes \a message and sends it to the server.
*/
void QGrpcClientStream::writeMessage(const QProtobufMessage &message)
{
    QByteArray data = QGrpcOperation::context().serializer()->serialize(&message);
    emit QGrpcOperation::context().writeMessageRequested(data);
}

/*!
    \since 6.8
    Ends the stream from the client side (half-closing). The server is still allowed to send
    responses after this call.
*/
void QGrpcClientStream::writesDone()
{
    emit QGrpcOperation::context().writesDoneRequested();
}

bool QGrpcClientStream::event(QEvent *event)
{
    return QGrpcOperation::event(event);
}

/*!
    \class QGrpcBidiStream
    \inmodule QtGrpc
    \since 6.7

    \brief The QGrpcBidiStream class provides the interface to access the
    bidirectional gRPC stream functionality from gRPC client side.
*/

/*!
    \fn void QGrpcBidiStream::messageReceived()

    The signal is emitted when the stream receives an updated value from server.
*/

QGrpcBidiStream::QGrpcBidiStream(std::shared_ptr<QGrpcOperationContext> operationContext,
                                 QObject *parent)
    : QGrpcOperation(std::move(operationContext), parent)
{
    QObject::connect(&QGrpcOperation::context(), &QGrpcOperationContext::messageReceived, this,
                     &QGrpcBidiStream::messageReceived);
}

/*!
    Destroys the QGrpcBidiStream object.
*/
QGrpcBidiStream::~QGrpcBidiStream() = default;

/*!
    Serializes \a message and sends it to the server.
*/
void QGrpcBidiStream::writeMessage(const QProtobufMessage &message)
{
    QByteArray data = QGrpcOperation::context().serializer()->serialize(&message);
    emit QGrpcOperation::context().writeMessageRequested(data);
}

/*!
    \since 6.8
    Ends the stream from the client side (half-closing). The server is still allowed to send
    responses after this call.
*/
void QGrpcBidiStream::writesDone()
{
    emit QGrpcOperation::context().writesDoneRequested();
}

bool QGrpcBidiStream::event(QEvent *event)
{
    return QGrpcOperation::event(event);
}

QT_END_NAMESPACE

#include "moc_qgrpcstream.cpp"
