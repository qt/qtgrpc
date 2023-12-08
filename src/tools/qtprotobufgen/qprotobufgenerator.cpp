// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#include "qprotobufgenerator.h"
#include "enumdeclarationprinter.h"
#include "enumdefinitionprinter.h"
#include "messagedeclarationprinter.h"
#include "messagedefinitionprinter.h"

#include "commontemplates.h"
#include "utils.h"
#include "options.h"

#include <array>
#include <cassert>
#include <numeric>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.h>

using namespace ::QtProtobuf;
using namespace ::qtprotoccommon;
using namespace ::google::protobuf;
using namespace ::google::protobuf::io;
using namespace ::google::protobuf::compiler;

QProtobufGenerator::QProtobufGenerator() : GeneratorBase()
{}

QProtobufGenerator::~QProtobufGenerator() = default;

bool QProtobufGenerator::Generate(const FileDescriptor *file,
                                  [[maybe_unused]] const std::string &parameter,
                                  GeneratorContext *generatorContext,
                                  [[maybe_unused]] std::string *error) const
{
    assert(file != nullptr && generatorContext != nullptr);

    return GenerateMessages(file, generatorContext);
}

void QProtobufGenerator::GenerateSources(const FileDescriptor *file,
                                         GeneratorContext *generatorContext) const
{
    assert(file != nullptr && generatorContext != nullptr);

    std::string filename = utils::extractFileBasename(file->name());
    std::string basename = generateBaseName(file, filename);
    std::unique_ptr<io::ZeroCopyOutputStream> sourceStream(
                generatorContext->Open(basename + CommonTemplates::ProtoFileSuffix() + ".cpp"));
    std::unique_ptr<io::ZeroCopyOutputStream> registrationStream(
                generatorContext->Open(basename + "_protobuftyperegistrations.cpp"));

    std::shared_ptr<Printer> sourcePrinter(new Printer(sourceStream.get(), '$'));
    std::shared_ptr<Printer> registrationPrinter(new Printer(registrationStream.get(), '$'));

    printDisclaimer(sourcePrinter.get());
    sourcePrinter->Print({{"include", basename + CommonTemplates::ProtoFileSuffix()}},
                         CommonTemplates::InternalIncludeTemplate());

    std::array<std::string, 2> registrationPrinterExternalIncludes {"QProtobufSerializer",
                                                                    "array"};
    for (const auto &include : registrationPrinterExternalIncludes) {
        registrationPrinter->Print({{"include", include}},
                                   CommonTemplates::ExternalIncludeTemplate());
    }
    registrationPrinter->Print({{"include", basename + CommonTemplates::ProtoFileSuffix()}},
                               CommonTemplates::InternalIncludeTemplate());

    sourcePrinter->Print({{"include", "QProtobufSerializer"}},
                         CommonTemplates::ExternalIncludeTemplate());
    if (Options::instance().hasQml()) {
        sourcePrinter->Print({{"include", "QQmlEngine"}},
                             CommonTemplates::ExternalIncludeTemplate());
    }

    OpenFileNamespaces(file, sourcePrinter.get());
    OpenFileNamespaces(file, registrationPrinter.get());

    for (int i = 0; i < file->enum_type_count(); ++i) {
        EnumDefinitionPrinter enumSourceDef(file->enum_type(i), sourcePrinter);
        enumSourceDef.run();
    }

    common::iterateMessages(
                file,
                [&sourcePrinter, &registrationPrinter](const Descriptor *message) {
        MessageDefinitionPrinter messageDef(message, sourcePrinter);
        messageDef.printClassDefinition();
        messageDef.printClassRegistration(registrationPrinter.get());
    });

    CloseFileNamespaces(file, registrationPrinter.get());
    CloseFileNamespaces(file, sourcePrinter.get());

    // Include the moc file:
    sourcePrinter->Print({{"source_file", filename + CommonTemplates::ProtoFileSuffix()}},
                         "#include \"moc_$source_file$.cpp\"\n");

}

void QProtobufGenerator::GenerateHeader(const FileDescriptor *file,
                                        GeneratorContext *generatorContext) const
{
    assert(file != nullptr && generatorContext != nullptr);

    std::string filename = utils::extractFileBasename(file->name());
    std::string basename = generateBaseName(file, filename);
    std::set<std::string> internalIncludes;
    std::set<std::string> externalIncludes;

    std::unique_ptr<io::ZeroCopyOutputStream> headerStream(
                generatorContext->Open(basename
                                       + CommonTemplates::ProtoFileSuffix() + ".h"));
    std::shared_ptr<Printer> headerPrinter(new Printer(headerStream.get(), '$'));

    printDisclaimer(headerPrinter.get());
    headerPrinter->Print({{"filename", filename}}, CommonTemplates::PreambleTemplate());

    headerPrinter->Print(CommonTemplates::DefaultProtobufIncludesTemplate());
    if (Options::instance().hasQml()) {
        headerPrinter->Print(CommonTemplates::QmlProtobufIncludesTemplate());
    }

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
        internalIncludes.insert(utils::removeFileSuffix(file->dependency(i)->name())
                                + CommonTemplates::ProtoFileSuffix());
    }

    for (const auto &include : externalIncludes) {
        headerPrinter->Print({{"include", include}}, CommonTemplates::ExternalIncludeTemplate());
    }

    for (const auto &include : internalIncludes) {
        headerPrinter->Print({{"include", include}}, CommonTemplates::InternalIncludeTemplate());
    }

    headerPrinter->PrintRaw("\n");
    if (!Options::instance().exportMacro().empty()) {
        headerPrinter->Print({ { "export_macro", Options::instance().exportMacro() } },
                             CommonTemplates::ExportMacroTemplate());
    }
    OpenFileNamespaces(file, headerPrinter.get());

    for (int i = 0; i < file->enum_type_count(); ++i) {
        EnumDeclarationPrinter enumDecl(file->enum_type(i), headerPrinter);
        enumDecl.run();
    }

    common::iterateMessages(file, [&headerPrinter](const Descriptor *message) {
        MessageDeclarationPrinter messageDecl(message, headerPrinter);
        messageDecl.printClassForwardDeclaration();
    });

    common::iterateMessages(
                file,
                [&headerPrinter](const Descriptor *message) {
        MessageDeclarationPrinter messageDecl(message, headerPrinter);
        messageDecl.printClassDeclaration();
    });

    CloseFileNamespaces(file, headerPrinter.get());

    common::iterateMessages(file, [&headerPrinter](const Descriptor *message) {
        MessageDeclarationPrinter messageDef(message, headerPrinter);
        messageDef.printMetaTypesDeclaration();
    });

    headerPrinter->Print({{"filename", filename}}, CommonTemplates::FooterTemplate());
}

bool QProtobufGenerator::GenerateMessages(const FileDescriptor *file,
                                          GeneratorContext *generatorContext) const
{
    assert(file != nullptr && generatorContext != nullptr);

    if (file->message_type_count() <= 0 && file->enum_type_count() <= 0)
        return true;

    GenerateHeader(file, generatorContext);
    GenerateSources(file, generatorContext);
    return true;
}
