// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QQMLGRPCFUNCTIONALHANDLERS_H
#define QQMLGRPCFUNCTIONALHANDLERS_H

#include <QtGrpcQuick/qtgrpcquickexports.h>

#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstream.h>

#include <QtQml/qqmlengine.h>

#include <QtCore/qobject.h>
#include <QtCore/qtconfigmacros.h>

QT_BEGIN_NAMESPACE

namespace QtGrpcQuickFunctional {

Q_ALWAYS_INLINE void validateEngineAndOperation(QJSEngine *jsEngine, QGrpcOperation *operation)
{
    Q_ASSERT(jsEngine != nullptr);
    Q_ASSERT(operation != nullptr);
}

Q_GRPCQUICK_EXPORT
void handleDeserializationError(QJSEngine *jsEngine, QGrpcOperation *operation,
                                const QJSValue &errorCallback);

template <typename Ret>
std::optional<QJSValue> readMessageToJSValue(QJSEngine *jsEngine, QGrpcOperation *operation)
{
    QtGrpcQuickFunctional::validateEngineAndOperation(jsEngine, operation);

    if (std::optional<Ret> result = operation->read<Ret>())
        return std::make_optional(jsEngine->toScriptValue(*result));
    return std::nullopt;
}

Q_GRPCQUICK_EXPORT
void handleReceivedMessageImpl(QJSEngine *jsEngine, QGrpcOperation *operation,
                               std::optional<QJSValue> message, const QJSValue &successCallback,
                               const QJSValue &errorCallback);

template <typename Ret>
void handleReceivedMessage(QJSEngine *jsEngine, QGrpcOperation *operation,
                           const QJSValue &successCallback, const QJSValue &errorCallback)
{
    auto message = QtGrpcQuickFunctional::readMessageToJSValue<Ret>(jsEngine, operation);
    QtGrpcQuickFunctional::handleReceivedMessageImpl(jsEngine, operation, std::move(message),
                                                     successCallback, errorCallback);
}

Q_GRPCQUICK_EXPORT
bool checkReceivedStatus(QJSEngine *jsEngine, const QGrpcStatus &status,
                         const QJSValue &errorCallback);

Q_GRPCQUICK_EXPORT
void connectMultipleReceiveOperationFinished(QJSEngine *jsEngine,
                                             const std::shared_ptr<QGrpcOperation> &operation,
                                             const QJSValue &successCallback,
                                             const QJSValue &errorCallback);

template <typename Ret>
void connectSingleReceiveOperationFinished(QJSEngine *jsEngine,
                                           const std::shared_ptr<QGrpcOperation> &operation,
                                           const QJSValue &successCallback,
                                           const QJSValue &errorCallback)
{
    QtGrpcQuickFunctional::validateEngineAndOperation(jsEngine, operation.get());

    auto finishConnection = std::make_shared<QMetaObject::Connection>();
    *finishConnection = QObject::
        connect(operation.get(), &QGrpcCallReply::finished, jsEngine,
                [jsEngine, successCallback, errorCallback, finishConnection,
                 operation](const QGrpcStatus &status) {
                    // We take 'operation' by copy so that its lifetime
                    // is extended until this lambda is destroyed.
                    if (QtGrpcQuickFunctional::checkReceivedStatus(jsEngine, status,
                                                                   errorCallback)) {
                        QtGrpcQuickFunctional::handleReceivedMessage<Ret>(jsEngine, operation.get(),
                                                                          successCallback,
                                                                          errorCallback);
                    }
                    QObject::disconnect(*finishConnection);
                });
}

template <typename Ret>
void makeCallConnections(QJSEngine *jsEngine, const std::shared_ptr<QGrpcCallReply> &reply,
                         const QJSValue &finishCallback, const QJSValue &errorCallback)
{
    QtGrpcQuickFunctional::connectSingleReceiveOperationFinished<Ret>(jsEngine, reply,
                                                                      finishCallback,
                                                                      errorCallback);
}

template <typename Ret>
void makeServerStreamConnections(QJSEngine *jsEngine,
                                 const std::shared_ptr<QGrpcServerStream> &stream,
                                 const QJSValue &messageCallback, const QJSValue &finishCallback,
                                 const QJSValue &errorCallback)
{
    QtGrpcQuickFunctional::connectMultipleReceiveOperationFinished(jsEngine, stream,
                                            finishCallback, errorCallback);
    QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, jsEngine,
                     [streamPtr = stream.get(), messageCallback, jsEngine, errorCallback]() {
                         QtGrpcQuickFunctional::handleReceivedMessage<Ret>(jsEngine, streamPtr,
                                                                           messageCallback,
                                                                           errorCallback);
                     });
}

template <typename Ret>
void makeClientStreamConnections(QJSEngine *jsEngine,
                                    const std::shared_ptr<QGrpcClientStream> &stream,
                                    const QJSValue &finishCallback, const QJSValue &errorCallback)
{
    QtGrpcQuickFunctional::connectSingleReceiveOperationFinished<Ret>(jsEngine, stream,
                                                                      finishCallback,
                                                                      errorCallback);
}

template <typename Ret>
void makeBidiStreamConnections(QJSEngine *jsEngine, const std::shared_ptr<QGrpcBidiStream> &stream,
                               const QJSValue &messageCallback, const QJSValue &finishCallback,
                               const QJSValue &errorCallback)
{
    QtGrpcQuickFunctional::connectMultipleReceiveOperationFinished(jsEngine, stream, finishCallback,
                                                                   errorCallback);

    QObject::connect(stream.get(), &QGrpcBidiStream::messageReceived, jsEngine,
                     [streamPtr = stream.get(), messageCallback, jsEngine, errorCallback] {
                         QtGrpcQuickFunctional::handleReceivedMessage<Ret>(jsEngine, streamPtr,
                                                                           messageCallback,
                                                                           errorCallback);
                     });
}

} // namespace QtGrpcQuickFunctional

QT_END_NAMESPACE

#endif // QQMLGRPCFUNCTIONALHANDLERS_H
