// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Giulio Girardi <giulio.girardi@protechgroup.it>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCCHANNEL_H
#define QGRPCCHANNEL_H

#include <QtCore/QUrl>
#include <QtGrpc/qabstractgrpcclient.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QAbstractGrpcCredentials;
struct QGrpcChannelPrivate;

class Q_GRPC_EXPORT QGrpcChannel final : public QAbstractGrpcChannel
{
public:
    enum NativeGrpcChannelCredentials : uint8_t {
        InsecureChannelCredentials = 0,
        GoogleDefaultCredentials,
        SslDefaultCredentials,
    };

    explicit QGrpcChannel(const QUrl &name, NativeGrpcChannelCredentials credentialsType,
                          const QStringList &credentialsList);
    explicit QGrpcChannel(const QUrl &name, NativeGrpcChannelCredentials credentialsType);
    ~QGrpcChannel() override;

    QGrpcStatus call(QLatin1StringView method, QLatin1StringView service, QByteArrayView args,
                     QByteArray &ret) override;
    std::shared_ptr<QGrpcCallReply> call(QAbstractGrpcClient *client, QLatin1StringView method,
                                         QLatin1StringView service, QByteArrayView args) override;
    void startStream(QGrpcStream *stream, QLatin1StringView service) override;
    std::shared_ptr<QAbstractProtobufSerializer> serializer() const override;

private:
    Q_DISABLE_COPY_MOVE(QGrpcChannel)

    std::unique_ptr<QGrpcChannelPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QGRPCCHANNEL_H
