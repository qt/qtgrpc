// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpccredentials.h>

QT_BEGIN_NAMESPACE

/*!
    \typealias QGrpcCredentialMap
    \relates QGrpcCallCredentials
    \relates QGrpcChannelCredentials

    Alias for \c{QMap<QByteArray, QVariant>}.
    This is the key-value map of credentials, such as user-password credentials
    or session parameters.
*/

/*!
    \class QGrpcCallCredentials
    \inmodule QtGrpc

    \brief The QGrpcCallCredentials is the base class for gRPC call credentials.

    You may inherrit this class to create your own credentials implementation,
    that will be used for each method call.
    Classes inheriting from QGrpcCallCredentials must reimplement
    \c{QGrpcCredentialMap operator()} and return a credentials map
    for the call.
*/

/*!
    \fn virtual QGrpcCredentialMap QGrpcCallCredentials::operator()() const = 0

    This pure virtual operator returns a key-value map of the credentials for RPC calls.

    This operator is called to obtain the QGrpcCredentialMap for the call.
*/

/*!
    \class QGrpcChannelCredentials
    \inmodule QtGrpc

    \brief The QGrpcChannelCredentials is the base class for gRPC
    channel credentials.

    Channel credentials are used by the channel when establishing
    an initial connection.
    For example Ssl credentials or some session tokens.
    Classes inheriting this class should reimplement
    \c{QGrpcCredentialMap channelCredentials() const},
    that returns session parameters for the specific channel.
*/

/*!
    \fn virtual QGrpcCredentialMap QGrpcChannelCredentials::channelCredentials() const = 0

    This pure virtual function shall return a key-value map of session parameters
    for the specific channel.

    This function is called to obtain the QGrpcCredentialMap for the channel.
*/

QGrpcCallCredentials::~QGrpcCallCredentials() = default;

QGrpcChannelCredentials::~QGrpcChannelCredentials() = default;

QT_END_NAMESPACE
