# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT QT_FEATURE_qtprotobufgen)
    return()
endif()

include("${PROJECT_SOURCE_DIR}/src/tools/qtprotobufgen/Qt6ProtobufToolsMacros.cmake")

if(QT_FEATURE_native_grpc)
    add_subdirectory(shared)
    add_subdirectory(client)
    add_subdirectory(server)
    if(QT_FEATURE_ssl)
        add_subdirectory(ssl_client)
    endif()
endif()
add_subdirectory(unattached_channel)
