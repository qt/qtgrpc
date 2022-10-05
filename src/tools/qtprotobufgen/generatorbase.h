// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef GENERATORBASE_H
#define GENERATORBASE_H

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <string>
#include <memory>
#include <functional>
#include <google/protobuf/descriptor.h>

namespace google::protobuf {
class FileDescriptor;
class Descriptor;
namespace compiler {
class GeneratorContext;
} // namespace compiler
} // namespace google::protobuf

namespace QtProtobuf::generator {
class GeneratorBase : public ::google::protobuf::compiler::CodeGenerator
{
    GeneratorBase(const GeneratorBase &) = delete;
    GeneratorBase &operator=(const GeneratorBase &) = delete;
    GeneratorBase(GeneratorBase &&) = delete;
    GeneratorBase &operator=(GeneratorBase &&) = delete;
public:
    GeneratorBase() = default;
    ~GeneratorBase() = default;
    bool GenerateAll(const std::vector<const ::google::protobuf::FileDescriptor *> &files,
                     const std::string &parameter,
                     ::google::protobuf::compiler::GeneratorContext *generatorContext,
                     std::string *error) const override;
    bool HasGenerateAll() const override { return true; }

    static void printDisclaimer(::google::protobuf::io::Printer *printer);

protected:
    static std::string generateBaseName(const ::google::protobuf::FileDescriptor *file,
                                        const std::string &name);
};
} // namespace QtProtobuf::generator

#endif // GENERATORBASE_H
