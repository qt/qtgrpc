# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include("${RunCMake_TEST_BINARY_DIR}/output_headers.cmake")
foreach(header IN LISTS output_headers)
    if(NOT EXISTS "${header}")
        string(APPEND RunCMake_TEST_FAILED "OUTPUT_HEADERS entry ${header} was not generated.\n")
    endif()
endforeach()
