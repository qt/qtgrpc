# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# messages.proto is only found if PROTO_FILES_BASE_DIR is respected.
qt_add_protobuf(protolib
    PROTO_FILES messages.proto
    PROTO_FILES_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto"
    OUTPUT_HEADERS headers
    OUTPUT_TARGETS targets
)

get_target_property(type protolib TYPE)
set(expected_headers "${CMAKE_CURRENT_BINARY_DIR}/messages.qpb.h")
set(expected_targets protolib)
if(type STREQUAL "SHARED_LIBRARY")
    list(APPEND expected_headers "${CMAKE_CURRENT_BINARY_DIR}/protolib_exports.qpb.h")
endif()
if(type STREQUAL "STATIC_LIBRARY" OR WIN32)
    list(APPEND expected_targets protolib_qtprotoreg)
endif()

foreach(var IN ITEMS headers targets)
    if(NOT "${${var}}" STREQUAL "${expected_${var}}")
        message(FATAL_ERROR "Unexpected OUTPUT_${var}: '${${var}}'."
            " Expected: '${expected_${var}}'.")
    endif()
endforeach()

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/output_headers.cmake"
    "set(output_headers \"${headers}\")\n")
