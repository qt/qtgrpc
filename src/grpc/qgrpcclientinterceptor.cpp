// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "qgrpcclientinterceptor.h"

QT_BEGIN_NAMESPACE

/*!
    \typealias QGrpcInterceptorContinuation::ReplyType
    \inmodule QtGrpc
    \since 6.7
    \internal
*/

/*!
    \typealias QGrpcInterceptorContinuation::ParamType
    \inmodule QtGrpc
    \since 6.7
    \internal
*/
/*!
    \class QGrpcInterceptorContinuation
    \inmodule QtGrpc
    \since 6.7
    \brief A template class for QtGRPC interceptor continuation.

    The QGrpcInterceptorContinuation class is a helper class that wraps std::function,
    based on the QGrpcOperation type for the specific type of the call or stream. It is used
    for managing the continuation of QtGRPC interceptor processing.
*/

/*!
    \fn template <typename T> QGrpcInterceptorContinuation<T>::QGrpcInterceptorContinuation(std::function<void(ReplyType, ParamType)> _func)

    Constructs a QGrpcInterceptorContinuation with the provided function \a _func.
*/
/*!
    \fn template <typename T> void QGrpcInterceptorContinuation<T>::operator()(ReplyType response,
        std::shared_ptr<QGrpcChannelOperation> param)

    Invokes the continuation function with the given \a response and \a param parameter.
*/
/*!
    \fn template <typename T, std::enable_if_t<std::is_base_of_v<QGrpcOperation, T>, bool> = true> void QGrpcClientInterceptor::intercept(std::shared_ptr<QGrpcChannelOperation> operation,
        typename QGrpcInterceptorContinuation<T>::ReplyType response, QGrpcInterceptorContinuation<T> &continuation)

    Intercepts a QtGRPC call or stream operation.

    This method provides a generic interface for intercepting QtGRPC operations based on the
    specified QGrpcOperation.
    It delegates to specialized methods for different types of QtGRPC operations
    and passes \a operation, \a response, and \a continuation parameters to specialized method.
*/

/*!
    \class QGrpcClientInterceptor
    \inmodule QtGrpc
    \since 6.7
    \brief Base class for QtGRPC client interceptors.

    The QGrpcClientInterceptor class provides a base for creating custom QtGRPC client interceptors.
    It defines methods for intercepting different types of QtGRPC calls and streams. Users can it
    to implement specific interception behavior.

    \sa {Qt GRPC Client Interceptors}
*/

/*!
    Default destructor of QGrpcClientInterceptor object.
*/
QGrpcClientInterceptor::~QGrpcClientInterceptor() = default;

/*!
    Intercepts a QtGRPC call operation.

    This method provides the default implementation of a virtual function. Users have the option
    to override this to offer specific functionality for QGrpcClientInterceptor.

    The \a operation carries the values associated with the call.
    The \a response carries a preallocated QGrpcCallReply of the call response.

    The \a continuation is a delegate that initiates the processing of the next interceptor or
    calls the underlying gRPC function. Implementations may call \a continuation zero or more times,
    depending on the desired outcome.
*/
void QGrpcClientInterceptor::interceptCall(std::shared_ptr<QGrpcChannelOperation> operation,
                                           std::shared_ptr<QGrpcCallReply> response,
                                           QGrpcInterceptorContinuation<QGrpcCallReply>
                                               &continuation)
{
    continuation(std::move(response), std::move(operation));
}

/*!
    Intercepts a QtGRPC server streaming operation.

    This method provides the default implementation of a virtual function. Users have the option
    to override this to offer specific functionality for QGrpcClientInterceptor.

    The \a operation carries the values associated with the call.
    The \a response carries a preallocated QGrpcServerStream of the stream response.

    The \a continuation is a delegate that initiates the processing of the next interceptor or
    calls the underlying gRPC function. Implementations may call \a continuation zero or more times,
    depending on the desired outcome.
*/
void QGrpcClientInterceptor::interceptServerStream(std::shared_ptr<QGrpcChannelOperation> operation,
                                                   std::shared_ptr<QGrpcServerStream> response,
                                                   QGrpcInterceptorContinuation<QGrpcServerStream>
                                                       &continuation)
{
    continuation(std::move(response), std::move(operation));
}

/*!
    Intercepts a QtGRPC client streaming operation.

    This method provides the default implementation of a virtual function. Users have the option
    to override this to offer specific functionality for QGrpcClientInterceptor.

    The \a operation carries the values associated with the call.
    The \a response carries a preallocated QGrpcClientStream of the stream response.

    The \a continuation is a delegate that initiates the processing of the next interceptor or
    calls the underlying gRPC function. Implementations may call \a continuation zero or more times,
    depending on the desired outcome.
*/
void QGrpcClientInterceptor::interceptClientStream(std::shared_ptr<QGrpcChannelOperation> operation,
                                                   std::shared_ptr<QGrpcClientStream> response,
                                                   QGrpcInterceptorContinuation<QGrpcClientStream>
                                                       &continuation)
{
    continuation(std::move(response), std::move(operation));
}

/*!
    Intercepts a QtGRPC bidirectional streaming operation.

    This method provides the default implementation of a virtual function. Users have the option
    to override this to offer specific functionality for QGrpcClientInterceptor.

    The \a operation carries the values associated with the call.
    The \a response carries a preallocated QGrpcBidirStream of the stream response.

    The \a continuation is a delegate that initiates the processing of the next interceptor or
    calls the underlying gRPC function. Implementations may call \a continuation zero or more times,
    depending on the desired outcome.
*/
void QGrpcClientInterceptor::interceptBidirStream(std::shared_ptr<QGrpcChannelOperation> operation,
                                                  std::shared_ptr<QGrpcBidirStream> response,
                                                  QGrpcInterceptorContinuation<QGrpcBidirStream>
                                                      &continuation)
{
    continuation(std::move(response), std::move(operation));
}

QT_END_NAMESPACE
