# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_internal_add_protobuf_module(ProtobufWellKnownTypes
    GENERATE_CPP_EXPORTS
    SOURCES
        qtprotobufwellknowntypesglobal.h
        qprotobufanysupport.cpp qprotobufanysupport.h
    PUBLIC_LIBRARIES
        Qt::Protobuf
    LIBRARIES
        Qt::ProtobufPrivate
)

qt_internal_add_protobuf_wellknown_types(ProtobufWellKnownTypes
    any
    api
    duration
    empty
    field_mask
    source_context
    struct
    timestamp
    type
    wrappers
)
