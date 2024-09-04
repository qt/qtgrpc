// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCLIENTBASE_H
#define QGRPCLIENTBASE_H

#include <QtGrpc/qabstractgrpcchannel.h>
#include <QtGrpc/qgrpccalloptions.h>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstatus.h>
#include <QtGrpc/qgrpcstream.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <QtCore/qobject.h>
#include <QtCore/qstringfwd.h>
#include <QtCore/qtclasshelpermacros.h>

#include <memory>
#include <type_traits>

QT_BEGIN_NAMESPACE

namespace QtGrpcPrivate {
template <typename T> inline constexpr bool is_qtgrpc_stream_v = false;
template <> inline constexpr bool is_qtgrpc_stream_v<QGrpcServerStream> = true;
template <> inline constexpr bool is_qtgrpc_stream_v<QGrpcClientStream> = true;
template <> inline constexpr bool is_qtgrpc_stream_v<QGrpcBidiStream> = true;
}

class QProtobufMessage;

class QGrpcClientBasePrivate;
class Q_GRPC_EXPORT QGrpcClientBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::shared_ptr<QAbstractGrpcChannel> channel READ channel WRITE attachChannel NOTIFY
                   channelChanged)

    template <typename StreamType>
    using if_qtgrpc_stream = std::enable_if_t<QtGrpcPrivate::is_qtgrpc_stream_v<StreamType>, bool>;

public:
    ~QGrpcClientBase() override;

    bool attachChannel(std::shared_ptr<QAbstractGrpcChannel> channel);
    [[nodiscard]] std::shared_ptr<QAbstractGrpcChannel> channel() const;

Q_SIGNALS:
    void channelChanged();

protected:
    explicit QGrpcClientBase(QLatin1StringView service, QObject *parent = nullptr);

    std::unique_ptr<QGrpcCallReply> call(QLatin1StringView method, const QProtobufMessage &arg,
                                         const QGrpcCallOptions &options);

    std::unique_ptr<QGrpcServerStream> serverStream(QLatin1StringView method,
                                                    const QProtobufMessage &arg,
                                                    const QGrpcCallOptions &options);

    std::unique_ptr<QGrpcClientStream> clientStream(QLatin1StringView method,
                                                    const QProtobufMessage &arg,
                                                    const QGrpcCallOptions &options);

    std::unique_ptr<QGrpcBidiStream> bidiStream(QLatin1StringView method,
                                                const QProtobufMessage &arg,
                                                const QGrpcCallOptions &options);

private:
    Q_DISABLE_COPY_MOVE(QGrpcClientBase)
    Q_DECLARE_PRIVATE(QGrpcClientBase)

public:
    bool event(QEvent *event) override;
};

QT_END_NAMESPACE

#endif // QGRPCLIENTBASE_H
