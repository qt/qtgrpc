// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#include <QtGrpc/private/qgrpcinterceptorchain_p.h>
#include <QtGrpc/private/qtgrpclogging_p.h>

QT_BEGIN_NAMESPACE

void QGrpcInterceptorChainPrivate::clear()
{
    m_combinedCapabilities = {};
    m_interceptors.clear();
}

bool QGrpcInterceptorChainPrivate::add(void *interceptor, DeleterFn deleter,
                                       QSpan<const QtGrpcPrivate::InterceptorCapabilityBinding>
                                           bindings)
{
    Q_ASSERT(interceptor);
    Q_ASSERT(!bindings.empty());

    // Entry takes ownership with RAII semantics.
    Entry e{
        interceptor,
        deleter,
        { bindings.cbegin(), bindings.cend() }
    };

    m_combinedCapabilities |= e.capabilities;
    m_interceptors.push_back(std::move(e));

    return true;
}

QGrpcInterceptorChain::QGrpcInterceptorChain() : d_ptr(new QGrpcInterceptorChainPrivate())
{
}

QGrpcInterceptorChain::~QGrpcInterceptorChain()
{
    delete d_ptr;
}

void QGrpcInterceptorChain::clear()
{
    Q_D(QGrpcInterceptorChain);
    d->clear();
}

qsizetype QGrpcInterceptorChain::size() const noexcept
{
    Q_D(const QGrpcInterceptorChain);
    return d->m_interceptors.size();
}

bool QGrpcInterceptorChain::hasHandlerFor(QtGrpc::InterceptorCapability cap) const noexcept
{
    Q_D(const QGrpcInterceptorChain);
    return d->hasHandlerFor(cap);
}

QtGrpc::InterceptorCapabilities QGrpcInterceptorChain::capabilities() const noexcept
{
    Q_D(const QGrpcInterceptorChain);
    return d->m_combinedCapabilities;
}

QtGrpc::InterceptorCapabilities
QGrpcInterceptorChain::capabilitiesAt(qsizetype index) const
{
    Q_D(const QGrpcInterceptorChain);
    Q_PRE(index >= 0 && index < size());
    return d->m_interceptors[index].capabilities;
}

bool QGrpcInterceptorChain::addImpl(void *interceptor, QGrpcInterceptorChain::DeleterFn deleter,
                                    QSpan<const QtGrpcPrivate::InterceptorCapabilityBinding>
                                        bindings)
{
    Q_D(QGrpcInterceptorChain);
    return d->add(interceptor, deleter, bindings);
}

void QGrpcInterceptorChain::warnNullInterceptor(qsizetype index, qsizetype total) const
{
    if (index == -1 && total == -1) {
        qGrpcWarning("Cannot add null interceptor");
    } else {
        qGrpcWarning("Cannot set null interceptor at index %lld of %lld",
                     static_cast<long long>(index), static_cast<long long>(total));
    }
}

QT_END_NAMESPACE
