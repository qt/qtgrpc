// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Giulio Girardi <giulio.girardi@protechgroup.it>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCCHANNEL_H
#define QGRPCCHANNEL_H

#include <QtGrpc/qabstractgrpcclient.h>
#include <QtGrpc/qgrpcchanneloptions.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <memory>

QT_BEGIN_NAMESPACE

struct QGrpcChannelPrivate;

class Q_GRPC_EXPORT QGrpcChannel final : public QAbstractGrpcChannel
{
public:
    enum NativeGrpcChannelCredentials : uint8_t {
        InsecureChannelCredentials = 0,
        GoogleDefaultCredentials,
        SslDefaultCredentials,
    };

    explicit QGrpcChannel(const QGrpcChannelOptions &options,
                          NativeGrpcChannelCredentials credentialsType);

    ~QGrpcChannel() override;

    void call(std::shared_ptr<QGrpcChannelOperation> channelOperation) override;
    void startServerStream(std::shared_ptr<QGrpcChannelOperation> channelOperation) override;
    void startClientStream(std::shared_ptr<QGrpcChannelOperation> channelOperation) override;
    void startBidirStream(std::shared_ptr<QGrpcChannelOperation> channelOperation) override;

    std::shared_ptr<QAbstractProtobufSerializer> serializer() const override;

private:
    Q_DISABLE_COPY_MOVE(QGrpcChannel)

    std::unique_ptr<QGrpcChannelPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QGRPCCHANNEL_H
