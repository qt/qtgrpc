# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(QtGrpcChat LANGUAGES CXX)

add_subdirectory(server)
add_subdirectory(client)
