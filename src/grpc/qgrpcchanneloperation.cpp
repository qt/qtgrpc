// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpcchanneloperation.h"

#include "qgrpcstatus.h"
#include "qgrpccalloptions.h"

#include <QtCore/private/qobject_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcChannelOperation
    \inmodule QtGrpc
    \brief The QGrpcChannelOperation class implements common logic to
           handle the gRPC communication from the channel side.
*/

/*!
    \fn void QGrpcChannelOperation::dataReady(const QByteArray &data)

    The signal should be emitted by the channel when the new chunk of \a data is
    received.

    For client streams and unary calls, this means that the response
    is received from the server, and communication will be successfully
    finished. For server and bidir streams this signal should be called by
    channel to signalize that receiving of new message is completed and user
    buisness logic now can process it at the client side.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void QGrpcChannelOperation::sendData(const QByteArray &data) const

    The signal is emitted when the client tries to send a new message to the
    channel.

    This signal can only be emitted by client or bidir streams. Implementations
    of \l QAbstractGrpcChannel should connect the sending logic to this signal.
    The \a data should be wrapped with the channel-related headers and sent
    to the wire.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void errorOccurred(const QGrpcStatus &status)

    The signal should be emitted by channel when error occurred in during the
    communication.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void finished()

    The signal is emitted when the gRPC communication is finished.

    It usually means that the server closed the connection with no errors.
    Implementations of \l QAbstractGrpcChannel should detect this situation and
    emit the signal.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

/*!
    \fn void cancelled()

    The signal is emitted when client requests to terminate the communication.

    The signal should only be emitted from the client counterpart.
    Implementations of \l QAbstractGrpcChannel should attempt to close
    connection client-server connection and return immediately. The successful
    connection break is not guarateed. Further processing of the data received
    from a channel is not required and is not recommended.

    The client side will be notificated by the
    \l QGrpcChannelOperation::errorOccurred signal with
    \l QGrpcStatus::Cancelled status code.

    The signal is implicitly connected to the QGrpcOperation counterpart.
*/

class QGrpcChannelOperationPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QGrpcChannelOperation)
public:
    QGrpcChannelOperationPrivate(QLatin1StringView _method, QLatin1StringView _service,
                                 QByteArrayView _arg, QGrpcCallOptions _options)
        : method(_method), service(_service), arg(_arg.toByteArray()), options(_options)
    {
    }

    QLatin1StringView method;
    QLatin1StringView service;
    QByteArray arg;
    QGrpcCallOptions options;
    QGrpcMetadata serverMetadata;
};

QGrpcChannelOperation::QGrpcChannelOperation(QLatin1StringView method, QLatin1StringView service,
                                             QByteArrayView arg, const QGrpcCallOptions &options)
    : QObject(*new QGrpcChannelOperationPrivate(method, service, arg, options))
{
}

/*!
    Returns the method name that is assigned to this operation.
*/
QLatin1StringView QGrpcChannelOperation::method() const
{
    return d_func()->method;
}

/*!
    Returns the service name that is assigned to this operation.
*/
QLatin1StringView QGrpcChannelOperation::service() const
{
    return d_func()->service;
}

/*!
    Returns the initial arguments that are used for this operation.
*/
QByteArrayView QGrpcChannelOperation::arg() const
{
    return d_func()->arg;
}

/*!
    Return the options that are assigned to this operation.
*/
const QGrpcCallOptions &QGrpcChannelOperation::options() const
{
    return d_func()->options;
}

/*!
    Returns the metadata that is received from server.

    The method is used implicitly by \l QGrpcOperation counterpart.
*/
const QGrpcMetadata &QGrpcChannelOperation::serverMetadata() const
{
    return d_func()->serverMetadata;
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

QT_END_NAMESPACE