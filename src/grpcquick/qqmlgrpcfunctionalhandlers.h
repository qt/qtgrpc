// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QQMLGRPCFUNCTIONALHANDLERS_H
#define QQMLGRPCFUNCTIONALHANDLERS_H

#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstream.h>

#include <QtQml/qqmlengine.h>

#include <QtCore/qobject.h>
#include <QtCore/qtconfigmacros.h>

QT_BEGIN_NAMESPACE

namespace QtGrpcQuickFunctional {

template <typename Ret>
void readReturnValue(QJSEngine *jsEngine, QGrpcOperation *operation,
                     const QJSValue &successCallback, const QJSValue &errorCallback)
{
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
    auto finishConnection = std::make_shared<QMetaObject::Connection>();
    *finishConnection = QObject::connect(reply.get(), &QGrpcCallReply::finished, jsEngine,
                                         [jsEngine, finishCallback, errorCallback, finishConnection,
                                          reply](const QGrpcStatus &status) {
                                             // We take 'reply' by copy so that its lifetime
                                             // is extended until this lambda is destroyed.
                                             if (status.code() == QtGrpc::StatusCode::Ok) {
                                                 readReturnValue<Ret>(jsEngine, reply.get(),
                                                                      finishCallback,
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
void makeServerStreamConnections(QJSEngine *jsEngine,
                                 const std::shared_ptr<QGrpcServerStream> &stream,
                                 const QJSValue &messageCallback, const QJSValue &finishCallback,
                                 const QJSValue &errorCallback)
{
    auto finishConnection = std::make_shared<QMetaObject::Connection>();
    *finishConnection = QObject::connect(stream.get(), &QGrpcServerStream::finished, jsEngine,
                                         [finishCallback, errorCallback, jsEngine, finishConnection,
                                          stream](const QGrpcStatus &status) {
                                             // We take 'stream' by copy so that its lifetime
                                             // is extended until this lambda is destroyed.
                                             if (status.code() == QtGrpc::StatusCode::Ok) {
                                                 if (finishCallback.isCallable())
                                                     finishCallback.call();
                                             } else {
                                                 if (errorCallback.isCallable())
                                                     errorCallback.call(QJSValueList{
                                                         jsEngine->toScriptValue(status) });
                                             }
                                             QObject::disconnect(*finishConnection);
                                         });
    QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, jsEngine,
                     [streamPtr = stream.get(), messageCallback, jsEngine, errorCallback]() {
                         readReturnValue<Ret>(jsEngine, streamPtr, messageCallback, errorCallback);
                     });
}

template <typename Sender, typename Ret>
Sender *makeClientStreamConnections(QJSEngine *jsEngine,
                                    const std::shared_ptr<QGrpcClientStream> &stream,
                                    const QJSValue &finishCallback, const QJSValue &errorCallback)
{
    Sender *sender = new Sender(stream);
    QQmlEngine::setObjectOwnership(sender, QQmlEngine::JavaScriptOwnership);
    auto finishConnection = std::make_shared<QMetaObject::Connection>();
    *finishConnection = QObject::connect(stream.get(), &QGrpcClientStream::finished, jsEngine,
                                         [stream, finishCallback, jsEngine, finishConnection,
                                          errorCallback](const QGrpcStatus &status) {
                                             // We take 'stream' by copy so that its lifetime
                                             // is extended until this lambda is destroyed.
                                             if (status.code() == QtGrpc::StatusCode::Ok) {
                                                 readReturnValue<Ret>(jsEngine, stream.get(),
                                                                      finishCallback,
                                                                      errorCallback);
                                             } else {
                                                 if (errorCallback.isCallable())
                                                     errorCallback.call(QJSValueList{
                                                         jsEngine->toScriptValue(status) });
                                             }
                                             QObject::disconnect(*finishConnection);
                                         });
    return sender;
}

template <typename Sender, typename Ret>
Sender *makeBidirStreamConnections(QJSEngine *jsEngine,
                                   const std::shared_ptr<QGrpcBidirStream> &stream,
                                   const QJSValue &messageCallback, const QJSValue &finishCallback,
                                   const QJSValue &errorCallback)
{
    Sender *sender = new Sender(stream);
    QQmlEngine::setObjectOwnership(sender, QQmlEngine::JavaScriptOwnership);
    auto finishConnection = std::make_shared<QMetaObject::Connection>();
    *finishConnection = QObject::connect(stream.get(), &QGrpcBidirStream::finished, jsEngine,
                                         [finishCallback, errorCallback, jsEngine, finishConnection,
                                          stream](const QGrpcStatus &status) {
                                             // We take 'stream' by copy so that its lifetime
                                             // is extended until this lambda is destroyed.
                                             if (status.code() == QtGrpc::StatusCode::Ok) {
                                                 if (finishCallback.isCallable())
                                                     finishCallback.call();
                                             } else {
                                                 if (errorCallback.isCallable())
                                                     errorCallback.call(QJSValueList{
                                                         jsEngine->toScriptValue(status) });
                                             }
                                             QObject::disconnect(*finishConnection);
                                         });
    QObject::connect(stream.get(), &QGrpcBidirStream::messageReceived, jsEngine,
                     [streamPtr = stream.get(), messageCallback, jsEngine, errorCallback] {
                         readReturnValue<Ret>(jsEngine, streamPtr, messageCallback, errorCallback);
                     });
    return sender;
}

} // namespace QtGrpcQuickFunctional

QT_END_NAMESPACE

#endif // QQMLGRPCFUNCTIONALHANDLERS_H
