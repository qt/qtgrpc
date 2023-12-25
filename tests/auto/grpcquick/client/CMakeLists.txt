# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_grpc_client_unarycall_qml LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

qt_internal_add_test(tst_grpc_client_unarycall_qml
    QMLTEST
    SOURCES
        tst_client_qml.cpp
    LIBRARIES
        Qt::Qml
        Qt::Grpc
)

qt_add_grpc(tst_grpc_client_unarycall_qml_gen CLIENT
    PROTO_FILES
        ../../shared/data/proto/testservice.proto
    QML
    OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/qt_grpc_generated_qml"
)

qt_add_protobuf(tst_grpc_client_unarycall_qml_gen
    PROTO_FILES
        ../../shared/data/proto/testservice.proto
    QML
    QML_URI qtgrpc.tests
    OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/qt_grpc_generated_qml"
)

qt_policy(SET QTP0001 NEW)

qt_add_qml_module(tst_grpc_client_unarycall_qml
    URI QmlTestUri
    VERSION 1.0
    QML_FILES
        qml/tst_client.qml
)

qt_autogen_tools_initial_setup(tst_grpc_client_unarycall_qml)
qt_autogen_tools_initial_setup(tst_grpc_client_unarycall_qml_gen)
qt_autogen_tools_initial_setup(tst_grpc_client_unarycall_qml_genplugin)

if(QT_BUILD_STANDALONE_TESTS)
    qt_import_qml_plugins(tst_grpc_client_unarycall_qml)
    qt_import_qml_plugins(tst_grpc_client_unarycall_qml_gen)
endif()

