// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>, Tatyana Borisova <tanusshhka@mail.ru>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "messagedeclarationprinter.h"
#include "utils.h"
#include "options.h"
#include "generatorcommon.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include <cassert>

using namespace ::QtProtobuf::generator;
using namespace ::google::protobuf;
using namespace ::google::protobuf::io;
using namespace ::google::protobuf::compiler;

MessageDeclarationPrinter::MessageDeclarationPrinter(const Descriptor *message,
                                                     std::shared_ptr<Printer> printer)
    : DescriptorPrinterBase<Descriptor>(message, std::move(printer),
                                        common::produceMessageTypeMap(message, nullptr))
{
}

MessageDeclarationPrinter::~MessageDeclarationPrinter() = default;

void MessageDeclarationPrinter::printClassForwardDeclarationPrivate()
{
    if (common::hasNestedMessages(m_descriptor)) {
        auto scopeNamespaces = common::getNestedScopeNamespace(m_typeMap["classname"]);
        m_printer->Print(scopeNamespaces, Templates::NamespaceTemplate());
        common::iterateNestedMessages(m_descriptor, [this](const Descriptor *nestedMessage) {
            MessageDeclarationPrinter nesterPrinter(nestedMessage, m_printer);
            nesterPrinter.printClassForwardDeclarationPrivate();
        });
        m_printer->Print(scopeNamespaces, Templates::NamespaceClosingTemplate());
    }

    m_printer->Print(m_typeMap, Templates::ClassMessageForwardDeclarationTemplate());
    m_printer->Print(m_typeMap, Templates::UsingMessageTemplate());
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
    if (common::hasNestedMessages(m_descriptor)) {
        auto scopeNamespaces = common::getNestedScopeNamespace(m_typeMap["classname"]);
        m_printer->Print(scopeNamespaces, Templates::NamespaceTemplate());
        common::iterateNestedMessages(m_descriptor, [this](const Descriptor *nestedMessage) {
            MessageDeclarationPrinter nesterPrinter(nestedMessage, m_printer);
            nesterPrinter.printClassDeclarationPrivate();
        });
        m_printer->Print(scopeNamespaces, Templates::NamespaceClosingTemplate());
    }
    printComments(m_descriptor);
    printClassDeclarationBegin();
    printClassBody();
    encloseClass();
}

void MessageDeclarationPrinter::printCopyFunctionality()
{
    assert(m_descriptor != nullptr);
    m_printer->Print(m_typeMap, Templates::CopyConstructorDeclarationTemplate());

    m_printer->Print(m_typeMap, Templates::AssignmentOperatorDeclarationTemplate());
}

void MessageDeclarationPrinter::printMoveSemantic()
{
    assert(m_descriptor != nullptr);
    m_printer->Print(m_typeMap, Templates::MoveConstructorDeclarationTemplate());

    m_printer->Print(m_typeMap, Templates::MoveAssignmentOperatorDeclarationTemplate());
}

void MessageDeclarationPrinter::printComparisonOperators()
{
    assert(m_descriptor != nullptr);
    m_printer->Print(m_typeMap, Templates::EqualOperatorDeclarationTemplate());

    m_printer->Print(m_typeMap, Templates::NotEqualOperatorDeclarationTemplate());
}

void MessageDeclarationPrinter::printConstructors()
{
    m_printer->Print(m_typeMap, Templates::ConstructorMessageDeclarationTemplate());

    if (m_descriptor->full_name() == "google.protobuf.Timestamp") {
        m_printer->Print("Timestamp(const QDateTime &datetime, QObject *parent = nullptr);\n"
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
                    ? Templates::UsingMapMessageTemplate()
                    : Templates::UsingMapTemplate();
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
                        Templates::UsingNestedMessageTemplate());
    });
    Outdent();
}

void MessageDeclarationPrinter::printClassDeclarationBegin()
{
    m_printer->Print(m_typeMap, Templates::ClassMessageBeginDeclarationTemplate());
}

void MessageDeclarationPrinter::printMetaTypesDeclaration()
{
    m_printer->Print(m_typeMap, Templates::DeclareMetaTypeTemplate());

    if (Options::instance().hasQml()) {
        m_printer->Print(m_typeMap, Templates::DeclareMetaTypeQmlListTemplate());
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
        const char *propertyTemplate = Templates::PropertyTemplate();
        if (common::isPureMessage(field)) {
            propertyTemplate = Templates::PropertyMessageTemplate();
        } else if (common::hasQmlAlias(field)) {
            propertyTemplate = Templates::PropertyNonScriptableTemplate();
        } else if (field->is_repeated() && !field->is_map()) {
            // Non-message list properties don't require an extra QQmlListProperty to access
            // their data, so the property name should not contain the 'Data' suffix
            if (field->type() == FieldDescriptor::TYPE_MESSAGE)
                propertyTemplate = Templates::PropertyRepeatedMessageTemplate();
            else
                propertyTemplate = Templates::PropertyRepeatedTemplate();
        }
        m_printer->Print(common::producePropertyMap(field, m_descriptor), propertyTemplate);
    }

    // Generate extra QmlListProperty that is mapped to list
    for (int i = 0; i < numFields; ++i) {
        const FieldDescriptor *field = m_descriptor->field(i);
        if (field->type() == FieldDescriptor::TYPE_MESSAGE && field->is_repeated()
            && !field->is_map() && Options::instance().hasQml()) {
            m_printer->Print(common::producePropertyMap(field, m_descriptor),
                            Templates::PropertyQmlListTemplate());
        } else if (common::hasQmlAlias(field)) {
            m_printer->Print(common::producePropertyMap(field, m_descriptor),
                            Templates::PropertyNonScriptableAliasTemplate());
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
                if (common::isPureMessage(field))
                    m_printer->Print(propertyMap, Templates::GetterMessageDeclarationTemplate());
                else
                    m_printer->Print(propertyMap, Templates::GetterTemplate());

                if (field->is_repeated()) {
                    m_printer->Print(propertyMap, Templates::GetterComplexTemplate());
                    if (field->type() == FieldDescriptor::TYPE_MESSAGE && !field->is_map()
                        && Options::instance().hasQml()) {
                        m_printer->Print(propertyMap, Templates::GetterQmlListDeclarationTemplate());
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
                    if (!field->is_map() && !field->is_repeated() && !common::isQtType(field))
                        m_printer->Print(propertyMap,
                                        Templates::SetterMessageDeclarationTemplate());
                    else
                        m_printer->Print(propertyMap,
                                        Templates::SetterComplexDeclarationTemplate());
                    break;
                case FieldDescriptor::FieldDescriptor::TYPE_STRING:
                case FieldDescriptor::FieldDescriptor::TYPE_BYTES:
                    m_printer->Print(propertyMap, Templates::SetterComplexDeclarationTemplate());
                    break;
                default:
                    m_printer->Print(propertyMap, Templates::SetterTemplate());
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
                if (common::isPureMessage(field))
                    m_printer->Print(propertyMap,
                                    Templates::PrivateGetterMessageDeclarationTemplate());
            });
    Outdent();
}

void MessageDeclarationPrinter::printPrivateSetters()
{
    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (common::isPureMessage(field))
                    m_printer->Print(propertyMap,
                                    Templates::PrivateSetterMessageDeclarationTemplate());
            });
    Outdent();
}

void MessageDeclarationPrinter::printSignals()
{
    Indent();
    const int numFields = m_descriptor->field_count();
    for (int i = 0; i < numFields; ++i) {
        m_printer->Print(common::producePropertyMap(m_descriptor->field(i), m_descriptor),
                        Templates::SignalTemplate());
    }
    Outdent();
}

void MessageDeclarationPrinter::printPrivateMethods()
{
    Indent();
    common::iterateMessageFields(
            m_descriptor, [&](const FieldDescriptor *field, const PropertyMap &propertyMap) {
                if (common::hasQmlAlias(field)) {
                    m_printer->Print(propertyMap, Templates::GetterNonScriptableTemplate());
                    m_printer->Print(propertyMap, Templates::SetterNonScriptableTemplate());
                }
            });
    Outdent();
}

void MessageDeclarationPrinter::printQEnums()
{
    if (Options::instance().generateFieldEnum()) {
        printFieldEnum();
        Indent();
        m_printer->Print({ { "type", Templates::QtProtobufFieldEnum() } }, Templates::QEnumTemplate());
        Outdent();
        m_printer->Print("\n");
    }

    if (m_descriptor->enum_type_count() <= 0)
        return;

    Indent();
    for (int i = 0; i < m_descriptor->enum_type_count(); ++i) {
        const auto *enumDescr = m_descriptor->enum_type(i);
        auto typeMap = common::produceEnumTypeMap(enumDescr, m_descriptor);
        m_printer->Print(typeMap, Templates::EnumDefinitionTemplate());

        Indent();
        for (int j = 0; j < enumDescr->value_count(); ++j) {
            const auto *valueDescr = enumDescr->value(j);
            m_printer->Print({ { "enumvalue", utils::capitalizeAsciiName(valueDescr->name()) },
                               { "value", std::to_string(valueDescr->number()) } },
                             Templates::EnumFieldTemplate());
        }
        Outdent();
        m_printer->Print(Templates::SemicolonBlockEnclosureTemplate());
        m_printer->Print(typeMap, Templates::QEnumTemplate());
    }

    for (int i = 0; i < m_descriptor->enum_type_count(); ++i) {
        const auto *enumDescr = m_descriptor->enum_type(i);
        auto typeMap = common::produceEnumTypeMap(enumDescr, m_descriptor);
        m_printer->Print(typeMap, Templates::UsingRepeatedEnumTemplate());
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
    m_printer->Print(m_typeMap, Templates::MetaTypeRegistrationDeclaration());
    Outdent();

    printSignalsBlock();
    printSignals();

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
                    m_printer->Print(propertyMap, Templates::MemberMessageTemplate());
                } else if (field->is_repeated() && !field->is_map()) {
                    m_printer->Print(propertyMap, Templates::MemberRepeatedTemplate());
                } else {
                    m_printer->Print(propertyMap, Templates::MemberTemplate());
                }
            });
    Outdent();
}

void MessageDeclarationPrinter::printDestructor()
{
    m_printer->Print(m_typeMap, Templates::DestructorMessageDeclarationTemplate());
}

void MessageDeclarationPrinter::printFieldEnum()
{
    Indent();
    m_printer->Print(Templates::FieldEnumTemplate());
    Indent();
    common::iterateMessageFields(m_descriptor,
                                 [&](const FieldDescriptor *, const PropertyMap &propertyMap) {
                                     m_printer->Print(propertyMap, Templates::FieldNumberTemplate());
                                 });
    Outdent();
    m_printer->Print(Templates::SemicolonBlockEnclosureTemplate());
    Outdent();
}
