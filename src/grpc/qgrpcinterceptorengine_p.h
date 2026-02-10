// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#ifndef QGRPCINTERCEPTORENGINE_P_H
#define QGRPCINTERCEPTORENGINE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGrpc/qgrpcinterceptor.h>
#include <QtGrpc/qgrpcoperationcontext.h>
#include <QtGrpc/qtgrpcnamespace.h>

#include <QtProtobuf/qprotobufmessage.h>

#include <QtCore/q20bit.h>
#include <QtCore/qspan.h>
#include <QtCore/qtconfigmacros.h>
#include <QtCore/qvarlengtharray.h>

#include <algorithm>
#include <array>
#include <iterator>
#include <limits>
#include <type_traits>

QT_BEGIN_NAMESPACE

namespace QtGrpcPrivate {

inline constexpr std::underlying_type_t<InterceptorCapability>
capabilityToIndex(InterceptorCapability c) noexcept
{
    return q20::countr_zero(qToUnderlying(c));
}
inline constexpr InterceptorCapability indexToCapability(uint8_t idx) noexcept
{
    Q_ASSERT(idx < std::numeric_limits<std::underlying_type_t<InterceptorCapability>>::digits);
    return InterceptorCapability(1u << idx);
}

static_assert(capabilityToIndex(InterceptorCapability::Start) == 0);
static_assert(capabilityToIndex(InterceptorCapability::InitialMetadata) == 1);
static_assert(capabilityToIndex(InterceptorCapability::MessageReceived) == 2);
static_assert(capabilityToIndex(InterceptorCapability::WriteMessage) == 3);
static_assert(capabilityToIndex(InterceptorCapability::WritesDone) == 4);
static_assert(capabilityToIndex(InterceptorCapability::TrailingMetadata) == 5);
static_assert(capabilityToIndex(InterceptorCapability::Finished) == 6);
static_assert(capabilityToIndex(InterceptorCapability::Cancel) == 7);
static_assert(InterceptorTypes::size == 8, "Please add assertions for all entries");

} // namespace QtGrpcPrivate

class QGrpcInterceptorEngine
{
public:
    explicit QGrpcInterceptorEngine(const QAbstractGrpcChannel &parentChannel) noexcept
        : m_parentChannel(parentChannel)
    {
    }

    void addInterceptor(void *interceptor,
                        QSpan<const QtGrpcPrivate::InterceptorCapabilityBinding> bindings)
    {
        Q_ASSERT(interceptor);
        Q_ASSERT(!bindings.empty());

        for (auto binding : bindings) {
            const auto idx = QtGrpcPrivate::capabilityToIndex(binding.capability);
            // Track the interceptor to correlate binding entries with their owning interceptor
            m_capabilityHandlers[idx].push_back({ binding.interface, interceptor });
            m_combinedCapabilities.setFlag(binding.capability);
        }
        m_storage.push_back(interceptor);
    }

    [[nodiscard]] bool removeInterceptor(const void *interceptor)
    {
        if (!interceptor)
            return false;

        const auto rit = std::find(m_storage.rbegin(), m_storage.rend(), interceptor);
        if (rit == m_storage.rend())
            return false;

        // Walk all capability handlers and remove the last handlers owned by this interceptor
        size_t i = 0;
        for (auto &handlers : m_capabilityHandlers) {
            // Remove only the most recent handler entry matching this interceptor
            for (auto rjt = handlers.rbegin(); rjt != handlers.rend(); ++rjt) {
                if (rjt->identity == interceptor) {
                    handlers.erase(std::next(rjt).base());
                    if (handlers.empty()) {
                        const auto capability = QtGrpcPrivate::indexToCapability(i);
                        m_combinedCapabilities.setFlag(capability, false);
                    }
                    break;
                }
            }
            ++i;
        }

        m_storage.erase(std::next(rit).base());
        return true;
    }

    void removeAllInterceptors() noexcept
    {
        for (auto &handlers : m_capabilityHandlers)
            handlers.clear();
        m_storage.clear();
        m_combinedCapabilities = {};
    }

    [[nodiscard]] bool hasHandlerFor(QtGrpcPrivate::InterceptorCapability capability) const noexcept
    {
        return m_combinedCapabilities.testFlag(capability);
    }

    [[nodiscard]]
    QGrpcStartInterceptor::Continuation onStart(QGrpcOperationContext &context,
                                                QProtobufMessage &message,
                                                QGrpcCallOptions &callOptions)
    {
        using Info = QtGrpcPrivate::InterceptorInfo<QGrpcStartInterceptor>;
        constexpr auto Idx = QtGrpcPrivate::capabilityToIndex(Info::capability);

        QGrpcInterceptionContext ctx(m_parentChannel, context);
        for (const auto &handler : m_capabilityHandlers[Idx]) {
            auto *handlerInterface = static_cast<QGrpcStartInterceptor *>(handler.interface);
            auto result = (handlerInterface->*Info::method)(ctx, message, callOptions);
            if (result == QGrpcStartInterceptor::Drop)
                return result;
        }

        return QGrpcStartInterceptor::Proceed;
    }

    void onInitialMetadata(QGrpcOperationContext &context,
                           QMultiHash<QByteArray, QByteArray> &metadata)
    {
        invoke<QGrpcInitialMetadataInterceptor>(context, metadata);
    }

    void onMessageReceived(QGrpcOperationContext &context, QByteArray &messageData)
    {
        invoke<QGrpcMessageReceivedInterceptor>(context, messageData);
    }

    void onWriteMessage(QGrpcOperationContext &context, QProtobufMessage &message)
    {
        invoke<QGrpcWriteMessageInterceptor>(context, message);
    }

    void onWritesDone(QGrpcOperationContext &context)
    {
        invoke<QGrpcWritesDoneInterceptor>(context);
    }

    void onTrailingMetadata(QGrpcOperationContext &context,
                            QMultiHash<QByteArray, QByteArray> &metadata)
    {
        invoke<QGrpcTrailingMetadataInterceptor>(context, metadata);
    }

    void onFinished(QGrpcOperationContext &context, QGrpcStatus &status)
    {
        invoke<QGrpcFinishedInterceptor>(context, status);
    }

    void onCancel(QGrpcOperationContext &context) { invoke<QGrpcCancelInterceptor>(context); }

private:
    template <typename Interface, typename... Args>
    void invoke(QGrpcOperationContext &context, Args &&...args)
    {
        using Info = QtGrpcPrivate::InterceptorInfo<Interface>;
        constexpr auto Idx = QtGrpcPrivate::capabilityToIndex(Info::capability);

        QGrpcInterceptionContext ctx(m_parentChannel, context);
        for (auto handler : m_capabilityHandlers[Idx]) {
            auto *handlerInterface = static_cast<Interface *>(handler.interface);
            (handlerInterface->*Info::method)(ctx, std::forward<Args>(args)...);
        }
    }

    const QAbstractGrpcChannel &m_parentChannel;

    template <typename T>
    using InterceptorList = QVarLengthArray<T, 5>;

    InterceptorList<void *> m_storage;
    struct Handler
    {
        void *interface; // The actual capability interface
        const void *identity; // The base interceptor object in storage
    };
    // Each interceptor has its capabilities mapped to a slot in the array
    std::array<InterceptorList<Handler>, QtGrpcPrivate::InterceptorTypes::size>
        m_capabilityHandlers;
    QtGrpcPrivate::InterceptorCapabilities m_combinedCapabilities = {};
};

QT_END_NAMESPACE

#endif // QGRPCINTERCEPTORENGINE_P_H
