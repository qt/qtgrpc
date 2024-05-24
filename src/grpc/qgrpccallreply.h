// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCCALLREPLY_H
#define QGRPCCALLREPLY_H

#include <QtCore/qobject.h>
#include <QtGrpc/qgrpcoperation.h>
#include <QtGrpc/qtgrpcglobal.h>
#include <QtGrpc/qgrpcchanneloperation.h>

#include <memory>
#include <utility>

QT_BEGIN_NAMESPACE

class Q_GRPC_EXPORT QGrpcCallReply final : public QGrpcOperation
{
    Q_OBJECT

public:
    explicit QGrpcCallReply(std::shared_ptr<QGrpcChannelOperation> channelOperation);
    ~QGrpcCallReply() override;

    template <typename ReceiverType, typename FinishCallback, typename ErrorCallback>
    void subscribe(ReceiverType *receiver, FinishCallback &&finishCallback, ErrorCallback &&errorCallback,
                   Qt::ConnectionType type = Qt::AutoConnection)
    {
        QObject::connect(
            this, &QGrpcCallReply::finished, receiver,
            [finishCallback, errorCallback](const auto &status) {
                if (status.code() != QGrpcStatus::StatusCode::Ok) {
                    errorCallback(status);
                } else {
                    finishCallback();
                }
            },
            type);
    }

    template <typename ReceiverType, typename FinishCallback>
    void subscribe(ReceiverType *receiver, FinishCallback &&finishCallback,
                   Qt::ConnectionType type = Qt::AutoConnection)
    {
        QObject::connect(this, &QGrpcCallReply::finished, receiver,
                         std::forward<FinishCallback>(finishCallback), type);
    }

private:
    Q_DISABLE_COPY_MOVE(QGrpcCallReply)
};

QT_END_NAMESPACE

#endif // QGRPCCALLREPLY_H
