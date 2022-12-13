// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpccredentials.h>

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcCallCredentials
    \inmodule QtGRPC

    \brief The QGrpcCallCredentials is the base class for gRPC call credentials.

    You may inherrit this class to create your own credentials implementation,
    that will be used for each method call.
    Classes inheriting from QGrpcCallCredentials must reimplement
    \c{QGrpcCredentialMap operator()} and return a credentials map
    for the call.
*/

/*!
    \class QGrpcChannelCredentials
    \inmodule QtGRPC

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
    \class QGrpcCredentials
    \inmodule QtGRPC

    \brief The QGrpcCredentials class is a combination of call
    and channel credentials that is used by gRPC channels to communicate
    with services, using the given authentication parameters.
*/

const char *SslConfigCredential = "sslConfig";

QGrpcCallCredentials::~QGrpcCallCredentials() = default;

QGrpcChannelCredentials::~QGrpcChannelCredentials() = default;

QT_END_NAMESPACE
