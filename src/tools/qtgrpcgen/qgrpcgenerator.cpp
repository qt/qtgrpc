// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "qgrpcgenerator.h"
#include "clientdeclarationprinter.h"
#include "clientdefinitionprinter.h"
#include "serverdeclarationprinter.h"

#include "grpctemplates.h"
#include "utils.h"
#include "options.h"

#include <set>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/stubs/logging.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.h>

using namespace ::QtGrpc;
using namespace ::qtprotoccommon;
using namespace ::google::protobuf;
using namespace ::google::protobuf::io;
using namespace ::google::protobuf::compiler;

QGrpcGenerator::QGrpcGenerator() : GeneratorBase()
{}

QGrpcGenerator::~QGrpcGenerator() = default;

bool QGrpcGenerator::Generate(const FileDescriptor *file,
                              [[maybe_unused]] const std::string &parameter,
                              GeneratorContext *generatorContext,
                              std::string *error) const
{
    assert(file != nullptr && generatorContext != nullptr);
    if (file->syntax() != FileDescriptor::SYNTAX_PROTO3) {
        *error = "Invalid proto used. qtgrpcgen only supports 'proto3' syntax";
        return false;
    }

    return GenerateServices(file, generatorContext);
}

void QGrpcGenerator::SetInternalIncludes(const FileDescriptor *file,
                                         std::set<std::string>& internalIncludes) const
{
    assert(file != nullptr);
    for (int i = 0; i < file->service_count(); ++i) {
        const ServiceDescriptor *service = file->service(i);
        for (int i = 0; i < service->method_count(); ++i) {
            const MethodDescriptor *method = service->method(i);
            if (method->input_type()->file() != service->file()) {
                internalIncludes.insert(
                            utils::removeFileSuffix(method->input_type()->file()->name())
                            + CommonTemplates::ProtoFileSuffix());
            }

            if (method->output_type()->file() != service->file()) {
                internalIncludes.insert(
                            utils::removeFileSuffix(method->output_type()->file()->name())
                            + CommonTemplates::ProtoFileSuffix());
            }
        }
    }
    if (file->message_type_count() > 0) {
        internalIncludes.insert(generateBaseName(file, utils::extractFileBasename(file->name()))
                                + CommonTemplates::ProtoFileSuffix());
    }
}

void QGrpcGenerator::RunPrinters(const FileDescriptor *file,
                                 std::shared_ptr<Printer> clientHeaderPrinter,
                                 std::shared_ptr<Printer> clientSourcePrinter,
                                 std::shared_ptr<Printer> serviceHeaderPrinter) const
{
    assert(file != nullptr);
    for (int i = 0; i < file->service_count(); ++i) {
        const ServiceDescriptor *service = file->service(i);

        ClientDeclarationPrinter clientDecl(service, clientHeaderPrinter);
        clientDecl.run();

        ClientDefinitionPrinter clientDef(service, clientSourcePrinter);
        clientDef.run();

        ServerDeclarationPrinter serverDecl(service, serviceHeaderPrinter);
        serverDecl.run();
    }
}

bool QGrpcGenerator::GenerateServices(const FileDescriptor *file,
                                      GeneratorContext *generatorContext) const
{
    assert(file != nullptr && generatorContext != nullptr);
    if (file->service_count() <= 0)
        return true;

    std::set<std::string> internalIncludes;

    const std::string filename = utils::extractFileBasename(file->name());
    const std::string basename = generateBaseName(file, filename);
    const std::string clientFileName = basename
            + GrpcTemplates::GrpcClientFileSuffix() + CommonTemplates::ProtoFileSuffix();
    std::unique_ptr<ZeroCopyOutputStream> clientHeaderStream(
                generatorContext->Open(clientFileName + ".h"));
    std::unique_ptr<ZeroCopyOutputStream> clientSourceStream(
                generatorContext->Open(clientFileName + ".cpp"));
    std::unique_ptr<ZeroCopyOutputStream> serviceHeaderStream(
                generatorContext->Open(basename
                                       + GrpcTemplates::GrpcServiceFileSuffix()
                                       + CommonTemplates::ProtoFileSuffix() + ".h"));
    std::shared_ptr<Printer> clientHeaderPrinter(new Printer(clientHeaderStream.get(), '$'));
    std::shared_ptr<Printer> clientSourcePrinter(new Printer(clientSourceStream.get(), '$'));
    std::shared_ptr<Printer> serviceHeaderPrinter(new Printer(serviceHeaderStream.get(), '$'));

    printDisclaimer(clientHeaderPrinter.get());
    clientHeaderPrinter->Print({ { "filename", filename + "_client" } },
                               CommonTemplates::PreambleTemplate());

    printDisclaimer(serviceHeaderPrinter.get());
    serviceHeaderPrinter->Print({ { "filename", filename + "_service" } },
                                CommonTemplates::PreambleTemplate());

    clientHeaderPrinter->Print(CommonTemplates::DefaultProtobufIncludesTemplate());
    if (Options::instance().hasQml())
        clientHeaderPrinter->Print(CommonTemplates::QmlProtobufIncludesTemplate());

    serviceHeaderPrinter->Print(CommonTemplates::DefaultProtobufIncludesTemplate());
    if (Options::instance().hasQml())
        serviceHeaderPrinter->Print(CommonTemplates::QmlProtobufIncludesTemplate());

    printDisclaimer(clientSourcePrinter.get());
    clientSourcePrinter->Print({ { "include", clientFileName } },
                               CommonTemplates::InternalIncludeTemplate());

    SetInternalIncludes(file, internalIncludes);

    std::set<std::string> externalIncludes
            = {"QAbstractGrpcClient", "QGrpcCallReply", "QGrpcStream"};

    for (const auto &include : externalIncludes) {
        clientHeaderPrinter->Print({ { "include", include } },
                                   CommonTemplates::ExternalIncludeTemplate());
        serviceHeaderPrinter->Print({ { "include", include } },
                                    CommonTemplates::ExternalIncludeTemplate());
    }

    const std::string serviceIncludes = std::string("QAbstractGrpcService");
    serviceHeaderPrinter->Print({ { "include", serviceIncludes } },
                                CommonTemplates::ExternalIncludeTemplate());

    for (const auto &include : internalIncludes) {
        clientHeaderPrinter->Print({ { "include", include } },
                                   CommonTemplates::InternalIncludeTemplate());
        serviceHeaderPrinter->Print({ { "include", include } },
                                    CommonTemplates::InternalIncludeTemplate());
    }

    if (Options::instance().hasQml()) {
        clientSourcePrinter->Print({ { "include", "QQmlEngine" } },
                                   CommonTemplates::ExternalIncludeTemplate());
        clientSourcePrinter->Print({ { "include", "QJSEngine" } },
                                   CommonTemplates::ExternalIncludeTemplate());
        clientSourcePrinter->Print({ { "include", "QJSValue" } },
                                   CommonTemplates::ExternalIncludeTemplate());
    }

    clientHeaderPrinter->PrintRaw("\n");
    serviceHeaderPrinter->PrintRaw("\n");
    if (!Options::instance().exportMacro().empty()) {
        clientHeaderPrinter->Print({ { "export_macro", Options::instance().exportMacro() } },
                                   CommonTemplates::ExportMacroTemplate());
        serviceHeaderPrinter->Print({ { "export_macro", Options::instance().exportMacro() } },
                                    CommonTemplates::ExportMacroTemplate());
    }

    OpenFileNamespaces(file, clientHeaderPrinter.get());
    OpenFileNamespaces(file, clientSourcePrinter.get());
    OpenFileNamespaces(file, serviceHeaderPrinter.get());

    RunPrinters(file, clientHeaderPrinter, clientSourcePrinter, serviceHeaderPrinter);

    CloseFileNamespaces(file, clientHeaderPrinter.get());
    CloseFileNamespaces(file, clientSourcePrinter.get());
    CloseFileNamespaces(file, serviceHeaderPrinter.get());

    clientHeaderPrinter->Print({ { "filename", filename + "_client" } },
                               CommonTemplates::FooterTemplate());
    serviceHeaderPrinter->Print({ { "filename", filename + "_service" } },
                                CommonTemplates::FooterTemplate());
    return true;
}
