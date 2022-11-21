# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(QT_FEATURE_qtprotobufgen)
    # Skip building qtprotoccommon when not building tools
    if((NOT CMAKE_CROSSCOMPILING OR QT_FORCE_BUILD_TOOLS)
            AND TARGET WrapProtobuf::WrapLibProtoc AND TARGET WrapProtobuf::WrapLibProtobuf)
        add_subdirectory(qtprotoccommon)
    endif()
    add_subdirectory(qtprotobufgen)
    add_subdirectory(qtgrpcgen)
endif()
