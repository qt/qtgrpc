// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCSTREAM_H
#define QGRPCSTREAM_H

#include <QtCore/QByteArray>
#include <QtCore/QList>
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
    ~QGrpcStream() override;

    void abort() override;

    QLatin1StringView method() const;
    QByteArrayView arg() const;
    void handler(const QByteArray &data);

Q_SIGNALS:
    void messageReceived();

protected:
    explicit QGrpcStream(QLatin1StringView method, QByteArrayView arg, const StreamHandler &handler,
                         QAbstractGrpcClient *client);

    void addHandler(const StreamHandler &handler);

    bool operator==(const QGrpcStream &other) const
    {
        return other.method() == method() && other.arg() == arg();
    }

private:
    friend class QAbstractGrpcClient;
    std::string m_method;
    QByteArray m_arg;
    QList<StreamHandler> m_handlers;
};

QT_END_NAMESPACE

#endif // QGRPCSTREAM_H
