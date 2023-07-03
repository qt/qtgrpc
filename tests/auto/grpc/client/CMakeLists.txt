# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_grpc_client LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)

    include("../shared/test_server/CMakeLists.txt")
endif()

if(NOT TARGET testserver)
    return()
endif()

qt6_add_protobuf(tst_grpc_client_qtgrpc_gen
    OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/qt_grpc_generated"
    PROTO_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/proto/testservice.proto
)

qt6_add_grpc(tst_grpc_client_qtgrpc_gen CLIENT
    PROTO_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/proto/testservice.proto
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qt_grpc_generated"
)

qt_autogen_tools_initial_setup(tst_grpc_client_qtgrpc_gen)
qt_internal_add_test(tst_grpc_client
    SOURCES
        tst_grpc_client.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared
    DEFINES
        TEST_GRPC_SERVER_PATH="$<TARGET_FILE:testserver>"
    LIBRARIES
        Qt::Test
        Qt::Core
        Qt::Grpc
        tst_grpc_client_qtgrpc_gen
)

qt_add_resources(tst_grpc_client
    "assets"
    PREFIX
        "/"
    BASE
        "../shared/"
    FILES
        "../shared/assets/testfile"
)

add_dependencies(tst_grpc_client testserver)
