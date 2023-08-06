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

#include <functional>

QT_BEGIN_NAMESPACE

class QAbstractGrpcClient;

class Q_GRPC_EXPORT QGrpcServerStream final : public QGrpcOperation
{
    Q_OBJECT

public:
    explicit QGrpcServerStream(std::shared_ptr<QGrpcChannelOperation> channelOperation,
                               std::shared_ptr<QAbstractProtobufSerializer> serializer);
    ~QGrpcServerStream() override;

Q_SIGNALS:
    void messageReceived();
};

QT_END_NAMESPACE

#endif // QGRPCSTREAM_H
