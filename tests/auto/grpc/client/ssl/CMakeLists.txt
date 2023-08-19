# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT TARGET securetestserver)
    return()
endif()

qt_internal_add_test(tst_grpc_ssl_client
    SOURCES
        tst_grpc_ssl_client.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared
    LIBRARIES
        Qt::Test
        Qt::Core
        Qt::Grpc
        Qt::Network
        tst_grpc_client_qtgrpc_gen
)

add_dependencies(tst_grpc_ssl_client securetestserver)
