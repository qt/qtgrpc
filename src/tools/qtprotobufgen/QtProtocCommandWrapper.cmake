#!${CMAKE_COMMAND} -P
# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(ENV{QT_PROTOBUF_OPTIONS} "${QT_PROTOBUF_OPTIONS}")

execute_process(COMMAND
    ${PROTOC_EXECUTABLE}
    ${PROTOC_ARGS}
    WORKING_DIRECTORY "${WORKING_DIRECTORY}"
    OUTPUT_VARIABLE output
    ERROR_VARIABLE output
    RESULT_VARIABLE result
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "Unable to execute ${PROTOC_EXECUTABLE}:(${result}) ${output}")
endif()
