# Copyright (C) 2025 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_grpc_client_end2end LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
    include("../shared/mockserver/CMakeLists.txt")
endif()

if(NOT TARGET grpc_mock_server)
    message(WARNING "Dependencies of 'tst_grpc_client_end2end' not found. Skipping.")
    return()
endif()

# grpc-c++ has issues when mixing ASan and non-ASan builds.
# See https://github.com/grpc/grpc/issues/40935.
# Re-enable once grpc supports ASan or when we build grpc with ASan in CI.
qt_internal_skip_sanitizer()

qt_internal_add_test(tst_grpc_client_end2end
    SOURCES
        tst_grpc_client_end2end.cpp
    DEFINES
        QTEST_THROW_ON_FAIL
        QTEST_THROW_ON_SKIP
    LIBRARIES
        Qt::Test
        Qt::Core
        Qt::Grpc
        Qt::Network
        grpc_mock_server
)

set(proto_files
  "${CMAKE_CURRENT_LIST_DIR}/eventhub.proto"
  "${CMAKE_CURRENT_LIST_DIR}/event.proto"
)
set(proto_import "${CMAKE_CURRENT_LIST_DIR}")
set(proto_out_include "${CMAKE_CURRENT_BINARY_DIR}/include")
set(proto_out_server "${proto_out_include}/proto/server")
set(proto_out_client "${proto_out_include}/proto/client")

qt_add_protobuf(tst_grpc_client_end2end
  PROTO_FILES
    ${proto_files}
  OUTPUT_DIRECTORY
    ${proto_out_client}
  EXTRA_NAMESPACE "qt"
)
qt_add_grpc(tst_grpc_client_end2end CLIENT
  PROTO_FILES
    ${proto_files}
  OUTPUT_DIRECTORY
    ${proto_out_client}
  EXTRA_NAMESPACE "qt"
)

protobuf_generate(
    PROTOS ${proto_files}
    TARGET tst_grpc_client_end2end
    IMPORT_DIRS ${proto_import}
    PROTOC_OUT_DIR ${proto_out_server}
)
protobuf_generate(
    PROTOS ${proto_files}
    TARGET tst_grpc_client_end2end
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=\$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
    IMPORT_DIRS ${proto_import}
    PROTOC_OUT_DIR "${proto_out_server}"
)

target_include_directories(tst_grpc_client_end2end PRIVATE
    "${proto_out_include}"
)

# Note the qt_internal_skip_sanitizer() above when removing.
qt_internal_skip_linking_sanitizer()
