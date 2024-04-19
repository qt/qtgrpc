// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCLIENTBASE_H
#define QGRPCLIENTBASE_H

#include <QtCore/qobject.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qbytearray.h>
#include <QtGrpc/qabstractgrpcchannel.h>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstatus.h>
#include <QtGrpc/qtgrpcglobal.h>
#include <QtProtobuf/qabstractprotobufserializer.h>

#include <memory>
#include <optional>
#include <type_traits>

QT_BEGIN_NAMESPACE

class QGrpcOperation;
class QAbstractGrpcChannel;
class QGrpcClientBasePrivate;

class Q_GRPC_EXPORT QGrpcClientBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::shared_ptr<QAbstractGrpcChannel> channel READ channel
                       WRITE attachChannel NOTIFY channelChanged)
public:
    void attachChannel(const std::shared_ptr<QAbstractGrpcChannel> &channel);
    [[nodiscard]] std::shared_ptr<QAbstractGrpcChannel> channel() const noexcept;
    ~QGrpcClientBase() override;

Q_SIGNALS:
    void errorOccurred(const QGrpcStatus &status);
    void channelChanged();

protected:
    explicit QGrpcClientBase(QLatin1StringView service, QObject *parent = nullptr);

    std::shared_ptr<QGrpcCallReply> call(QLatin1StringView method, const QProtobufMessage &arg,
                                         const QGrpcCallOptions &options);

#ifdef Q_QDOC
    template <typename StreamType>
#else
    template <typename StreamType,
              typename IsCompatibleStream = typename std::enable_if_t<
                  std::disjunction_v<std::is_same<StreamType, QGrpcServerStream>,
                                     std::is_same<StreamType, QGrpcClientStream>,
                                     std::is_same<StreamType, QGrpcBidirStream>>,
                  bool>>
#endif
    std::shared_ptr<StreamType> startStream(QLatin1StringView method, const QProtobufMessage &arg,
                                            const QGrpcCallOptions &options)
    {
        std::optional<QByteArray> argData = trySerialize(arg);
        if (!argData) {
            Q_EMIT errorOccurred(QGrpcStatus{ QGrpcStatus::Unknown,
                                              tr("Serializing failed. Serializer is not ready.") });
            return {};
        }
        if constexpr (std::is_same_v<StreamType, QGrpcServerStream>) {
            return startServerStream(method, *argData, options);
        } else if constexpr (std::is_same_v<StreamType, QGrpcClientStream>) {
            return startClientStream(method, *argData, options);
        } else if constexpr (std::is_same_v<StreamType, QGrpcBidirStream>) {
            return startBidirStream(method, *argData, options);
        }
        Q_UNREACHABLE_RETURN({});
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

    std::optional<QByteArray> trySerialize(const QProtobufMessage &arg) const;

    Q_DISABLE_COPY_MOVE(QGrpcClientBase)
    Q_DECLARE_PRIVATE(QGrpcClientBase)
};

QT_END_NAMESPACE

#endif // QGRPCLIENTBASE_H
