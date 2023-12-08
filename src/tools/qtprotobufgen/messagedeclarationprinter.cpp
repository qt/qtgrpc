// Copyright (C) 2023 The Qt Company Ltd.
// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>, Tatyana Borisova <tanusshhka@mail.ru>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#include "messagedeclarationprinter.h"
#include "utils.h"
#include "options.h"
#include "generatorcommon.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include <cassert>

using namespace ::QtProtobuf;
using namespace ::qtprotoccommon;
using namespace ::google::protobuf;
using namespace ::google::protobuf::io;
using namespace ::google::protobuf::compiler;

MessageDeclarationPrinter::MessageDeclarationPrinter(const Descriptor *message,
                                                     std::shared_ptr<Printer> printer)
    : DescriptorPrinterBase<Descriptor>(message,
                                        std::move(printer),
                                        common::produceMessageTypeMap(message, nullptr))
{
}

MessageDeclarationPrinter::~MessageDeclarationPrinter() = default;

void MessageDeclarationPrinter::printClassForwardDeclarationPrivate()
{
    m_printer->Print(m_typeMap, CommonTemplates::ClassMessageForwardDeclarationTemplate());
    m_printer->Print(m_typeMap, CommonTemplates::UsingMessageTemplate());

    if (common::hasNestedMessages(m_descriptor)) {
        auto scopeNamespaces = common::getNestedScopeNamespace(m_typeMap["classname"]);
        m_printer->Print(scopeNamespaces, CommonTemplates::NamespaceTemplate());
        common::iterateNestedMessages(m_descriptor, [this](const Descriptor *nestedMessage) {
            MessageDeclarationPrinter nesterPrinter(nestedMessage, m_printer);
            nesterPrinter.printClassForwardDeclarationPrivate();
        });
        m_printer->Print(scopeNamespaces, CommonTemplates::NamespaceClosingTemplate());
    }
}

void MessageDeclarationPrinter::printClassForwardDeclaration()
{
    printClassForwardDeclarationPrivate();
}

void MessageDeclarationPrinter::printClassDeclaration()
{
    printClassDeclarationPrivate();
}

void MessageDeclarationPrinter::printClassDeclarationPrivate()
{
    printComments(m_descriptor);
    printClassDeclarationBegin();
    printClassBody();
    encloseClass();

    if (common::hasNestedMessages(m_descriptor)) {
        auto scopeNamespaces = common::getNestedScopeNamespace(m_typeMap["classname"]);
        m_printer->Print(scopeNamespaces, CommonTemplates::NamespaceTemplate());
        common::iterateNestedMessages(m_descriptor, [this](const Descriptor *nestedMessage) {
            MessageDeclarationPrinter nesterPrinter(nestedMessage, m_printer);
            nesterPrinter.printClassDeclarationPrivate();
        });
        m_printer->Print(scopeNamespaces, CommonTemplates::NamespaceClosingTemplate());
    }
}

void MessageDeclarationPrinter::printCopyFunctionality()
{
    assert(m_descriptor != nullptr);
    m_printer->Print(m_typeMap, CommonTemplates::CopyConstructorDeclarationTemplate());

    m_printer->Print(m_typeMap, CommonTemplates::AssignmentOperatorDeclarationTemplate());
}

void MessageDeclarationPrinter::printMoveSemantic()
{
    assert(m_descriptor != nullptr);
    m_printer->Print(m_typeMap, CommonTemplates::MoveConstructorDeclarationTemplate());

    m_printer->Print(m_typeMap, CommonTemplates::MoveAssignmentOperatorDeclarationTemplate());
}

void MessageDeclarationPrinter::printComparisonOperators()
{
    assert(m_descriptor != nullptr);
    m_printer->Print(m_typeMap, CommonTemplates::EqualOperatorDeclarationTemplate());

    m_printer->Print(m_typeMap, CommonTemplates::NotEqualOperatorDeclarationTemplate());
}

void MessageDeclarationPrinter::printConstructors()
{
    m_printer->Print(m_typeMap, CommonTemplates::ConstructorMessageDeclarationTemplate());

    if (m_descriptor->full_name() == "google.protobuf.Timestamp") {
        m_printer->Print("Timestamp(const QDateTime &datetime);\n"
                        "operator QDateTime() const;\n");
    }
}

void MessageDeclarationPrinter::printMaps()
{
    Indent();
    const int numFields = m_descriptor->field_count();
    for (int i = 0; i < numFields; ++i) {
        const FieldDescriptor *field = m_descriptor->field(i);
        if (field->is_map()) {
            const Descriptor *type = field->message_type();
            const char *mapTemplate = type->field(1)->type() == FieldDescriptor::TYPE_MESSAGE
                    ? CommonTemplates::UsingMapMessageTemplate()
                    : CommonTemplates::UsingMapTemplate();
            m_printer->Print(common::producePropertyMap(field, m_descriptor), mapTemplate);
        }
    }
    Outdent();
}

void MessageDeclarationPrinter::printNested()
{
    Indent();
    common::iterateNestedMessages(m_descriptor, [&](const Descriptor *nestedMessage) {
        m_printer->Print(common::produceMessageTypeMap(nestedMessage, m_descriptor),
                        CommonTemplates::UsingNestedMessageTemplate());
    });
    Outdent();
}

void MessageDeclarationPrinter::printClassDeclarationBegin()
{
    m_printer->Print(m_typeMap, CommonTemplates::ClassMessageBeginDeclarationTemplate());
}

void MessageDeclarationPrinter::printMetaTypesDeclaration()
{
    m_printer->Print(m_typeMap, CommonTemplates::DeclareMetaTypeTemplate());

    if (Options::instance().hasQml()) {
        m_printer->Print(m_typeMap, CommonTemplates::DeclareMetaTypeQmlListTemplate());
    }

    common::iterateNestedMessages(m_descriptor, [&](const Descriptor *nestedMessage) {
        MessageDeclarationPrinter nestedPrinter(nestedMessage, m_printer);
        nestedPrinter.printMetaTypesDeclaration();
    });
}

void MessageDeclarationPrinter::printProperties()
{
    assert(m_descriptor != nullptr);
    // private section
    Indent();

    const int numFields = m_descriptor->field_count();
    for (int i = 0; i < numFields; ++i) {
        const FieldDescriptor *field = m_descriptor->field(i);
        const char *propertyTemplate = CommonTemplates::PropertyTemplate();
        if (common::isPureMessage(field)) {
            propertyTemplate = CommonTemplates::PropertyMessageTemplate();
        } else if (common::hasQmlAlias(field)) {
            propertyTemplate = CommonTemplates::PropertyNonScriptableTemplate();
        } else if (field->is_repeated() && !field->is_map()) {
            // Non-message list properties don't require an extra QQmlListProperty to access
            // their data, so the property name should not contain the 'Data' suffix
            if (field->type() == FieldDescriptor::TYPE_MESSAGE)
                propertyTemplate = CommonTemplates::PropertyRepeatedMessageTemplate();
            else
                propertyTemplate = CommonTemplates::PropertyRepeatedTemplate();
        }
        m_printer->Print(common::producePropertyMap(field, m_descriptor), propertyTemplate);
    }

    // Generate extra QmlListProperty that is mapped to list
    for (int i = 0; i < numFields; ++i) {
        const FieldDescriptor *field = m_descriptor->field(i);
        if (field->type() == FieldDescriptor::TYPE_MESSAGE && field->is_repeated()
            && !field->is_map() && Options::instance().hasQml()) {
            m_printer->Print(common::producePropertyMap(field, m_descriptor),
                            CommonTemplates::PropertyQmlListTemplate());
        } else if (common::hasQmlAlias(field)) {
            m_printer->Print(common::producePropertyMap(field, m_descriptor),
                            CommonTemplates::PropertyNonScriptableAliasTemplate());
        }
    }

    Outdent();
}

void MessageDeclarationPrinter::printGetters()
{
    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                printComments(field);
                m_printer->Print("\n");
                m_printer->Print(propertyMap,
                                 common::isPureMessage(field) ?
                                     CommonTemplates::GetterMessageDeclarationTemplate() :
                                     CommonTemplates::GetterTemplate());

                if (field->is_repeated()) {
                    m_printer->Print(propertyMap, CommonTemplates::GetterComplexTemplate());
                    if (field->type() == FieldDescriptor::TYPE_MESSAGE && !field->is_map()
                        && Options::instance().hasQml()) {
                        m_printer->Print(propertyMap,
                                         CommonTemplates::GetterQmlListDeclarationTemplate());
                    }
                }
            });
    Outdent();
}

void MessageDeclarationPrinter::printSetters()
{
    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                switch (field->type()) {
                case FieldDescriptor::TYPE_MESSAGE:
                    if (!field->is_map() && !field->is_repeated() && !common::isQtType(field)) {
                        m_printer->Print(propertyMap,
                                        CommonTemplates::SetterMessageDeclarationTemplate());
                    } else {
                        m_printer->Print(propertyMap,
                                        CommonTemplates::SetterComplexDeclarationTemplate());
                    }
                    break;
                case FieldDescriptor::FieldDescriptor::TYPE_STRING:
                case FieldDescriptor::FieldDescriptor::TYPE_BYTES:
                    m_printer->Print(propertyMap,
                                     CommonTemplates::SetterComplexDeclarationTemplate());
                    break;
                default:
                    m_printer->Print(propertyMap, CommonTemplates::SetterTemplate());
                    break;
                }
            });
    Outdent();
}

void MessageDeclarationPrinter::printPrivateGetters()
{
    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (common::isPureMessage(field)) {
                    m_printer->Print(propertyMap,
                                    CommonTemplates::PrivateGetterMessageDeclarationTemplate());
                }
            });
    Outdent();
}

void MessageDeclarationPrinter::printPrivateSetters()
{
    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (common::isPureMessage(field)) {
                    m_printer->Print(propertyMap,
                                    CommonTemplates::PrivateSetterMessageDeclarationTemplate());
                }
            });
    Outdent();
}

void MessageDeclarationPrinter::printPrivateMethods()
{
    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (common::hasQmlAlias(field)) {
                    m_printer->Print(propertyMap, CommonTemplates::GetterNonScriptableTemplate());
                    m_printer->Print(propertyMap, CommonTemplates::SetterNonScriptableTemplate());
                }
            });
    Outdent();
}

void MessageDeclarationPrinter::printQEnums()
{
    if (Options::instance().generateFieldEnum()) {
        printFieldEnum();
    }

    if (m_descriptor->enum_type_count() <= 0)
        return;

    Indent();
    for (int i = 0; i < m_descriptor->enum_type_count(); ++i) {
        const auto *enumDescr = m_descriptor->enum_type(i);
        auto typeMap = common::produceEnumTypeMap(enumDescr, m_descriptor);
        m_printer->Print(typeMap, CommonTemplates::EnumDefinitionTemplate());

        Indent();
        for (int j = 0; j < enumDescr->value_count(); ++j) {
            const auto *valueDescr = enumDescr->value(j);
            m_printer->Print({ { "enumvalue", utils::capitalizeAsciiName(valueDescr->name()) },
                               { "value", std::to_string(valueDescr->number()) } },
                             CommonTemplates::EnumFieldTemplate());
        }
        Outdent();
        m_printer->Print(CommonTemplates::SemicolonBlockEnclosureTemplate());
        m_printer->Print(typeMap, CommonTemplates::QEnumTemplate());
    }

    for (int i = 0; i < m_descriptor->enum_type_count(); ++i) {
        const auto *enumDescr = m_descriptor->enum_type(i);
        auto typeMap = common::produceEnumTypeMap(enumDescr, m_descriptor);
        m_printer->Print(typeMap, CommonTemplates::UsingRepeatedEnumTemplate());
    }
    Outdent();
}

void MessageDeclarationPrinter::printClassBody()
{
    printProperties();

    printPublicBlock();
    printQEnums();
    printNested();
    printMaps();

    Indent();
    printConstructors();
    printDestructor();

    printCopyFunctionality();
    printMoveSemantic();

    printComparisonOperators();
    Outdent();

    printGetters();
    printSetters();

    Indent();
    m_printer->Print(m_typeMap, CommonTemplates::MetaTypeRegistrationDeclaration());
    Outdent();

    printPrivateBlock();
    printPrivateGetters();
    printPrivateSetters();
    printPrivateMethods();

    printClassMembers();
}

void MessageDeclarationPrinter::printClassMembers()
{
    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (common::isPureMessage(field)) {
                    m_printer->Print(propertyMap, CommonTemplates::MemberMessageTemplate());
                } else if (field->is_repeated() && !field->is_map()) {
                    m_printer->Print(propertyMap, CommonTemplates::MemberRepeatedTemplate());
                } else {
                    m_printer->Print(propertyMap, CommonTemplates::MemberTemplate());
                }
            });
    Outdent();
}

void MessageDeclarationPrinter::printDestructor()
{
    m_printer->Print(m_typeMap, CommonTemplates::DestructorMessageDeclarationTemplate());
}

void MessageDeclarationPrinter::printFieldEnum()
{
    if (m_descriptor->field_count() > 0) {
        Indent();
        m_printer->Print(CommonTemplates::FieldEnumTemplate());
        Indent();
        common::iterateMessageFields(m_descriptor,
                                    [&](const FieldDescriptor *, const PropertyMap &propertyMap) {
                                        m_printer->Print(propertyMap,
                                                         CommonTemplates::FieldNumberTemplate());
                                    });
        Outdent();
        m_printer->Print(CommonTemplates::SemicolonBlockEnclosureTemplate());
        m_printer->Print({ { "type", CommonTemplates::QtProtobufFieldEnum() } },
                         CommonTemplates::QEnumTemplate());
        Outdent();
        m_printer->Print("\n");
    }
}
