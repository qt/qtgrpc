// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCSSLCREDENTIALS_H
#define QGRPCSSLCREDENTIALS_H

#include <QtGrpc/qgrpccredentials.h>
#include <QtGrpc/qtgrpcglobal.h>

QT_REQUIRE_CONFIG(ssl);
#include <QtNetwork/QSslConfiguration>

QT_BEGIN_NAMESPACE

class Q_GRPC_EXPORT QGrpcSslCredentials final : public QGrpcChannelCredentials
{
public:
    explicit QGrpcSslCredentials(const QSslConfiguration &configuration);
    ~QGrpcSslCredentials() override;

    QGrpcCredentialMap channelCredentials() const override;

private:
    QGrpcSslCredentials();
    QGrpcCredentialMap m_map;
};

QT_END_NAMESPACE

#endif // QGRPCSSLCREDENTIALS_H
