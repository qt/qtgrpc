# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

include(.cmake.conf)
project(QtGrpc
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "Qt gRPC and Protobuf generator and bindings."
    HOMEPAGE_URL "https://qt.io"
    LANGUAGES CXX C
)

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Core)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/QtProtobufBuildInternals.cmake")

qt_build_repo()

