# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Starting the test from the build directory, e.g.:
# /path/to/conformance_test_runner tests/manual/protobuf/conformance/tst_protobuf_conformance
# If you use Multi-Config build the path may include config-specific infixes.

if(NOT "$ENV{QT_PROTOBUF_CONFORMANCE_TEST_RUNNER}" STREQUAL "")
    set(QT_PROTOBUF_CONFORMANCE_TEST_RUNNER $ENV{QT_PROTOBUF_CONFORMANCE_TEST_RUNNER})
endif()

find_program(QT_PROTOBUF_CONFORMANCE_TEST_RUNNER
    NAMES
        conformance_test_runner conformance_test_runner.exe
)

if(NOT EXISTS "${QT_PROTOBUF_CONFORMANCE_TEST_RUNNER}")
    message("conformance_test_runner is not found. Set QT_PROTOBUF_CONFORMANCE_TEST_RUNNER"
        " to specify the path to the conformance_test_runner executable. Skipping"
        " tst_protobuf_conformance."
    )
    return()
endif()

# The failure list must match the runner, since test names change between
# releases. Protobuf_VERSION only identifies the runner when both come from
# the same install. Strip the trailing tweak and the 3.x language-version
# prefix.
if(QT_PROTOBUF_CONFORMANCE_VERSION)
    set(conformance_version "${QT_PROTOBUF_CONFORMANCE_VERSION}")
elseif(NOT "$ENV{QT_PROTOBUF_CONFORMANCE_VERSION}" STREQUAL "")
    set(conformance_version "$ENV{QT_PROTOBUF_CONFORMANCE_VERSION}")
elseif(Protobuf_VERSION MATCHES "([0-9]+)\\.([0-9]+)\\.[0-9]+$")
    set(conformance_version "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}")
else()
    message(WARNING "Cannot tell which protobuf release conformance_test_runner"
        " came from: Protobuf_VERSION is '${Protobuf_VERSION}'. Set"
        " QT_PROTOBUF_CONFORMANCE_VERSION to the runner's release. Skipping"
        " tst_protobuf_conformance."
    )
    return()
endif()

set(failure_list
    "${CMAKE_CURRENT_SOURCE_DIR}/expect_failing_tests_v${conformance_version}.txt")
if(NOT EXISTS "${failure_list}")
    message(WARNING "No conformance failure list for protobuf"
        " ${conformance_version}. Expected ${failure_list}. Run the suite with"
        " --output_dir to generate one, or point"
        " QT_PROTOBUF_CONFORMANCE_VERSION at a release that has one. Skipping"
        " tst_protobuf_conformance."
    )
    return()
endif()

message(STATUS "tst_protobuf_conformance: baselining protobuf"
    " ${conformance_version} (Protobuf_VERSION '${Protobuf_VERSION}') against"
    " ${QT_PROTOBUF_CONFORMANCE_TEST_RUNNER}"
)

# The protos only need to match the vcpkg protobuf release, not the runner.
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data/conformance.proto" DESTINATION
    "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data/test_messages_proto3.proto" DESTINATION
    "${CMAKE_CURRENT_BINARY_DIR}")

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

add_test(NAME tst_protobuf_conformance
    COMMAND "${QT_PROTOBUF_CONFORMANCE_TEST_RUNNER}"
        --failure_list "${failure_list}"
        --output_dir "${CMAKE_CURRENT_BINARY_DIR}"
        $<TARGET_FILE:tst_protobuf_conformance>
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
