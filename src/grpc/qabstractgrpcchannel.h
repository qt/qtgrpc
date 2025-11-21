// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTGRPCCHANNEL_H
#define QABSTRACTGRPCCHANNEL_H

#include <QtGrpc/qgrpcinterceptor.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <QtCore/qmetatype.h>
#include <QtCore/qspan.h>
#include <QtCore/qstringfwd.h>
#include <QtCore/qtclasshelpermacros.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QAbstractProtobufSerializer;
class QGrpcBidiStream;
class QGrpcCallReply;
class QGrpcOperationContext;
class QGrpcChannelOptions;
class QGrpcCallOptions;
class QGrpcClientBase;
class QGrpcClientStream;
class QGrpcServerStream;

class QAbstractGrpcChannelPrivate;
class Q_GRPC_EXPORT QAbstractGrpcChannel
{
public:
    virtual ~QAbstractGrpcChannel();

    [[nodiscard]] virtual std::shared_ptr<QAbstractProtobufSerializer> serializer() const = 0;

    [[nodiscard]] const QGrpcChannelOptions &channelOptions() const & noexcept;
    void channelOptions() const && = delete;

    void setChannelOptions(const QGrpcChannelOptions &options);
    void setChannelOptions(QGrpcChannelOptions &&options);

    template <typename T, if_grpc_interceptor<T> = true>
    bool addInterceptor(T *interceptor)
    {
        if (!interceptor)
            return false;
        auto bindings = QtGrpcPrivate::InterceptorCapabilityBinding::extractFrom(interceptor);
        addInterceptorImpl(interceptor, bindings);
        return true;
    }

    template <typename T, if_grpc_interceptor<T> = true>
    bool removeInterceptor(T *interceptor)
    {
        return removeInterceptorImpl(interceptor);
    }

    void removeAllInterceptors();

protected:
    QAbstractGrpcChannel();
    explicit QAbstractGrpcChannel(QAbstractGrpcChannelPrivate &dd);
    explicit QAbstractGrpcChannel(const QGrpcChannelOptions &options);

private:
    virtual void call(QGrpcOperationContext *operationContext, QByteArray &&messageData) = 0;
    virtual void serverStream(QGrpcOperationContext *operationContext,
                              QByteArray &&messageData) = 0;
    virtual void clientStream(QGrpcOperationContext *operationContext,
                              QByteArray &&messageData) = 0;
    virtual void bidiStream(QGrpcOperationContext *operationContext, QByteArray &&messageData) = 0;

    void addInterceptorImpl(void *interceptor,
                            QSpan<const QtGrpcPrivate::InterceptorCapabilityBinding> bindings);
    bool removeInterceptorImpl(void *interceptor);

private:
    friend class QGrpcClientBase;
    friend class QGrpcClientBasePrivate;

    std::unique_ptr<QAbstractGrpcChannelPrivate> d_ptr;

    Q_DISABLE_COPY_MOVE(QAbstractGrpcChannel)
    Q_DECLARE_PRIVATE(QAbstractGrpcChannel)
};

QT_END_NAMESPACE

#endif // QABSTRACTGRPCCHANNEL_H
