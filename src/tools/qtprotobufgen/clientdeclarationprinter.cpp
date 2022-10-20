// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "clientdeclarationprinter.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/logging.h>

#include <string>
#include <unordered_set>

#include "generatorcommon.h"
#include "templates.h"
#include "utils.h"

using namespace ::QtProtobuf::generator;
using namespace ::google::protobuf;
using namespace ::google::protobuf::compiler;
ClientDeclarationPrinter::ClientDeclarationPrinter(
        const ::google::protobuf::ServiceDescriptor *service,
        const std::shared_ptr<::google::protobuf::io::Printer> &printer)
    : DescriptorPrinterBase<::google::protobuf::ServiceDescriptor>(
            service, printer, common::produceClientTypeMap(service, nullptr))

{
}

void ClientDeclarationPrinter::printOpenNamespace()
{
    m_printer->Print({ { "scope_namespaces", m_typeMap["scope_type"] } },
                     Templates::NamespaceTemplate());
}

void ClientDeclarationPrinter::printClientClass()
{
    m_printer->Print(m_typeMap, Templates::ChildClassDeclarationTemplate());
}

void ClientDeclarationPrinter::printConstructor()
{
    Indent();
    m_printer->Print(m_typeMap, Templates::QObjectConstructorMessageDeclarationTemplate());
    Outdent();
}

void ClientDeclarationPrinter::printClientMethodsDeclaration()
{
    Indent();
    for (int i = 0; i < m_descriptor->method_count(); i++) {
        const MethodDescriptor *method = m_descriptor->method(i);
        MethodMap parameters = common::produceMethodMap(method, m_typeMap["classname"]);

        if (method->server_streaming()) {
            m_printer->Print(parameters, Templates::ClientMethodServerStreamDeclarationTemplate());
            m_printer->Print(parameters, Templates::ClientMethodServerStream2DeclarationTemplate());
        } else {
            m_printer->Print(parameters, Templates::ClientMethodDeclarationSyncTemplate());
            m_printer->Print(parameters, Templates::ClientMethodDeclarationAsyncTemplate());
            m_printer->Print(parameters, Templates::ClientMethodDeclarationAsync2Template());
            if (Options::instance().hasQml()) {
                m_printer->Print(parameters, Templates::ClientMethodDeclarationQmlTemplate());
                m_printer->Print(parameters, Templates::ClientMethodDeclarationQml2Template());
            }
        }
        m_printer->Print("\n");
    }
    Outdent();

    printPrivateBlock();
    Indent();
    for (int i = 0; i < m_descriptor->method_count(); i++) {
        const MethodDescriptor *method = m_descriptor->method(i);
        MethodMap parameters = common::produceMethodMap(method, m_typeMap["classname"]);
        if (method->server_streaming()) {
            if (Options::instance().hasQml()) {
                m_printer->Print(parameters,
                                 Templates::ClientMethodServerStreamQmlDeclarationTemplate());
            }
        }
    }
    Outdent();
    m_printer->Print("\n");
}

void ClientDeclarationPrinter::printCloseNamespace()
{
    m_printer->Print({ { "scope_namespaces", m_typeMap["scope_type"] } },
                     Templates::NamespaceClosingTemplate());
}
