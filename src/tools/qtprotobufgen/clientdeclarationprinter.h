// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#pragma once

#include "descriptorprinterbase.h"
#include <google/protobuf/io/printer.h>

#include <memory>

namespace google {
namespace protobuf {
class ServiceDescriptor;
class Message;
} // namespace protobuf
} // namespace google

namespace QtProtobuf::generator {
class ClientDeclarationPrinter : public DescriptorPrinterBase<::google::protobuf::ServiceDescriptor>
{
public:
    ClientDeclarationPrinter(const ::google::protobuf::ServiceDescriptor *service,
                             const std::shared_ptr<::google::protobuf::io::Printer> &printer);
    ~ClientDeclarationPrinter() = default;

    void run()
    {
        printOpenNamespace();
        printClientClass();
        printPublicBlock();
        printConstructor();
        printClientMethodsDeclaration();
        encloseClass();
        printCloseNamespace();
    }

private:
    void printOpenNamespace();
    void printClientClass();
    void printConstructor();
    void printClientMethodsDeclaration();
    void printCloseNamespace();
};

} // namespace QtProtobuf::generator
