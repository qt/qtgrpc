// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCCREDENTIALS_H
#define QGRPCCREDENTIALS_H

#include <QtGrpc/qtgrpcglobal.h>

#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

using QGrpcCredentialMap = QMap<QByteArray, QVariant>;

class Q_GRPC_EXPORT QGrpcCallCredentials
{
public:
    virtual ~QGrpcCallCredentials();
    virtual QGrpcCredentialMap operator()() const = 0;
};

class Q_GRPC_EXPORT QGrpcChannelCredentials
{
public:
    virtual ~QGrpcChannelCredentials();
    virtual QGrpcCredentialMap channelCredentials() const = 0;
};

QT_END_NAMESPACE

#endif // QGRPCCREDENTIALS_H
