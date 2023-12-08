// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#include <google/protobuf/compiler/plugin.h>

#include "qprotobufgenerator.h"
#include "options.h"
#include "utils.h"

using namespace ::QtProtobuf;
int main(int argc, char *argv[])
{
    char *optionsPtr = getenv("QT_PROTOBUF_OPTIONS");
    if (optionsPtr != nullptr) {
        QT_PROTOBUF_DEBUG("QT_PROTOBUF_OPTIONS: " << optionsPtr);
        qtprotoccommon::Options::setFromString(optionsPtr);
    }
    QProtobufGenerator generator;
    return ::google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
