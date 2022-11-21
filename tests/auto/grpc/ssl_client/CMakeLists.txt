# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT TARGET securetestserver)
    return()
endif()

qt6_add_protobuf(tst_grpc_ssl_client_qtgrpc_gen
    PROTO_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/proto/testservice.proto
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qt_grpc_generated"
)

qt6_add_grpc(tst_grpc_ssl_client_qtgrpc_gen CLIENT
    PROTO_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/proto/testservice.proto
    OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/qt_grpc_generated"
)

qt_autogen_tools_initial_setup(tst_grpc_ssl_client_qtgrpc_gen)

qt_internal_add_test(tst_grpc_ssl_client
    SOURCES
        tst_grpc_ssl_client.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared
    LIBRARIES
        Qt::Test
        Qt::Core
        Qt::Grpc
        Qt::Network
        tst_grpc_ssl_client_qtgrpc_gen
)

add_dependencies(tst_grpc_ssl_client securetestserver)
