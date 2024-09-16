// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/private/qabstractgrpcchannel_p.h>
#include <QtGrpc/private/qtgrpclogging_p.h>
#include <QtGrpc/qgrpcclientbase.h>
#include <QtGrpc/qgrpcoperation.h>

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/private/qminimalflatset_p.h>
#include <QtCore/private/qobject_p.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qlatin1stringview.h>

#include <optional>
#include <type_traits>

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcClientBase
    \inmodule QtGrpc
    \brief The QGrpcClientBase class is bridge between gRPC clients
    and channels.

    QGrpcClientBase provides a set of functions for client classes
    generated out of protobuf services.
    QGrpcClientBase enforces thread safety for startStream() and call() methods
    of generated clients.
    The methods QGrpcClientBase::call() and QGrpcClientBase::startStream()
    should only be called by the generated client classes.
*/

/*!
    \fn template <typename StreamType, QGrpcClientBase::if_qtgrpc_stream<StreamType> = true> std::shared_ptr<StreamType> QGrpcClientBase::startStream(QLatin1StringView method, const QProtobufMessage &arg, const QGrpcCallOptions &options)

    Starts the stream \a method of the \e StreamType type with the message
    argument \a arg to the attached channel.

    Uses \a options argument to set additional parameter in the stream
    communication.

    The implementation is only available for \e StreamType:
    QGrpcServerStream, QGrpcClientStream, and QGrpcBidiStream.
*/

/*!
    \fn void QGrpcClientBase::channelChanged()
    \since 6.7

    Indicates that a new channel is attached to the client.
*/

/*!
    \fn void QGrpcClientBase::errorOccurred(const QGrpcStatus &status);

    Indicates that an error occurred during serialization.

    This signal is emitted when an error with \a status occurs in the channel
    or during serialization.

    \sa QGrpcOperation::errorOccurred
*/

namespace {
template <typename Operation>
inline constexpr bool IsStream = false;
template <>
inline constexpr bool IsStream<QGrpcServerStream> = true;
template <>
inline constexpr bool IsStream<QGrpcClientStream> = true;
template <>
inline constexpr bool IsStream<QGrpcBidiStream> = true;
}

class QGrpcClientBasePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QGrpcClientBase)
public:
    explicit QGrpcClientBasePrivate(QLatin1StringView service) : service(service) { }

    void addStream(QGrpcOperation *stream);
    std::optional<QByteArray> trySerialize(const QProtobufMessage &arg) const;
    bool isReady() const;

    std::shared_ptr<QAbstractProtobufSerializer> serializer() const
    {
        return channel ? channel->serializer() : nullptr;
    }

    template <typename T>
    [[nodiscard]] std::unique_ptr<T> initOperation(QLatin1StringView method,
                                                   const QProtobufMessage &arg,
                                                   const QGrpcCallOptions &options)
    {
        if (!isReady())
            return {};

        const auto argData = trySerialize(arg);
        if (!argData)
            return {};

        using ChannelFn = std::unique_ptr<T> (QAbstractGrpcChannel::*)(QLatin1StringView,
                                                                       QLatin1StringView,
                                                                       QByteArrayView,
                                                                       const QGrpcCallOptions &);
        constexpr ChannelFn initializer = [&]() -> ChannelFn {
            if constexpr (std::is_same_v<T, QGrpcServerStream>) {
                return &QAbstractGrpcChannel::serverStream;
            } else if constexpr (std::is_same_v<T, QGrpcClientStream>) {
                return &QAbstractGrpcChannel::clientStream;
            } else if constexpr (std::is_same_v<T, QGrpcBidiStream>) {
                return &QAbstractGrpcChannel::bidiStream;
            } else if constexpr (std::is_same_v<T, QGrpcCallReply>) {
                return &QAbstractGrpcChannel::call;
            } else {
                Q_UNREACHABLE_RETURN(nullptr);
            }
        }();

        std::unique_ptr<T> operation = ((*channel).*(initializer))(method, service, *argData,
                                                                   options);

        if constexpr (IsStream<T>)
            addStream(operation.get());
        return operation;
    }

    std::shared_ptr<QAbstractGrpcChannel> channel;
    const QLatin1StringView service;
    QMinimalFlatSet<QGrpcOperation *> activeStreams;
};

void QGrpcClientBasePrivate::addStream(QGrpcOperation *grpcStream)
{
    Q_ASSERT(grpcStream);

    Q_Q(QGrpcClientBase);
    // Remove the operation pointer upon QObject destruction if it hasn't
    // already been gracefully removed by receiving finished()
    QObject::connect(grpcStream, &QObject::destroyed, q, [this, grpcStream](QObject *obj) {
        Q_ASSERT(obj == grpcStream);
        activeStreams.remove(grpcStream);
    });

    auto finishedConnection = std::make_shared<QMetaObject::Connection>();
    *finishedConnection = QObject::connect(grpcStream, &QGrpcOperation::finished, q,
                                           [this, grpcStream, finishedConnection] {
                                               Q_ASSERT(activeStreams.contains(grpcStream));
                                               activeStreams.remove(grpcStream);
                                               QObject::disconnect(*finishedConnection);
                                           });
    const auto it = activeStreams.insert(grpcStream);
    Q_ASSERT(it.second);
}

std::optional<QByteArray> QGrpcClientBasePrivate::trySerialize(const QProtobufMessage &arg) const
{
    if (auto s = serializer())
        return s->serialize(&arg);

    qGrpcWarning("Serializing failed. Serializer is not ready");
    return std::nullopt;
}

bool QGrpcClientBasePrivate::isReady() const
{
    Q_Q(const QGrpcClientBase);
    if (q->thread() != QThread::currentThread()) {
        qGrpcWarning("QtGrpc doesn't support invocation from a different thread");
        return false;
    }

    if (!channel) {
        qGrpcWarning("No channel(s) attached");
        return false;
    }
    return true;
}

QGrpcClientBase::QGrpcClientBase(QLatin1StringView service, QObject *parent)
    : QObject(*new QGrpcClientBasePrivate(service), parent)
{
}

QGrpcClientBase::~QGrpcClientBase() = default;

/*!
    Attaches \a channel to client as transport layer for gRPC.

    Parameters and return values will be serialized to the channel
    in a format it supports.

    \note \b Warning: Qt GRPC doesn't guarantee thread safety on the channel level.
    You have to invoke the channel-related functions on the same thread as
    QGrpcClientBase.
*/
bool QGrpcClientBase::attachChannel(std::shared_ptr<QAbstractGrpcChannel> channel)
{
    Q_D(QGrpcClientBase);
    // channel is not a QObject so we compare against the threadId set on construction.
    if (channel->d_func()->threadId != QThread::currentThreadId()) {
        qGrpcWarning("QtGrpc doesn't allow attaching the channel from a different thread");
        return false;
    }

    for (const auto &stream : d->activeStreams) {
        assert(stream != nullptr);
        stream->cancel();
    }

    d->channel = std::move(channel);
    emit channelChanged();
    return true;
}

/*!
    \since 6.7
    Returns the channel attached to this client.
*/
std::shared_ptr<QAbstractGrpcChannel> QGrpcClientBase::channel() const
{
    Q_D(const QGrpcClientBase);
    return d->channel;
}

std::unique_ptr<QGrpcCallReply> QGrpcClientBase::call(QLatin1StringView method,
                                                      const QProtobufMessage &arg,
                                                      const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);
    return d->initOperation<QGrpcCallReply>(method, arg, options);
}

std::unique_ptr<QGrpcServerStream> QGrpcClientBase::serverStream(QLatin1StringView method,
                                                                 const QProtobufMessage &arg,
                                                                 const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);
    return d->initOperation<QGrpcServerStream>(method, arg, options);
}

std::unique_ptr<QGrpcClientStream> QGrpcClientBase::clientStream(QLatin1StringView method,
                                                                 const QProtobufMessage &arg,
                                                                 const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);
    return d->initOperation<QGrpcClientStream>(method, arg, options);
}

std::unique_ptr<QGrpcBidiStream> QGrpcClientBase::bidiStream(QLatin1StringView method,
                                                             const QProtobufMessage &arg,
                                                             const QGrpcCallOptions &options)
{
    Q_D(QGrpcClientBase);
    return d->initOperation<QGrpcBidiStream>(method, arg, options);
}

bool QGrpcClientBase::event(QEvent *event)
{
    return QObject::event(event);
}

QT_END_NAMESPACE

#include "moc_qgrpcclientbase.cpp"
