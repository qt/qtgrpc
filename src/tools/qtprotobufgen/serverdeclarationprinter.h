// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef SERVERDECLARATIONPRINTER_H
#define SERVERDECLARATIONPRINTER_H

#include "descriptorprinterbase.h"
#include <google/protobuf/io/printer.h>
#include <string>

namespace google {
namespace protobuf {
class ServiceDescriptor;
class Message;
}
}

namespace QtProtobuf {
namespace generator {

/*!
 \ingroup generator
 \private
 \brief The ServerDeclarationPrinter class
 */
class ServerDeclarationPrinter : public DescriptorPrinterBase<::google::protobuf::ServiceDescriptor>
{
public:
    ServerDeclarationPrinter(const google::protobuf::ServiceDescriptor *service,
                             const std::shared_ptr<google::protobuf::io::Printer> &printer);
    virtual ~ServerDeclarationPrinter() = default;

    void run()
    {
        printOpenNamespace();
        printClassName();
        printPublicBlock();
        printMethodsDeclaration(Templates::ServerMethodDeclarationTemplate());
        encloseClass();
        printCloseNamespace();
    }

private:
    void printOpenNamespace();
    void printClassName();
    void printMethodsDeclaration(const char *methodTemplate, const char *methodAsyncTemplate = "",
                                 const char *methodAsync2Template = "");
    void printCloseNamespace();
};

} // namespace generator
} // namespace QtProtobuf
#endif // SERVERDECLARATIONPRINTER_H
