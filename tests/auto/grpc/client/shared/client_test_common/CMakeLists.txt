# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT TARGET grpc_testserver)
    return()
endif()

set(CMAKE_AUTOMOC TRUE)

qt6_add_library(tst_grpc_client_test_common STATIC
    ${CMAKE_CURRENT_LIST_DIR}/grpcclienttestbase.h
    ${CMAKE_CURRENT_LIST_DIR}/grpcclienttestbase.cpp
)

target_compile_definitions(tst_grpc_client_test_common PRIVATE
        TEST_GRPC_SERVER_PATH="$<TARGET_FILE:grpc_testserver>"
)

if(qt_internal_grpc_use_golang_test_server)
    target_compile_definitions(tst_grpc_client_test_common PRIVATE
            TEST_GRPC_SERVER_SUPPORTS_JSON
    )
endif()

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

get_target_property(grpc_testserver_bin_dir grpc_testserver BINARY_DIR)
set(ssl_cert "${grpc_testserver_bin_dir}/assets/cert.pem")

if(EXISTS "${ssl_cert}")
    set_source_files_properties(${ssl_cert}
        PROPERTIES QT_RESOURCE_ALIAS assets/cert.pem)

    qt_add_resources(tst_grpc_client_test_common
        "keys"
        PREFIX
            "/"
        FILES
            ${ssl_cert}
    )
endif()
qt_autogen_tools_initial_setup(tst_grpc_client_test_common)
