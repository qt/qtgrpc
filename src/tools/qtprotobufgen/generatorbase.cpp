// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "generatorbase.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/stubs/logging.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include "utils.h"
#include "templates.h"
#include "options.h"
#include "generatorcommon.h"

#include <string>
#include <vector>
#include <cassert>

using namespace ::google::protobuf;
using namespace ::google::protobuf::io;
using namespace ::google::protobuf::compiler;
using namespace ::QtProtobuf::generator;

bool GeneratorBase::GenerateAll(const std::vector<const FileDescriptor *> &files,
                                const std::string &parameter, GeneratorContext *generatorContext,
                                std::string *error) const
{
    Options::setFromString(parameter);
    return CodeGenerator::GenerateAll(files, parameter, generatorContext, error);
}

std::string GeneratorBase::generateBaseName(const FileDescriptor *file, const std::string &name)
{
    std::string outFileBasename;
    if (Options::instance().isFolder()) {
        outFileBasename = file->package();
        outFileBasename = utils::replace(outFileBasename, ".", "/");
        outFileBasename += '/';
    }
    outFileBasename += name;

    return outFileBasename;
}

void GeneratorBase::printDisclaimer(Printer *printer)
{
    printer->Print(Templates::DisclaimerTemplate());
}
