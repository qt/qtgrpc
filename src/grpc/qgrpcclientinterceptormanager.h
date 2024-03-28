// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCCLIENTINTERCEPTORMANAGER_H
#define QGRPCCLIENTINTERCEPTORMANAGER_H

#include <QtGrpc/qgrpcclientinterceptor.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <memory>
#include <vector>

QT_BEGIN_NAMESPACE

class QGrpcClientInterceptorManagerPrivate;

class Q_GRPC_EXPORT QGrpcClientInterceptorManager
{
public:
    QGrpcClientInterceptorManager();
    ~QGrpcClientInterceptorManager();
    QGrpcClientInterceptorManager(const QGrpcClientInterceptorManager &other);
    QGrpcClientInterceptorManager &operator=(const QGrpcClientInterceptorManager &other);
    QGrpcClientInterceptorManager(QGrpcClientInterceptorManager &&other) noexcept;
    QGrpcClientInterceptorManager &operator=(QGrpcClientInterceptorManager &&other) noexcept;

    void registerInterceptor(std::shared_ptr<QGrpcClientInterceptor> next);
    void
    registerInterceptors(std::vector<std::shared_ptr<QGrpcClientInterceptor>> nextInterceptors);

    template <typename T>
    void run(QGrpcInterceptorContinuation<T> &finalCall,
             typename QGrpcInterceptorContinuation<T>::ReplyType response,
             std::shared_ptr<QGrpcChannelOperation> operation, size_t pos = 0);

private:
    std::unique_ptr<QGrpcClientInterceptorManagerPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QGRPCCLIENTINTERCEPTORMANAGER_H
