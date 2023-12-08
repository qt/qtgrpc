// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef GRPCTEMPLATES_H
#define GRPCTEMPLATES_H

namespace QtGrpc {

class GrpcTemplates
{
public:
    // gRPC
    static const char *ChildClassDeclarationTemplate();

    static const char *GrpcClientFileSuffix();
    static const char *GrpcServiceFileSuffix();

    static const char *ClientConstructorDefinitionTemplate();

    static const char *ClientMethodDeclarationSyncTemplate();
    static const char *ClientMethodDeclarationAsyncTemplate();
    static const char *ClientMethodDeclarationAsync2Template();
    static const char *ClientMethodDeclarationQmlTemplate();
    static const char *ClientMethodDeclarationQml2Template();

    static const char *ServerMethodDeclarationTemplate();

    static const char *ClientMethodDefinitionSyncTemplate();
    static const char *ClientMethodDefinitionAsyncTemplate();
    static const char *ClientMethodDefinitionAsync2Template();
    static const char *ClientMethodDefinitionQmlTemplate();
    static const char *ClientMethodDefinitionQml2Template();

    static const char *ClientMethodServerStreamDeclarationTemplate();
    static const char *ClientMethodServerStream2DeclarationTemplate();
    static const char *ClientMethodServerStreamQmlDeclarationTemplate();

    static const char *ClientMethodServerStreamDefinitionTemplate();
    static const char *ClientMethodServerStream2DefinitionTemplate();
    static const char *ClientMethodServerStreamQmlDefinitionTemplate();
};

} // namespace QtGrpc

#endif // GRPCTEMPLATES_H
