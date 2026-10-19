# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include(${CMAKE_CURRENT_LIST_DIR}/../RunCMakeTestSetup.cmake)

run_cmake_configure(unknown_type)

set(RunCMake_TEST_BINARY_DIR "${RunCMake_BINARY_DIR}/output_variables-build")
run_cmake_with_options(output_variables ${config_flags})
set(RunCMake_TEST_NO_CLEAN 1)
set(RunCMake_TEST_OUTPUT_MERGE 1)
run_cmake_command(output_variables-build ${CMAKE_COMMAND} --build .)
