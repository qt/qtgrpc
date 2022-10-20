// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpccredentials.h>

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcCallCredentials
    \inmodule QtGrpc

    \brief The QGrpcCallCredentials is the base class for gRPC call credentials.

    You may inherrit this class to create your own credentials implementation, that will be used
    for each method call. Classes inheriting of QGrpcCallCredentials must reimplement
    \c{QGrpcCredentialMap operator()} method and return a credentials map for the call.
 */

/*!
    \class QGrpcChannelCredentials
    \inmodule QtGrpc

    \brief The QGrpcChannelCredentials is the base class for gRPC channel credentials.

    Channel credentials are used by the channel when establishing initial connection.
    For example Ssl credentials or some session tokens.
    Classes inheriting this class should reimplement
    \c{QGrpcCredentialMap channelCredentials() const} method, that returns session parameters
    for the specific channel.
 */

/*!
    \class QGrpcCredentials
    \inmodule QtGrpc

    \brief The QGrpcCredentials class is a combination of call and channel credentials
    that is used by gRPC channels to communicate with the services with given
    authentications parameters.
 */

const char *SslConfigCredential = "sslConfig";

QT_END_NAMESPACE
