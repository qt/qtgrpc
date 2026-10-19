# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_add_library(protolib SHARED)
qt_add_protobuf(protolib
    PROTO_FILES proto/messages.proto
    EXPORT_MACRO "INVALID-MACRO"
)
