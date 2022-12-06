# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# How to test:
# 1. Build the project
# 2. Check that the generated annotation.qpb.h file in the build directory contains all
#    comments from the proto/annotation.proto file
cmake_minimum_required(VERSION 3.18)

project(tst_manual_protobuf_annotation)
find_package(Qt6 COMPONENTS Protobuf)

qt_add_protobuf(tst_manual_protobuf_annotation
    PROTO_FILES
        proto/annotation.proto
    COPY_COMMENTS
)
