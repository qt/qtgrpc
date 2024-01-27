// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QQMLGRPCFUNCTIONALHANDLERS_H
#define QQMLGRPCFUNCTIONALHANDLERS_H

#include <QtCore/qobject.h>
#include <QtCore/qtconfigmacros.h>

#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstream.h>

#include <QtQml/qqmlengine.h>

QT_BEGIN_NAMESPACE

namespace QtGrpcQuickFunctional {

template <typename Ret, typename Operation>
void readReturnValue(QJSEngine *jsEngine, Operation *operation, const QJSValue &successCallback,
                     const QJSValue &errorCallback)
{
    if (successCallback.isCallable()) {
        std::optional<Ret> result = operation->template read<Ret>();
        if (result) {
            successCallback.call(QJSValueList{ jsEngine->toScriptValue(*result) });
        } else if (errorCallback.isCallable()) {
            using StatusCode = QGrpcStatus::StatusCode;
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
    *finishConnection = QObject::connect(reply.get(), &QGrpcCallReply::finished, jsEngine, [=]() {
        readReturnValue<Ret, QGrpcCallReply>(jsEngine, reply.get(), finishCallback, errorCallback);
        QObject::disconnect(*finishConnection);
    });
    QObject::connect(reply.get(), &QGrpcCallReply::errorOccurred, jsEngine,
                     [errorCallback, jsEngine](const QGrpcStatus &status) {
                         if (errorCallback.isCallable())
                             errorCallback.call(QJSValueList{ jsEngine->toScriptValue(status) });
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
                                         [finishCallback, finishConnection] {
                                             if (finishCallback.isCallable())
                                                 finishCallback.call();
                                             QObject::disconnect(*finishConnection);
                                         });
    QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, jsEngine,
                     [streamPtr = stream.get(), messageCallback, jsEngine, errorCallback] {
                         readReturnValue<Ret, QGrpcServerStream>(jsEngine, streamPtr,
                                                                 messageCallback, errorCallback);
                     });
    QObject::connect(stream.get(), &QGrpcServerStream::errorOccurred, jsEngine,
                     [errorCallback, jsEngine](const QGrpcStatus &status) {
                         if (errorCallback.isCallable())
                             errorCallback.call(QJSValueList{ jsEngine->toScriptValue(status) });
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
                                         [streamPtr = stream.get(), finishCallback, jsEngine,
                                          finishConnection, errorCallback] {
                                             readReturnValue<Ret, QGrpcClientStream>(jsEngine,
                                                                                     streamPtr,
                                                                                     finishCallback,
                                                                                     errorCallback);
                                         });
    QObject::connect(stream.get(), &QGrpcClientStream::errorOccurred, jsEngine,
                     [errorCallback, jsEngine](const QGrpcStatus &status) {
                         if (errorCallback.isCallable())
                             errorCallback.call(QJSValueList{ jsEngine->toScriptValue(status) });
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
                                         [finishCallback, finishConnection] {
                                             if (finishCallback.isCallable())
                                                 finishCallback.call();
                                             QObject::disconnect(*finishConnection);
                                         });
    QObject::connect(stream.get(), &QGrpcBidirStream::messageReceived, jsEngine,
                     [streamPtr = stream.get(), messageCallback, jsEngine, errorCallback] {
                         readReturnValue<Ret, QGrpcBidirStream>(jsEngine, streamPtr,
                                                                messageCallback, errorCallback);
                     });
    QObject::connect(stream.get(), &QGrpcBidirStream::errorOccurred, jsEngine,
                     [errorCallback, jsEngine](const QGrpcStatus &status) {
                         if (errorCallback.isCallable())
                             errorCallback.call(QJSValueList{ jsEngine->toScriptValue(status) });
                     });
    return sender;
}

} // namespace QtGrpcQuickFunctional

QT_END_NAMESPACE

#endif // QQMLGRPCFUNCTIONALHANDLERS_H
