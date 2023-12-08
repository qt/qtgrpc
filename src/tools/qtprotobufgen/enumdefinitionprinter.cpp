// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>, Tatyana Borisova <tanusshhka@mail.ru>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#include "enumdefinitionprinter.h"

#include "options.h"

using namespace ::QtProtobuf;
using namespace ::qtprotoccommon;
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
    m_printer->Print(m_typeMap, CommonTemplates::RegistrarEnumTemplate());
    m_printer->Print(m_typeMap, CommonTemplates::MetaTypeRegistrationGlobalEnumDefinition());
    Indent();
    if (Options::instance().hasQml())
        m_printer->Print(m_typeMap, CommonTemplates::QmlRegisterTypeEnumTemplate());

    m_printer->Print(m_typeMap, CommonTemplates::MetaTypeRegistrationGlobalEnumTemplate());
    m_printer->Print(m_typeMap, CommonTemplates::RegisterEnumSerializersTemplate());
    Outdent();
    m_printer->Print(CommonTemplates::SimpleBlockEnclosureTemplate());
}
