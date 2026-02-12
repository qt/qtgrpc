// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#include <QtGrpc/qgrpcinterceptor.h>
#include <QtGrpc/qgrpcoperationcontext.h>

using namespace QtGrpc;
using QtGrpcPrivate::InterceptorTypes;

static_assert(InterceptorTypes::size <= std::numeric_limits<
                  std::underlying_type_t<QtGrpc::InterceptorCapability>>::digits,
              "Interceptors exceed the capability limit!");

QT_BEGIN_NAMESPACE

const QGrpcCallOptions &QGrpcInterceptionContext::callOptions() const & noexcept
{
    return m_operationContext->callOptions();
}

RpcDescriptor QGrpcInterceptionContext::descriptor() const noexcept
{
    return m_operationContext->descriptor();
}

quint64 QGrpcInterceptionContext::operationId() const noexcept
{
    return m_operationContext->operationId();
}

QGrpcStartInterceptor::~QGrpcStartInterceptor() = default;
QGrpcInitialMetadataInterceptor::~QGrpcInitialMetadataInterceptor() = default;
QGrpcMessageReceivedInterceptor::~QGrpcMessageReceivedInterceptor() = default;
QGrpcWriteMessageInterceptor::~QGrpcWriteMessageInterceptor() = default;
QGrpcWritesDoneInterceptor::~QGrpcWritesDoneInterceptor() = default;
QGrpcTrailingMetadataInterceptor::~QGrpcTrailingMetadataInterceptor() = default;
QGrpcFinishedInterceptor::~QGrpcFinishedInterceptor() = default;
QGrpcCancelInterceptor::~QGrpcCancelInterceptor() = default;

QT_END_NAMESPACE
