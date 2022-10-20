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
    enum NativeGrpcChannelCredentials {
        InsecureChannelCredentials = 0,
        GoogleDefaultCredentials,
        SslDefaultCredentials
    };

    QGrpcChannel(const QUrl &name, NativeGrpcChannelCredentials credentialsType,
                 const QStringList &credentialsList = QStringList());
    ~QGrpcChannel();

    QGrpcStatus call(const QString &method, const QString &service, const QByteArray &args,
                     QByteArray &ret) override;
    void call(const QString &method, const QString &service, const QByteArray &args,
              QGrpcCallReply *reply) override;
    void stream(QGrpcStream *stream, const QString &service, QAbstractGrpcClient *client) override;
    QSharedPointer<QAbstractProtobufSerializer> serializer() const override;

private:
    Q_DISABLE_COPY_MOVE(QGrpcChannel)

    std::unique_ptr<QGrpcChannelPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QGRPCCHANNEL_H
