// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCSTREAM_H
#define QGRPCSTREAM_H

#include <QtCore/qbytearray.h>
#include <QtCore/qlist.h>
#include <QtCore/qstring.h>
#include <QtGrpc/qgrpcoperation.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <memory>
#include <type_traits>

QT_BEGIN_NAMESPACE

class Q_GRPC_EXPORT QGrpcServerStream final : public QGrpcOperation
{
    Q_OBJECT

public:
    explicit QGrpcServerStream(std::shared_ptr<QGrpcChannelOperation> channelOperation,
                               QObject *parent = nullptr);
    ~QGrpcServerStream() override;

Q_SIGNALS:
    void messageReceived();

private:
    Q_DISABLE_COPY_MOVE(QGrpcServerStream)
};

class Q_GRPC_EXPORT QGrpcClientStream final : public QGrpcOperation
{
    Q_OBJECT

public:
    explicit QGrpcClientStream(std::shared_ptr<QGrpcChannelOperation> channelOperation,
                               QObject *parent = nullptr);
    ~QGrpcClientStream() override;
#ifdef Q_QDOC
    template <typename T>
#else
    template <typename T, std::enable_if_t<!std::is_pointer_v<T>, bool> = true>
#endif
    void sendMessage(const T &message)
    {
        sendMessage(&message);
    }

    void sendMessage(const QProtobufMessage *message);
    void writesDone();

private:
    Q_DISABLE_COPY_MOVE(QGrpcClientStream)
};

class Q_GRPC_EXPORT QGrpcBidirStream final : public QGrpcOperation
{
    Q_OBJECT

public:
    explicit QGrpcBidirStream(std::shared_ptr<QGrpcChannelOperation> channelOperation,
                              QObject *parent = nullptr);
    ~QGrpcBidirStream() override;

#ifdef Q_QDOC
    template <typename T>
#else
    template <typename T, std::enable_if_t<!std::is_pointer_v<T>, bool> = true>
#endif
    void sendMessage(const T &message)
    {
        sendMessage(&message);
    }

    void sendMessage(const QProtobufMessage *message);

    void writesDone();
Q_SIGNALS:
    void messageReceived();

private:
    Q_DISABLE_COPY_MOVE(QGrpcBidirStream)
};

QT_END_NAMESPACE

#endif // QGRPCSTREAM_H
