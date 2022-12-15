# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(protobufqtcoretypes)

if(TARGET Qt6::Gui)
    add_subdirectory(protobufqtguitypes)
endif()
