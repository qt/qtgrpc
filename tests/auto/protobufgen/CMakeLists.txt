# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_internal_add_test(tst_qtprotobufgen
    SOURCES
        tst_qtprotobufgen.cpp
    LIBRARIES
        Qt::Test
        Qt::Protobuf
    DEFINES
        PROTOC_EXECUTABLE="$<TARGET_FILE:WrapProtoc::WrapProtoc>"
)

qt_add_protobuf(tst_qtprotobufgen
    PROTO_FILES
        ../shared/data/proto/annotation.proto
    COPY_COMMENTS
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qt_protobuf_generated/comments"
)

qt_add_protobuf(tst_qtprotobufgen
    PROTO_FILES
        ../shared/data/proto/fieldindexrange.proto
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qt_protobuf_generated/fieldenum"
)

qt_add_protobuf(tst_qtprotobufgen
    PROTO_FILES
        ../shared/data/proto/extranamespace.proto
    EXTRA_NAMESPACE
        MyTopLevelNamespace
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qt_protobuf_generated/extra-namespace"
)

qt_add_protobuf(tst_qtprotobufgen
    PROTO_FILES
        ../shared/data/proto/basicmessages.proto
        ../shared/data/proto/mapmessages.proto
        ../shared/data/proto/repeatedmessages.proto
    GENERATE_PACKAGE_SUBFOLDERS
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qt_protobuf_generated/folder"
)

qt_add_protobuf(tst_qtprotobufgen
    PROTO_FILES
        ../shared/data/proto/repeatednonpackedmessages.proto
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qt_protobuf_generated/packed"
)

qt_autogen_tools_initial_setup(tst_qtprotobufgen)
