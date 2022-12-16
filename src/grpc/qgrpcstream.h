// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCSTREAM_H
#define QGRPCSTREAM_H

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtGrpc/qabstractgrpcclient.h>
#include <QtGrpc/qgrpcoperation.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <functional>

QT_BEGIN_NAMESPACE

class QAbstractGrpcClient;

class Q_GRPC_EXPORT QGrpcStream final : public QGrpcOperation
{
    Q_OBJECT
    using StreamHandler = std::function<void(const QByteArray &)>;

public:
    void abort() override;

    QString method() const;
    QByteArray arg() const;
    void handler(const QByteArray &data);

Q_SIGNALS:
    void messageReceived();

protected:
    QGrpcStream(const QString &method, const QByteArray &arg, const StreamHandler &handler,
                QAbstractGrpcClient *parent);
    virtual ~QGrpcStream() = default;

    void addHandler(const StreamHandler &handler);

    bool operator==(const QGrpcStream &other) const
    {
        return other.method() == this->method() && other.arg() == this->arg();
    }

private:
    friend class QAbstractGrpcClient;
    QString m_method;
    QByteArray m_arg;
    QList<StreamHandler> m_handlers;
};

QT_END_NAMESPACE

#endif // QGRPCSTREAM_H
