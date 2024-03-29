# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_protobuf_messagelists_qml LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

qt_internal_add_test(tst_protobuf_repeated_qml
    QMLTEST
    SOURCES
        tst_protobuf_repeated_qml.cpp
    LIBRARIES
        Qt::Qml
        Qt::Protobuf
)

qt_add_protobuf(tst_protobuf_repeated_qml_gen
    PROTO_FILES
        ../../shared/data/proto/basicmessages.proto
        ../../shared/data/proto/repeatedmessages.proto
    QML
    FIELD_ENUM
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qt_protobuf_generated"
)

qt_policy(SET QTP0001 NEW)

qt_add_qml_module(tst_protobuf_repeated_qml
    URI QmlTestUri
    VERSION 1.0
    QML_FILES
        qml/tst_repeated.qml
)

qt_autogen_tools_initial_setup(tst_protobuf_repeated_qml)
qt_autogen_tools_initial_setup(tst_protobuf_repeated_qml_gen)
qt_autogen_tools_initial_setup(tst_protobuf_repeated_qml_genplugin)

if(QT_BUILD_STANDALONE_TESTS)
    qt_import_qml_plugins(tst_protobuf_repeated_qml)
endif()

