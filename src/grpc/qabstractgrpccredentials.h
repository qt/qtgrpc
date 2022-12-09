// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTGRPCCREDENTIALS_H
#define QABSTRACTGRPCCREDENTIALS_H

#include <QtGrpc/qtgrpcglobal.h>

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/qbytearray.h>

QT_BEGIN_NAMESPACE
using QGrpcCredentialMap = QMap<QByteArray, QVariant>;

class Q_GRPC_EXPORT QAbstractGrpcCredentials
{
public:
    virtual ~QAbstractGrpcCredentials();
    virtual QGrpcCredentialMap callCredentials() const = 0;
    virtual QGrpcCredentialMap channelCredentials() const = 0;
};

QT_END_NAMESPACE

#endif // QABSTRACTGRPCCREDENTIALS_H
