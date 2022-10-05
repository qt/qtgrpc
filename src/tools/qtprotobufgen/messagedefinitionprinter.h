// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>, Tatyana Borisova <tanusshhka@mail.ru>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef MESSAGEDEFINITIONPRINTER_H
#define MESSAGEDEFINITIONPRINTER_H

#include "descriptorprinterbase.h"

#include <stddef.h>

namespace QtProtobuf::generator {

/*!
 \ingroup generator
 \private
 \brief The MessageDefinitionPrinter class
 */

class MessageDefinitionPrinter final : public DescriptorPrinterBase<google::protobuf::Descriptor>
{
public:
    explicit MessageDefinitionPrinter(const google::protobuf::Descriptor *message,
                                      std::shared_ptr<::google::protobuf::io::Printer> printer);
    ~MessageDefinitionPrinter();

    void printClassDefinition();
    void printClassRegistration(::google::protobuf::io::Printer *printer);

private:
    void printRegisterBody();
    void printFieldsOrdering();
    void printUintData(const char *templateString);
    void printCharData();
    size_t charDataSize() const;

    void printConstructors();
    void printInitializationList();
    void printCopyFunctionality();
    void printMoveSemantic();
    void printComparisonOperators();
    void printGetters();
    void printDestructor();

    void printClassDefinitionPrivate();
};

} // namespace QtProtobuf::generator
#endif // MESSAGEDEFINITIONPRINTER_H
