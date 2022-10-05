// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>, Tatyana Borisova <tanusshhka@mail.ru>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "enumdefinitionprinter.h"

#include "options.h"

using namespace ::QtProtobuf::generator;
using namespace ::google::protobuf;
using namespace ::google::protobuf::io;

EnumDefinitionPrinter::EnumDefinitionPrinter(const EnumDescriptor *descriptor,
                                             std::shared_ptr<Printer> printer)
    : DescriptorPrinterBase<EnumDescriptor>(descriptor, std::move(printer),
                                            common::produceEnumTypeMap(descriptor, nullptr))
{
}

EnumDefinitionPrinter::~EnumDefinitionPrinter() = default;

void EnumDefinitionPrinter::printRegisterBody()
{
    m_printer->Print(m_typeMap, Templates::RegistrarEnumTemplate());
    m_printer->Print(m_typeMap, Templates::MetaTypeRegistrationGlobalEnumDefinition());
    Indent();
    if (Options::instance().hasQml())
        m_printer->Print(m_typeMap, Templates::QmlRegisterTypeEnumTemplate());

    m_printer->Print(m_typeMap, Templates::MetaTypeRegistrationGlobalEnumTemplate());
    m_printer->Print(m_typeMap, Templates::RegisterEnumSerializersTemplate());
    Outdent();
    m_printer->Print(Templates::SimpleBlockEnclosureTemplate());
}
