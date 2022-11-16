// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>, Tatyana Borisova <tanusshhka@mail.ru>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "messagedefinitionprinter.h"

#include <google/protobuf/descriptor.h>
#include "options.h"
#include "templates.h"
#include "generatorcommon.h"

#include <cassert>

using namespace ::QtProtobuf::generator;
using namespace ::google::protobuf;
using namespace ::google::protobuf::io;

MessageDefinitionPrinter::MessageDefinitionPrinter(const Descriptor *message,
                                                   std::shared_ptr<Printer> printer)
    : DescriptorPrinterBase<Descriptor>(message, std::move(printer),
                                        common::produceMessageTypeMap(message, nullptr))
{
}

MessageDefinitionPrinter::~MessageDefinitionPrinter() = default;

void MessageDefinitionPrinter::printClassDefinitionPrivate()
{
    if (common::hasNestedMessages(m_descriptor)) {
        auto scopeNamespaces = common::getNestedScopeNamespace(m_typeMap["classname"]);
        m_printer->Print(scopeNamespaces, Templates::NamespaceTemplate());
        common::iterateNestedMessages(m_descriptor, [this](const Descriptor *nestedMessage) {
            MessageDefinitionPrinter nestedPrinter(nestedMessage, m_printer);
            nestedPrinter.printClassDefinitionPrivate();
        });
        m_printer->Print(scopeNamespaces, Templates::NamespaceClosingTemplate());
    }

    printDestructor();
    printFieldsOrdering();
    printRegisterBody();
    printConstructors();
    printCopyFunctionality();
    printMoveSemantic();
    printComparisonOperators();
    printGetters();
}

void MessageDefinitionPrinter::printClassDefinition()
{
    printClassDefinitionPrivate();
}

void MessageDefinitionPrinter::printRegisterBody()
{
    std::vector<std::string> registredMetaTypes;

    m_printer->Print(m_typeMap, Templates::MetaTypeRegistrationMessageDefinition());
    Indent();
    if (Options::instance().hasQml()) {
        m_printer->Print(m_typeMap, Templates::MetaTypeRegistrationQmlListTemplate());
        m_printer->Print(m_typeMap, Templates::QmlRegisterTypeTemplate());
    }

    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                auto it = propertyMap.find("full_type");
                assert(it != propertyMap.end());
                const auto &fullTypeName = it->second;

                // Check if type is already registered
                if (utils::contains(registredMetaTypes, fullTypeName))
                    return;
                registredMetaTypes.push_back(fullTypeName);
                if (field->type() == FieldDescriptor::TYPE_ENUM
                    && common::isLocalEnum(field->enum_type(), m_descriptor)) {
                    m_printer->Print(propertyMap, Templates::MetaTypeRegistrationLocalEnumTemplate());
                } else if (field->is_map()) {
                    m_printer->Print(propertyMap, Templates::MetaTypeRegistrationMapTemplate());
                }
            });

    Outdent();
    m_printer->Print(Templates::SimpleBlockEnclosureTemplate());
}

void MessageDefinitionPrinter::printFieldsOrdering()
{
    const int fieldCount = m_descriptor->field_count();
    constexpr int MetaFieldsCount = 4;
    constexpr int Version = 0;

    int uint_dataOffset = 1; // the json name offsets are always stored at offset 0
    auto char_dataSize = charDataSize();
    std::map<std::string, std::string> dataVariables = {
        { "version_number", std::to_string(Version) },
        { "classname", m_typeMap["classname"] },
        { "num_fields", std::to_string(fieldCount) },
        // +1 for EOS offset in JSON data
        { "field_number_offset", std::to_string(fieldCount * (uint_dataOffset++) + 1) },
        // +1 for EOS offset in JSON data
        { "property_index_offset", std::to_string(fieldCount * (uint_dataOffset++) + 1) },
        // +1 for EOS offset in JSON data
        { "field_flags_offset", std::to_string(fieldCount * (uint_dataOffset++) + 1) },
        // +1 for EOS offset in JSON data
        { "uint_size", std::to_string(fieldCount * MetaFieldsCount + 1) },
        { "char_size", std::to_string(char_dataSize) },
    };
    assert(uint_dataOffset == MetaFieldsCount);

    m_printer->Print(dataVariables, Templates::PropertyOrderingDataOpeningTemplate());
    Indent();
    // uint_data
    m_printer->Print("// uint_data\n{\n");
    Indent();

    m_printer->Print("// JSON name offsets:\n");
    printUintData(Templates::JsonNameOffsetsUintDataTemplate());
    // Include an extra offset which points to the end-of-string, so we can efficiently get the
    // length of all the strings by subtracting adjacent offsets:
    m_printer->Print({ { "json_name_offset", std::to_string(char_dataSize - 1) },
                       { "json_name", "end-of-string-marker" } },
                     Templates::JsonNameOffsetsUintDataTemplate());

    m_printer->Print("// Field numbers:\n");
    printUintData(Templates::FieldNumbersUintDataTemplate());

    m_printer->Print("// Property indices:\n");
    printUintData(Templates::QtPropertyIndicesUintDataTemplate());

    m_printer->Print("// Field flags:\n");
    printUintData(Templates::FieldFlagsUintDataTemplate());

    Outdent();
    m_printer->Print("},\n");

    // char_data
    m_printer->Print("// char_data\n\"");
    printCharData();
    m_printer->Print("\"\n");
    Outdent();

    m_printer->Print(m_typeMap, Templates::PropertyOrderingDataClosingTemplate());

    m_printer->Print(m_typeMap, Templates::PropertyOrderingDefinitionTemplate());
}

void MessageDefinitionPrinter::printUintData(const char *templateString)
{
    size_t jsonOffset = 0;
    int index = 0;
    const int numFields = m_descriptor->field_count();
    for (int i = 0; i < numFields; ++i) {
        const FieldDescriptor *field = m_descriptor->field(i);
        const std::map<std::string, std::string> variables = {
            { "json_name_offset", std::to_string(jsonOffset) },
            { "field_number", std::to_string(field->number()) },
            { "property_index", std::to_string(++index) },
            { "field_flags", common::collectFieldFlags(field) },
            { "json_name", field->json_name() },
        };
        m_printer->Print(variables, templateString);

        const size_t length = field->json_name().length();
        jsonOffset += length + 1; // +1 for the embedded null terminator
    }
}

void MessageDefinitionPrinter::printCharData()
{
    const int numFields = m_descriptor->field_count();
    for (int i = 0; i < numFields; ++i) {
        const FieldDescriptor *field = m_descriptor->field(i);
        if (i && i % 5 == 0) // add some newlines to avoid overly long lines
            m_printer->Print("\"\n\"");
        m_printer->Print({ { "json_name", field->json_name() } }, "$json_name$\\0");
    }
}

size_t MessageDefinitionPrinter::charDataSize() const
{
    size_t size = 1; // 1 for the trailing null terminator
    const int numFields = m_descriptor->field_count();
    for (int i = 0; i < numFields; ++i) {
        const FieldDescriptor *field = m_descriptor->field(i);
        size += field->json_name().length() + 1; // +1 for the embedded null terminator
    }
    return size;
}

void MessageDefinitionPrinter::printConstructors()
{
    m_printer->Print(m_typeMap, Templates::ConstructorMessageDefinitionTemplate());
    printInitializationList();
    m_printer->Print(Templates::EmptyBracesTemplate());

    if (m_descriptor->full_name() == "google.protobuf.Timestamp") {
        m_printer->Print(
                "Timestamp::Timestamp(const QDateTime &datetime, QObject *parent) : "
                "QObject(parent)\n"
                ", m_seconds(datetime.toMSecsSinceEpoch() / 1000)\n"
                ", m_nanos((datetime.toMSecsSinceEpoch() % 1000) * 1000)\n"
                "{}\n"
                "Timestamp::operator QDateTime() const\n"
                "{\n"
                "    return QDateTime::fromMSecsSinceEpoch(m_seconds * 1000 + m_nanos / 1000);\n"
                "}\n");
    }
}

void MessageDefinitionPrinter::printInitializationList()
{
    const auto numFields = m_descriptor->field_count();
    Indent();
    for (int i = 0; i < numFields; ++i) {
        const FieldDescriptor *field = m_descriptor->field(i);
        auto propertyMap = common::producePropertyMap(field, m_descriptor);
        if (!field->is_repeated() && !field->is_map()) {
            switch (field->type()) {
            case FieldDescriptor::TYPE_DOUBLE:
            case FieldDescriptor::TYPE_FLOAT:
                propertyMap["initializer"] = "0.0";
                break;
            case FieldDescriptor::TYPE_FIXED32:
            case FieldDescriptor::TYPE_FIXED64:
            case FieldDescriptor::TYPE_INT32:
            case FieldDescriptor::TYPE_INT64:
            case FieldDescriptor::TYPE_SINT32:
            case FieldDescriptor::TYPE_SINT64:
            case FieldDescriptor::TYPE_UINT32:
            case FieldDescriptor::TYPE_UINT64:
                propertyMap["initializer"] = "0";
                break;
            case FieldDescriptor::TYPE_BOOL:
                propertyMap["initializer"] = "false";
                break;
            case FieldDescriptor::TYPE_ENUM:
                propertyMap["initializer"] =
                        propertyMap["scope_type"] + "::" + field->enum_type()->value(0)->name();
                break;
            default:
                propertyMap["initializer"] = "";
                break;
            }
        }

        if (common::isPureMessage(field)) {
            m_printer->Print(",\n");
            m_printer->Print(propertyMap, Templates::InitializerMemberMessageTemplate());
        } else {
            if (!propertyMap["initializer"].empty()) {
                m_printer->Print(",\n");
                m_printer->Print(propertyMap, Templates::InitializerMemberTemplate());
            }
        }
    }
    Outdent();
}

void MessageDefinitionPrinter::printCopyFunctionality()
{
    assert(m_descriptor != nullptr);
    if (m_descriptor->field_count() == 0) {
        m_printer->Print(m_typeMap, Templates::EmptyCopyConstructorDefinitionTemplate());
        m_printer->Print(m_typeMap, Templates::EmptyAssignmentOperatorDefinitionTemplate());
        return;
    }

    m_printer->Print(m_typeMap, Templates::CopyConstructorDefinitionTemplate());
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (common::isPureMessage(field)) {
                    m_printer->Print(",\n");
                    m_printer->Print(propertyMap,
                                    Templates::InitializerMemberMessageTemplate());
                }
            });
    m_printer->Print("\n{\n");

    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (common::isPureMessage(field)) {
                    m_printer->Print(propertyMap, Templates::CopyMemberMessageTemplate());
                } else {
                    m_printer->Print(propertyMap, Templates::CopyMemberTemplate());
                }
            });
    Outdent();
    m_printer->Print(Templates::SimpleBlockEnclosureTemplate());

    m_printer->Print(m_typeMap, Templates::AssignmentOperatorDefinitionTemplate());
    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (common::isPureMessage(field)) {
                    m_printer->Print(propertyMap, Templates::AssignMemberMessageTemplate());
                } else {
                    m_printer->Print(propertyMap, Templates::CopyMemberTemplate());
                }
            });
    m_printer->Print(Templates::AssignmentOperatorReturnTemplate());
    Outdent();
    m_printer->Print(Templates::SimpleBlockEnclosureTemplate());
}

void MessageDefinitionPrinter::printMoveSemantic()
{
    assert(m_descriptor != nullptr);
    if (m_descriptor->field_count() == 0) {
        m_printer->Print(m_typeMap, Templates::EmptyMoveConstructorDefinitionTemplate());
        m_printer->Print(m_typeMap, Templates::EmptyMoveAssignmentOperatorDefinitionTemplate());
        return;
    }

    m_printer->Print(m_typeMap, Templates::MoveConstructorDefinitionTemplate());
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (common::isPureMessage(field)) {
                    m_printer->Print(",\n");
                    m_printer->Print(propertyMap,
                                    Templates::InitializerMemberMessageTemplate());
                }
            });
    m_printer->Print("\n{\n");

    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (field->type() == FieldDescriptor::TYPE_MESSAGE
                    || field->type() == FieldDescriptor::TYPE_STRING
                    || field->type() == FieldDescriptor::TYPE_BYTES || field->is_repeated()) {
                    if (common::isPureMessage(field)) {
                        m_printer->Print(propertyMap, Templates::MoveMemberMessageTemplate());
                    } else {
                        m_printer->Print(propertyMap,
                                        Templates::MoveConstructorMemberComplexTemplate());
                    }
                } else {
                    if (field->type() != FieldDescriptor::TYPE_ENUM) {
                        m_printer->Print(propertyMap, Templates::MoveMemberTemplate());
                    } else {
                        m_printer->Print(propertyMap, Templates::MoveMemberEnumTemplate());
                    }
                }
            });
    Outdent();
    m_printer->Print(Templates::SimpleBlockEnclosureTemplate());

    m_printer->Print(m_typeMap, Templates::MoveAssignmentOperatorDefinitionTemplate());
    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (field->type() == FieldDescriptor::TYPE_MESSAGE
                    || field->type() == FieldDescriptor::TYPE_STRING
                    || field->type() == FieldDescriptor::TYPE_BYTES || field->is_repeated()) {
                    if (common::isPureMessage(field)) {
                        m_printer->Print(propertyMap, Templates::MoveAssignMemberMessageTemplate());
                    } else {
                        m_printer->Print(propertyMap, Templates::MoveAssignMemberComplexTemplate());
                    }
                } else {
                    if (field->type() != FieldDescriptor::TYPE_ENUM) {
                        m_printer->Print(propertyMap, Templates::MoveMemberTemplate());
                    } else {
                        m_printer->Print(propertyMap, Templates::MoveMemberEnumTemplate());
                    }
                }
            });
    m_printer->Print(Templates::AssignmentOperatorReturnTemplate());
    Outdent();
    m_printer->Print(Templates::SimpleBlockEnclosureTemplate());
}

void MessageDefinitionPrinter::printComparisonOperators()
{
    assert(m_descriptor != nullptr);
    if (m_descriptor->field_count() == 0) {
        m_printer->Print(m_typeMap, Templates::EmptyEqualOperatorDefinitionTemplate());
        m_printer->Print(m_typeMap, Templates::NotEqualOperatorDefinitionTemplate());
        return;
    }

    m_printer->Print(m_typeMap, Templates::EqualOperatorDefinitionTemplate());

    bool isFirst = true;
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, PropertyMap &propertyMap) {
                if (!isFirst) {
                    m_printer->Print("\n&& ");
                } else {
                    Indent();
                    Indent();
                    isFirst = false;
                }
                if (common::isPureMessage(field)) {
                    m_printer->Print(propertyMap, Templates::EqualOperatorMemberMessageTemplate());
                } else if (field->type() == FieldDescriptor::TYPE_MESSAGE && field->is_repeated()) {
                    m_printer->Print(propertyMap, Templates::EqualOperatorMemberRepeatedTemplate());
                } else {
                    m_printer->Print(propertyMap, Templates::EqualOperatorMemberTemplate());
                }
            });

    // Only if at least one field "copied"
    if (!isFirst) {
        Outdent();
        Outdent();
    }

    m_printer->Print(";\n");
    m_printer->Print(Templates::SimpleBlockEnclosureTemplate());

    m_printer->Print(m_typeMap, Templates::NotEqualOperatorDefinitionTemplate());
}

void MessageDefinitionPrinter::printGetters()
{
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, PropertyMap &propertyMap) {
                if (common::isPureMessage(field)) {
                    m_printer->Print(propertyMap, Templates::PrivateGetterMessageDefinitionTemplate());
                    m_printer->Print(propertyMap, Templates::GetterMessageDefinitionTemplate());
                }
                if (field->is_repeated()) {
                    if (field->type() == FieldDescriptor::TYPE_MESSAGE && !field->is_map()
                        && !common::isQtType(field) && Options::instance().hasQml()) {
                        m_printer->Print(propertyMap, Templates::GetterQmlListDefinitionTemplate());
                    }
                }
            });

    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, PropertyMap &propertyMap) {
                switch (field->type()) {
                case FieldDescriptor::TYPE_MESSAGE:
                    if (!field->is_map() && !field->is_repeated() && !common::isQtType(field)) {
                        m_printer->Print(propertyMap,
                                        Templates::PrivateSetterMessageDefinitionTemplate());
                        m_printer->Print(propertyMap,
                                        Templates::SetterMessageDefinitionTemplate());
                    } else {
                        m_printer->Print(propertyMap,
                                        Templates::SetterComplexDefinitionTemplate());
                    }
                    break;
                case FieldDescriptor::FieldDescriptor::TYPE_STRING:
                case FieldDescriptor::FieldDescriptor::TYPE_BYTES:
                    m_printer->Print(propertyMap, Templates::SetterComplexDefinitionTemplate());
                    break;
                default:
                    break;
                }
            });
}

void MessageDefinitionPrinter::printDestructor()
{
    m_printer->Print(m_typeMap, "$classname$::~$classname$() = default;\n\n");
}

void MessageDefinitionPrinter::printClassRegistration(Printer *printer)
{
    if (common::hasNestedMessages(m_descriptor)) {
        auto scopeNamespaces = common::getNestedScopeNamespace(m_typeMap["classname"]);
        printer->Print(scopeNamespaces, Templates::NamespaceTemplate());
        common::iterateNestedMessages(
                m_descriptor, [this, &printer](const Descriptor *nestedMessage) {
                    MessageDefinitionPrinter nestedPrinter(nestedMessage, m_printer);
                    nestedPrinter.printClassRegistration(printer);
                });
        printer->Print(scopeNamespaces, Templates::NamespaceClosingTemplate());
    }

    printer->Print(m_typeMap, Templates::RegistrarTemplate());
}
