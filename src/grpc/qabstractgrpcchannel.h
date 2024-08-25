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

    [[nodiscard]] const QGrpcChannelOptions &channelOptions() const & noexcept;
    void channelOptions() && = delete;

    void setChannelOptions(const QGrpcChannelOptions &options) noexcept;
    void setChannelOptions(QGrpcChannelOptions &&options) noexcept;

protected:
    QAbstractGrpcChannel();
    explicit QAbstractGrpcChannel(QAbstractGrpcChannelPrivate &dd);
    explicit QAbstractGrpcChannel(const QGrpcChannelOptions &options);

    virtual void call(std::shared_ptr<QGrpcOperationContext> operationContext) = 0;
    virtual void serverStream(std::shared_ptr<QGrpcOperationContext> operationContext) = 0;
    virtual void clientStream(std::shared_ptr<QGrpcOperationContext> operationContext) = 0;
    virtual void bidiStream(std::shared_ptr<QGrpcOperationContext> operationContext) = 0;

private:
    std::shared_ptr<QGrpcCallReply> call(QLatin1StringView method, QLatin1StringView service,
                                         QByteArrayView arg,
                                         const QGrpcCallOptions &options);
    std::shared_ptr<QGrpcServerStream> serverStream(QLatin1StringView method,
                                                    QLatin1StringView service, QByteArrayView arg,
                                                    const QGrpcCallOptions &options);
    std::shared_ptr<QGrpcClientStream> clientStream(QLatin1StringView method,
                                                    QLatin1StringView service, QByteArrayView arg,
                                                    const QGrpcCallOptions &options);
    std::shared_ptr<QGrpcBidiStream> bidiStream(QLatin1StringView method, QLatin1StringView service,
                                                QByteArrayView arg,
                                                const QGrpcCallOptions &options);

    friend class QGrpcClientBase;
    Q_DISABLE_COPY_MOVE(QAbstractGrpcChannel)

    Q_DECLARE_PRIVATE(QAbstractGrpcChannel)
    std::unique_ptr<QAbstractGrpcChannelPrivate> d_ptr;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(std::shared_ptr<QAbstractGrpcChannel>)

#endif // QABSTRACTGRPCCHANNEL_H
