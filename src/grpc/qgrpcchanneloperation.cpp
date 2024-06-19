// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpccalloptions.h>
#include <QtGrpc/qgrpcchanneloperation.h>
#include <QtGrpc/qgrpcstatus.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/private/qobject_p.h>

#include <utility>

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcChannelOperation
    \inmodule QtGrpc
    \since 6.7
    \brief The QGrpcChannelOperation class implements common logic to
           handle the gRPC communication from the channel side.
*/

/*!
    \fn void finished(const QGrpcStatus &status)

    The signal is emitted when the gRPC communication is finished.

    It usually means that the server sent the \a status and closed the
    connection. Implementations of \l QAbstractGrpcChannel should detect this
    situation and emit the signal.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void QGrpcChannelOperation::messageReceived(const QByteArray &data)

    The signal should be emitted by the channel when the new chunk of \a data is
    received.

    For client streams and unary calls, this means that the response
    is received from the server, and communication will be successfully
    finished. For server and bidir streams this signal should be called by
    channel to signalize that receiving of new message is completed and user
    business logic now can process it at the client side.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void QGrpcChannelOperation::cancelRequested()

    The signal is emitted when client requests to terminate the communication.

    The signal should only be emitted from the client counterpart.
    Implementations of \l QAbstractGrpcChannel should attempt to close
    connection client-server connection and return immediately. The successful
    connection break is not guarateed. Further processing of the data received
    from a channel is not required and is not recommended.

    The client side will be notificated by the
    \l QGrpcChannelOperation::finished signal with
    \l QGrpcStatus::Cancelled status code.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void QGrpcChannelOperation::writeMessageRequested(const QByteArray &data)

    The signal is emitted when the client tries to send a new message to the
    channel.

    This signal can only be emitted by client or bidir streams. Implementations
    of \l QAbstractGrpcChannel should connect the sending logic to this signal.
    The \a data should be wrapped with the channel-related headers and sent
    to the wire.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void QGrpcChannelOperation::writesDoneRequested()

    T.B.A
*/

class QGrpcChannelOperationPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QGrpcChannelOperation)
public:
    QGrpcChannelOperationPrivate(QLatin1StringView method_, QLatin1StringView service_,
                                 QByteArrayView argument_, QGrpcCallOptions options_,
                                 std::shared_ptr<QAbstractProtobufSerializer> &&serializer_)
        : method(method_), service(service_), argument(argument_.toByteArray()),
          options(std::move(options_)), serializer(std::move(serializer_))
    {
    }

    QLatin1StringView method;
    QLatin1StringView service;
    QByteArray argument;
    QGrpcCallOptions options;
    std::shared_ptr<QAbstractProtobufSerializer> serializer;
    QGrpcMetadata serverMetadata;
};

QGrpcChannelOperation::QGrpcChannelOperation(QLatin1StringView method, QLatin1StringView service,
                                             QByteArrayView arg, const QGrpcCallOptions &options,
                                             std::shared_ptr<QAbstractProtobufSerializer>
                                                 serializer,
                                             PrivateConstructor /*unused*/)
    : QObject(*new QGrpcChannelOperationPrivate(method, service, arg, options,
                                                std::move(serializer)))
{
}

QGrpcChannelOperation::~QGrpcChannelOperation() = default;

/*!
    Returns the method name that is assigned to this operation.
*/
QLatin1StringView QGrpcChannelOperation::method() const noexcept
{
    Q_D(const QGrpcChannelOperation);
    return d->method;
}

/*!
    Returns the service name that is assigned to this operation.
*/
QLatin1StringView QGrpcChannelOperation::service() const noexcept
{
    Q_D(const QGrpcChannelOperation);
    return d->service;
}

/*!
    Returns the serialized arguments that are used for this operation.
*/
QByteArrayView QGrpcChannelOperation::argument() const noexcept
{
    Q_D(const QGrpcChannelOperation);
    return d->argument;
}

/*!
    Return the options that are assigned to this operation.
*/
const QGrpcCallOptions &QGrpcChannelOperation::callOptions() const & noexcept
{
    Q_D(const QGrpcChannelOperation);
    return d->options;
}

/*!
    Return the serializer that is assigned to this operation.
*/
std::shared_ptr<const QAbstractProtobufSerializer>
QGrpcChannelOperation::serializer() const noexcept
{
    Q_D(const QGrpcChannelOperation);
    return d->serializer;
}

/*!
    Returns the metadata that is received from server.

    The method is used implicitly by \l QGrpcOperation counterpart.
*/
const QGrpcMetadata &QGrpcChannelOperation::serverMetadata() const & noexcept
{
    Q_D(const QGrpcChannelOperation);
    return d->serverMetadata;
}

/*!
    Updates the arg attribute that will be used for this operation.

    The method expects \a arg to be serialized data.
*/
void QGrpcChannelOperation::setArgument(QByteArrayView arg)
{
    Q_D(QGrpcChannelOperation);
    d->argument = arg.toByteArray();
}

/*!
    Updates the call \a options attribute.
*/
void QGrpcChannelOperation::setCallOptions(const QGrpcCallOptions &options)
{
    Q_D(QGrpcChannelOperation);
    d->options = options;
}

/*!
    Stores the recently received server metadata.

    The \a metadata then can be processed on the client side.
*/
void QGrpcChannelOperation::setServerMetadata(const QGrpcMetadata &metadata)
{
    Q_D(QGrpcChannelOperation);
    d->serverMetadata = metadata;
}

/*!
    Stores the recently received server metadata.

    The \a metadata then can be processed on the client side.
*/
void QGrpcChannelOperation::setServerMetadata(QGrpcMetadata &&metadata)
{
    Q_D(QGrpcChannelOperation);
    d->serverMetadata = std::move(metadata);
}

// For future extensions
bool QGrpcChannelOperation::event(QEvent *event)
{
    return QObject::event(event);
}

QT_END_NAMESPACE

#include "moc_qgrpcchanneloperation.cpp"
