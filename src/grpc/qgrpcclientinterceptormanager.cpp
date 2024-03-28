// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpcclientinterceptormanager.h>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstream.h>

#include <iterator> // std::make_move_iterator

QT_BEGIN_NAMESPACE

class QGrpcClientInterceptorManagerPrivate
{
public:
    std::vector<std::shared_ptr<QGrpcClientInterceptor>> interceptors;
};

/*!
    \class QGrpcClientInterceptorManager
    \inmodule QtGrpc
    \since 6.7
    \brief Manages QGrpcClientInterceptor interceptors.

    The QGrpcClientInterceptorManager class provides methods for registering and executing
    QGrpcClientInterceptor interceptors.

    \sa {Qt GRPC Client Interceptors}
*/

/*!
    Constructs an empty QGrpcClientInterceptorManager object.
*/
QGrpcClientInterceptorManager::QGrpcClientInterceptorManager()
    : dPtr(std::make_unique<QGrpcClientInterceptorManagerPrivate>())
{
}

/*!
    Destroys the QGrpcClientInterceptorManager object.
*/
QGrpcClientInterceptorManager::~QGrpcClientInterceptorManager() = default;

QGrpcClientInterceptorManager::QGrpcClientInterceptorManager(const QGrpcClientInterceptorManager
                                                                 &other)
    : dPtr(std::make_unique<QGrpcClientInterceptorManagerPrivate>(*other.dPtr))
{
}

QGrpcClientInterceptorManager &
QGrpcClientInterceptorManager::operator=(const QGrpcClientInterceptorManager &other)
{
    if (this != &other)
        dPtr = std::make_unique<QGrpcClientInterceptorManagerPrivate>(*other.dPtr);
    return *this;
}

QGrpcClientInterceptorManager::QGrpcClientInterceptorManager(QGrpcClientInterceptorManager
                                                                 &&) noexcept = default;

QGrpcClientInterceptorManager &
QGrpcClientInterceptorManager::operator=(QGrpcClientInterceptorManager &&) noexcept = default;

/*!
    Registers a QGrpcClientInterceptor interceptor.

    Places the \a next interceptor at the beginning of the interceptor chain.
    Interceptors are executed in reverse order of registration.
    For instance,
    \code
        manager.registerInterceptor(myInterceptor1);
        manager.registerInterceptor(myInterceptor2);
    \endcode
    will result in:
    \badcode
        myInterceptor2 -> myInterceptor1 -> Qt GRPC operation
    \endcode
    order of execution.
*/
void QGrpcClientInterceptorManager::registerInterceptor(std::shared_ptr<QGrpcClientInterceptor>
                                                            next)
{
    dPtr->interceptors.push_back(std::move(next));
}

/*!
    \fn void QGrpcClientInterceptorManager::registerInterceptors(std::vector<std::shared_ptr<QGrpcClientInterceptor>> nextInterceptors)
    Registers multiple QGrpcClientInterceptor interceptors.

    Adds the given interceptors to the start of the interceptor chain.
    While execution occurs in the reverse order of registration,
    the original order in the \a nextInterceptors vector is maintained,
    for example:
    \code
        manager.registerInterceptor(myInterceptor1);
        manager.registerInterceptors({myInterceptor2, myInterceptor3});
    \endcode
    will result in:
    \badcode
        myInterceptor2 -> myInterceptor3 -> myInterceptor1 -> Qt GRPC operation
    \endcode
    order of execution.
 */
void QGrpcClientInterceptorManager::
    registerInterceptors(std::vector<std::shared_ptr<QGrpcClientInterceptor>> nextInterceptors)
{
    dPtr->interceptors.insert(dPtr->interceptors.end(),
                              std::make_move_iterator(nextInterceptors.rbegin()),
                              std::make_move_iterator(nextInterceptors.rend()));
}

/*!
    \fn template <typename T> void QGrpcClientInterceptorManager::run(QGrpcInterceptorContinuation<T> &finalCall,
                                        typename QGrpcInterceptorContinuation<T>::ReplyType response,
                                        std::shared_ptr<QGrpcChannelOperation> operation,
                                        size_t pos)
    Executes the Qt GRPC interceptors in the chain for a specific QGrpcOperation type.

    The process initiates with the interceptor located at position
    \a pos in the QGrpcClientInterceptor chain.
    Both \a response and \a operation parameters are relayed to
    the QGrpcClientInterceptor::intercept() method.
    Upon reaching the end of the interceptor chain, the \a finalCall is invoked.
 */
template <typename T>
void QGrpcClientInterceptorManager::run(QGrpcInterceptorContinuation<T> &finalCall,
                                        typename QGrpcInterceptorContinuation<T>::ReplyType
                                            response,
                                        std::shared_ptr<QGrpcChannelOperation> operation,
                                        size_t pos)
{
    if (response->isFinished())
        return;

    if (pos < dPtr->interceptors.size()) {
        auto nextCall =
            [this, pos, &finalCall](typename QGrpcInterceptorContinuation<T>::ReplyType response,
                                    typename QGrpcInterceptorContinuation<T>::ParamType operation) {
                this->run(finalCall, response, operation, pos + 1);
            };
        auto nextInterceptor = QGrpcInterceptorContinuation<T>(nextCall);
        // Execute interceptors in reversed order
        const auto rpos = dPtr->interceptors.size() - 1 - pos;
        dPtr->interceptors[rpos]->intercept<T>(operation, response, nextInterceptor);
        return;
    }
    // It's the time to call actuall call
    finalCall(response, operation);
}

template Q_GRPC_EXPORT void
QGrpcClientInterceptorManager::run(QGrpcInterceptorContinuation<QGrpcCallReply> &finalCall,
                                   typename QGrpcInterceptorContinuation<QGrpcCallReply>::ReplyType
                                       response,
                                   std::shared_ptr<QGrpcChannelOperation> operation, size_t pos);
template Q_GRPC_EXPORT void
QGrpcClientInterceptorManager::run(QGrpcInterceptorContinuation<QGrpcServerStream> &finalCall,
                                   typename QGrpcInterceptorContinuation<
                                       QGrpcServerStream>::ReplyType response,
                                   std::shared_ptr<QGrpcChannelOperation> operation, size_t pos);
template Q_GRPC_EXPORT void
QGrpcClientInterceptorManager::run(QGrpcInterceptorContinuation<QGrpcClientStream> &finalCall,
                                   typename QGrpcInterceptorContinuation<
                                       QGrpcClientStream>::ReplyType response,
                                   std::shared_ptr<QGrpcChannelOperation> operation, size_t pos);
template Q_GRPC_EXPORT void
QGrpcClientInterceptorManager::run(QGrpcInterceptorContinuation<QGrpcBidirStream> &finalCall,
                                   typename QGrpcInterceptorContinuation<
                                       QGrpcBidirStream>::ReplyType response,
                                   std::shared_ptr<QGrpcChannelOperation> operation, size_t pos);

QT_END_NAMESPACE
