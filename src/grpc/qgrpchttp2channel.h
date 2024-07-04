// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>, Viktor Kopp <vifactor@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCHTTP2CHANNEL_H
#define QGRPCHTTP2CHANNEL_H

#include <QtGrpc/qabstractgrpcchannel.h>
#include <QtGrpc/qgrpcchanneloptions.h>

#include <memory>

QT_BEGIN_NAMESPACE

struct QGrpcHttp2ChannelPrivate;

class Q_GRPC_EXPORT QGrpcHttp2Channel final : public QAbstractGrpcChannel
{
public:
    explicit QGrpcHttp2Channel(const QUrl &hostUri);
    explicit QGrpcHttp2Channel(const QUrl &hostUri, const QGrpcChannelOptions &options);
    ~QGrpcHttp2Channel() override;

    [[nodiscard]] QUrl hostUri() const noexcept;

private:
    void call(std::shared_ptr<QGrpcOperationContext> operationContext) override;
    void startServerStream(std::shared_ptr<QGrpcOperationContext> operationContext) override;
    void startClientStream(std::shared_ptr<QGrpcOperationContext> operationContext) override;
    void startBidirStream(std::shared_ptr<QGrpcOperationContext> operationContext) override;

    [[nodiscard]] std::shared_ptr<QAbstractProtobufSerializer> serializer() const noexcept override;

    Q_DISABLE_COPY_MOVE(QGrpcHttp2Channel)

    std::unique_ptr<QGrpcHttp2ChannelPrivate> d_ptr;
};

QT_END_NAMESPACE

#endif // QGRPCHTTP2CHANNEL_H
