# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause


# Sets out_var to the value of the first non-empty environment variable given in
# CANDIDATE_ENV_VARS, as a CMake-style path. The candidates default to the name of out_var.
# Does nothing if out_var already has a value.
function(qt_internal_grpc_set_var_from_env out_var)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "" "CANDIDATE_ENV_VARS")

    if(${out_var})
        return()
    endif()

    set(env_var_names "${arg_CANDIDATE_ENV_VARS}")
    if(NOT env_var_names)
        set(env_var_names "${out_var}")
    endif()

    foreach(env_var_name IN LISTS env_var_names)
        if(NOT "$ENV{${env_var_name}}" STREQUAL "")
            file(TO_CMAKE_PATH "$ENV{${env_var_name}}" path)
            set(${out_var} "${path}" PARENT_SCOPE)
            return()
        endif()
    endforeach()
endfunction()

# Pass 3rd party dependency locations to examples as external projects.
# Protobuf needs both spellings, the one that is set depends on the host platform.
qt_internal_grpc_set_var_from_env(Protobuf_ROOT)
qt_internal_grpc_set_var_from_env(protobuf_ROOT)
qt_internal_grpc_set_var_from_env(gRPC_ROOT)
qt_internal_grpc_set_var_from_env(absl_ROOT)

# Qt CI sets various env var names for openssl depending on platform, check all of them.
qt_internal_grpc_set_var_from_env(OPENSSL_ROOT_DIR
    CANDIDATE_ENV_VARS
        OPENSSL_ROOT_DIR
        OPENSSL_DIR
        OPENSSL_HOME
        OPENSSL_ROOT
)

# In the CI, we assume the absl dependency is next to the gRPC one.
if(gRPC_ROOT AND NOT absl_ROOT)
    get_filename_component(grpc_parent_dir "${gRPC_ROOT}" DIRECTORY)
    if(EXISTS "${grpc_parent_dir}/absl")
        set(absl_ROOT "${grpc_parent_dir}/absl")
    endif()
endif()


# Variables that are not defined are silently skipped when passed on.
set(QT_EXAMPLE_CMAKE_VARS_TO_PASS
    protobuf_ROOT:STRING
    Protobuf_ROOT:STRING
    absl_ROOT:STRING
    OPENSSL_ROOT_DIR:STRING
)

# Our current provisioning (and also apparently upstream) don't support Mingw grpc builds.
# Similarly, we don't build vcpkg ports for mingw, so don't pass that either.
if(NOT MINGW)
    list(APPEND QT_EXAMPLE_CMAKE_VARS_TO_PASS
        gRPC_ROOT:STRING

        QT_USE_VCPKG:STRING
        VCPKG_MANIFEST_INSTALL:BOOL
        VCPKG_HOST_TRIPLET:STRING
        VCPKG_INSTALLED_DIR:STRING
    )
endif()

# Some of the grpc examples have a vcpkg.json manifest file. Building the examples as external
# projects with vcpkg support enabled would cause building and installing the grpc deps into each
# separate example build dir. Disable the automatic installation of dependencies from the manifest
# file to avoid that.
set(VCPKG_MANIFEST_INSTALL OFF)

qt_examples_build_begin(EXTERNAL_BUILD)

add_subdirectory(protobuf)

if(TARGET Qt6::Grpc)
    add_subdirectory(grpc)
endif()
qt_examples_build_end()
