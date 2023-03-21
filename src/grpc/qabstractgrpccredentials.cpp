// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qabstractgrpccredentials.h"

QT_BEGIN_NAMESPACE

/*!
    \class QAbstractGrpcCredentials
    \inmodule QtGrpc
    \brief The QAbstractGrpcClient is an abstract storage class used by authentication parameters.

    QAbstractGrpcCredentials provides a set of functions to access the call and
    the channel credentials that are used by gRPC channels to communicate with the services.

    You may implement this interface to create your own authentication provider class, but we
    recommend using the QGrpcCredentials with QGrpcUserPasswordCredentials and QGrpcSslCredentials.
*/

/*!
    \typealias QGrpcCredentialMap
    \relates QAbstractGrpcCredentials

    Alias for \c{QMap<QByteArray, QVariant>}.
    This is the key-value map of credentials, such as user-password credentials
    or session parameters.
*/

/*!
    \fn virtual QGrpcCredentialMap QAbstractGrpcCredentials::callCredentials() const = 0

    This pure virtual function returns a key-value map of the credentials for RPC calls.

    This function is called to obtain the QGrpcCredentialMap for the call.
*/

/*!
    \fn virtual QGrpcCredentialMap QAbstractGrpcCredentials::channelCredentials() const = 0

    This pure virtual function shall return a key-value map of session parameters
    for the specific channel.

    This function is called to obtain the QGrpcCredentialMap for the channel.
*/

QAbstractGrpcCredentials::~QAbstractGrpcCredentials() = default;

QT_END_NAMESPACE
