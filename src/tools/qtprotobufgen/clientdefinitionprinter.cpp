// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "clientdefinitionprinter.h"

#include <google/protobuf/io/zero_copy_stream.h>

#include "generatorcommon.h"
#include "templates.h"
#include "utils.h"

using namespace ::QtProtobuf::generator;
using namespace ::google::protobuf;
using namespace ::google::protobuf::compiler;

ClientDefinitionPrinter::ClientDefinitionPrinter(
        const google::protobuf::ServiceDescriptor *service,
        const std::shared_ptr<::google::protobuf::io::Printer> &printer)
    : DescriptorPrinterBase<google::protobuf::ServiceDescriptor>(
            service, printer, common::produceClientTypeMap(service, nullptr))
{
}

void ClientDefinitionPrinter::printOpenNamespace()
{
    m_printer->Print({ { "scope_namespaces", m_typeMap["scope_type"] } },
                     Templates::NamespaceTemplate());
}

void ClientDefinitionPrinter::printMethods()
{
    for (int i = 0; i < m_descriptor->method_count(); i++) {
        const MethodDescriptor *method = m_descriptor->method(i);
        MethodMap parameters = common::produceMethodMap(method, m_typeMap["classname"]);
        if (method->server_streaming()) {
            m_printer->Print(parameters, Templates::ClientMethodServerStreamDefinitionTemplate());
            m_printer->Print(parameters, Templates::ClientMethodServerStream2DefinitionTemplate());
            if (Options::instance().hasQml()) {
                m_printer->Print(parameters,
                                 Templates::ClientMethodServerStreamQmlDefinitionTemplate());
            }
        } else {
            m_printer->Print(parameters, Templates::ClientMethodDefinitionSyncTemplate());
            m_printer->Print(parameters, Templates::ClientMethodDefinitionAsyncTemplate());
            m_printer->Print(parameters, Templates::ClientMethodDefinitionAsync2Template());
            if (Options::instance().hasQml()) {
                m_printer->Print(parameters, Templates::ClientMethodDefinitionQmlTemplate());
                m_printer->Print(parameters, Templates::ClientMethodDefinitionQml2Template());
            }
        }
    }
}

void ClientDefinitionPrinter::printConstructor()
{
    m_printer->Print({ { "classname", m_typeMap["classname"] },
                       { "parent_class", m_typeMap["parent_class"] },
                       { "service_name", m_descriptor->full_name() } },
                     Templates::ClientConstructorDefinitionTemplate());
}

void ClientDefinitionPrinter::printCloseNamespace()
{
    m_printer->Print({ { "scope_namespaces", m_typeMap["scope_type"] } },
                     Templates::NamespaceClosingTemplate());
}
