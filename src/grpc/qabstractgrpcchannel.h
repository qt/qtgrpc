// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTGRPCCHANNEL_H
#define QABSTRACTGRPCCHANNEL_H

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/qbytearray.h>
#include <QtGrpc/qgrpccalloptions.h>
#include <QtGrpc/qgrpcstatus.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QAbstractGrpcClient;
class QAbstractProtobufSerializer;
struct QAbstractGrpcChannelPrivate;
class QGrpcServerStream;
class QGrpcCallReply;
class QGrpcChannelOperation;

class Q_GRPC_EXPORT QAbstractGrpcChannel
{
public:
    std::shared_ptr<QGrpcCallReply> call(QLatin1StringView method, QLatin1StringView service,
                                         QByteArrayView arg,
                                         const QGrpcCallOptions &options = QGrpcCallOptions());
    std::shared_ptr<QGrpcServerStream>
    startServerStream(QLatin1StringView method, QLatin1StringView service, QByteArrayView arg,
                      const QGrpcCallOptions &options = QGrpcCallOptions());
    virtual std::shared_ptr<QAbstractProtobufSerializer> serializer() const = 0;

protected:
    virtual void call(std::shared_ptr<QGrpcChannelOperation> channelOperation) = 0;
    virtual void startServerStream(std::shared_ptr<QGrpcChannelOperation> channelOperation) = 0;

    friend class QAbstractGrpcClient;
    QAbstractGrpcChannel();
    virtual ~QAbstractGrpcChannel();

private:
    Q_DISABLE_COPY(QAbstractGrpcChannel)
    std::unique_ptr<QAbstractGrpcChannelPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QABSTRACTGRPCCHANNEL_H
