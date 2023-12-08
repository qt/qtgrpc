// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#include <google/protobuf/compiler/plugin.h>

#include "qgrpcgenerator.h"
#include "options.h"
#include "utils.h"

using namespace QtGrpc;

int main(int argc, char *argv[])
{
    char *optionsPtr = getenv("QT_GRPC_OPTIONS");
    if (optionsPtr != nullptr) {
        QT_PROTOBUF_DEBUG("QT_GRPC_OPTIONS: " << optionsPtr);
        qtprotoccommon::Options::setFromString(optionsPtr);
    }
    QGrpcGenerator generator;
    return ::google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
