// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qabstractgrpcchannel.h"
#include "qabstractgrpcchannel_p.h"

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
    \fn virtual QGrpcStatus QAbstractGrpcChannel::call(QLatin1StringView method, QLatin1StringView service,
                                                        QByteArrayView args, QByteArray &ret) = 0

    This pure virtual function synchronously calls the RPC method concatenated
    from the \a method and \a service parameters with the given \a args
    and writes the result to the output parameter \a ret.

    You may reimplement this function in a subclass to define your own call
    mechanism behavior. QGrpcChannel or QGrpcHttp2Channel, which are full
    implementations of QAbstractGrpcChannel are recommended to use.
*/

/*!
    \fn virtual std::shared_ptr<QGrpcCallReply> QAbstractGrpcChannel::call(QAbstractGrpcClient *client,
                                                                        QLatin1StringView method,
                                                                        QLatin1StringView service,
                                                                        QByteArrayView args) = 0

    This pure virtual function asynchronously calls the RPC method combined
    with the \a method and \a service parameters with the given \a args
    and returns an asynchronous response in the form of QGrpcCallReply,
    which uses the \a client to deserialize messages.

    You may reimplement this function in a subclass to define your own call
    mechanism behavior. QGrpcChannel or QGrpcHttp2Channel, which are full
    implementations of QAbstractGrpcChannel are recommended to use.
*/

/*!
    \fn virtual void QAbstractGrpcChannel::startStream(QGrpcStream *stream, QLatin1StringView service) = 0

    This pure virtual function starts a stream on a \a stream using
    QGrpcStream::method() and the \a service to get the name of the RPC method.

    You may reimplement this function in a subclass to define your own stream
    mechanism behavior. QGrpcChannel or QGrpcHttp2Channel, which are full
    implementations of QAbstractGrpcChannel are recommended to use.
*/

/*!
    \fn virtual std::shared_ptr<QAbstractProtobufSerializer> QAbstractGrpcChannel::serializer() const = 0

    This pure virtual function shall return a shared pointer
    to QAbstractProtobufSerializer.

    This function is called to obtain the QAbstractProtobufSerializer used by
    QAbstractGrpcClient to perform serialization and deserialization of the message.
*/

QAbstractGrpcChannel::QAbstractGrpcChannel() : dPtr(std::make_unique<QAbstractGrpcChannelPrivate>())
{
}
QAbstractGrpcChannel::~QAbstractGrpcChannel() = default;

QT_END_NAMESPACE
