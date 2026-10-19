# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_add_grpc(grpclib CLIENT
    PROTO_FILES service.proto
    OUTPUT_HEADERS headers
    OUTPUT_TARGETS targets
)
# The generated client code needs the message classes to compile.
qt_add_protobuf(grpclib PROTO_FILES service.proto)

get_target_property(type grpclib TYPE)
set(expected_headers "${CMAKE_CURRENT_BINARY_DIR}/service_client.grpc.qpb.h")
set(expected_targets grpclib)
if(type STREQUAL "SHARED_LIBRARY")
    list(APPEND expected_headers "${CMAKE_CURRENT_BINARY_DIR}/grpclib_exports.qpb.h")
endif()

foreach(var IN ITEMS headers targets)
    if(NOT "${${var}}" STREQUAL "${expected_${var}}")
        message(FATAL_ERROR "Unexpected OUTPUT_${var}: '${${var}}'."
            " Expected: '${expected_${var}}'.")
    endif()
endforeach()

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/output_headers.cmake"
    "set(output_headers \"${headers}\")\n")
