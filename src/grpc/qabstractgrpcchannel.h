// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTGRPCCHANNEL_H
#define QABSTRACTGRPCCHANNEL_H

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/qbytearray.h>
#include <QtGrpc/qgrpccredentials.h>
#include <QtGrpc/qgrpcstatus.h>
#include <QtGrpc/qtgrpcglobal.h>

QT_BEGIN_NAMESPACE

class QAbstractGrpcClient;
class QAbstractProtobufSerializer;
struct QAbstractGrpcChannelPrivate;
class QGrpcStream;
class QGrpcCallReply;

class Q_GRPC_EXPORT QAbstractGrpcChannel
{
public:
    virtual QGrpcStatus call(const QString &method, const QString &service, const QByteArray &args,
                             QByteArray &ret) = 0;
    virtual void call(const QString &method, const QString &service, const QByteArray &args,
                      QGrpcCallReply *ret) = 0;
    virtual void stream(QGrpcStream *stream, const QString &service,
                        QAbstractGrpcClient *client) = 0;
    virtual QSharedPointer<QAbstractProtobufSerializer> serializer() const = 0;

    const QThread *thread() const;

protected:
    QAbstractGrpcChannel();
    virtual ~QAbstractGrpcChannel();

private:
    Q_DISABLE_COPY(QAbstractGrpcChannel)
    std::unique_ptr<QAbstractGrpcChannelPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QABSTRACTGRPCCHANNEL_H
