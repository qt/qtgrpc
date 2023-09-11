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

    template <typename ParamType, typename ReturnType>
    QGrpcStatus call(QLatin1StringView method, const QProtobufMessage &arg, ReturnType *ret,
                     const QGrpcCallOptions &options)
    {
        Q_ASSERT(ret != nullptr);

        using namespace Qt::StringLiterals;
        QGrpcStatus status{ QGrpcStatus::Unknown,
                            "Serializing failed. Serializer is not ready."_L1 };

        std::optional<QByteArray> argData = trySerialize<ParamType>(arg);
        if (argData) {
            std::shared_ptr<QGrpcCallReply> reply = call(method, *argData, options);
            if (!reply)
                return QGrpcStatus{ QGrpcStatus::Unknown, "No channel(s) attached."_L1 };

            status = reply->waitForFinished();
            if (status == QGrpcStatus::Ok)
                *ret = reply->read<ReturnType>();
        }
        return status;
    }

    template <typename ParamType>
    std::shared_ptr<QGrpcCallReply> call(QLatin1StringView method, const QProtobufMessage &arg,
                                         const QGrpcCallOptions &options)
    {
        std::optional<QByteArray> argData = trySerialize<ParamType>(arg);
        if (!argData)
            return {};
        return call(method, *argData, options);
    }

    template <typename ParamType, typename StreamType,
              std::enable_if_t<std::is_same_v<StreamType, QGrpcServerStream>
                                       || std::is_same_v<StreamType, QGrpcClientStream>
                                       || std::is_same_v<StreamType, QGrpcBidirStream>,
                               bool> = true>
    std::shared_ptr<StreamType> startStream(QLatin1StringView method, const QProtobufMessage &arg,
                                            const QGrpcCallOptions &options)
    {
        std::optional<QByteArray> argData = trySerialize<ParamType>(arg);
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

    template <typename ParamType>
    std::optional<QByteArray> trySerialize(const QProtobufMessage &arg)
    {
        using namespace Qt::StringLiterals;
        auto _serializer = serializer();
        if (_serializer == nullptr) {
            Q_EMIT errorOccurred({ QGrpcStatus::Unknown,
                                   "Serializing failed. Serializer is not ready."_L1 });
            return std::nullopt;
        }
        return _serializer->serialize<ParamType>(&arg);
    }

    std::shared_ptr<QAbstractProtobufSerializer> serializer() const;

    Q_DISABLE_COPY_MOVE(QAbstractGrpcClient)
    Q_DECLARE_PRIVATE(QAbstractGrpcClient)
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QAbstractGrpcChannel *)
Q_DECLARE_METATYPE(std::shared_ptr<QAbstractGrpcChannel>)

#endif // QABSTRACTGRPCLIENT_H
