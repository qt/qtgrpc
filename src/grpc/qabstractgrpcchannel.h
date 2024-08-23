// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTGRPCCHANNEL_H
#define QABSTRACTGRPCCHANNEL_H

#include <QtGrpc/qtgrpcglobal.h>

#include <QtCore/qmetatype.h>
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

    [[nodiscard]] virtual std::shared_ptr<QAbstractProtobufSerializer>
    serializer() const noexcept = 0;

protected:
    QAbstractGrpcChannel();
    explicit QAbstractGrpcChannel(const QGrpcChannelOptions &options);

    virtual void call(std::shared_ptr<QGrpcOperationContext> operationContext) = 0;
    virtual void startServerStream(std::shared_ptr<QGrpcOperationContext> operationContext) = 0;
    virtual void startClientStream(std::shared_ptr<QGrpcOperationContext> operationContext) = 0;
    virtual void startBidiStream(std::shared_ptr<QGrpcOperationContext> operationContext) = 0;

    [[nodiscard]] const QGrpcChannelOptions &channelOptions() const & noexcept;
    void channelOptions() && = delete;

private:
    std::shared_ptr<QGrpcCallReply> call(QLatin1StringView method, QLatin1StringView service,
                                         QByteArrayView arg, const QGrpcCallOptions &options);
    std::shared_ptr<QGrpcServerStream> startServerStream(QLatin1StringView method,
                                                         QLatin1StringView service,
                                                         QByteArrayView arg,
                                                         const QGrpcCallOptions &options);
    std::shared_ptr<QGrpcClientStream> startClientStream(QLatin1StringView method,
                                                         QLatin1StringView service,
                                                         QByteArrayView arg,
                                                         const QGrpcCallOptions &options);
    std::shared_ptr<QGrpcBidiStream> startBidiStream(QLatin1StringView method,
                                                     QLatin1StringView service, QByteArrayView arg,
                                                     const QGrpcCallOptions &options);

    friend class QGrpcClientBase;
    Q_DISABLE_COPY_MOVE(QAbstractGrpcChannel)
    std::unique_ptr<QAbstractGrpcChannelPrivate> dPtr;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(std::shared_ptr<QAbstractGrpcChannel>)

#endif // QABSTRACTGRPCCHANNEL_H
