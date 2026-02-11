// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#ifndef QGRPCINTERCEPTORCHAIN_H
#define QGRPCINTERCEPTORCHAIN_H

#include <QtGrpc/qgrpcinterceptor.h>
#include <QtGrpc/qtgrpcglobal.h>
#include <QtGrpc/qtgrpcnamespace.h>

#include <QtCore/qcompilerdetection.h>
#include <QtCore/qspan.h>
#include <QtCore/qstring.h>

#include <memory>
#include <type_traits>

QT_BEGIN_NAMESPACE

class QGrpcInterceptorChainPrivate;
class QGrpcInterceptorChain final
{
public:
    Q_GRPC_EXPORT QGrpcInterceptorChain();
    Q_GRPC_EXPORT ~QGrpcInterceptorChain();

    Q_DISABLE_COPY(QGrpcInterceptorChain)

    QGrpcInterceptorChain(QGrpcInterceptorChain &&other) noexcept
        : d_ptr(std::exchange(other.d_ptr, nullptr))
    {
    }
    QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QGrpcInterceptorChain)

    void swap(QGrpcInterceptorChain &other) noexcept { qt_ptr_swap(d_ptr, other.d_ptr); }

    template <typename T, QtGrpcPrivate::if_interceptor<T> = true>
    [[nodiscard]] bool add(std::unique_ptr<T> &&interceptor)
    {
        return addHelper(std::move(interceptor));
    }

    template <typename... Ts,
              std::enable_if_t<(QtGrpcPrivate::is_interceptor_v<Ts> && ...), bool> = true>
    [[nodiscard]] bool set(std::unique_ptr<Ts> &&...interceptors)
    {
        return setHelper(std::move(interceptors)...);
    }

    template <typename T, QtGrpcPrivate::if_interceptor<T> = true>
    [[nodiscard]] bool add(T *interceptor)
    {
        return addHelper(interceptor);
    }

    template <typename... Ts,
              std::enable_if_t<(QtGrpcPrivate::is_interceptor_v<Ts> && ...), bool> = true>
    [[nodiscard]] bool set(Ts *...interceptors)
    {
        return setHelper(interceptors...);
    }

    Q_GRPC_EXPORT void clear();

    [[nodiscard]] Q_GRPC_EXPORT qsizetype size() const noexcept;
    [[nodiscard]] bool isEmpty() const noexcept { return !size(); }
    [[nodiscard]] bool empty() const noexcept { return isEmpty(); }

    [[nodiscard]] Q_GRPC_EXPORT bool
    hasHandlerFor(QtGrpc::InterceptorCapability cap) const noexcept;
    [[nodiscard]] Q_GRPC_EXPORT QtGrpc::InterceptorCapabilities capabilities() const noexcept;
    [[nodiscard]] Q_GRPC_EXPORT QtGrpc::InterceptorCapabilities
    capabilitiesAt(qsizetype index) const;

private:
    using DeleterFn = void (*)(void *);

    template <typename T>
    struct is_unique_ptr : std::false_type
    {
        using element_type = std::remove_pointer_t<T>; // For raw pointers: T* -> T
    };
    template <typename T>
    struct is_unique_ptr<std::unique_ptr<T>> : std::true_type
    {
        using element_type = T;
    };

    template <typename T>
    [[nodiscard]] bool addHelper(T &&interceptor)
    {
        using namespace QtGrpcPrivate;
        using DecayedT = std::decay_t<T>;
        constexpr bool is_uptr = is_unique_ptr<DecayedT>::value;
        using ElementType = typename is_unique_ptr<DecayedT>::element_type;

        if (!interceptor) {
            warnNullInterceptor();
            return false;
        }

        ElementType *ptr = nullptr;
        if constexpr (is_uptr)
            ptr = interceptor.get();
        else
            ptr = interceptor;

        DeleterFn deleter = nullptr;
        if constexpr (is_uptr)
            deleter = [](void *p) { delete static_cast<ElementType *>(p); };

        auto bindings = InterceptorCapabilityBinding::extractFrom(ptr);

        if (!addImpl(ptr, deleter, bindings))
            return false;

        if constexpr (is_uptr)
            interceptor.release(); // we take ownership on success

        return true;
    }

    template <typename... Ts>
    [[nodiscard]] bool setHelper(Ts &&...interceptors)
    {
        qsizetype failedIndex = -1;
        qsizetype idx = 0;
        auto checkNull = [&](const auto &p) {
            if (!p && failedIndex == -1)
                failedIndex = idx;
            ++idx;
        };
        (checkNull(interceptors), ...);

        if (failedIndex != -1) {
            warnNullInterceptor(failedIndex, sizeof...(Ts) - 1);
            return false;
        }

        QGrpcInterceptorChain tmp;
        bool ok = (... && tmp.addHelper(std::forward<Ts>(interceptors)));

        if (ok)
            *this = std::move(tmp);

        return ok;
    }

    [[nodiscard]] Q_GRPC_EXPORT bool
    addImpl(void *interceptor, DeleterFn deleter,
            QSpan<const QtGrpcPrivate::InterceptorCapabilityBinding> bindings);

    Q_DECL_COLD_FUNCTION Q_GRPC_EXPORT void warnNullInterceptor(qsizetype index = -1,
                                                                qsizetype total = -1) const;

    // prevent users from implementing their own, as the data-members don't qualify for it.
    constexpr friend size_t qHash(const QGrpcInterceptorChain &, size_t) = delete;
#ifndef QT_NO_DEBUG_STREAM
    friend QDebug operator<<(QDebug debug, const QGrpcInterceptorChain &) = delete;
#endif
#ifndef QT_NO_DATASTREAM
    friend QDataStream &operator<<(QDataStream &, const QGrpcInterceptorChain &) = delete;
    friend QDataStream &operator>>(QDataStream &, QGrpcInterceptorChain &) = delete;
#endif

    QGrpcInterceptorChainPrivate *d_ptr;

    friend class QGrpcInterceptorEngine;
    Q_DECLARE_PRIVATE(QGrpcInterceptorChain)
};

QT_END_NAMESPACE

#endif // QGRPCINTERCEPTOR_H
