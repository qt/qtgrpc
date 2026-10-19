# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Projects that build their dependencies in-tree may provide Threads::Threads as an ALIAS.
add_library(my_threads INTERFACE)
add_library(Threads::Threads ALIAS my_threads)

find_package(Qt6 REQUIRED COMPONENTS Protobuf ProtobufTools)

if(NOT TARGET WrapProtoc::WrapProtoc)
    message(FATAL_ERROR "WrapProtoc::WrapProtoc was not found.")
endif()
