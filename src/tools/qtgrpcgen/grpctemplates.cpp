// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#include "grpctemplates.h"

using namespace ::QtGrpc;
const char *GrpcTemplates::ChildClassDeclarationTemplate()
{
    return "\nclass $export_macro$ $classname$ : public $parent_class$\n"
           "{\n"
           "    Q_OBJECT\n";
}

const char *GrpcTemplates::ClientMethodDeclarationSyncTemplate()
{
    return "QGrpcStatus $method_name$(const $param_type$ &$param_name$, "
           "$return_type$ *$return_name$);\n";
}

const char *GrpcTemplates::ClientMethodDeclarationAsyncTemplate()
{
    return "std::shared_ptr<QGrpcCallReply> $method_name$(const $param_type$ &$param_name$);\n";
}

const char *GrpcTemplates::ClientMethodDeclarationAsync2Template()
{
    return "Q_INVOKABLE void $method_name$(const $param_type$ &$param_name$, const QObject "
           "*context, "
           "const std::function<void(std::shared_ptr<QGrpcCallReply>)> &callback);\n";
}

const char *GrpcTemplates::ClientMethodDeclarationQmlTemplate()
{
    return "Q_INVOKABLE void $method_name$($param_type$ *$param_name$, const QJSValue &callback, "
           "const QJSValue &errorCallback);\n";
}

const char *GrpcTemplates::ClientMethodDeclarationQml2Template()
{
    return "Q_INVOKABLE void $method_name$($param_type$ *$param_name$, $return_type$ "
           "*$return_name$, const QJSValue &errorCallback);\n";
}

const char *GrpcTemplates::ServerMethodDeclarationTemplate()
{
    return "Q_INVOKABLE virtual $return_type$ $method_name$(const $param_type$ &$param_name$) = "
           "0;\n";
}

const char *GrpcTemplates::ClientConstructorDefinitionTemplate()
{
    return "\n$classname$::$classname$(QObject *parent) : $parent_class$(\"$service_name$\"_L1, "
           "parent)\n"
           "{\n"
           "}\n";
}

const char *GrpcTemplates::ClientMethodDefinitionSyncTemplate()
{
    return "\nQGrpcStatus $classname$::$method_name$(const $param_type$ &$param_name$, "
           "$return_type$ *$return_name$)\n"
           "{\n"
           "    return call<$param_type$>(\"$method_name$\"_L1, $param_name$, $return_name$);\n"
           "}\n";
}

const char *GrpcTemplates::ClientMethodDefinitionAsyncTemplate()
{
    return "\nstd::shared_ptr<QGrpcCallReply> $classname$::$method_name$(const $param_type$ "
           "&$param_name$)\n"
           "{\n"
           "    return call<$param_type$>(\"$method_name$\"_L1, $param_name$);\n"
           "}\n";
}

const char *GrpcTemplates::ClientMethodDefinitionAsync2Template()
{
    return "\nvoid $classname$::$method_name$(const $param_type$ &$param_name$, const QObject "
           "*context, const std::function<void(std::shared_ptr<QGrpcCallReply>)> &callback)\n"
           "{\n"
           "    std::shared_ptr<QGrpcCallReply> reply = call<$param_type$>(\"$method_name$\"_L1, "
           "$param_name$);\n"
           "    QObject::connect(reply.get(), &QGrpcCallReply::finished, context, [reply, "
           "callback]() "
           "{\n"
           "        callback(reply);\n"
           "    });\n"
           "}\n";
}

const char *GrpcTemplates::ClientMethodDefinitionQmlTemplate()
{
    return "\nvoid $classname$::$method_name$($param_type$ *$param_name$, const QJSValue "
           "&callback, "
           "const QJSValue &errorCallback)\n"
           "{\n"
           "    if (!callback.isCallable()) {\n"
           "        qWarning() << \"Unable to call $classname$::$method_name$, callback is not "
           "callable\";\n"
           "        return;\n"
           "    }\n\n"
           "    if (arg == nullptr) {\n"
           "        qWarning() << \"Invalid argument provided for method "
           "$classname$::$method_name$, "
           "argument of type '$param_type$ *' expected\";\n"
           "        return;\n"
           "    }\n\n"
           "    QJSEngine *jsEngine = qjsEngine(this);\n"
           "    if (jsEngine == nullptr) {\n"
           "        qWarning() << \"Unable to call $classname$::$method_name$, it's only callable "
           "from JS engine context\";\n"
           "        return;\n"
           "    }\n\n"
           "    std::shared_ptr<QGrpcCallReply> reply = call<$param_type$>(\"$method_name$\"_L1, "
           "*$param_name$);\n"
           "    reply->subscribe(jsEngine, [this, reply, callback, jsEngine]() {\n"
           "        auto result = new $return_type$(reply->read<$return_type$>());\n"
           "        qmlEngine(this)->setObjectOwnership(result, QQmlEngine::JavaScriptOwnership);\n"
           "        QJSValue(callback).call(QJSValueList{jsEngine->toScriptValue(result)});\n"
           "    }, [errorCallback, jsEngine](const QGrpcStatus &status) {\n"
           "        QJSValue(errorCallback).call(QJSValueList{jsEngine->toScriptValue(status)});\n"
           "    });\n"
           "}\n";
}

const char *GrpcTemplates::ClientMethodDefinitionQml2Template()
{
    return "\nvoid $classname$::$method_name$($param_type$ *$param_name$, $return_type$ "
           "*$return_name$, const QJSValue &errorCallback)\n"
           "{\n"
           "    if ($return_name$ == nullptr) {\n"
           "        qWarning() << \"Invalid argument provided for method "
           "$classname$::$method_name$, "
           "argument of type '$return_type$ *' expected\";\n"
           "        return;\n"
           "    }\n\n"
           "    QWeakPointer<$return_type$> safeReturn($return_name$);\n\n"
           "    if ($param_name$ == nullptr) {\n"
           "        qWarning() << \"Invalid argument provided for method "
           "$classname$::$method_name$, "
           "argument of type '$param_type$ *' expected\";\n"
           "        return;\n"
           "    }\n\n"
           "    QJSEngine *jsEngine = qjsEngine(this);\n"
           "    if (jsEngine == nullptr) {\n"
           "        qWarning() << \"Unable to call $classname$::$method_name$, it's only callable "
           "from JS engine context\";\n"
           "        return;\n"
           "    }\n\n"
           "    std::shared_ptr<QGrpcCallReply> reply = call<$param_type$>(\"$method_name$\"_L1, "
           "*$param_name$);\n"
           "    reply->subscribe(jsEngine, [this, reply, jsEngine, safeReturn]() {\n"
           "        if (safeReturn.isNull()) {\n"
           "            qWarning() << \"Return value is destroyed. Ignore call result\";\n"
           "            return;\n"
           "        }\n"
           "        *safeReturn = $return_type$(reply->read<$return_type$>());\n"
           "    }, [errorCallback, jsEngine](const QGrpcStatus &status) {\n"
           "        QJSValue(errorCallback).call(QJSValueList{jsEngine->toScriptValue(status)});\n"
           "    });\n"
           "}\n";
}

const char *GrpcTemplates::ClientMethodServerStreamDeclarationTemplate()
{
    return "std::shared_ptr<QGrpcStream> stream$method_name_upper$(const $param_type$ "
           "&$param_name$);\n";
}

const char *GrpcTemplates::ClientMethodServerStream2DeclarationTemplate()
{
    return "std::shared_ptr<QGrpcStream> stream$method_name_upper$(const $param_type$ "
           "&$param_name$, const "
           "QWeakPointer<$return_type$> &$return_name$);\n";
}

const char *GrpcTemplates::ClientMethodServerStreamQmlDeclarationTemplate()
{
    return "Q_INVOKABLE std::shared_ptr<QGrpcStream> qmlStream$method_name_upper$_p($param_type$ "
           "*$param_name$, "
           "$return_type$ *$return_name$);\n";
}

const char *GrpcTemplates::ClientMethodServerStreamDefinitionTemplate()
{
    return "std::shared_ptr<QGrpcStream> $classname$::stream$method_name_upper$(const $param_type$ "
           "&$param_name$)\n"
           "{\n"
           "    return startStream<$param_type$>(\"$method_name$\"_L1, $param_name$);\n"
           "}\n";
}

const char *GrpcTemplates::ClientMethodServerStream2DefinitionTemplate()
{
    return "std::shared_ptr<QGrpcStream> $classname$::stream$method_name_upper$(const $param_type$ "
           "&$param_name$, const QWeakPointer<$return_type$> &$return_name$)\n"
           "{\n"
           "    return startStream<$param_type$>(\"$method_name$\"_L1, $param_name$, "
           "$return_name$);\n"
           "}\n";
}

const char *GrpcTemplates::GrpcClientFileSuffix()
{
    return "_client.grpc";
}

const char *GrpcTemplates::GrpcServiceFileSuffix()
{
    return "_service.grpc";
}

const char *GrpcTemplates::ClientMethodServerStreamQmlDefinitionTemplate()
{
    return "std::shared_ptr<QGrpcStream> $classname$::qmlStream$method_name_upper$_p($param_type$ "
           "*$param_name$, "
           "$return_type$ *$return_name$)\n"
           "{\n"
           "    return startStream<$param_type$>(\"$method_name$\"_L1, *$param_name$, "
           "QWeakPointer<$return_type$>($return_name$));\n"
           "}\n";
}
