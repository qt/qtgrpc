# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_protobuf_interop LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT TARGET protobuf::libprotobuf OR NOT TARGET protobuf::protoc
        OR NOT TARGET WrapProtoc::WrapProtoc)
    message(STATUS "libprotobuf is not available. Skipping tst_protobuf_interop.")
    return()
endif()

# protoc's C++ output only compiles against libprotobuf of the same release, so
# the reference sources come from the package's own protoc, not WrapProtoc. When
# cross-compiling, that protoc is usable only if the package points it at a host
# binary.
if(CMAKE_CROSSCOMPILING)
    get_target_property(protoc_location protobuf::protoc IMPORTED_LOCATION)
    if(NOT protoc_location)
        get_target_property(protoc_configs protobuf::protoc IMPORTED_CONFIGURATIONS)
        list(GET protoc_configs 0 protoc_config)
        get_target_property(protoc_location protobuf::protoc
            IMPORTED_LOCATION_${protoc_config})
    endif()
    execute_process(COMMAND "${protoc_location}" --version
        RESULT_VARIABLE protoc_result OUTPUT_QUIET ERROR_QUIET)
    if(NOT protoc_result EQUAL 0)
        message(STATUS "protoc of the target libprotobuf does not run on the host."
            " Skipping tst_protobuf_interop.")
        return()
    endif()
endif()

set(proto_file "${CMAKE_CURRENT_LIST_DIR}/../../shared/data/proto/proto2messages.proto")
set(out_dir "${CMAKE_CURRENT_BINARY_DIR}/reference")
set(generated_files "${out_dir}/proto2messages.pb.h" "${out_dir}/proto2messages.pb.cc")

file(MAKE_DIRECTORY "${out_dir}")

add_custom_command(
    OUTPUT ${generated_files}
    COMMAND
        $<TARGET_FILE:protobuf::protoc>
    ARGS
        --cpp_out "${out_dir}"
        -I "${CMAKE_CURRENT_LIST_DIR}/../../shared/data/proto"
        "${proto_file}"
    WORKING_DIRECTORY "${out_dir}"
    DEPENDS "${proto_file}" $<TARGET_FILE:protobuf::protoc>
    COMMENT "Generating reference protobuf sources for tst_protobuf_interop..."
    COMMAND_EXPAND_LISTS
    VERBATIM
)

set_source_files_properties(${generated_files} PROPERTIES GENERATED TRUE)

# Kept out of the Qt test target so that Qt's warning flags are not applied to
# code protoc generates.
add_library(tst_protobuf_interop_reference STATIC ${generated_files})
target_include_directories(tst_protobuf_interop_reference PRIVATE "${out_dir}")
target_link_libraries(tst_protobuf_interop_reference PRIVATE protobuf::libprotobuf)

# protoc and qtprotobufgen derive the same C++ names from a .proto. Without a
# distinct namespace both define the same symbols and the linker binds calls to
# whichever it sees first.
qt6_add_protobuf(tst_protobuf_interop_gen
    PROTO_FILES
        "${proto_file}"
    EXTRA_NAMESPACE "QtProtobufInterop"
)
qt_autogen_tools_initial_setup(tst_protobuf_interop_gen)

qt_internal_add_test(tst_protobuf_interop
    SOURCES
        tst_protobuf_interop.cpp
    LIBRARIES
        Qt::Test
        Qt::Protobuf
        protobuf::libprotobuf
        tst_protobuf_interop_gen
        tst_protobuf_interop_reference
)
# SYSTEM so the generated reference header does not warn in the test itself.
target_include_directories(tst_protobuf_interop SYSTEM PRIVATE "${out_dir}")
qt_autogen_tools_initial_setup(tst_protobuf_interop)
