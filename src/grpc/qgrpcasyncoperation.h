// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCASYNCOPERATIONBASE_H
#define QGRPCASYNCOPERATIONBASE_H

#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtGrpc/qabstractgrpcclient.h>
#include <QtGrpc/qtgrpcglobal.h>

QT_BEGIN_NAMESPACE

class Q_GRPC_EXPORT QGrpcAsyncOperation : public QObject
{
    Q_OBJECT
public:
    template<typename T>
    T read()
    {
        T value;
        auto client = qobject_cast<QAbstractGrpcClient *>(parent());
        if (client)
            client->tryDeserialize(&value, m_data);
        return value;
    }

    void setData(const QByteArray &data);

    virtual void abort() = 0;

Q_SIGNALS:
    void finished();
    void errorOccurred(const QGrpcStatus &status);

protected:
    QGrpcAsyncOperation(QAbstractGrpcClient *parent);
    virtual ~QGrpcAsyncOperation();

private:
    QGrpcAsyncOperation();
    Q_DISABLE_COPY_MOVE(QGrpcAsyncOperation)

    friend class QAbstractGrpcClient;

    QByteArray m_data;
};

QT_END_NAMESPACE

#endif // QGRPCASYNCOPERATIONBASE_H
