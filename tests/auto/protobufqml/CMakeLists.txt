# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT ANDROID) # ProtobufQuick breaks the assignment in QML for protobuf scalar types.
                # See QTBUG-127174
    add_subdirectory(basic)
    add_subdirectory(enums)
    add_subdirectory(nopackage)
    add_subdirectory(syntax)
    add_subdirectory(nested)
    add_subdirectory(messagelists)
    add_subdirectory(oneof)
endif()
