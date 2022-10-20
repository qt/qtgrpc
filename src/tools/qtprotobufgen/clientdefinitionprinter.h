// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#pragma once

#include "descriptorprinterbase.h"

namespace QtProtobuf::generator {

/*!
 * \ingroup generator
 * \private
 * \brief The ClientDefinitionPrinter class
 */
class ClientDefinitionPrinter : public DescriptorPrinterBase<google::protobuf::ServiceDescriptor>
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
    void printMethods();
    void printConstructor();
    void printCloseNamespace();
};

} // namespace QtProtobuf::generator
