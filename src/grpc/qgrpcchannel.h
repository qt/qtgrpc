// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Giulio Girardi <giulio.girardi@protechgroup.it>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCCHANNEL_H
#define QGRPCCHANNEL_H

#include <QtCore/QSharedPointer>
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

    QGrpcStatus call(const QString &method, const QString &service, const QByteArray &args,
                     QByteArray &ret) override;
    void call(const QString &method, const QString &service, const QByteArray &args,
              QGrpcCallReply *reply) override;
    void stream(QGrpcStream *stream, const QString &service) override;
    std::shared_ptr<QAbstractProtobufSerializer> serializer() const override;

private:
    Q_DISABLE_COPY_MOVE(QGrpcChannel)

    std::unique_ptr<QGrpcChannelPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QGRPCCHANNEL_H
