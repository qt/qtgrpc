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
    \brief The QGrpcServerStream class provides access in handling server-streaming RPCs.

    The QGrpcServerStream class provides the interface for handling
    server-streaming remote procedure calls (RPCs), which is one of the four
    \gRPC \l{Service Methods}{service methods}.

    For a high-level overview, refer to the \l {Server Streaming} {Qt GRPC
    Client Guide}.

    \include qtgrpc-shared.qdocinc rpc-lifetime-note
*/

/*!
    \fn void QGrpcServerStream::messageReceived()

//! [message-received-desc]
    This signal is emitted when the streaming RPC has received a new message
    from the server. The read() methods can then be used to deserialize the
    received message.
//! [message-received-desc]
*/

/*!
    \since 6.13
    \fn void QGrpcClientStream::messageWritten()

//! [message-written-desc]
    This signal is emitted when the channel has passed an outgoing message on
    to the transport: once for the initial request message and once per
    writeMessage() call, in write order.

    QGrpcHttp2Channel emits it after the message has been fully written to the
    connection, which respects HTTP/2 flow control; when the server stops
    consuming data, the emission is delayed until the server grants more send
    window.

    Messages queue up without limit when they are written faster than the
    network transmits them. To bound the queue, write the next message only
    after this signal reports the previous one written, matching the
    write-then-wait discipline commonly seen in \gRPC implementations.

    For medium and large messages this one-at-a-time pacing costs no throughput,
    and usually improves it by avoiding a large backlog. For very small,
    high-rate messages the per-message wait can dominate instead; there, keep a
    few messages in flight: write a small batch up front and write one more on
    each emission.

    Once finished() has been emitted the signal is not emitted anymore, even
    for messages that were still in transmission when the operation ended.
//! [message-written-desc]

    \sa writeMessage()
*/

/*!
    \internal

    Constructs a new QGrpcServerStream from QGrpcClientBase.

    This is indirectly called by the generated client interface.

    \sa QGrpcClientBase::serverStream QAbstractGrpcChannel::serverStream
*/
QGrpcServerStream::QGrpcServerStream(QLatin1StringView service, QLatin1StringView method,
                                     const QGrpcCallOptions &options,
                                     std::weak_ptr<QAbstractGrpcChannel> channel,
                                     PrivateConstructor /* unused */)
    : QGrpcOperation({ service, method, QtGrpc::RpcType::ServerStreaming }, options,
                     std::move(channel))
{
    QObject::connect(&QGrpcOperation::context(), &QGrpcOperationContext::messageReceived, this,
                     &QGrpcServerStream::messageReceived);
}

/*!
    Destroys the QGrpcServerStream.
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
    \brief The QGrpcClientStream class provides access in handling client-streaming RPCs.

    The QGrpcClientStream class provides the interface for handling
    client-streaming remote procedure calls (RPCs), which is one of the four
    \gRPC \l{Service Methods}{service methods}.

    For a high-level overview, refer to the \l {Client Streaming} {Qt GRPC
    Client Guide}.

    \include qtgrpc-shared.qdocinc rpc-lifetime-note
*/

/*!
    \internal

    Constructs a new QGrpcClientStream from QGrpcClientBase.

    This is indirectly called by the generated client interface.

    \sa QGrpcClientBase::clientStream QAbstractGrpcChannel::clientStream
*/
QGrpcClientStream::QGrpcClientStream(QLatin1StringView service, QLatin1StringView method,
                                     const QGrpcCallOptions &options,
                                     std::weak_ptr<QAbstractGrpcChannel> channel,
                                     PrivateConstructor /* unused */)
    : QGrpcOperation({ service, method, QtGrpc::RpcType::ClientStreaming }, options,
                     std::move(channel))
{
    QObject::connect(&QGrpcOperation::context(), &QGrpcOperationContext::messageWritten, this,
                     &QGrpcClientStream::messageWritten);
}

/*!
    Destroys the QGrpcClientStream.
*/
QGrpcClientStream::~QGrpcClientStream() = default;

/*!
//! [write-message-desc]
    Serializes \a message and sends it to the server.

    Messages are queued and handed to the transport. The queue is unbounded, so
    pace high-rate writes using the messageWritten() signal.
//! [write-message-desc]

    \sa messageWritten()
*/
void QGrpcClientStream::writeMessage(const QProtobufMessage &message)
{
    QGrpcOperation::writeMessage(message);
}

/*!
    \since 6.8
//! [writes-done-desc]
    Ends the stream from the client side (half-closing). The server is still allowed to send
    responses after this call.
//! [writes-done-desc]
*/
void QGrpcClientStream::writesDone()
{
    QGrpcOperation::writesDone();
}

bool QGrpcClientStream::event(QEvent *event)
{
    return QGrpcOperation::event(event);
}

/*!
    \class QGrpcBidiStream
    \inmodule QtGrpc
    \since 6.7
    \brief The QGrpcBidiStream class provides access in handling
    bidirectional-streaming RPCs.

    The QGrpcBidiStream class provides the interface for handling
    bidirectional-streaming remote procedure calls (RPCs), which is one of the
    four \gRPC \l{Service Methods}{service methods}.

    For a high-level overview, refer to the \l {Bidirectional Streaming} {Qt
    GRPC Client Guide}.

    \include qtgrpc-shared.qdocinc rpc-lifetime-note
*/

/*!
    \fn void QGrpcBidiStream::messageReceived()

    \include qgrpcstream.cpp message-received-desc
*/

/*!
    \since 6.13
    \fn void QGrpcBidiStream::messageWritten()

    \include qgrpcstream.cpp message-written-desc

    \sa writeMessage()
*/

/*!
    \internal

    Constructs a new QGrpcBidiStream from QGrpcClientBase.

    This is indirectly called by the generated client interface.

    \sa QGrpcClientBase::bidiStream QAbstractGrpcChannel::bidiStream
*/
QGrpcBidiStream::QGrpcBidiStream(QLatin1StringView service, QLatin1StringView method,
                                 const QGrpcCallOptions &options,
                                 std::weak_ptr<QAbstractGrpcChannel> channel,
                                 PrivateConstructor /* unused */)
    : QGrpcOperation({ service, method, QtGrpc::RpcType::BidiStreaming }, options,
                     std::move(channel))
{
    QObject::connect(&QGrpcOperation::context(), &QGrpcOperationContext::messageReceived,
                     this, &QGrpcBidiStream::messageReceived);
    QObject::connect(&QGrpcOperation::context(), &QGrpcOperationContext::messageWritten, this,
                     &QGrpcBidiStream::messageWritten);
}

/*!
    Destroys the QGrpcBidiStream.
*/
QGrpcBidiStream::~QGrpcBidiStream() = default;

/*!
    \include qgrpcstream.cpp write-message-desc

    \sa messageWritten()
*/
void QGrpcBidiStream::writeMessage(const QProtobufMessage &message)
{
    QGrpcOperation::writeMessage(message);
}

/*!
    \since 6.8
    \include qgrpcstream.cpp writes-done-desc
*/
void QGrpcBidiStream::writesDone()
{
    QGrpcOperation::writesDone();
}

bool QGrpcBidiStream::event(QEvent *event)
{
    return QGrpcOperation::event(event);
}

QT_END_NAMESPACE

#include "moc_qgrpcstream.cpp"
