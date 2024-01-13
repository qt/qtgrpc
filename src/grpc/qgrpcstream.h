// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCSTREAM_H
#define QGRPCSTREAM_H

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGrpc/qgrpcoperation.h>
#include <QtGrpc/qtgrpcglobal.h>

QT_BEGIN_NAMESPACE

class QAbstractGrpcClient;

class Q_GRPC_EXPORT QGrpcServerStream final : public QGrpcOperation
{
    Q_OBJECT

public:
    explicit QGrpcServerStream(std::shared_ptr<QGrpcChannelOperation> channelOperation);
    ~QGrpcServerStream() override;

Q_SIGNALS:
    void messageReceived();
};

class Q_GRPC_EXPORT QGrpcClientStream final : public QGrpcOperation
{
    Q_OBJECT

public:
    explicit QGrpcClientStream(std::shared_ptr<QGrpcChannelOperation> channelOperation);
    ~QGrpcClientStream() override;

    template<typename T>
    void sendMessage(const T &message)
    {
        sendMessage(serializer()->serialize<T>(&message));
    }

private:
    void sendMessage(const QByteArray &data);
};

class Q_GRPC_EXPORT QGrpcBidirStream final : public QGrpcOperation
{
    Q_OBJECT

public:
    explicit QGrpcBidirStream(std::shared_ptr<QGrpcChannelOperation> channelOperation);
    ~QGrpcBidirStream() override;

    template<typename T>
    void sendMessage(const T &message)
    {
        sendMessage(serializer()->serialize<T>(&message));
    }

Q_SIGNALS:
    void messageReceived();

private:
    void sendMessage(const QByteArray &data);
};

QT_END_NAMESPACE

#endif // QGRPCSTREAM_H
