# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

find_package(Qt6 ${PROJECT_VERSION} QUIET CONFIG OPTIONAL_COMPONENTS TestInternalsPrivate)
if(NOT TARGET Qt6::TestInternalsPrivate)
    return()
endif()

include("${_Qt6CTestMacros}")

# Add all tests using CMake's RunCMake test module
add_subdirectory(RunCMake)
