// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "singlefilegenerator.h"
#include "messagedeclarationprinter.h"
#include "messagedefinitionprinter.h"
#include "enumdeclarationprinter.h"
#include "enumdefinitionprinter.h"

#include "templates.h"
#include "utils.h"
#include "options.h"

#include <iostream>
#include <numeric>
#include <set>
#include <google/protobuf/stubs/logging.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.h>

using namespace ::QtProtobuf::generator;
using namespace ::google::protobuf;
using namespace ::google::protobuf::io;
using namespace ::google::protobuf::compiler;

SingleFileGenerator::SingleFileGenerator() : GeneratorBase()
{}
SingleFileGenerator::~SingleFileGenerator() = default;

bool SingleFileGenerator::Generate(const FileDescriptor *file,
                                   [[maybe_unused]] const std::string &parameter,
                                   GeneratorContext *generatorContext,
                                   std::string *error) const
{
    if (file->syntax() != FileDescriptor::SYNTAX_PROTO3) {
        *error = "Invalid proto used. qtprotobufgen only supports 'proto3' syntax";
        return false;
    }

    return GenerateMessages(file, generatorContext);
}

bool SingleFileGenerator::GenerateMessages(const FileDescriptor *file,
                                           GeneratorContext *generatorContext) const
{
    if (file->message_type_count() <= 0 && file->enum_type_count() <= 0) {
        return true;
    }

    std::set<std::string> internalIncludes;
    std::set<std::string> externalIncludes;

    auto filename = utils::extractFileBasename(file->name());
    std::string basename = generateBaseName(file, filename);
    std::unique_ptr<io::ZeroCopyOutputStream> headerStream(generatorContext->Open(basename + Templates::ProtoFileSuffix() + ".h"));
    std::unique_ptr<io::ZeroCopyOutputStream> sourceStream(generatorContext->Open(basename + Templates::ProtoFileSuffix() + ".cpp"));
    std::unique_ptr<io::ZeroCopyOutputStream> registrationStream(generatorContext->Open(basename + "_protobuftyperegistrations.cpp"));

    std::shared_ptr<Printer> headerPrinter(new Printer(headerStream.get(), '$'));
    std::shared_ptr<Printer> sourcePrinter(new Printer(sourceStream.get(), '$'));
    std::shared_ptr<Printer> registrationPrinter(new Printer(registrationStream.get(), '$'));

    printDisclaimer(headerPrinter.get());
    headerPrinter->Print({{"filename", filename}}, Templates::PreambleTemplate());

    headerPrinter->Print(Templates::DefaultProtobufIncludesTemplate());
    if (Options::instance().hasQml()) {
        headerPrinter->Print(Templates::QmlProtobufIncludesTemplate());
    }

    printDisclaimer(sourcePrinter.get());
    sourcePrinter->Print({{"include", basename + Templates::ProtoFileSuffix()}}, Templates::InternalIncludeTemplate());

    registrationPrinter->Print({{"include", "QProtobufSerializer"}},
        Templates::ExternalIncludeTemplate());
    registrationPrinter->Print({ { "include", "array" } }, Templates::ExternalIncludeTemplate());
    registrationPrinter->Print({{"include", basename + Templates::ProtoFileSuffix()}}, Templates::InternalIncludeTemplate());

    externalIncludes.insert("QByteArray");
    externalIncludes.insert("QString");


    bool hasQtTypes = false;
    common::iterateMessages(file, [&externalIncludes, &hasQtTypes](const Descriptor *message) {
        for (int i = 0; i < message->field_count(); ++i) {
            const auto *field = message->field(i);
            if (field->type() == FieldDescriptor::TYPE_MESSAGE && !field->is_map()
                && !field->is_repeated() && common::isQtType(field)) {
                externalIncludes.insert(field->message_type()->name());
                hasQtTypes = true;
            }
        }
        if (message->full_name() == "google.protobuf.Timestamp") {
            externalIncludes.insert("QDateTime");
        }
    });

    if (hasQtTypes) {
        externalIncludes.insert("QtProtobufQtTypes");
    }

    for (int i = 0; i < file->dependency_count(); ++i) {
        if (file->dependency(i)->name() == "QtProtobuf/QtCore.proto"
                || file->dependency(i)->name() == "QtProtobuf/QtGui.proto") {
            continue;
        }
        internalIncludes.insert(utils::removeFileSuffix(file->dependency(i)->name()) + Templates::ProtoFileSuffix());
    }

    for (auto include : externalIncludes) {
        headerPrinter->Print({{"include", include}}, Templates::ExternalIncludeTemplate());
    }

    for (auto include : internalIncludes) {
        headerPrinter->Print({{"include", include}}, Templates::InternalIncludeTemplate());
    }


    sourcePrinter->Print({{"include", "QProtobufSerializer"}},
        Templates::ExternalIncludeTemplate());
    if (Options::instance().hasQml()) {
        sourcePrinter->Print({{"include", "QQmlEngine"}}, Templates::ExternalIncludeTemplate());
    }
    headerPrinter->PrintRaw("\n");
    if (!Options::instance().exportMacro().empty()) {
        headerPrinter->Print({ { "export_macro", Options::instance().exportMacro() } },
                             Templates::ExportMacroTemplate());
    }

    const bool hasQtNamespace = (Options::instance().extraNamespace() == "QT_NAMESPACE");
    const std::string scopeNamespaces = file->message_type_count() > 0
            ? common::getFullNamespace(file->message_type(0), "::")
            : common::getFullNamespace(file->enum_type(0), "::");
    auto openNamespaces = [&](auto printer) { // open namespaces
        printer->Print("\n");
        if (hasQtNamespace)
            printer->PrintRaw("QT_BEGIN_NAMESPACE\n");
        if (!scopeNamespaces.empty())
            printer->Print({ { "scope_namespaces", scopeNamespaces } },
                           Templates::NamespaceTemplate());
    };
    openNamespaces(headerPrinter);
    openNamespaces(sourcePrinter);
    openNamespaces(registrationPrinter);

    for (int i = 0; i < file->enum_type_count(); ++i) {
        EnumDeclarationPrinter enumDecl(file->enum_type(i), headerPrinter);
        enumDecl.run();
        EnumDefinitionPrinter enumSourceDef(file->enum_type(i), sourcePrinter);
        enumSourceDef.run();
    }

    common::iterateMessages(file, [&headerPrinter](const Descriptor *message) {
        MessageDeclarationPrinter messageDecl(message, headerPrinter);
        messageDecl.printClassForwardDeclaration();
    });

    common::iterateMessages(
            file,
            [&headerPrinter, &sourcePrinter, &registrationPrinter](const Descriptor *message) {
                MessageDeclarationPrinter messageDecl(message, headerPrinter);
                messageDecl.printClassDeclaration();

                MessageDefinitionPrinter messageDef(message, sourcePrinter);
                messageDef.printClassDefinition();
                messageDef.printClassRegistration(registrationPrinter.get());
            });

    auto closeNamespaces = [&](auto printer) {
        if (!scopeNamespaces.empty())
            printer->Print({ { "scope_namespaces", scopeNamespaces } },
                           Templates::NamespaceClosingTemplate());
        if (hasQtNamespace)
            printer->PrintRaw("QT_END_NAMESPACE\n");
        printer->Print("\n");
    };
    closeNamespaces(registrationPrinter);
    closeNamespaces(headerPrinter);
    closeNamespaces(sourcePrinter);

    common::iterateMessages(file, [&headerPrinter](const Descriptor *message) {
        MessageDeclarationPrinter messageDef(message, headerPrinter);
        messageDef.printMetaTypesDeclaration();
    });

    // Include the moc file:
    sourcePrinter->Print({{"source_file", filename + Templates::ProtoFileSuffix()}},
                         "#include \"moc_$source_file$.cpp\"\n");

    headerPrinter->Print({{"filename", filename}}, Templates::FooterTemplate());
    return true;
}
