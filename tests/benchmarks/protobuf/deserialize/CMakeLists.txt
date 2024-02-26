# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_internal_add_benchmark(tst_bench_deserialize_protobuf
    SOURCES
        tst_bench_deserialize_protobuf.cpp
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        Qt::Test
        Qt::Protobuf
)

qt6_add_protobuf(tst_bench_deserialize_protobuf
    PROTO_FILES
        data/bench.proto
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qt_protobuf_generated"
)
