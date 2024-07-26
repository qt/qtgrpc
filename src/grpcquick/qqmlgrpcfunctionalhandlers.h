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
void connectMultipleReceiveOperationFinished(QJSEngine *jsEngine,
                                            const std::shared_ptr<QGrpcOperation> &operation,
                                            const QJSValue &successCallback,
                                            const QJSValue &errorCallback);

template <typename Ret>
void handleReceivedMessage(QJSEngine *jsEngine, QGrpcOperation *operation,
                           const QJSValue &successCallback, const QJSValue &errorCallback)
{
    QtGrpcQuickFunctional::validateEngineAndOperation(jsEngine, operation);

    if (successCallback.isCallable()) {
        std::optional<Ret> result = operation->read<Ret>();
        if (result) {
            successCallback.call(QJSValueList{ jsEngine->toScriptValue(*result) });
        } else if (errorCallback.isCallable()) {
            using StatusCode = QtGrpc::StatusCode;
            auto status = QGrpcStatus{
                operation->deserializationError()
                        == QAbstractProtobufSerializer::UnexpectedEndOfStreamError
                    ? StatusCode::OutOfRange
                    : StatusCode::InvalidArgument,
                operation->deserializationErrorString()
            };
            errorCallback.call(QJSValueList{ jsEngine->toScriptValue(status) });
        }
    }
}

template <typename Ret>
void makeCallConnections(QJSEngine *jsEngine, const std::shared_ptr<QGrpcCallReply> &reply,
                         const QJSValue &finishCallback, const QJSValue &errorCallback)
{
    QtGrpcQuickFunctional::validateEngineAndOperation(jsEngine, reply.get());

    auto finishConnection = std::make_shared<QMetaObject::Connection>();
    *finishConnection = QObject::connect(reply.get(), &QGrpcCallReply::finished, jsEngine,
                                         [jsEngine, finishCallback, errorCallback, finishConnection,
                                          reply](const QGrpcStatus &status) {
                                             // We take 'reply' by copy so that its lifetime
                                             // is extended until this lambda is destroyed.
                                             if (status.code() == QtGrpc::StatusCode::Ok) {
                                                 QtGrpcQuickFunctional::handleReceivedMessage<
                                                     Ret>(jsEngine, reply.get(),
                                                          finishCallback, errorCallback);
                                             } else {
                                                 if (errorCallback.isCallable())
                                                     errorCallback.call(QJSValueList{
                                                         jsEngine->toScriptValue(status) });
                                             }
                                             QObject::disconnect(*finishConnection);
                                         });
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
    QtGrpcQuickFunctional::validateEngineAndOperation(jsEngine, stream.get());
    auto finishConnection = std::make_shared<QMetaObject::Connection>();
    *finishConnection = QObject::connect(stream.get(), &QGrpcClientStream::finished, jsEngine,
                                         [stream, finishCallback, jsEngine, finishConnection,
                                          errorCallback](const QGrpcStatus &status) {
                                             // We take 'stream' by copy so that its lifetime
                                             // is extended until this lambda is destroyed.
                                             if (status.code() == QtGrpc::StatusCode::Ok) {
                                                 QtGrpcQuickFunctional::handleReceivedMessage<
                                                     Ret>(jsEngine, stream.get(), finishCallback,
                                                          errorCallback);
                                             } else {
                                                 if (errorCallback.isCallable())
                                                     errorCallback.call(QJSValueList{
                                                         jsEngine->toScriptValue(status) });
                                             }
                                             QObject::disconnect(*finishConnection);
                                         });
}

template <typename Ret>
void makeBidirStreamConnections(QJSEngine *jsEngine,
                                   const std::shared_ptr<QGrpcBidirStream> &stream,
                                   const QJSValue &messageCallback, const QJSValue &finishCallback,
                                   const QJSValue &errorCallback)
{
    QtGrpcQuickFunctional::connectMultipleReceiveOperationFinished(jsEngine, stream,
                                            finishCallback, errorCallback);

    QObject::connect(stream.get(), &QGrpcBidirStream::messageReceived, jsEngine,
                     [streamPtr = stream.get(), messageCallback, jsEngine, errorCallback] {
                         QtGrpcQuickFunctional::handleReceivedMessage<Ret>(jsEngine, streamPtr,
                                                                           messageCallback,
                                                                           errorCallback);
                     });
}

} // namespace QtGrpcQuickFunctional

QT_END_NAMESPACE

#endif // QQMLGRPCFUNCTIONALHANDLERS_H
