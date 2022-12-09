// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCINSECURECREDENTIALS_H
#define QGRPCINSECURECREDENTIALS_H

#include <QtGrpc/qgrpccredentials.h>
#include <QtGrpc/qtgrpcglobal.h>

QT_BEGIN_NAMESPACE

class Q_GRPC_EXPORT QGrpcInsecureCallCredentials final : public QGrpcCallCredentials
{
public:
    ~QGrpcInsecureCallCredentials() override;
    QGrpcCredentialMap operator()() const override;
};

class Q_GRPC_EXPORT QGrpcInsecureChannelCredentials final : public QGrpcChannelCredentials
{
public:
    ~QGrpcInsecureChannelCredentials() override;
    QGrpcCredentialMap channelCredentials() const override;
};

QT_END_NAMESPACE

#endif // QGRPCINSECURECREDENTIALS_H
