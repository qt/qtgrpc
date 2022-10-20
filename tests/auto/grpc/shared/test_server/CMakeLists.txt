# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

find_package(WrapgRPCPlugin REQUIRED)
find_package(WrapProtobuf REQUIRED)
find_package(WrapgRPC REQUIRED)

set(proto_files "${CMAKE_CURRENT_LIST_DIR}/../proto/testservice.proto")
set(out_dir ${CMAKE_CURRENT_BINARY_DIR})

set(generated_files
    "${out_dir}/testservice.pb.h" "${out_dir}/testservice.pb.cc"
    "${out_dir}/testservice.grpc.pb.h" "${out_dir}/testservice.grpc.pb.cc")

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
add_library(TestServerRunner_grpc_gen STATIC ${generated_files})
target_include_directories(TestServerRunner_grpc_gen
    PRIVATE
        ${out_dir}
        WrapgRPC_INCLUDE_PATH
)

target_link_libraries(TestServerRunner_grpc_gen
    PRIVATE
        WrapProtobuf::WrapLibProtobuf
        WrapgRPC::WrapLibgRPC
)

add_library(TestServerRunner
    STATIC
        testserverrunner.cpp
        testserverrunner.h
)

target_include_directories(TestServerRunner PRIVATE ${out_dir})

target_link_libraries(TestServerRunner
    PRIVATE
        TestServerRunner_grpc_gen
        WrapgRPC::WrapLibgRPC
        Qt6::Core
)

qt_add_resources(TestServerRunner
    "keys"
    PREFIX
        "/"
    FILES
        "assets/cert.pem"
        "assets/key.pem"
)

qt_internal_add_executable(testserver
    SOURCES
        testserver.cpp
    LIBRARIES
        TestServerRunner
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    INSTALL_DIRECTORY "${INSTALL_TESTSDIR}/shared"
)

qt_internal_add_executable(securetestserver
    SOURCES
        securetestserver.cpp
    LIBRARIES
        TestServerRunner
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    INSTALL_DIRECTORY "${INSTALL_TESTSDIR}/shared"
)
