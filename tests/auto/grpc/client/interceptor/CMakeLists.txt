# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_grpc_client_interceptor LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)

    include("../shared/client_test_common/CMakeLists.txt")
endif()

qt_internal_add_test(tst_grpc_client_interceptor
    SOURCES
    tst_grpc_client_interceptor.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared
    LIBRARIES
        Qt::Test
        Qt::Core
        Qt::Grpc
        tst_grpc_client_test_common
)
