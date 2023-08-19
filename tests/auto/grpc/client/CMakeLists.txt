# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
add_subdirectory(shared)

add_subdirectory(unattached_channel)
add_subdirectory(no_channel)
add_subdirectory(unarycall)
add_subdirectory(serverstream)
add_subdirectory(clientstream)
add_subdirectory(bidirstream)
if(QT_FEATURE_ssl)
    add_subdirectory(ssl)
endif()
