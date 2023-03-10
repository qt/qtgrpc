// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpccredentials.h>

QT_BEGIN_NAMESPACE

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

/*!
    \class QGrpcCredentials
    \inmodule QtGrpc

    \brief The QGrpcCredentials class is a combination of call
    and channel credentials that is used by gRPC channels to communicate
    with services, using the given authentication parameters.
*/

/*!
    \fn template <typename Call, typename Channel> QGrpcCredentials<Call, Channel>::QGrpcCredentials(const Call &call,
    const Channel &channel)

    Constructs QGrpcCredentials with \a call and \a channel credentials.
    The Call type must be the base of QGrpcCallCredentials and
    the Channel type must be the base of QGrpcChannelCredentials.
*/

/*!
    \fn template <typename Call, typename Channel> QGrpcCredentials<Call, Channel>::QGrpcCredentials(const Call &call)

    Constructs QGrpcCredentials with \a call credentials.
    The Call type must be the base of QGrpcCallCredentials.
*/

/*!
    \fn template <typename Call, typename Channel> QGrpcCredentials<Call, Channel>::QGrpcCredentials(const Channel &channel)

    Constructs QGrpcCredentials with \a channel credentials.
    The Channel type must be the base of QGrpcChannelCredentials.
*/

/*!
    \fn template <typename Call, typename Channel> QGrpcCredentials<Call, Channel>::~QGrpcCredentials()

    Destroys the QGrpcCredentials object.
*/

/*!
    \fn template <typename Call, typename Channel> QGrpcCredentialMap QGrpcCredentials<Call, Channel>::callCredentials() const

    Returns a map of call credentials by calling QGrpcCallCredentials::operator()
    on a stored Call object.
*/

/*!
    \fn template <typename Call, typename Channel> QGrpcCredentialMap QGrpcCredentials<Call, Channel>::channelCredentials() const

    Returns a map of channel credentials by calling QGrpcChannelCredentials::channelCredentials()
    on a stored Channel object.
*/

const char *SslConfigCredential = "sslConfig";

QGrpcCallCredentials::~QGrpcCallCredentials() = default;

QGrpcChannelCredentials::~QGrpcChannelCredentials() = default;

QT_END_NAMESPACE
