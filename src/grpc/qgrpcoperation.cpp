// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/private/qgrpcoperation_p.h>
#include <QtGrpc/private/qtgrpclogging_p.h>
#include <QtGrpc/qabstractgrpcchannel.h>
#include <QtGrpc/qgrpcoperation.h>
#include <QtGrpc/qgrpcoperationcontext.h>

#include <QtProtobuf/private/qprotobufmessage_p.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qeventloop.h>
#include <QtCore/qpointer.h>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

void QGrpcOperationPrivate::asyncFinishInvalid(QGrpcStatus &&status)
{
    Q_ASSERT(!status.isOk());
    Q_Q(QGrpcOperation);
    state = State::Invalid;
    QMetaObject::invokeMethod(
        q,
        [qPtr = QPointer(q), s = std::move(status)] {
            if (!qPtr)
                return;
            qPtr->onFinished(s);
        },
        Qt::QueuedConnection);
}

std::optional<QByteArray>
QGrpcOperationPrivate::serializeInitialMessage(const QProtobufMessage &message)
{
    const auto serializer = operationContext->serializer();
    Q_ASSERT(state == State::Valid);
    Q_ASSERT(serializer);

    return serializer->serialize(&message);
}

QGrpcOperationPrivate::~QGrpcOperationPrivate()
    = default;

/*!
    \class QGrpcOperation
    \inmodule QtGrpc
    \brief The QGrpcOperation class provides common operations to handle the
    \gRPC communication from the client side.

    QGrpcOperation serves as the base class for the four \gRPC method types:
    QGrpcCallReply (unary calls), QGrpcServerStream (server streaming),
    QGrpcClientStream (client streaming), and QGrpcBidiStream (bidirectional
    streaming). It provides a common interface for interacting with these
    remote procedure calls (RPCs).

    Each QGrpcOperation corresponds to a specific RPC requested through the
    generated client interface.

    For a high-level overview, refer to the \l {clientguide} {Qt GRPC
    Client Guide}.
*/

/*!
    \fn void QGrpcOperation::finished(const QGrpcStatus &status)

    This signal is emitted when a previously started RPC has finished. The \a
    status provides additional information about the outcome of the RPC.

    After this signal is received, no further write or read operations should
    be performed on the operation object. At this point, it is safe to reuse or
    destroy the RPC object.

    \note This signal is emitted only once, and in most cases, you will want to
    disconnect right after receiving it to avoid issues, such as lambda
    captures not being destroyed after receiving the signal. An easy way to
    achieve this is by using the \l {Qt::} {SingleShotConnection} connection
    type. See \l {Single Shot RPCs} for further details.
*/

/*!
    \fn void QGrpcOperation::serverInitialMetadataReceived()
    \since 6.11

//![serverInitialMetadataReceived]
    This signal is emitted when the server responds with its initial metadata.
    Initial metadata from the server is sent to the client before the initial
    response of an RPC.

    After this signal is received, you can access the \l{serverInitialMetadata}.
//![serverInitialMetadataReceived]

    \include qtgrpc-shared.qdocinc http2-metadata-note

    \sa serverInitialMetadata()
*/

/*!
    \internal

    Constructs a QGrpcOperation from its subclass.

    This is indirectly called by the generated client interface.
*/
QGrpcOperation::QGrpcOperation(QtGrpc::RpcDescriptor descriptor, const QGrpcCallOptions &options,
                               const std::weak_ptr<QAbstractGrpcChannel> &channel)
    : QObject(*new QGrpcOperationPrivate())
{
    Q_D(QGrpcOperation);

    const auto lockedChannel = channel.lock();
    d->operationContext = new QGrpcOperationContext(std::move(descriptor), options,
                                                    lockedChannel ? lockedChannel->serializer()
                                                                  : nullptr,
                                                    this,
                                                    QGrpcOperationContext::PrivateConstructor{});

    [[maybe_unused]] bool valid = false;
    valid = connect(d->operationContext, &QGrpcOperationContext::messageReceived, this,
                    &QGrpcOperation::onMessageReceived);
    Q_ASSERT_X(valid, "QGrpcOperation::QGrpcOperation",
               "Unable to make connection to the 'messageReceived' signal");

    valid = connect(d->operationContext, &QGrpcOperationContext::finished, this,
                    &QGrpcOperation::onFinished);
    Q_ASSERT_X(valid, "QGrpcOperation::QGrpcOperation",
               "Unable to make connection to the 'finished' signal");

    valid = connect(d->operationContext, &QGrpcOperationContext::serverInitialMetadataReceived,
                    this, &QGrpcOperation::serverInitialMetadataReceived);
    Q_ASSERT_X(valid, "QGrpcOperation::QGrpcOperation",
               "Unable to make connection to the 'serverInitialMetadataReceived' signal");

    if (!lockedChannel) {
        d->asyncFinishInvalid({ QtGrpc::StatusCode::Aborted, tr("Channel is not available") });
        return;
    }
    d->channel = channel;

    if (!d->operationContext->serializer()) {
        d->asyncFinishInvalid({ QtGrpc::StatusCode::Aborted, tr("Serializer is not available") });
        return;
    }

    auto *chPriv = QAbstractGrpcChannelPrivate::get(lockedChannel.get());
    Q_ASSERT(chPriv);
    if (chPriv->threadId != QThread::currentThreadId()) {
        d->asyncFinishInvalid({ QtGrpc::StatusCode::Aborted, tr("RPC started from wrong thread") });
        return;
    }

    d->state = QGrpcOperationPrivate::State::Valid;
}

/*!
    Destroys the QGrpcOperation.
*/
QGrpcOperation::~QGrpcOperation() = default;

/*!
    \fn template <typename T, QtProtobuf::if_protobuf_message<T> = true> std::optional<T> QGrpcOperation::read() const

    Reads a message from a raw byte array stored within this operation object.

    Returns an optional deserialized message. On failure, \c {std::nullopt} is
    returned.

    \note This function only participates in overload resolution if \c T is a
    subclass of QProtobufMessage.

    \sa read(QProtobufMessage *)
*/

/*!
    \since 6.8

    Reads a message from a raw byte array stored within this operation object.

    The function writes the deserialized value to the \a message pointer.

    If the deserialization is successful, this function returns \c true.
    Otherwise, it returns \c false.

    \sa read()
*/
bool QGrpcOperation::read(QProtobufMessage *message) const
{
    if (!message) {
        qGrpcWarning("Read called on nullptr message");
        return false;
    }

    Q_D(const QGrpcOperation);
    const auto ser = d->operationContext->serializer();
    if (!ser) {
        qGrpcWarning("Serializer unavailable");
        return false;
    }

    if (auto responseMetaType = d->operationContext->responseMetaType(); responseMetaType.isValid()
        && QProtobufMessagePrivate::get(message)->metaObject != responseMetaType.metaObject()) {
        qGrpcWarning("Operation result meta type doesn't match the message meta type.");
    }

    if (!ser->deserialize(message, d->data)) {
        qGrpcWarning() << "Unable to deserialize message(" << qToUnderlying(ser->lastError()) <<"): "
                       << ser->lastErrorString();
        return false;
    }
    return true;
}

/*!
    Tries to cancel the RPC immediately. Successful cancellation cannot be
    guaranteed. Emits the \l finished signal with a \l {QtGrpc::StatusCode::}
    {Cancelled} status code.

    \sa QGrpcOperationContext::cancelRequested
*/
void QGrpcOperation::cancel()
{
    if (isFinished())
        return;
    Q_D(QGrpcOperation);
    emit d->operationContext->cancelRequested();
}

#if QT_DEPRECATED_SINCE(6, 13)

/*!
    \deprecated [6.13] Use serverInitialMetadata() and serverTrailingMetadata() instead.

    \include qgrpcoperation.cpp serverInitialMetadata

    \sa serverInitialMetadata() serverTrailingMetadata()
*/
const QHash<QByteArray, QByteArray> &QGrpcOperation::metadata() const & noexcept
{
    Q_D(const QGrpcOperation);
    QT_IGNORE_DEPRECATIONS(return d->operationContext->serverMetadata();)
}

#endif // QT_DEPRECATED_SINCE(6, 13)

/*!
    \since 6.10

//! [serverInitialMetadata]
    Returns the initial metadata received from the server before any response
    messages.

    Initial metadata is sent by the server immediately after the call is
    established. It may include key-value pairs that provide context for the
    call.

    \include qtgrpc-shared.qdocinc http2-metadata-note
//! [serverInitialMetadata]

    The metadata may contain multiple entries under the same key.

    \sa serverInitialMetadataReceived() serverTrailingMetadata()
*/
const QMultiHash<QByteArray, QByteArray> &QGrpcOperation::serverInitialMetadata() const & noexcept
{
    Q_D(const QGrpcOperation);
    return d->operationContext->serverInitialMetadata();
}

/*!
    \since 6.10

//! [serverTrailingMetadata]
    Returns the trailing metadata received from the server after all response
    messages.

    Trailing metadata is sent only by the server once all response messages
    have been sent and just before the RPC completes. It may include key-value
    pairs providing additional context about the completed call.

    \include qtgrpc-shared.qdocinc http2-metadata-note
//! [serverTrailingMetadata]

    The metadata may contain multiple entries under the same key.

    \sa serverInitialMetadata()
*/
const QMultiHash<QByteArray, QByteArray> &QGrpcOperation::serverTrailingMetadata() const & noexcept
{
    Q_D(const QGrpcOperation);
    return d->operationContext->serverTrailingMetadata();
}

/*!
    Returns the method name associated with this RPC operation.
*/
QLatin1StringView QGrpcOperation::method() const noexcept
{
    Q_D(const QGrpcOperation);
    return d->operationContext->method();
}

/*!
    Returns true if this operation has finished, meaning that no more
    operations can happen on the corresponding RPC, otherwise returns false.
*/
bool QGrpcOperation::isFinished() const noexcept
{
    Q_D(const QGrpcOperation);
    return d->state == QGrpcOperationPrivate::State::Finished;
}

/*!
    \internal
    \fn const QGrpcOperationContext &QGrpcOperation::context() const &
    \fn QGrpcOperationContext &QGrpcOperation::context() &

    Returns a reference to the internal operation context.
*/
const QGrpcOperationContext &QGrpcOperation::context() const & noexcept
{
    Q_D(const QGrpcOperation);
    return *d->operationContext;
}

void QGrpcOperation::onMessageReceived(const QByteArray &data)
{
    Q_D(QGrpcOperation);
    d->data = data;
}

void QGrpcOperation::onFinished(const QGrpcStatus &status)
{
    Q_D(QGrpcOperation);
    if (isFinished())
        return;
    d->state = QGrpcOperationPrivate::State::Finished;
    emit finished(status);
}

bool QGrpcOperation::event(QEvent *event)
{
    return QObject::event(event);
}

/*!
    Returns the meta type of the RPC response message.
 */
QMetaType QGrpcOperation::responseMetaType() const
{
    Q_D(const QGrpcOperation);
    return d->operationContext->responseMetaType();
}

QT_END_NAMESPACE

#include "moc_qgrpcoperation.cpp"
