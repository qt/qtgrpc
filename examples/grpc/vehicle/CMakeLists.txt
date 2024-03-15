# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(vehicle_cluster LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/grpc/vehicle")

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Protobuf
    ProtobufWellKnownTypes
    Grpc
    Quick
)

qt_standard_project_setup()

add_subdirectory(grpc_vehicle_server)

qt_add_executable(vehicle_cluster
    main.cpp
    clusterdatamanager.h
    clusterdatamanager.cpp
    vehiclethread.h
    vehiclethread.cpp
    navithread.h
    navithread.cpp
)

qt_add_protobuf(vehiclelib
    PROTO_FILES
        proto/vehicleservice.proto
        proto/naviservice.proto
)

qt_add_grpc(vehiclelib CLIENT
    PROTO_FILES
        proto/vehicleservice.proto
        proto/naviservice.proto
)

target_link_libraries(vehiclelib PRIVATE Qt6::ProtobufWellKnownTypes)

set_target_properties(vehicle_cluster PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

qt_add_qml_module(vehicle_cluster
    URI qtgrpc.examples.vehicle
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        "ClusterText.qml"
        "ClusterProgressBar.qml"
        "Main.qml"
)

qt_add_resources(vehicle_cluster
    "vehicle_cluster"
    PREFIX "/"
    FILES
        "left.png"
        "right.png"
        "forward.png"
        "fuel_lvl.png"
)

target_link_libraries(vehicle_cluster PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Protobuf
    Qt6::ProtobufWellKnownTypes
    Qt6::Grpc
    vehiclelib
)

install(TARGETS vehicle_cluster vehiclelib
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
