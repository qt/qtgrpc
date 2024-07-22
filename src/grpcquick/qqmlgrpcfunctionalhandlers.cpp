// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpcQuick/qqmlgrpcfunctionalhandlers.h>

QT_BEGIN_NAMESPACE

namespace QtGrpcQuickFunctional {

void connectMultipleReceiveOperationFinished(QJSEngine *jsEngine,
                                             const std::shared_ptr<QGrpcOperation> &operation,
                                             const QJSValue &successCallback,
                                             const QJSValue &errorCallback)
{
    QtGrpcQuickFunctional::validateEngineAndOperation(jsEngine, operation.get());

    auto finishConnection = std::make_shared<QMetaObject::Connection>();
    *finishConnection = QObject::connect(operation.get(), &QGrpcOperation::finished, jsEngine,
                                         [successCallback, errorCallback, jsEngine,
                                          finishConnection,
                                          operation](const QGrpcStatus &status) {
                                             // We take 'operation' by copy so that its lifetime
                                             // is extended until this lambda is destroyed.
                                             if (status.code() == QtGrpc::StatusCode::Ok) {
                                                 if (successCallback.isCallable())
                                                     successCallback.call();
                                             } else if (errorCallback.isCallable()) {
                                                 errorCallback.call(QJSValueList{
                                                     jsEngine->toScriptValue(status) });
                                             }
                                             QObject::disconnect(*finishConnection);
                                         });
}

} // namespace QtGrpcQuickFunctional

QT_END_NAMESPACE
