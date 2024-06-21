// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpcoperationcontext.h>
#include <QtGrpc/qgrpcstream.h>

#include <QtCore/qthread.h>

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
    QObject::connect(QGrpcOperation::operationContext(), &QGrpcOperationContext::messageReceived,
                     this, [this] { emit messageReceived(); });
}

/*!
    Destroys the QGrpcServerStream object.
*/
QGrpcServerStream::~QGrpcServerStream() = default;

/*!
    \class QGrpcClientStream
    \inmodule QtGrpc
    \since 6.7

    \brief The QGrpcClientStream class provides the interface to access the
    client-side gRPC stream functionality from gRPC client side.
*/

/*!
    \fn template<typename T> void QGrpcClientStream::writeMessage(const T &message)

    Serializes \a message and sends it to the server.
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
    \since 6.8
    Serializes \a message and sends it to the server.
*/
void QGrpcClientStream::writeMessage(const QProtobufMessage *message)
{
    QByteArray data = QGrpcOperation::operationContext()->serializer()->serialize(message);
    emit QGrpcOperation::operationContext()->writeMessageRequested(data);
}

/*!
    \since 6.8
    Ends the stream from the client side (half-closing). The server is still allowed to send
    responses after this call.
*/
void QGrpcClientStream::writesDone()
{
    emit QGrpcOperation::operationContext()->writesDoneRequested();
}

/*!
    \class QGrpcBidirStream
    \inmodule QtGrpc
    \since 6.7

    \brief The QGrpcBidirStream class provides the interface to access the
    bidirectional gRPC stream functionality from gRPC client side.
*/

/*!
    \fn void QGrpcBidirStream::messageReceived()

    The signal is emitted when the stream receives an updated value from server.
*/

/*!
    \fn template<typename T> void QGrpcBidirStream::writeMessage(const T &message)

    Serializes \a message and sends it to the server.
*/

QGrpcBidirStream::QGrpcBidirStream(std::shared_ptr<QGrpcOperationContext> operationContext,
                                   QObject *parent)
    : QGrpcOperation(std::move(operationContext), parent)
{
    QObject::connect(QGrpcOperation::operationContext(), &QGrpcOperationContext::messageReceived,
                     this, [this] { emit messageReceived(); });
}

/*!
    Destroys the QGrpcBidirStream object.
*/
QGrpcBidirStream::~QGrpcBidirStream() = default;

/*!
    \since 6.8
    Serializes \a message and sends it to the server.
*/
void QGrpcBidirStream::writeMessage(const QProtobufMessage *message)
{
    QByteArray data = QGrpcOperation::operationContext()->serializer()->serialize(message);
    emit QGrpcOperation::operationContext()->writeMessageRequested(data);
}

/*!
    \since 6.8
    Ends the stream from the client side (half-closing). The server is still allowed to send
    responses after this call.
*/
void QGrpcBidirStream::writesDone()
{
    emit QGrpcOperation::operationContext()->writesDoneRequested();
}

QT_END_NAMESPACE

#include "moc_qgrpcstream.cpp"
