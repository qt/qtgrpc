// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCOPERATION_H
#define QGRPCOPERATION_H

#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtGrpc/qabstractgrpcclient.h>
#include <QtGrpc/qtgrpcglobal.h>

QT_BEGIN_NAMESPACE

class Q_GRPC_EXPORT QGrpcOperation : public QObject
{
    Q_OBJECT

public:
    template <typename T>
    T read() const
    {
        T value;
        auto client = qobject_cast<QAbstractGrpcClient *>(parent());
        if (client)
            client->tryDeserialize(&value, m_data);
        return value;
    }

    void setData(const QByteArray &data);
    void setData(QByteArray &&data);

    virtual void abort() = 0;

Q_SIGNALS:
    void finished();
    void errorOccurred(const QGrpcStatus &status);

protected:
    explicit QGrpcOperation(QAbstractGrpcClient *parent);
    ~QGrpcOperation() override;

private:
    QGrpcOperation();
    Q_DISABLE_COPY_MOVE(QGrpcOperation)

    friend class QAbstractGrpcClient;

    QByteArray m_data;
};

QT_END_NAMESPACE

#endif // QGRPCOPERATION_H
