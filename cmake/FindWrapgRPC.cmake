# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(TARGET WrapgRPC::WrapLibgRPC)
    set(WrapgRPC_FOUND TRUE)
    return()
endif()

set(WrapgRPC_FOUND FALSE)

set(__WrapgRPC_find_package_args "")
if(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
    list(APPEND __WrapgRPC_find_package_args QUIET)
endif()

if(NOT TARGET Threads::Threads)
    find_package(Threads ${__WrapgRPC_find_package_args})
endif()
if(TARGET Threads::Threads)
    qt_internal_disable_find_package_global_promotion(Threads::Threads)
endif()

set(__WrapgRPC_CMAKE_FIND_PACKAGE_PREFER_CONFIG_save ${CMAKE_FIND_PACKAGE_PREFER_CONFIG})
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
find_package(gRPC ${__WrapgRPC_find_package_args})
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ${__WrapgRPC_CMAKE_FIND_PACKAGE_PREFER_CONFIG_save})
unset(__WrapgRPC_CMAKE_FIND_PACKAGE_PREFER_CONFIG_save)

if(TARGET gRPC::grpc_cpp_plugin)
    qt_internal_disable_find_package_global_promotion(gRPC::grpc_cpp_plugin)
endif()

if(TARGET ZLIB::ZLIB)
    qt_internal_disable_find_package_global_promotion(ZLIB::ZLIB)
endif()

if(gRPC_FOUND AND TARGET gRPC::grpc AND TARGET gRPC::grpc++)
    add_library(WrapgRPC::WrapLibgRPC INTERFACE IMPORTED)
    target_link_libraries(WrapgRPC::WrapLibgRPC INTERFACE gRPC::grpc gRPC::grpc++)
    get_target_property(Grpc_INCLUDE_PATH gRPC::grpc++ INTERFACE_INCLUDE_DIRECTORIES)
    set_property(GLOBAL PROPERTY WrapgRPC_INCLUDE_PATH "${Grpc_INCLUDE_PATH}")

    set(WrapgRPC_FOUND TRUE)
endif()

unset(__WrapgRPC_find_package_args)
