// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTGRPSERVICE_H
#define QABSTRACTGRPSERVICE_H

#include <QtCore/QObject>
#include <QtGrpc/qtgrpcglobal.h>

QT_BEGIN_NAMESPACE

class QAbstractGrpcServicePrivate;

class Q_GRPC_EXPORT QAbstractGrpcService : public QObject
{
    Q_OBJECT

protected:
    explicit QAbstractGrpcService(QObject *parent = nullptr);
    ~QAbstractGrpcService() override;

private:
    Q_DISABLE_COPY_MOVE(QAbstractGrpcService)
    Q_DECLARE_PRIVATE(QAbstractGrpcService)
};

QT_END_NAMESPACE

#endif // QABSTRACTGRPSERVICE_H
