// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "grpctemplates.h"

using namespace ::QtGrpc;
const char *GrpcTemplates::ChildClassDeclarationTemplate()
{
    return "\nclass $export_macro$ $classname$ : public $parent_class$\n"
           "{\n"
           "    Q_OBJECT\n";
}

const char *GrpcTemplates::ClientQmlDeclarationTemplate()
{
    return "    QML_NAMED_ELEMENT($service_name$Client)\n";
}

const char *GrpcTemplates::ClientMethodDeclarationAsyncTemplate()
{
    return "std::shared_ptr<QGrpcCallReply> $method_name$(const $param_type$ &$param_name$, const "
           "QGrpcCallOptions &options = {});\n";
}

const char *GrpcTemplates::ClientMethodDeclarationAsync2Template()
{
    return "Q_INVOKABLE void $method_name$(const $param_type$ &$param_name$, const QObject "
           "*context, "
           "const std::function<void(std::shared_ptr<QGrpcCallReply>)> &callback, const "
           "QGrpcCallOptions &options = {});\n";
}

const char *GrpcTemplates::ClientMethodDeclarationQmlTemplate()
{
    return "Q_INVOKABLE void $method_name$(const $param_type$ &$param_name$, "
           "const QJSValue &callback, "
           "const QJSValue &errorCallback, "
           "const QGrpcCallOptions &options = {});\n";
}

const char *GrpcTemplates::ServerMethodDeclarationTemplate()
{
    return "Q_INVOKABLE virtual $return_type$ $method_name$(const $param_type$ &$param_name$) = "
           "0;\n";
}

const char *GrpcTemplates::ClientConstructorDefinitionTemplate()
{
    return "\n$classname$::$classname$(QObject *parent)\n"
           "    : $parent_class$(\"$service_name$\"_L1, "
           "parent)\n"
           "{\n"
           "}\n\n";
}

const char *GrpcTemplates::ClientQmlConstructorDefinitionTemplate()
{
    return "\n$classname$::$classname$(QObject *parent)\n"
           "    : $parent_class$(parent)\n"
           "{\n"
           "}\n\n";
}

const char *GrpcTemplates::ClientMethodDefinitionAsyncTemplate()
{
    return "\nstd::shared_ptr<QGrpcCallReply> $classname$::$method_name$(const $param_type$ "
           "&$param_name$, const QGrpcCallOptions &options)\n"
           "{\n"
           "    return call(\"$method_name$\"_L1, $param_name$, options);\n"
           "}\n";
}

const char *GrpcTemplates::ClientMethodDefinitionAsync2Template()
{
    return "\nvoid $classname$::$method_name$(const $param_type$ &$param_name$, const QObject "
           "*context, const std::function<void(std::shared_ptr<QGrpcCallReply>)> &callback, const "
           "QGrpcCallOptions &options)\n"
           "{\n"
           "    std::shared_ptr<QGrpcCallReply> reply = call(\"$method_name$\"_L1, "
           "$param_name$, options);\n"
           "    QObject::connect(reply.get(), &QGrpcCallReply::finished, context, [reply, "
           "callback]() "
           "{\n"
           "        callback(reply);\n"
           "    }, Qt::SingleShotConnection);\n"
           "}\n\n";
}

const char *GrpcTemplates::ClientMethodDefinitionQmlTemplate()
{
    return "\nvoid $classname$::$method_name$(const $param_type$ &$param_name$,\n"
           "                const QJSValue &finishCallback,\n"
           "                const QJSValue &errorCallback,\n"
           "                const QGrpcCallOptions &options)\n"
           "{\n"
           "    QJSEngine *jsEngine = qjsEngine(this);\n"
           "    if (jsEngine == nullptr) {\n"
           "        qWarning() << \"Unable to call $classname$::$method_name$, it's only callable "
           "from JS engine context\";\n"
           "        return;\n"
           "    }\n\n"
           "    QtGrpcQuickFunctional::makeCallConnections<$return_type$>(jsEngine,\n"
           "                        call(\"$method_name$\"_L1, $param_name$, options),"
           "                        finishCallback, errorCallback);\n"
           "}\n\n";
}

const char *GrpcTemplates::ClientMethodStreamDeclarationTemplate()
{
    return "std::shared_ptr<$stream_type$> $method_name$(const $param_type$ "
           "&$param_name$, const QGrpcCallOptions &options = {});\n";
}

const char *GrpcTemplates::ClientMethodStreamDefinitionTemplate()
{
    return "std::shared_ptr<$stream_type$> $classname$::$method_name$("
           "const $param_type$ &$param_name$, const QGrpcCallOptions &options)\n"
           "{\n"
           "    return startStream<$stream_type$>(\"$method_name$\"_L1, "
           "$param_name$, options);\n"
           "}\n\n";
}

const char *GrpcTemplates::StreamSenderDeclarationQmlTemplate()
{
    // We use the Q_MOC_RUN trick here to fool qmltyperegistrar. The template base class doesn't
    // have any meta-type extras, and it cannot have them. So from the meta-type information
    // perspective it behaves in exactly the same way as QObject.
    return "class $export_macro$ $sender_class_name$ : public\n"
           "#ifdef Q_MOC_RUN\n"
           "    QObject\n"
           "#else\n"
           "    QQmlGrpcStreamSender<$stream_type$, $param_type$>\n"
           "#endif\n"
           "{\n"
           "    Q_OBJECT\n"
           "    QML_NAMED_ELEMENT($sender_qml_name$)\n"
           "    QML_UNCREATABLE(\"$sender_qml_name$ can only be created by gRPC "
           "client instance\")\n"
           "public:\n"
           "    $sender_class_name$(std::shared_ptr<$stream_type$> stream) : "
           "QQmlGrpcStreamSender(std::move(stream)) {}\n"
           "    Q_INVOKABLE void sendMessage(const $param_type$ &$param_name$)\n"
           "    {\n"
           "        sendMessageImpl($param_name$);\n"
           "    }\n"
           "};\n\n";
}

const char *GrpcTemplates::ClientMethodServerStreamDeclarationQmlTemplate()
{
    return "Q_INVOKABLE void $method_name$(const $param_type$ &$param_name$,\n"
           "                const QJSValue &messageCallback,\n"
           "                const QJSValue &finishCallback,\n"
           "                const QJSValue &errorCallback,\n"
           "                const QGrpcCallOptions &options = {});\n";
}

const char *GrpcTemplates::ClientMethodClientStreamDeclarationQmlTemplate()
{
    return "Q_INVOKABLE $sender_class_name$ *$method_name$("
           "const $param_type$ &$param_name$,\n"
           "                const QJSValue &finishCallback,\n"
           "                const QJSValue &errorCallback,\n"
           "                const QGrpcCallOptions &options = {});\n";
}

const char *GrpcTemplates::ClientMethodBidirStreamDeclarationQmlTemplate()
{
    return "Q_INVOKABLE $sender_class_name$ *$method_name$("
           "const $param_type$ &$param_name$,\n"
           "                const QJSValue &messageCallback,\n"
           "                const QJSValue &finishCallback,\n"
           "                const QJSValue &errorCallback,\n"
           "                const QGrpcCallOptions &options = {});\n";
}

const char *GrpcTemplates::ClientMethodServerStreamDefinitionQmlTemplate()
{
    return "\nvoid $classname$::$method_name$(const $param_type$ &$param_name$,\n"
           "            const QJSValue &messageCallback,\n"
           "            const QJSValue &finishCallback,\n"
           "            const QJSValue &errorCallback,\n"
           "            const QGrpcCallOptions &options)\n"
           "{\n"
           "    QJSEngine *jsEngine = qjsEngine(this);\n"
           "    if (jsEngine == nullptr) {\n"
           "        qWarning() << \"Unable to call $classname$::$method_name$, it's only callable "
           "from JS engine context\";\n"
           "        return;\n"
           "    }\n\n"
           "    QtGrpcQuickFunctional::makeServerStreamConnections<$return_type$>(jsEngine,\n"
           "                        startStream<QGrpcServerStream>(\"$method_name$\"_L1,"
           " $param_name$, options),\n"
           "                        messageCallback, finishCallback, errorCallback);\n"
           "}\n\n";
}

const char *GrpcTemplates::ClientMethodClientStreamDefinitionQmlTemplate()
{
    return "\n$sender_class_name$ *$classname$::$method_name$("
           "const $param_type$ &$param_name$,\n"
           "        const QJSValue &finishCallback,\n"
           "        const QJSValue &errorCallback,\n"
           "        const QGrpcCallOptions &options)\n"
           "{\n"
           "    QJSEngine *jsEngine = qjsEngine(this);\n"
           "    if (jsEngine == nullptr) {\n"
           "        qWarning() << \"Unable to call $classname$::$method_name$, it's only callable "
           "from JS engine context\";\n"
           "        return nullptr;\n"
           "    }\n\n"
           "    return QtGrpcQuickFunctional::makeClientStreamConnections<$sender_class_name$, "
           "$return_type$>(jsEngine,\n"
           "                        startStream<QGrpcClientStream>(\"$method_name$\"_L1,"
           " $param_name$, options),\n"
           "                        finishCallback, errorCallback);\n"

           "}\n\n";
}

const char *GrpcTemplates::ClientMethodBidirStreamDefinitionQmlTemplate()
{
    return "\n$sender_class_name$ *$classname$::$method_name$("
           "const $param_type$ &$param_name$,\n"
           "    const QJSValue &messageCallback,\n"
           "    const QJSValue &finishCallback,\n"
           "    const QJSValue &errorCallback,\n"
           "    const QGrpcCallOptions &options)\n"
           "{\n"
           "    QJSEngine *jsEngine = qjsEngine(this);\n"
           "    if (jsEngine == nullptr) {\n"
           "        qWarning() << \"Unable to call $classname$::$method_name$, it's only callable "
           "from JS engine context\";\n"
           "        return nullptr;\n"
           "    }\n\n"
           "    return QtGrpcQuickFunctional::makeBidirStreamConnections<$sender_class_name$, "
           "$return_type$>(jsEngine,\n"
           "                        startStream<QGrpcBidirStream>(\"$method_name$\"_L1,"
           " $param_name$, options),\n"
           "                        messageCallback, finishCallback, errorCallback);\n"
           "}\n\n";
}

const char *GrpcTemplates::GrpcClientFileSuffix()
{
    return "_client.grpc";
}

const char *GrpcTemplates::GrpcServiceFileSuffix()
{
    return "_service.grpc";
}
