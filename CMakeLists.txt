# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

include(.cmake.conf)
project(QtGrpc
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "Qt GRPC and Protobuf generator and bindings."
    HOMEPAGE_URL "https://qt.io"
    LANGUAGES CXX C
)

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Core)
find_package(Qt6 ${PROJECT_VERSION} CONFIG OPTIONAL_COMPONENTS Network Gui Widgets)

# Try to find Qt6::qtprotobufgen and Qt6::qtgrpcgen targets from host tools
# when cross-compiling.
if(NOT "${QT_HOST_PATH}" STREQUAL "")
    # Set up QT_HOST_PATH as an extra root path to look for the ProtobufTools and GrpcTools packages
    set(_qt_backup_qtgrpc_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
    set(_qt_backup_qtgrpc_CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH})
    list(PREPEND CMAKE_PREFIX_PATH "${QT_HOST_PATH_CMAKE_DIR}")
    list(PREPEND CMAKE_FIND_ROOT_PATH "${QT_HOST_PATH}")

    # This can't use the find_package(Qt6 COMPONENTS) signature, because Qt6Config uses NO_DEFAULT
    # and won't look at the prepend extra find root paths.
    find_package(Qt6ProtobufTools ${PROJECT_VERSION} CONFIG
        PATHS
            ${_qt_additional_packages_prefix_path}
            ${_qt_additional_packages_prefix_path_env}
    )

    find_package(Qt6GrpcTools ${PROJECT_VERSION} CONFIG
        PATHS
            ${_qt_additional_packages_prefix_path}
            ${_qt_additional_packages_prefix_path_env}
    )

    set(CMAKE_PREFIX_PATH ${_qt_backup_qtgrpc_CMAKE_PREFIX_PATH})
    set(CMAKE_FIND_ROOT_PATH ${_qt_backup_qtgrpc_CMAKE_FIND_ROOT_PATH})
endif()

include("${CMAKE_CURRENT_LIST_DIR}/src/tools/qtprotobufgen/Qt6ProtobufToolsMacros.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/src/tools/qtgrpcgen/Qt6GrpcToolsMacros.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/src/protobuf/Qt6ProtobufBuildInternals.cmake")

qt_build_repo()
