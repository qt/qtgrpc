// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpcsslcredentials.h"

/*!
    \class QGrpcSslCredentials
    \inmodule QtGrpc

    \brief The QGrpcSslCredentials class provides SSL credentials for channel.
*/

/*!
    Constructs the QGrpcSslCredentials object with SSL \a configuration.

    Fills a stored QGrpcCredentialMap with key \c{sslConfig} and
    QVariant with \a configuration.
*/
QGrpcSslCredentials::QGrpcSslCredentials(const QSslConfiguration &configuration)
    : m_map(QGrpcCredentialMap{ { QByteArray(SslConfigCredential),
                                  QVariant::fromValue<QSslConfiguration>(configuration) } })
{
}

/*!
    Destroys the QGrpcSslCredentials object.
*/
QGrpcSslCredentials::~QGrpcSslCredentials() = default;

/*!
    Returns the stored QGrpcCredentialMap.
*/
QGrpcCredentialMap QGrpcSslCredentials::channelCredentials() const
{
    return m_map;
}

/*!
    \internal

    Construct the QGrpcSslCredentials object with empty QGrpcCredentialMap.
*/
QGrpcSslCredentials::QGrpcSslCredentials() = default;
