# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(TARGET WrapgRPC::WrapLibgRPC)
    set(WrapgRPC_FOUND TRUE)
    return()
endif()

set(WrapgRPC_FOUND FALSE)

set(__WrapgRPC_find_package_args QUIET)

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

function(WrapgRPC_ensure_grpc_lib libname)
    if(TARGET gRPC::${libname})
        return()
    endif()
    pkg_check_modules(PC_${libname} QUIET ${libname})
    if(NOT PC_${libname}_FOUND)
        return()
    endif()

    find_library(${libname}_LIBRARY NAMES ${libname} HINTS ${PC_${libname}_LIBDIR})
    if(NOT ${libname}_LIBRARY)
        return()
    endif()

    find_path(${libname}_INCLUDE_DIRS NAMES ${libname}/${libname}.h ${PC_${libname}_INCLUDEDIR})
    if(NOT ${libname}_INCLUDE_DIRS)
        return()
    endif()

    add_library(gRPC::${libname} UNKNOWN IMPORTED)
    set_target_properties(gRPC::${libname} PROPERTIES
        IMPORTED_LOCATION ${${libname}_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES "${${libname}_INCLUDE_DIRS}"
    )
endfunction()


if(NOT TARGET gRPC::grpc OR NOT TARGET gRPC::grpc++)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        WrapgRPC_ensure_grpc_lib(grpc)
        WrapgRPC_ensure_grpc_lib(grpc++)
    endif()
endif()

unset(WrapgRPC_ensure_grpc_lib)

if(TARGET gRPC::grpc
    AND TARGET gRPC::grpc++)
    add_library(WrapgRPC::WrapLibgRPC INTERFACE IMPORTED)
    target_link_libraries(WrapgRPC::WrapLibgRPC INTERFACE gRPC::grpc gRPC::grpc++)
    get_target_property(Grpc_INCLUDE_PATH gRPC::grpc++ INTERFACE_INCLUDE_DIRECTORIES)
    set_property(GLOBAL PROPERTY WrapgRPC_INCLUDE_PATH "${Grpc_INCLUDE_PATH}")

    set(WrapgRPC_FOUND TRUE)
endif()

unset(__WrapgRPC_find_package_args)
