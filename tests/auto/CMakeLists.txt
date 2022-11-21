# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(protobuf)
if(QT_FEATURE_qtprotobufgen AND NOT CMAKE_CROSSCOMPILING)
    add_subdirectory(protobufgen)
    add_subdirectory(grpcgen)
endif()

if(QT_FEATURE_grpc)
    add_subdirectory(grpc)
endif()
