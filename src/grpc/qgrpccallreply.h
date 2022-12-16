// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCCALLREPLY_H
#define QGRPCCALLREPLY_H

#include <QtCore/QMutex>
#include <QtGrpc/qabstractgrpcclient.h>
#include <QtGrpc/qgrpcoperation.h>
#include <QtGrpc/qtgrpcglobal.h>

QT_BEGIN_NAMESPACE

class Q_GRPC_EXPORT QGrpcCallReply final : public QGrpcOperation
{
    Q_OBJECT
public:
    void abort() override;

    template <typename Func1, typename Func2>
    inline void subscribe(QObject *receiver, Func1 finishCallback, Func2 errorCallback,
                          Qt::ConnectionType type = Qt::AutoConnection)
    {
        QObject::connect(this, &QGrpcCallReply::finished, receiver, finishCallback, type);
        QObject::connect(this, &QGrpcCallReply::errorOccurred, receiver, errorCallback, type);
    }

    template <typename Func1>
    inline void subscribe(QObject *receiver, Func1 finishCallback,
                          Qt::ConnectionType type = Qt::AutoConnection)
    {
        QObject::connect(this, &QGrpcCallReply::finished, receiver, finishCallback, type);
    }

protected:
    QGrpcCallReply(QAbstractGrpcClient *parent) : QGrpcOperation(parent) { }
    ~QGrpcCallReply() = default;

private:
    QGrpcCallReply();
    Q_DISABLE_COPY_MOVE(QGrpcCallReply)

    friend class QAbstractGrpcClient;
};

QT_END_NAMESPACE

#endif // QGRPCCALLREPLY_H
