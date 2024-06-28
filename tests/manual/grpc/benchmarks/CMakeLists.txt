# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(proto_path "${CMAKE_CURRENT_LIST_DIR}/proto/bench.proto")
set(proto_include "${CMAKE_CURRENT_LIST_DIR}/proto")
set(qrpcbench_common_include "${CMAKE_CURRENT_LIST_DIR}")

add_subdirectory(asyncbenchserver)
add_subdirectory(bench_qtgrpcclient)
