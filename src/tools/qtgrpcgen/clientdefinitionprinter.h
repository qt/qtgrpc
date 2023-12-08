// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#pragma once

#include "descriptorprinterbase.h"

namespace QtGrpc {

class ClientDefinitionPrinter :
        public qtprotoccommon::DescriptorPrinterBase<google::protobuf::ServiceDescriptor>
{
public:
    ClientDefinitionPrinter(const google::protobuf::ServiceDescriptor *service,
                            const std::shared_ptr<::google::protobuf::io::Printer> &printer);
    void run()
    {
        printOpenNamespace();
        printConstructor();
        printMethods();
        printCloseNamespace();
    }
    void printOpenNamespace();
    void printConstructor();
    void printMethods();
    void printCloseNamespace();
private:
    void printMethod(const ::google::protobuf::MethodDescriptor *method);
};

} // namespace QtGrpc
