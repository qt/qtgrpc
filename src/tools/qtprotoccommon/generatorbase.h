// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef GENERATORBASE_H
#define GENERATORBASE_H

#include <google/protobuf/compiler/code_generator.h>

#include <string>
#include <functional>

namespace google::protobuf {
class FileDescriptor;
class Descriptor;
namespace compiler {
class GeneratorContext;
} // namespace compiler
namespace io {
class Printer;
} // namespace io
} // namespace google::protobuf

namespace qtprotoccommon {
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

    uint64_t GetSupportedFeatures() const override;

    static void printDisclaimer(::google::protobuf::io::Printer *printer);
    void OpenFileNamespaces(const ::google::protobuf::FileDescriptor *file,
                            google::protobuf::io::Printer *printer) const;
    void CloseFileNamespaces(const ::google::protobuf::FileDescriptor *file,
                             google::protobuf::io::Printer *printer) const;

protected:
    static std::string generateBaseName(const ::google::protobuf::FileDescriptor *file,
                                        const std::string &name);
};
} // namespace qtprotoccommon

#endif // GENERATORBASE_H
