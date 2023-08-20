# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(CMAKE_AUTOMOC TRUE)

qt6_add_library(tst_grpc_client_test_common STATIC
    ${CMAKE_CURRENT_LIST_DIR}/grpcclienttestbase.h
    ${CMAKE_CURRENT_LIST_DIR}/grpcclienttestbase.cpp
)

target_compile_definitions(tst_grpc_client_test_common PRIVATE
        TEST_GRPC_SERVER_PATH="$<TARGET_FILE:testserver>"
)

target_include_directories(tst_grpc_client_test_common
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/../../../shared"
)

target_link_libraries(tst_grpc_client_test_common PRIVATE
    tst_grpc_client_qtgrpc_gen
    Qt::Core
    Qt::Test
    Qt::Grpc
)
