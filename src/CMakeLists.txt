# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Evaluate features before including repository subparts.
qt_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/protobuf/configure.cmake")

add_subdirectory(protobuf)
if(QT_FEATURE_qtprotobufgen)
    add_subdirectory(tools)
endif()
