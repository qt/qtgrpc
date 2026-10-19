# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

_qt_internal_get_cmake_test_configure_options(config_flags)

# Some CI images provide protobuf only through vcpkg.
if(QT_USE_VCPKG)
    foreach(var IN ITEMS QT_USE_VCPKG VCPKG_INSTALLED_DIR VCPKG_HOST_TRIPLET VCPKG_TARGET_TRIPLET)
        if(NOT "${${var}}" STREQUAL "")
            file(TO_CMAKE_PATH "${${var}}" value)
            list(APPEND config_flags "-D${var}=${value}")
        endif()
    endforeach()
endif()

if(QT_FEATURE_qtprotobufgen)
    qt_internal_add_RunCMake_test(FindWrapProtoc ${config_flags})
    qt_internal_add_RunCMake_test(qt_add_protobuf ${config_flags})
endif()
if(QT_FEATURE_qtgrpcgen AND TARGET Qt6::Grpc)
    qt_internal_add_RunCMake_test(qt_add_grpc ${config_flags})
endif()
