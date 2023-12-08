// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>, Tatyana Borisova <tanusshhka@mail.ru>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QTPROTOCCOMMON_TEMPLATES_H
#define QTPROTOCCOMMON_TEMPLATES_H

#include <unordered_map>
#include <string>
#include <google/protobuf/descriptor.h>

namespace qtprotoccommon {

class CommonTemplates
{
public:
    static const std::vector<std::string> &ListOfQmlExceptions();
    static const char *ProtoSuffix();
    static const char *DefaultProtobufIncludesTemplate();
    static const char *QmlProtobufIncludesTemplate();
    static const char *PreambleTemplate();
    static const char *FooterTemplate();
    static const char *DisclaimerTemplate();
    static const char *InternalIncludeTemplate();
    static const char *ExternalIncludeTemplate();
    static const char *GlobalEnumIncludeTemplate();
    static const char *MetaTypeRegistrationDeclaration();
    static const char *EnumRegistrationDeclaration();
    static const char *MetaTypeRegistrationMessageDefinition();
    static const char *MetaTypeRegistrationGlobalEnumDefinition();
    static const char *MetaTypeRegistrationGlobalEnumTemplate();
    static const char *UsingMessageTemplate();
    static const char *UsingMapTemplate();
    static const char *UsingMapMessageTemplate();
    static const char *UsingNestedMessageTemplate();
    static const char *UsingRepeatedEnumTemplate();
    static const char *NamespaceTemplate();
    static const char *NamespaceClosingTemplate();
    static const char *EnumDeclarationTemplate();
    static const char *ClassMessageForwardDeclarationTemplate();
    static const char *ClassMessageBeginDeclarationTemplate();
    static const char *EmptyConstructorTemplate();
    static const char *UseNamespace();

    static const char *PropertyTemplate();
    static const char *PropertyRepeatedTemplate();
    static const char *PropertyRepeatedMessageTemplate();
    static const char *PropertyNonScriptableTemplate();
    static const char *PropertyNonScriptableAliasTemplate();
    static const char *PropertyMessageTemplate();
    static const char *PropertyQmlListTemplate();

    static const char *ConstructorMessageDeclarationTemplate();
    static const char *DestructorMessageDeclarationTemplate();

    static const char *ConstructorMessageDefinitionTemplate();

    static const char *MemberTemplate();
    static const char *MemberRepeatedTemplate();
    static const char *MemberMessageTemplate();
    static const char *PublicBlockTemplate();
    static const char *PrivateBlockTemplate();
    static const char *EnumDefinitionTemplate();
    static const char *EnumFieldTemplate();
    static const char *CopyConstructorDeclarationTemplate();
    static const char *MoveConstructorDeclarationTemplate();
    static const char *CopyConstructorDefinitionTemplate();
    static const char *MoveConstructorDefinitionTemplate();
    static const char *DeletedCopyConstructorTemplate();
    static const char *DeletedMoveConstructorTemplate();
    static const char *CopyMemberTemplate();
    static const char *CopyMemberMessageTemplate();
    static const char *AssignMemberMessageTemplate();
    static const char *MoveMemberMessageTemplate();
    static const char *MoveAssignMemberMessageTemplate();
    static const char *MoveAssignMemberComplexTemplate();
    static const char *MoveConstructorMemberComplexTemplate();
    static const char *MoveMemberTemplate();
    static const char *MoveMemberEnumTemplate();
    static const char *AssignmentOperatorDeclarationTemplate();
    static const char *AssignmentOperatorDefinitionTemplate();
    static const char *AssignmentOperatorReturnTemplate();
    static const char *MoveAssignmentOperatorDeclarationTemplate();
    static const char *MoveAssignmentOperatorDefinitionTemplate();
    static const char *EqualOperatorDeclarationTemplate();
    static const char *EqualOperatorDefinitionTemplate();
    static const char *EqualOperatorMemberTemplate();
    static const char *EqualOperatorMemberMessageTemplate();
    static const char *EqualOperatorMemberRepeatedTemplate();
    static const char *NotEqualOperatorDeclarationTemplate();
    static const char *NotEqualOperatorDefinitionTemplate();
    static const char *PrivateGetterMessageDeclarationTemplate();
    static const char *PrivateGetterMessageDefinitionTemplate();
    static const char *GetterMessageDeclarationTemplate();
    static const char *GetterMessageDefinitionTemplate();
    static const char *GetterTemplate();
    static const char *GetterNonScriptableTemplate();
    static const char *GetterComplexTemplate();
    static const char *GetterQmlListDeclarationTemplate();
    static const char *GetterQmlListDefinitionTemplate();
    static const char *PrivateSetterMessageDeclarationTemplate();
    static const char *PrivateSetterMessageDefinitionTemplate();
    static const char *SetterMessageDeclarationTemplate();
    static const char *SetterMessageDefinitionTemplate();
    static const char *SetterComplexDeclarationTemplate();
    static const char *SetterComplexDefinitionTemplate();
    static const char *SetterTemplate();
    static const char *SetterNonScriptableTemplate();
    static const char *SignalsBlockTemplate();
    static const char *SignalTemplate();
    static const char *MetaDataUintDataEntryTemplate();
    static const char *MetaDataCharDataEntryTemplate();
    static const char *JsonNameOffsetsUintDataTemplate();
    static const char *FieldNumbersUintDataTemplate();
    static const char *QtPropertyIndicesUintDataTemplate();
    static const char *FieldFlagsUintDataTemplate();
    static const char *PropertyOrderingDataOpeningTemplate();
    static const char *PropertyOrderingDataClosingTemplate();
    static const char *PropertyOrderingDefinitionTemplate();
    static const char *SimpleBlockEnclosureTemplate();
    static const char *SemicolonBlockEnclosureTemplate();
    static const char *InitializerMemberTemplate();
    static const char *InitializerMemberMessageTemplate();
    static const char *CopyInitializerMemberTemplate();
    static const char *EmptyBracesTemplate();
    static const char *DeclareMetaTypeTemplate();
    static const char *DeclareMetaTypeMapTemplate();
    static const char *DeclareMetaTypeQmlListTemplate();
    static const char *MetaTypeRegistrationLocalEnumTemplate();
    static const char *MetaTypeRegistrationMapTemplate();
    static const char *MetaTypeRegistrationQmlListTemplate();
    static const char *QEnumTemplate();
    static const char *QEnumNSTemplate();
    static const char *RegisterEnumSerializersTemplate();
    static const char *RegistrarTemplate();
    static const char *RegistrarEnumTemplate();
    static const char *QmlRegisterTypeTemplate();
    static const char *QmlRegisterTypeEnumTemplate();

    static const char *RepeatedSuffix();
    static const char *ProtoFileSuffix();
    static const char *EnumClassSuffix();

    static const std::unordered_map<::google::protobuf::FieldDescriptor::Type, std::string> &
    TypeReflection();

    static const char *QtProtobufNamespace();
    static const char *QtProtobufNestedNamespace();

    static const char *FieldEnumTemplate();
    static const char *FieldNumberTemplate();
    static const char *QtProtobufFieldEnum();

    static const char *ExportMacroTemplate();
    static const char *QObjectConstructorMessageDeclarationTemplate();
};

} // namespace qtprotoccommon
#endif // QTPROTOCCOMMON_TEMPLATES_H
