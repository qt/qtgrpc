// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCCREDENTIALS_H
#define QGRPCCREDENTIALS_H

#include <QtGrpc/qabstractgrpccredentials.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <type_traits>

QT_BEGIN_NAMESPACE

class Q_GRPC_EXPORT QGrpcCallCredentials
{
public:
    virtual ~QGrpcCallCredentials();
    virtual QGrpcCredentialMap operator()() const = 0;
};

class Q_GRPC_EXPORT QGrpcChannelCredentials
{
public:
    virtual ~QGrpcChannelCredentials();
    virtual QGrpcCredentialMap channelCredentials() const = 0;
};

#ifdef Q_QDOC
template <typename Call, typename Channel>
#else
template <typename Call, typename Channel,
          typename std::enable_if_t<
                  std::conjunction_v<std::is_base_of<QGrpcCallCredentials, Call>,
                                     std::is_base_of<QGrpcChannelCredentials, Channel>>,
                  int> = 0>
#endif
class QGrpcCredentials final : public QAbstractGrpcCredentials
{
public:
    explicit QGrpcCredentials(const Call &call, const Channel &channel)
        : mCall(call), mChannel(channel)
    {
    }
    explicit QGrpcCredentials(const Call &call) : mCall(call) { }
    explicit QGrpcCredentials(const Channel &channel) : mChannel(channel) { }
    ~QGrpcCredentials() override = default;

    QGrpcCredentialMap callCredentials() const override { return mCall(); }

    QGrpcCredentialMap channelCredentials() const override { return mChannel.channelCredentials(); }

private:
    QGrpcCredentials() = default;

    Call mCall;
    Channel mChannel;
};

extern Q_GRPC_EXPORT const char *SslConfigCredential;

template <typename Call, typename Channel,
          typename std::enable_if_t<
                  std::conjunction_v<std::is_base_of<QGrpcCallCredentials, Call>,
                                     std::is_base_of<QGrpcChannelCredentials, Channel>>,
                  int> = 0>
std::unique_ptr<QAbstractGrpcCredentials> operator|(const Call &call, const Channel &channel)
{
    return std::make_unique<QGrpcCredentials<Call, Channel>>(call, channel);
}

template <typename Call, typename Channel,
          typename std::enable_if_t<
                  std::conjunction_v<std::is_base_of<QGrpcCallCredentials, Call>,
                                     std::is_base_of<QGrpcChannelCredentials, Channel>>,
                  int> = 0>
std::unique_ptr<QAbstractGrpcCredentials> operator|(const Channel &channel, const Call &call)
{
    return std::make_unique<QGrpcCredentials<Call, Channel>>(call, channel);
}

QT_END_NAMESPACE

#endif // QGRPCCREDENTIALS_H
