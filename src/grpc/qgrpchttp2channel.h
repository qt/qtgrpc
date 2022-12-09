// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>, Viktor Kopp <vifactor@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCHTTP2CHANNEL_H
#define QGRPCHTTP2CHANNEL_H

#include <QtCore/QUrl>
#include <QtGrpc/qabstractgrpcchannel.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QAbstractGrpcCredentials;
struct QGrpcHttp2ChannelPrivate;

class Q_GRPC_EXPORT QGrpcHttp2Channel final : public QAbstractGrpcChannel
{
public:
    QGrpcHttp2Channel(const QUrl &url, std::unique_ptr<QAbstractGrpcCredentials> credentials);
    ~QGrpcHttp2Channel();

    QGrpcStatus call(const QString &method, const QString &service, const QByteArray &args,
                     QByteArray &ret) override;
    void call(const QString &method, const QString &service, const QByteArray &args,
              QGrpcCallReply *reply) override;
    void stream(QGrpcStream *stream, const QString &service, QAbstractGrpcClient *client) override;
    std::shared_ptr<QAbstractProtobufSerializer> serializer() const override;

private:
    Q_DISABLE_COPY_MOVE(QGrpcHttp2Channel)

    std::unique_ptr<QGrpcHttp2ChannelPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QGRPCHTTP2CHANNEL_H
