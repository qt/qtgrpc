# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

# Set up QT_HOST_PATH as an extra root path to look for the ProtobufTools and GrpcTools packages
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
