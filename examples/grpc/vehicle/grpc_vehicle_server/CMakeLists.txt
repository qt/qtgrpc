# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(VehicleServerRunner LANGUAGES CXX)

# Qt6::Grpc module is not used directly in this project. But this allows to find Qt6::Grpc's
# dependencies without setting extra cmake module paths.
find_package(Qt6 COMPONENTS Core)
find_package(WrapgRPCPlugin)
find_package(WrapgRPC)

if(NOT TARGET WrapgRPC::WrapgRPCPlugin OR NOT TARGET WrapProtoc::WrapProtoc
    OR NOT TARGET WrapgRPC::WrapLibgRPC)
    message(WARNING "Dependencies of SimpleVehicleServer not found. Skipping.")
    return()
endif()

if(MINGW)
    message(WARNING "${PROJECT_NAME} uses reference grpc++ library that doesn't officially support"
        " MinGW. Please use the MSVC compiler to build this example. The correct work is not"
        " guaranteed otherwise.")
endif()

set(proto_files
    "${CMAKE_CURRENT_LIST_DIR}/../proto/vehicleservice.proto"
    "${CMAKE_CURRENT_LIST_DIR}/../proto/naviservice.proto")
set(out_dir ${CMAKE_CURRENT_BINARY_DIR})

set(generated_files
    "${out_dir}/vehicleservice.pb.h" "${out_dir}/vehicleservice.pb.cc"
    "${out_dir}/vehicleservice.grpc.pb.h" "${out_dir}/vehicleservice.grpc.pb.cc"
    "${out_dir}/naviservice.pb.h" "${out_dir}/naviservice.pb.cc"
    "${out_dir}/naviservice.grpc.pb.h" "${out_dir}/naviservice.grpc.pb.cc")

add_custom_command(
    OUTPUT ${generated_files}
    COMMAND
    $<TARGET_FILE:WrapProtoc::WrapProtoc>
    ARGS
    --grpc_out "${out_dir}"
    --cpp_out "${out_dir}"
    -I "${CMAKE_CURRENT_LIST_DIR}/../proto/"
    --plugin=protoc-gen-grpc=$<TARGET_FILE:WrapgRPC::WrapgRPCPlugin>
    "${proto_files}"
    WORKING_DIRECTORY ${out_dir}
    DEPENDS "${proto_files}"
    COMMENT "Generating gRPC ${target} sources..."
    COMMAND_EXPAND_LISTS
    VERBATIM
)

set_source_files_properties(${generated_files} PROPERTIES GENERATED TRUE)

qt_add_executable(SimpleVehicleServer
    ${generated_files}
    ${CMAKE_CURRENT_LIST_DIR}/serverrunner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/serverrunner.h
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_include_directories(SimpleVehicleServer
    PRIVATE
    ${out_dir}
)

target_link_libraries(SimpleVehicleServer PRIVATE
    WrapgRPC::WrapLibgRPC
    Qt6::Core
)

install(TARGETS SimpleVehicleServer
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
