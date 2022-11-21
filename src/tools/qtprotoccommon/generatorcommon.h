// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef GENERATORCOMMON_H
#define GENERATORCOMMON_H

#include <google/protobuf/io/printer.h>

#include <map>
#include <string>
#include <string_view>
#include <functional>

#include <google/protobuf/descriptor.h>

namespace qtprotoccommon {

using TypeMap = std::map<std::string, std::string>;
using PropertyMap = std::map<std::string, std::string>;
using MethodMap = std::map<std::string, std::string>;

struct common {
    using Descriptor = ::google::protobuf::Descriptor;
    using FieldDescriptor = ::google::protobuf::FieldDescriptor;
    using EnumDescriptor = ::google::protobuf::EnumDescriptor;
    using FileDescriptor = ::google::protobuf::FileDescriptor;
    using MethodDescriptor = ::google::protobuf::MethodDescriptor;
    using ServiceDescriptor = ::google::protobuf::ServiceDescriptor;

    enum EnumVisibility {
        GLOBAL_ENUM,
        LOCAL_ENUM,
        NEIGHBOR_ENUM
    };

    static std::string buildExportMacro(std::string identifier)
    {
        if (identifier.empty())
            return identifier;
        return "QPB_" + identifier + "_EXPORT";
    }

    static std::string getFullNamespace(std::string_view fullDescriptorName,
                                        std::string_view separator);
    template<typename T>
    static std::string getFullNamespace(const T *type, std::string_view separator)
    {
        if (type == nullptr)
            return {};
        return getFullNamespace(type->full_name(), separator);
    }

    static std::string getNestedNamespace(const Descriptor *type, std::string_view separator);
    static std::string getScopeNamespace(std::string_view original, std::string_view scope);
    static std::map<std::string, std::string> getNestedScopeNamespace(const std::string &className);
    static TypeMap produceQtTypeMap(const Descriptor *type, const Descriptor *scope);
    static TypeMap produceMessageTypeMap(const Descriptor *type, const Descriptor *scope);
    static TypeMap produceEnumTypeMap(const EnumDescriptor *type, const Descriptor *scope);
    static TypeMap produceSimpleTypeMap(FieldDescriptor::Type type);
    static TypeMap produceTypeMap(const FieldDescriptor *field, const Descriptor *scope);
    static PropertyMap producePropertyMap(const FieldDescriptor *field, const Descriptor *scope);
    static MethodMap produceMethodMap(const MethodDescriptor *method, const std::string &scope);
    static TypeMap produceServiceTypeMap(const ServiceDescriptor *service, const Descriptor *scope);
    static TypeMap produceClientTypeMap(const ServiceDescriptor *service, const Descriptor *scope);
    static std::string qualifiedName(const std::string &name);
    static bool isLocalEnum(const EnumDescriptor *type, const google::protobuf::Descriptor *scope);
    static EnumVisibility enumVisibility(const EnumDescriptor *type, const Descriptor *scope);
    static bool hasQmlAlias(const FieldDescriptor *field);
    static bool isQtType(const FieldDescriptor *field);
    static bool isPureMessage(const FieldDescriptor *field);

    using IterateMessageLogic = std::function<void(const FieldDescriptor *, PropertyMap &)>;
    static void iterateMessageFields(const Descriptor *message, const IterateMessageLogic &callback);

    static void iterateMessages(const FileDescriptor *file,
                                const std::function<void(const Descriptor *)> &callback);
    static void iterateNestedMessages(const Descriptor *message,
                                      const std::function<void(const Descriptor *)> &callback);

    static bool hasNestedMessages(const Descriptor *message);

    static bool isNested(const Descriptor *message);
    static const Descriptor *findHighestMessage(const Descriptor *message);

    static std::string collectFieldFlags(const google::protobuf::FieldDescriptor *field);
};
} // namespace qtprotoccommon

#endif // GENERATORCOMMON_H
