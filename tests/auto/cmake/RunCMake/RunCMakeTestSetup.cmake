# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include(QtRunCMake)

include(${_Qt6CTestMacros})
# Stub function to make `_qt_internal_get_cmake_test_configure_options` work
function(_qt_internal_get_build_vars_for_external_projects)
endfunction()
_qt_internal_get_cmake_test_configure_options(config_flags)
list(APPEND config_flags "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
foreach(var IN ITEMS QT_USE_VCPKG VCPKG_INSTALLED_DIR VCPKG_HOST_TRIPLET VCPKG_TARGET_TRIPLET)
    if(DEFINED ${var})
        list(APPEND config_flags "-D${var}=${${var}}")
    endif()
endforeach()

function(run_cmake_configure case)
    set(RunCMake_TEST_BINARY_DIR "${RunCMake_BINARY_DIR}/${case}-build")
    run_cmake_with_options(${case} ${config_flags})
endfunction()
