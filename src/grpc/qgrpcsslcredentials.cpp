// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpcsslcredentials.h"

/*!
    \class QGrpcSslCredentials
    \inmodule QtGrpc

    \brief The QGrpcSslCredentials class.

    TODO: Fill this out with more detail later
*/

QGrpcSslCredentials::QGrpcSslCredentials(const QSslConfiguration &configuation)
    : m_map(QGrpcCredentialMap{ { QByteArray(SslConfigCredential),
                                  QVariant::fromValue<QSslConfiguration>(configuation) } })
{
}
QGrpcSslCredentials::~QGrpcSslCredentials() = default;

QGrpcCredentialMap QGrpcSslCredentials::channelCredentials() const
{
    return m_map;
}

QGrpcSslCredentials::QGrpcSslCredentials() = default;
