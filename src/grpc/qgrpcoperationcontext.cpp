// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpccalloptions.h>
#include <QtGrpc/qgrpcoperationcontext.h>
#include <QtGrpc/qgrpcstatus.h>

#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/private/qobject_p.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qbytearrayview.h>
#include <QtCore/qlatin1stringview.h>

#include <utility>

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcOperationContext
    \inmodule QtGrpc
    \since 6.7
    \brief The QGrpcOperationContext class implements common logic to
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
    \fn void QGrpcOperationContext::messageReceived(const QByteArray &data)

    The signal should be emitted by the channel when the new chunk of \a data is
    received.

    For client streams and unary calls, this means that the response
    is received from the server, and communication will be successfully
    finished. For server and bidi streams this signal should be called by
    channel to signalize that receiving of new message is completed and user
    business logic now can process it at the client side.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void QGrpcOperationContext::cancelRequested()

    The signal is emitted when client requests to terminate the communication.

    The signal should only be emitted from the client counterpart.
    Implementations of \l QAbstractGrpcChannel should attempt to close
    connection client-server connection and return immediately. The successful
    connection break is not guarateed. Further processing of the data received
    from a channel is not required and is not recommended.

    The client side will be notificated by the
    \l QGrpcOperationContext::finished signal with
    \l QtGrpc::StatusCode::Cancelled status code.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void QGrpcOperationContext::writeMessageRequested(const QByteArray &data)

    The signal is emitted when the client tries to send a new message to the
    channel.

    This signal can only be emitted by client or bidi streams. Implementations
    of \l QAbstractGrpcChannel should connect the sending logic to this signal.
    The \a data should be wrapped with the channel-related headers and sent
    to the wire.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void QGrpcOperationContext::writesDoneRequested()

    T.B.A
*/

class QGrpcOperationContextPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QGrpcOperationContext)
public:
    QGrpcOperationContextPrivate(QLatin1StringView method_, QLatin1StringView service_,
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
    QHash<QByteArray, QByteArray> serverMetadata;
};

QGrpcOperationContext::QGrpcOperationContext(QLatin1StringView method, QLatin1StringView service,
                                             QByteArrayView arg, const QGrpcCallOptions &options,
                                             std::shared_ptr<QAbstractProtobufSerializer>
                                                 serializer,
                                             PrivateConstructor /*unused*/)
    : QObject(*new QGrpcOperationContextPrivate(method, service, arg, options,
                                                std::move(serializer)))
{
}

QGrpcOperationContext::~QGrpcOperationContext() = default;

/*!
    Returns the method name that is assigned to this operation.
*/
QLatin1StringView QGrpcOperationContext::method() const noexcept
{
    Q_D(const QGrpcOperationContext);
    return d->method;
}

/*!
    Returns the service name that is assigned to this operation.
*/
QLatin1StringView QGrpcOperationContext::service() const noexcept
{
    Q_D(const QGrpcOperationContext);
    return d->service;
}

/*!
    Returns the serialized arguments that are used for this operation.
*/
QByteArrayView QGrpcOperationContext::argument() const noexcept
{
    Q_D(const QGrpcOperationContext);
    return d->argument;
}

/*!
    Return the options that are assigned to this operation.
*/
const QGrpcCallOptions &QGrpcOperationContext::callOptions() const & noexcept
{
    Q_D(const QGrpcOperationContext);
    return d->options;
}

/*!
    Return the serializer that is assigned to this operation.
*/
std::shared_ptr<const QAbstractProtobufSerializer>
QGrpcOperationContext::serializer() const
{
    Q_D(const QGrpcOperationContext);
    return d->serializer;
}

/*!
    Returns the metadata that is received from server.

    The method is used implicitly by \l QGrpcOperation counterpart.
*/
const QHash<QByteArray, QByteArray> &QGrpcOperationContext::serverMetadata() const & noexcept
{
    Q_D(const QGrpcOperationContext);
    return d->serverMetadata;
}

/*!
    Stores the recently received server metadata.

    The \a metadata then can be processed on the client side.
*/
void QGrpcOperationContext::setServerMetadata(const QHash<QByteArray, QByteArray> &metadata)
{
    Q_D(QGrpcOperationContext);
    d->serverMetadata = metadata;
}

/*!
    Stores the recently received server metadata.

    The \a metadata then can be processed on the client side.
*/
void QGrpcOperationContext::setServerMetadata(QHash<QByteArray, QByteArray> &&metadata)
{
    Q_D(QGrpcOperationContext);
    d->serverMetadata = std::move(metadata);
}

// For future extensions
bool QGrpcOperationContext::event(QEvent *event)
{
    return QObject::event(event);
}

QT_END_NAMESPACE

#include "moc_qgrpcoperationcontext.cpp"
