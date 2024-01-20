// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTGRPCLIENT_H
#define QABSTRACTGRPCLIENT_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QWeakPointer>
#include <QtCore/qbytearray.h>
#include <QtGrpc/qabstractgrpcchannel.h>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstatus.h>
#include <QtGrpc/qtgrpcglobal.h>
#include <QtProtobuf/qabstractprotobufserializer.h>

#include <memory>
#include <optional>

QT_BEGIN_NAMESPACE

class QGrpcOperation;
class QAbstractGrpcChannel;
class QAbstractGrpcClientPrivate;

class Q_GRPC_EXPORT QAbstractGrpcClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::shared_ptr<QAbstractGrpcChannel> channel READ channel
                       WRITE attachChannel NOTIFY channelChanged)
public:
    void attachChannel(const std::shared_ptr<QAbstractGrpcChannel> &channel);
    const std::shared_ptr<QAbstractGrpcChannel> &channel();

Q_SIGNALS:
    void errorOccurred(const QGrpcStatus &status);
    void channelChanged();

protected:
    explicit QAbstractGrpcClient(QLatin1StringView service, QObject *parent = nullptr);
    ~QAbstractGrpcClient() override;

    std::shared_ptr<QGrpcCallReply> call(QLatin1StringView method, const QProtobufMessage &arg,
                                         const QGrpcCallOptions &options);

#ifdef Q_QDOC
    template <typename StreamType>
#else
    template <typename StreamType,
              std::enable_if_t<std::is_same_v<StreamType, QGrpcServerStream>
                                   || std::is_same_v<StreamType, QGrpcClientStream>
                                   || std::is_same_v<StreamType, QGrpcBidirStream>,
                               bool> = true>
#endif
    std::shared_ptr<StreamType> startStream(QLatin1StringView method, const QProtobufMessage &arg,
                                            const QGrpcCallOptions &options)
    {
        std::optional<QByteArray> argData = trySerialize(arg);
        if (!argData)
            return {};
        if constexpr (std::is_same_v<StreamType, QGrpcServerStream>) {
            return startServerStream(method, *argData, options);
        } else if constexpr (std::is_same_v<StreamType, QGrpcClientStream>) {
            return startClientStream(method, *argData, options);
        } else if constexpr (std::is_same_v<StreamType, QGrpcBidirStream>) {
            return startBidirStream(method, *argData, options);
        }
        return {};
    }

private:
    std::shared_ptr<QGrpcCallReply> call(QLatin1StringView method, QByteArrayView arg,
                                         const QGrpcCallOptions &options);

    std::shared_ptr<QGrpcServerStream> startServerStream(QLatin1StringView method,
                                                         QByteArrayView arg,
                                                         const QGrpcCallOptions &options);
    std::shared_ptr<QGrpcClientStream> startClientStream(QLatin1StringView method,
                                                         QByteArrayView arg,
                                                         const QGrpcCallOptions &options);
    std::shared_ptr<QGrpcBidirStream> startBidirStream(QLatin1StringView method, QByteArrayView arg,
                                                       const QGrpcCallOptions &options);

    std::optional<QByteArray> trySerialize(const QProtobufMessage &arg);
    std::shared_ptr<QAbstractProtobufSerializer> serializer() const;

    Q_DISABLE_COPY_MOVE(QAbstractGrpcClient)
    Q_DECLARE_PRIVATE(QAbstractGrpcClient)
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QAbstractGrpcChannel *)
Q_DECLARE_METATYPE(std::shared_ptr<QAbstractGrpcChannel>)

#endif // QABSTRACTGRPCLIENT_H
