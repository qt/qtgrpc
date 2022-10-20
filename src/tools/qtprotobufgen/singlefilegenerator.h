// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef SINGLEFILEGENERATOR_H
#define SINGLEFILEGENERATOR_H

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <string>
#include <memory>
#include <functional>
#include "generatorbase.h"

namespace google::protobuf {
class FileDescriptor;
class Descriptor;
namespace compiler {
class GeneratorContext;
} // namespace compiler
} // namespace google::protobuf

namespace QtProtobuf::generator {
class SingleFileGenerator : public GeneratorBase
{
public:
    SingleFileGenerator();
    ~SingleFileGenerator();
    bool Generate(const ::google::protobuf::FileDescriptor *file,
                          const std::string &parameter,
                          ::google::protobuf::compiler::GeneratorContext *generatorContext,
                          std::string *error) const override;
private:
    bool GenerateMessages(const ::google::protobuf::FileDescriptor *file,
                          ::google::protobuf::compiler::GeneratorContext *generatorContext) const;
    bool GenerateServices(const ::google::protobuf::FileDescriptor *file,
                          ::google::protobuf::compiler::GeneratorContext *generatorContext) const;
};
} // namespace QtProtobuf

#endif // SINGLEFILEGENERATOR_H
