// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "generatorcommon.h"
#include "options.h"
#include "utils.h"

#include <cassert>
#include <algorithm>

using namespace ::google::protobuf;
using namespace ::google::protobuf::io;
using namespace ::QtProtobuf::generator;

std::vector<std::string> common::getNamespaces(const std::string &fullTypeName)
{
    std::vector<std::string> namespacesList;
    std::string extraNamespace = Options::instance().extraNamespace();
    if (!extraNamespace.empty() && extraNamespace != "QT_NAMESPACE")
        namespacesList.push_back(extraNamespace);
    for (auto &ns : utils::split(fullTypeName, "."))
        namespacesList.push_back(std::move(ns));
    if (!namespacesList.empty())
        namespacesList.pop_back(); // Remove name
    return namespacesList;
}

std::vector<std::string> common::getNestedNamespaces(const Descriptor *type)
{
    auto namespaces = getNamespaces(type);
    for (size_t i = utils::count(type->file()->package(), '.') + 1; i < namespaces.size(); ++i)
        namespaces[i] += Templates::QtProtobufNestedNamespace();
    return namespaces;
}

std::string common::getNamespacesString(const std::vector<std::string> &namespacesList,
                                        std::string_view separator)
{
    return utils::join(namespacesList, separator);
}

std::string common::getScopeNamespacesString(std::string original, const std::string &scope)
{
    if (scope.empty())
        return original;

    if (original == scope)
        return "";

    if (utils::startsWith(original, scope + "::"))
        return original.substr(scope.size() + 2);
    return original;
}

std::map<std::string, std::string> common::getNestedScopeNamespace(const std::string &className)
{
    return { { "scope_namespaces", className + Templates::QtProtobufNestedNamespace() } };
}

TypeMap common::produceQtTypeMap(const Descriptor *type, const Descriptor *scope)
{
    std::vector<std::string> namespaceList = getNamespaces(type);
    std::string namespaces = getNamespacesString(namespaceList, "::");
    std::string scopeNamespaces =
            getScopeNamespacesString(namespaces, getNamespacesString(getNamespaces(scope), "::"));
    std::string qmlPackage = getNamespacesString(namespaceList, ".");

    std::string name = type->name();
    std::string fullName = name;
    std::string scopeName = name;

    std::string listName = std::string("QList<") + Templates::RepeatedSuffix() + ">";
    std::string fullListName = listName;
    std::string scopeListName = listName;

    return { { "type", name },
             { "full_type", fullName },
             { "scope_type", scopeName },
             { "list_type", listName },
             { "full_list_type", fullListName },
             { "scope_list_type", scopeListName },
             { "scope_namespaces", scopeNamespaces },
             { "qml_package", qmlPackage },
             { "property_type", fullName },
             { "property_list_type", fullListName },
             { "getter_type", scopeName },
             { "setter_type", scopeName } };
}

TypeMap common::produceMessageTypeMap(const Descriptor *type, const Descriptor *scope)
{
    std::vector<std::string> namespaceList = getNamespaces(type);

    std::vector<std::string> nestedNamespaceList =
            isNested(type) ? getNestedNamespaces(type) : namespaceList;

    std::string namespaces = getNamespacesString(namespaceList, "::");
    std::string scopeNamespaces =
            getScopeNamespacesString(getNamespacesString(nestedNamespaceList, "::"),
                                     getNamespacesString(getNamespaces(scope), "::"));
    std::string qmlPackage = getNamespacesString(namespaceList, ".");
    if (qmlPackage.empty())
        qmlPackage = "QtProtobuf";

    std::string name = utils::capitalizeAsciiName(type->name());
    std::string fullName = namespaces.empty() ? name : (namespaces + "::" + name);
    std::string scopeName = scopeNamespaces.empty() ? name : (scopeNamespaces + "::" + name);

    std::string listName = name + Templates::RepeatedSuffix();
    std::string fullListName = namespaces.empty() ? listName : (namespaces + "::" + listName);
    std::string scopeListName =
            scopeNamespaces.empty() ? listName : (scopeNamespaces + "::" + listName);

    std::string exportMacro = Options::instance().exportMacro();
    exportMacro = common::buildExportMacro(exportMacro);

    return {
        { "classname", name },
        { "type", name },
        { "full_type", fullName },
        { "scope_type", scopeName },
        { "list_type", listName },
        { "full_list_type", fullListName },
        { "scope_list_type", scopeListName },
        { "scope_namespaces", scopeNamespaces },
        { "qml_package", qmlPackage },
        { "property_type", fullName },
        { "property_list_type", fullListName },
        { "getter_type", scopeName },
        { "setter_type", scopeName },
        { "export_macro", exportMacro },
    };
}

TypeMap common::produceEnumTypeMap(const EnumDescriptor *type, const Descriptor *scope)
{
    EnumVisibility visibility = enumVisibility(type, scope);
    std::vector<std::string> namespaceList = getNamespaces(type);

    std::string name = utils::capitalizeAsciiName(type->name());
    // qml package should consist only from proto spackage
    std::string qmlPackage = getNamespacesString(namespaceList, ".");
    if (qmlPackage.empty())
        qmlPackage = "QtProtobuf";
    // Not used:
    std::string enumGadget = scope != nullptr ? utils::capitalizeAsciiName(scope->name()) : "";
    if (visibility == GLOBAL_ENUM) {
        enumGadget = name + Templates::EnumClassSuffix();
        namespaceList.push_back(enumGadget); // Global enums are stored in helper Gadget
    }

    std::string namespaces = getNamespacesString(namespaceList, "::");
    std::string scopeNamespaces =
            getScopeNamespacesString(namespaces, getNamespacesString(getNamespaces(scope), "::"));

    std::string fullName = namespaces.empty() ? name : (namespaces + "::" + name);
    std::string scopeName = scopeNamespaces.empty() ? name : (scopeNamespaces + "::" + name);

    std::string listName = name + Templates::RepeatedSuffix();
    std::string fullListName = namespaces.empty() ? listName : (namespaces + "::" + listName);
    std::string scopeListName =
            scopeNamespaces.empty() ? listName : (scopeNamespaces + "::" + listName);

    // Note: For local enum classes it's impossible to use class name space in Q_PROPERTY
    // declaration. So please avoid addition of namespaces in line bellow
    std::string propertyType = visibility == LOCAL_ENUM ? name : fullName;
    std::string exportMacro = Options::instance().exportMacro();
    exportMacro = common::buildExportMacro(exportMacro);

    return {
        { "classname", enumGadget },
        { "type", name },
        { "full_type", fullName },
        { "scope_type", scopeName },
        { "list_type", listName },
        { "full_list_type", fullListName },
        { "scope_list_type", scopeListName },
        { "scope_namespaces", scopeNamespaces },
        { "qml_package", qmlPackage },
        { "property_type", propertyType },
        { "property_list_type", fullListName },
        { "getter_type", scopeName },
        { "setter_type", scopeName },
        { "enum_gadget", enumGadget },
        { "export_macro", exportMacro },
    };
}

TypeMap common::produceSimpleTypeMap(FieldDescriptor::Type type)
{
    std::string namespaces;
    if (type != FieldDescriptor::TYPE_STRING && type != FieldDescriptor::TYPE_BYTES
        && type != FieldDescriptor::TYPE_BOOL && type != FieldDescriptor::TYPE_FLOAT
        && type != FieldDescriptor::TYPE_DOUBLE) {
        namespaces = Templates::QtProtobufNamespace();
    }

    std::string name;
    std::string qmlPackage = Templates::QtProtobufNamespace();

    auto it = Templates::TypeReflection().find(type);
    if (it != std::end(Templates::TypeReflection()))
        name = it->second;
    else
        assert(name.empty());

    std::string fullName = namespaces.empty() ? name : (namespaces + "::" + name);
    std::string listName = name + "List";
    using namespace std::string_literals;
    std::string fullListName = listName;
    if (type != FieldDescriptor::TYPE_STRING && type != FieldDescriptor::TYPE_BYTES)
        fullListName = Templates::QtProtobufNamespace() + "::"s + listName;
    std::string scopeListName = fullListName;

    std::string qmlAliasType;
    switch (type) {
    case FieldDescriptor::TYPE_INT32:
    case FieldDescriptor::TYPE_SFIXED32:
        qmlAliasType = "int";
        break;
    case FieldDescriptor::TYPE_FIXED32:
        qmlAliasType = "unsigned int";
        break;
    default:
        qmlAliasType = fullName;
        break;
    }

    return { { "type", name },
             { "full_type", fullName },
             { "scope_type", fullName },
             { "list_type", listName },
             { "full_list_type", fullListName },
             { "scope_list_type", scopeListName },
             { "scope_namespaces", namespaces },
             { "qml_package", qmlPackage },
             { "property_type", fullName },
             { "qml_alias_type", qmlAliasType },
             { "property_list_type", fullListName },
             { "getter_type", fullName },
             { "setter_type", fullName } };
}

bool common::isQtType(const FieldDescriptor *field)
{
    auto namespaces = getNamespaces(field->message_type());
    return namespaces.size() == 1 && namespaces.front() == "QtProtobuf"
            && field->file()->package() != "QtProtobuf"; // Used for qttypes library to avoid types
                                                         // conversion inside library
}

bool common::isPureMessage(const FieldDescriptor *field)
{
    return field->type() == FieldDescriptor::TYPE_MESSAGE && !field->is_map()
            && !field->is_repeated() && !common::isQtType(field);
}

void common::iterateMessageFields(const Descriptor *message, const IterateMessageLogic &callback)
{
    int numFields = message->field_count();
    for (int i = 0; i < numFields; ++i) {
        const FieldDescriptor *field = message->field(i);
        auto propertyMap = common::producePropertyMap(field, message);
        callback(field, propertyMap);
    }
}

TypeMap common::produceTypeMap(const FieldDescriptor *field, const Descriptor *scope)
{
    assert(field != nullptr);

    switch (field->type()) {
    case FieldDescriptor::TYPE_MESSAGE:
        return isQtType(field) ? produceQtTypeMap(field->message_type(), nullptr)
                               : produceMessageTypeMap(field->message_type(), scope);
    case FieldDescriptor::TYPE_ENUM:
        return produceEnumTypeMap(field->enum_type(), scope);
    default:
        break;
    }

    return produceSimpleTypeMap(field->type());
}

PropertyMap common::producePropertyMap(const FieldDescriptor *field, const Descriptor *scope)
{
    assert(field != nullptr);

    PropertyMap propertyMap = produceTypeMap(field, scope);

    std::string scriptable = "true";
    if (!field->is_map() && !field->is_repeated()
        && (field->type() == FieldDescriptor::TYPE_INT64
            || field->type() == FieldDescriptor::TYPE_SINT64
            || field->type() == FieldDescriptor::TYPE_FIXED64
            || field->type() == FieldDescriptor::TYPE_SFIXED64)) {
        scriptable = "false";
    }

    std::string propertyName = qualifiedName(utils::deCapitalizeAsciiName(field->camelcase_name()));
    std::string propertyNameCap = utils::capitalizeAsciiName(propertyName);

    propertyMap["property_name"] = propertyName;
    propertyMap["property_name_cap"] = propertyNameCap;
    propertyMap["scriptable"] = scriptable;

    auto scopeTypeMap = produceMessageTypeMap(scope, nullptr);
    propertyMap["key_type"] = "";
    propertyMap["value_type"] = "";
    propertyMap["classname"] = scope != nullptr ? scopeTypeMap["classname"] : "";
    propertyMap["number"] = std::to_string(field->number());

    if (field->is_map()) {
        const Descriptor *type = field->message_type();
        auto keyMap = common::producePropertyMap(type->field(0), scope);
        auto valueMap = common::producePropertyMap(type->field(1), scope);
        propertyMap["key_type"] = keyMap["scope_type"];
        propertyMap["value_type"] = valueMap["scope_type"];
        propertyMap["value_list_type"] = valueMap["scope_list_type"];
    } else if (field->is_repeated()) {
        propertyMap["getter_type"] = propertyMap["scope_list_type"];
        propertyMap["setter_type"] = propertyMap["scope_list_type"];
    }

    return propertyMap;
}

std::string common::qualifiedName(const std::string &name)
{
    std::string fieldName(name);
    const std::vector<std::string> &searchExceptions = Templates::ListOfQmlExceptions();

    if (utils::contains(searchExceptions, fieldName))
        return fieldName.append(Templates::ProtoSuffix());
    return fieldName;
}

bool common::isLocalEnum(const EnumDescriptor *type, const Descriptor *scope)
{
    if (scope == nullptr) {
        return false;
    }

    assert(type != nullptr);
    int numEnumTypes = scope->enum_type_count();
    for (int i = 0; i < numEnumTypes; ++i) {
        const EnumDescriptor *scopeEnum = scope->enum_type(i);
        if (scopeEnum && scopeEnum->full_name() == type->full_name()) {
            return true;
        }
    }
    return false;
}

common::EnumVisibility common::enumVisibility(const EnumDescriptor *type, const Descriptor *scope)
{
    assert(type != nullptr);

    if (isLocalEnum(type, scope)) {
        return LOCAL_ENUM;
    }

    const FileDescriptor *typeFile = type->file();

    int numMessageTypes = typeFile->message_type_count();
    for (int i = 0; i < numMessageTypes; ++i) {
        const Descriptor *msg = typeFile->message_type(i);
        int numEnumTypes = msg->enum_type_count();
        for (int j = 0; j < numEnumTypes; ++j) {
            if (type->full_name() == msg->enum_type(j)->full_name())
                return NEIGHBOR_ENUM;
        }
    }

    return GLOBAL_ENUM;
}

bool common::hasQmlAlias(const FieldDescriptor *field)
{
    return !field->is_map() && !field->is_repeated()
            && (field->type() == FieldDescriptor::TYPE_INT32
                || field->type() == FieldDescriptor::TYPE_SFIXED32
                || field->type() == FieldDescriptor::TYPE_FIXED32)
            && Options::instance().hasQml();
}

void common::iterateMessages(const FileDescriptor *file,
                             const std::function<void(const Descriptor *)> &callback)
{
    int numMessageTypes = file->message_type_count();
    for (int i = 0; i < numMessageTypes; ++i)
        callback(file->message_type(i));
}

void common::iterateNestedMessages(const Descriptor *message,
                                   const std::function<void(const Descriptor *)> &callback)
{
    int numNestedTypes = message->nested_type_count();
    for (int i = 0; i < numNestedTypes; ++i) {
        const Descriptor *nestedMessage = message->nested_type(i);
        if (message->field_count() <= 0) {
            callback(nestedMessage);
            continue;
        }
        int numFields = message->field_count();
        for (int j = 0; j < numFields; ++j) {
            const FieldDescriptor *field = message->field(j);
            // Probably there is more correct way to detect map in
            // nested messages.
            // TODO: Have idea to make maps nested classes instead of typedefs.
            if (!field->is_map() && field->message_type() == nestedMessage) {
                callback(nestedMessage);
                break;
            }
        }
    }
}

bool common::hasNestedMessages(const Descriptor *message)
{
    int numNestedTypes = message->nested_type_count();
    int numFields = message->field_count();
    if (numNestedTypes > 0 && numFields <= 0)
        return true;

    for (int i = 0; i < numNestedTypes; ++i) {
        const Descriptor *nestedMessage = message->nested_type(i);
        for (int j = 0; j < numFields; ++j) {
            const FieldDescriptor *field = message->field(j);
            // Probably there is more correct way to detect map in
            // nested messages.
            // TODO: Have idea to make maps nested classes instead of typedefs.
            if (!field->is_map() && field->message_type() == nestedMessage)
                return true;
        }
    }

    return false;
}

bool common::isNested(const Descriptor *message)
{
    if (message->containing_type() == nullptr)
        return false;

    const Descriptor *containingType = message->containing_type();

    int numFields = containingType->field_count();
    for (int i = 0; i < numFields; ++i) {
        const FieldDescriptor *field = containingType->field(i);
        if (field->message_type() == message) {
            return !field->is_map();
        }
    }

    return true;
}

const Descriptor *common::findHighestMessage(const Descriptor *message)
{
    const Descriptor *highestMessage = message;
    while (highestMessage->containing_type() != nullptr)
        highestMessage = highestMessage->containing_type();
    return highestMessage;
}
