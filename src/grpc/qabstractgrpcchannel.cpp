// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qabstractgrpcchannel.h"
#include "qabstractgrpcchannel_p.h"
#include "qgrpcchanneloperation.h"
#include "qgrpccallreply.h"
#include "qgrpcstream.h"

QT_BEGIN_NAMESPACE

/*!
    \class QAbstractGrpcChannel
    \inmodule QtGrpc
    \brief The QAbstractGrpcChannel class is interface that represents common
    gRPC channel functionality.

    You may implement this interface to create your own channels for gRPC transport.
    QGrpcChannel or QGrpcHttp2Channel, which are full implementations of
    QAbstractGrpcChannel are recommended to use.
*/

/*!
    \fn virtual std::shared_ptr<QAbstractProtobufSerializer> QAbstractGrpcChannel::serializer() const = 0

    This pure virtual function shall return a shared pointer
    to QAbstractProtobufSerializer.

    This function is called to obtain the QAbstractProtobufSerializer used
    to perform serialization and deserialization of the message.
*/

/*!
    \fn virtual void call(std::shared_ptr<QGrpcChannelOperation> channelOperation) = 0

    This pure virtual function is called by public QAbstractGrpcChannel::call
    method when making unary gRPC call. The \a channelOperation is the
    pointer to a channel side \l QGrpcChannelOperation primitive that is
    connected with \l QGrpcCallReply primitive, that is used in
    \l QAbstractGrpcClient implementations.

    The function should implement the channel-side logic of unary call. The
    implementation must be asynchronous and must not block the thread where
    the function was called.
*/

/*!
    \fn virtual void startStream(std::shared_ptr<QGrpcChannelOperation> channelOperation) = 0

    This pure virtual function that the starts of the server-side stream. The
    \a channelOperation is the pointer to a channel side
    \l QGrpcChannelOperation primitive that is connected with \l QGrpcStream
    primitive, that is used in \l QAbstractGrpcClient implementations.

    The function should implement the channel-side logic of server-side stream.
    The implementation must be asynchronous and must not block the thread where
    the function was called.
*/
QAbstractGrpcChannel::QAbstractGrpcChannel() : dPtr(std::make_unique<QAbstractGrpcChannelPrivate>())
{
}
QAbstractGrpcChannel::~QAbstractGrpcChannel() = default;

/*!
    \internal
    Function constructs \l QGrpcCallReply and \l QGrpcChannelOperation
    primitives and makes the required for unary gRPC call connections
    between them.

    The function should not be called directly, but only by
    \l QAbstractGrpcClient implementations.
*/
std::shared_ptr<QGrpcCallReply> QAbstractGrpcChannel::call(QLatin1StringView method,
                                                           QLatin1StringView service,
                                                           QByteArrayView arg,
                                                           const QGrpcCallOptions &options)
{
    auto channelOperation = std::make_shared<QGrpcChannelOperation>(method, service, arg, options);
    QObject::connect(channelOperation.get(), &QGrpcChannelOperation::sendData, []() {
        Q_ASSERT_X(false, "QAbstractGrpcChannel::call",
                   "QAbstractGrpcChannel::call disallows sendData signal from "
                   "QGrpcChannelOperation");
    });
    std::shared_ptr<QGrpcCallReply> reply(new QGrpcCallReply(channelOperation, serializer()),
                                          [](QGrpcCallReply *reply) { reply->deleteLater(); });
    call(channelOperation);
    return reply;
}

/*!
    \internal
    Function constructs \l QGrpcStream and \l QGrpcChannelOperation
    primitives and makes the required for server-side gRPC stream connections
    between them.

    The function should not be called directly, but only by
    \l QAbstractGrpcClient implementations.
*/
std::shared_ptr<QGrpcStream>
QAbstractGrpcChannel::startStream(QLatin1StringView method, QLatin1StringView service,
                                        QByteArrayView arg, const QGrpcCallOptions &options)
{
    auto channelOperation = std::make_shared<QGrpcChannelOperation>(method, service, arg, options);
    QObject::connect(channelOperation.get(), &QGrpcChannelOperation::sendData, []() {
        Q_ASSERT_X(false, "QAbstractGrpcChannel::startStream",
                   "QAbstractGrpcChannel::startStream disallows sendData signal from "
                   "QGrpcChannelOperation");
    });

    std::shared_ptr<QGrpcStream> stream(
            new QGrpcStream(channelOperation, serializer()));
    startStream(channelOperation);
    return stream;
}

QT_END_NAMESPACE
