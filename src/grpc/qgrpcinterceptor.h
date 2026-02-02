// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#ifndef QGRPCINTERCEPTOR_H
#define QGRPCINTERCEPTOR_H

#include <QtGrpc/qtgrpcglobal.h>
#include <QtGrpc/qtgrpcnamespace.h>

#include <QtCore/qcontainerfwd.h>
#include <QtCore/qflags.h>
#include <QtCore/qstringfwd.h>
#include <QtCore/qtclasshelpermacros.h>

#include <array>
#include <type_traits>
#include <utility>

QT_BEGIN_NAMESPACE

class QGrpcStatus;
class QGrpcOperationContext;
class QGrpcCallOptions;
class QAbstractGrpcChannel;
class QProtobufMessage;

class QGrpcInterceptionContext
{
    const quint32 m_version = 1;

public:
    explicit QGrpcInterceptionContext(const QAbstractGrpcChannel &channel,
                                      const QGrpcOperationContext &context) noexcept
        : m_channel(channel), m_operationContext(context)
    {
    }
    ~QGrpcInterceptionContext() = default;
    Q_DISABLE_COPY_MOVE(QGrpcInterceptionContext)

    quint32 version() const noexcept { return m_version; }

    const QAbstractGrpcChannel &channel() const noexcept { return m_channel; }

    Q_GRPC_EXPORT QtGrpc::RpcDescriptor descriptor() const noexcept;
    Q_GRPC_EXPORT const QGrpcCallOptions &callOptions() const noexcept;

protected:
    const QAbstractGrpcChannel &m_channel;
    const QGrpcOperationContext &m_operationContext;
};

struct Q_GRPC_EXPORT QGrpcStartInterceptor
{
    enum Continuation : uint8_t { Proceed, Drop };

    virtual ~QGrpcStartInterceptor();
    virtual Continuation onStart(QGrpcInterceptionContext &context, QProtobufMessage &message,
                                 QGrpcCallOptions &callOptions) = 0;
};

struct Q_GRPC_EXPORT QGrpcInitialMetadataInterceptor
{
    virtual ~QGrpcInitialMetadataInterceptor();
    virtual void onInitialMetadata(QGrpcInterceptionContext &context,
                                   QMultiHash<QByteArray, QByteArray> &metadata) = 0;
};

struct Q_GRPC_EXPORT QGrpcMessageReceivedInterceptor
{
    virtual ~QGrpcMessageReceivedInterceptor();
    virtual void onMessageReceived(QGrpcInterceptionContext &context, QByteArray &messageData) = 0;
};

struct Q_GRPC_EXPORT QGrpcWriteMessageInterceptor
{
    virtual ~QGrpcWriteMessageInterceptor();
    virtual void onWriteMessage(QGrpcInterceptionContext &context, QProtobufMessage &message) = 0;
};

struct Q_GRPC_EXPORT QGrpcWritesDoneInterceptor
{
    virtual ~QGrpcWritesDoneInterceptor();
    virtual void onWritesDone(QGrpcInterceptionContext &context) = 0;
};

struct Q_GRPC_EXPORT QGrpcTrailingMetadataInterceptor
{
    virtual ~QGrpcTrailingMetadataInterceptor();
    virtual void onTrailingMetadata(QGrpcInterceptionContext &context,
                                    QMultiHash<QByteArray, QByteArray> &metadata) = 0;
};

struct Q_GRPC_EXPORT QGrpcFinishedInterceptor
{
    virtual ~QGrpcFinishedInterceptor();
    virtual void onFinished(QGrpcInterceptionContext &context, QGrpcStatus &status) = 0;
};

struct Q_GRPC_EXPORT QGrpcCancelInterceptor
{
    virtual ~QGrpcCancelInterceptor();
    virtual void onCancel(QGrpcInterceptionContext &context) = 0;
};

namespace QtGrpcPrivate {
// clang-format off

// Stable mapping of interceptor types to bit flags.
// WARNING: Do not reorder or modify existing values. Append only.
enum class InterceptorCapability : quint64 {
    Start            = 0x01,
    InitialMetadata  = 0x02,
    MessageReceived  = 0x04,
    WriteMessage     = 0x08,
    WritesDone       = 0x10,
    TrailingMetadata = 0x20,
    Finished         = 0x40,
    Cancel           = 0x80,
};
Q_DECLARE_FLAGS(InterceptorCapabilities, InterceptorCapability)

template <typename... Ts>
struct TypeList
{
    static constexpr size_t size = sizeof...(Ts);

    template <typename T>
    static constexpr size_t countBaseOf() noexcept
    {
        return (... + (std::is_base_of_v<Ts, T> ? 1u : 0u));
    }

    template <typename T>
    static constexpr bool containsBase() noexcept
    {
        return std::disjunction_v<std::is_base_of<Ts, T>...>;
    }

    template <typename T>
    static constexpr bool containsExact() noexcept
    {
        return std::disjunction_v<std::is_same<T, Ts>...>;
    }

    template <typename Fn>
    static constexpr void forEach(Fn &&fn) noexcept
    {
        forEachImpl(std::forward<Fn>(fn), std::make_index_sequence<size>{});
    }

private:
    template <typename Fn, size_t... Is>
    static constexpr void forEachImpl(Fn &&fn, std::index_sequence<Is...>) noexcept
    {
        (fn(std::integral_constant<quint32, Is>{}, static_cast<Ts *>(nullptr)), ...);
    }
};

using InterceptorTypes = TypeList<
    QGrpcStartInterceptor,
    QGrpcInitialMetadataInterceptor,
    QGrpcMessageReceivedInterceptor,
    QGrpcWriteMessageInterceptor,
    QGrpcWritesDoneInterceptor,
    QGrpcTrailingMetadataInterceptor,
    QGrpcFinishedInterceptor,
    QGrpcCancelInterceptor
>;

// Maps each individual interceptor interface to its capability and method
template <typename T>
struct InterceptorInfo;

#define QGRPC_INTERCEPTOR_INFO(Type, Cap, Method)                      \
    template <>                                                        \
    struct InterceptorInfo<Type>                                       \
    {                                                                  \
        static constexpr auto capability = InterceptorCapability::Cap; \
        static constexpr auto method = &Type::Method;                  \
    }

QGRPC_INTERCEPTOR_INFO(QGrpcStartInterceptor, Start, onStart);
QGRPC_INTERCEPTOR_INFO(QGrpcInitialMetadataInterceptor, InitialMetadata, onInitialMetadata);
QGRPC_INTERCEPTOR_INFO(QGrpcMessageReceivedInterceptor, MessageReceived, onMessageReceived);
QGRPC_INTERCEPTOR_INFO(QGrpcWriteMessageInterceptor, WriteMessage, onWriteMessage);
QGRPC_INTERCEPTOR_INFO(QGrpcWritesDoneInterceptor, WritesDone, onWritesDone);
QGRPC_INTERCEPTOR_INFO(QGrpcTrailingMetadataInterceptor, TrailingMetadata, onTrailingMetadata);
QGRPC_INTERCEPTOR_INFO(QGrpcFinishedInterceptor, Finished, onFinished);
QGRPC_INTERCEPTOR_INFO(QGrpcCancelInterceptor, Cancel, onCancel);

#undef QGRPC_INTERCEPTOR_INFO

struct InterceptorCapabilityBinding
{
    InterceptorCapability capability;
    void *interface;

    // Returns the list of interfaces found in T and extracted from \a interceptor, bound
    // to their capability. A generic interceptor T probably uses multiple inheritance.
    template <typename T>
    static constexpr auto extractFrom(T *interceptor) noexcept
    {
        constexpr auto N = InterceptorTypes::countBaseOf<T>();
        std::array<InterceptorCapabilityBinding, N> bindings{};
        quint32 i = 0;

        InterceptorTypes::forEach(
            [&](auto , auto interfaceTag) constexpr {
                using Interface = std::remove_pointer_t<decltype(interfaceTag)>;
                if constexpr (std::is_base_of_v<Interface, T>) {
                    bindings[i++] = {
                        InterceptorInfo<Interface>::capability,
                        static_cast<Interface *>(interceptor),
                    };
                }
            });

        return bindings;
    }
};

// clang-format on
} // namespace QtGrpcPrivate

template <typename T>
inline constexpr bool is_grpc_interceptor_v = std::conjunction_v<
    std::bool_constant<QtGrpcPrivate::InterceptorTypes::containsBase<T>()>,
    std::bool_constant<!QtGrpcPrivate::InterceptorTypes::containsExact<T>()>>;

template <typename T>
using if_grpc_interceptor = std::enable_if_t<is_grpc_interceptor_v<T>, bool>;

QT_END_NAMESPACE

#endif // QGRPCINTERCEPTOR_H
