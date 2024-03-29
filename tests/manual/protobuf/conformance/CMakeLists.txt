# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Starting the test from the build directory, e.g.:
# /path/to/conformance_test_runner tests/manual/protobuf/conformance/tst_protobuf_conformance
# If you use Multi-Config build the path may include config-specific infixes.

find_program(patch NAMES patch patch.exe)

if(NOT patch)
    message(VERBOSE "tst_protobuf_conformance requires `patch` in PATH. Skip the test.")
    return()
endif()

if(QT_PROTOBUF_CONFORMANCE_VERSION)
    set(version "v${QT_PROTOBUF_CONFORMANCE_VERSION}")
elseif(ENV{QT_PROTOBUF_CONFORMANCE_VERSION})
    set(version "v$ENV{QT_PROTOBUF_CONFORMANCE_VERSION}")
else()
    set(version "v21.9")
endif()

set(protobuf_github_base_url
    "https://raw.githubusercontent.com/protocolbuffers/protobuf/${version}")

file(DOWNLOAD
    "${protobuf_github_base_url}/conformance/conformance.proto"
    "${CMAKE_CURRENT_BINARY_DIR}/conformance.proto"
    TIMEOUT 10
)

file(DOWNLOAD
    "${protobuf_github_base_url}/src/google/protobuf/test_messages_proto3.proto"
    "${CMAKE_CURRENT_BINARY_DIR}/test_messages_proto3.proto"
    TIMEOUT 10
)

add_executable(tst_protobuf_conformance
    tst_protobuf_conformance.cpp
)


qt6_add_protobuf(tst_protobuf_conformance
    PROTO_FILES
        "${CMAKE_CURRENT_BINARY_DIR}/conformance.proto"
        "${CMAKE_CURRENT_BINARY_DIR}/test_messages_proto3.proto"
)

target_compile_definitions(tst_protobuf_conformance
    PRIVATE
        QT_NO_DEBUG_OUTPUT
        QT_NO_INFO_OUTPUT
        QT_NO_WARNING_OUTPUT
)

target_link_libraries(tst_protobuf_conformance PRIVATE Qt6::ProtobufWellKnownTypes)

qt_autogen_tools_initial_setup(tst_protobuf_conformance)

set(test_output_dir "${CMAKE_CURRENT_BINARY_DIR}/results")

if(NOT ENV{QT_PROTOBUF_CONFORMANCE_TEST_RUNNER} STREQUAL "")
    set(QT_PROTOBUF_CONFORMANCE_TEST_RUNNER $ENV{QT_PROTOBUF_CONFORMANCE_TEST_RUNNER})
endif()

find_program(QT_PROTOBUF_CONFORMANCE_TEST_RUNNER
    NAMES
        conformance_test_runner conformance_test_runner.exe
)

if(EXISTS "${QT_PROTOBUF_CONFORMANCE_TEST_RUNNER}")
    add_test(NAME tst_protobuf_conformance
        COMMAND "${QT_PROTOBUF_CONFORMANCE_TEST_RUNNER}"
            --failure_list "${CMAKE_CURRENT_SOURCE_DIR}/expect_failing_tests.txt"
            --output_dir "${CMAKE_CURRENT_BINARY_DIR}"
            $<TARGET_FILE:tst_protobuf_conformance>
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
else()
    message(VERBOSE "QT_PROTOBUF_CONFORMANCE_TEST_RUNNER is not set. The tst_protobuf_conformance"
        " test won't start automatically. You may run the test manually after it's built using the"
        " following command:\n"
        "/path/to/conformance_test_runner --failure_list"
        " ${CMAKE_CURRENT_SOURCE_DIR}/expect_failing_tests.txt"
        " --output_dir ${CMAKE_CURRENT_BINARY_DIR}"
        " ${CMAKE_CURRENT_BINARY_DIR}/tst_protobuf_conformance"
    )
endif()
