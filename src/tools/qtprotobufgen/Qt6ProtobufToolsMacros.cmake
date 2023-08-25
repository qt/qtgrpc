# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(__qt_protobuf_macros_module_base_dir "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")

# List of the common protoc generator options.
macro(_qt_internal_get_protoc_common_options option_args single_args multi_args)
    set(${option_args}
        COPY_COMMENTS
        GENERATE_PACKAGE_SUBFOLDERS
    )
    set(${single_args}
        EXTRA_NAMESPACE
        EXPORT_MACRO
    )

    set(${multi_args} "")
endmacro()

# List of arguments common for the protoc generating functions.
macro(_qt_internal_get_protoc_generate_arguments option_args single_args multi_args)
    set(${option_args} "")
    set(${single_args}
        OUTPUT_DIRECTORY
        PROTO_FILES_BASE_DIR
        OUTPUT_HEADERS
        OUTPUT_TARGETS
    )
    set(${multi_args}
        PROTO_FILES
        PROTO_INCLUDES
    )
endmacro()

# The macro collects options in protoc compatible format. Options are written into out_var.
# All input arguments are names of the lists containing the corresponding options.
macro(_qt_internal_get_protoc_options out_var prefix option single multi)
    set(${out_var} "")
    foreach(opt IN LISTS ${option})
        if(${prefix}_${opt})
            list(APPEND ${out_var} ${opt})
        endif()
    endforeach()

    foreach(opt IN LISTS ${single} ${multi})
        if(${prefix}_${opt})
            list(APPEND ${out_var} "${opt}=${${prefix}_${opt}}")
        endif()
    endforeach()
endmacro()

# The base function that generates rules to call the protoc executable with the custom generator
# plugin.
# Multi-value Arguments:
#   PROTO_FILES - list of the .proto file paths. Paths should be absolute for the correct work of
#       this function.
#   PROTO_INCLUDES - list of the protobuf include paths.
#   GENERATED_FILES - list of files that are expected
#                     to be genreated by the custom generator plugin.
#   OPTIONS - list of the generator-specific options.
function(_qt_internal_protoc_generate target generator output_directory)
    cmake_parse_arguments(arg "" "" "PROTO_FILES;PROTO_INCLUDES;GENERATED_FILES;OPTIONS" ${ARGN})

    if(NOT arg_PROTO_FILES)
        message(FATAL_ERROR "PROTO_FILES list is empty.")
    endif()

    set(proto_includes "")
    if(arg_PROTO_INCLUDES)
        set(proto_includes "${arg_PROTO_INCLUDES}")
    endif()

    if(NOT arg_GENERATED_FILES)
        message(FATAL_ERROR
            "List of generated sources for target '${target}' is empty")
    endif()

    set(generated_files "${arg_GENERATED_FILES}")

    get_filename_component(output_directory "${output_directory}" REALPATH)
    get_target_property(is_generator_imported ${QT_CMAKE_EXPORT_NAMESPACE}::${generator} IMPORTED)
    if(QT_INTERNAL_AVOID_USING_PROTOBUF_TMP_OUTPUT_DIR OR is_generator_imported)
        set(tmp_output_directory "${output_directory}")
    else()
        set(tmp_output_directory "${output_directory}/.tmp")
    endif()
    file(MAKE_DIRECTORY ${tmp_output_directory})

    unset(num_deps)
    if(TARGET ${target})
        get_target_property(num_deps ${target} _qt_${generator}_deps_num)
    endif()
    if(NOT num_deps)
        set(num_deps 0)
    endif()
    set(deps_target ${target}_${generator}_deps_${num_deps})
    math(EXPR num_deps "${num_deps} + 1")

    set(generator_file $<TARGET_FILE:${QT_CMAKE_EXPORT_NAMESPACE}::${generator}>)

    set(proto_includes_string "")
    if(proto_includes)
        list(JOIN proto_includes "$<SEMICOLON>" proto_includes)
        set(proto_includes_genex "$<GENEX_EVAL:${proto_includes}>")
        set(proto_includes_condition "$<BOOL:${proto_includes_genex}>")
        string(JOIN "" proto_includes_string
            "$<${proto_includes_condition}:"
                "-I$<JOIN:${proto_includes_genex},\\$<SEMICOLON>-I>"
            ">"
        )
    endif()
    list(JOIN arg_PROTO_FILES "\\$<SEMICOLON>"  proto_files_string)
    if(arg_OPTIONS)
        list(JOIN arg_OPTIONS "\\\\$<SEMICOLON>" generation_options_string)
    else()
        set(generation_options_string "")
    endif()
    string(JOIN "\\$<SEMICOLON>" protoc_arguments
        "--plugin=protoc-gen-${generator}=${generator_file}"
        "--${generator}_out=${tmp_output_directory}"
        "--${generator}_opt=${generation_options_string}"
        "${proto_files_string}"
        "${proto_includes_string}"
    )

    unset(extra_copy_commands)
    if(NOT tmp_output_directory STREQUAL output_directory)
        foreach(f IN LISTS generated_files)
            get_filename_component(filename "${f}" NAME)
            if(IS_ABSOLUTE "${f}")
                file(RELATIVE_PATH f_rel "${output_directory}" "${f}")
            else()
                message(AUTHOR_WARNING
                    "Path to the generated file ${f} should be absolute, when \
                    calling _qt_internal_protoc_generate"
                )
            endif()
            list(APPEND extra_copy_commands COMMAND
                ${CMAKE_COMMAND} -E copy_if_different "${tmp_output_directory}/${f_rel}" "${f}")
        endforeach()
    endif()

    add_custom_command(OUTPUT ${generated_files}
        COMMAND ${CMAKE_COMMAND}
            -DPROTOC_EXECUTABLE=$<TARGET_FILE:WrapProtoc::WrapProtoc>
            "-DPROTOC_ARGS=${protoc_arguments}"
            -DWORKING_DIRECTORY=${output_directory}
            -DGENERATOR_NAME=${generator}
            -P
            ${__qt_protobuf_macros_module_base_dir}/QtProtocCommandWrapper.cmake
        ${extra_copy_commands}
        WORKING_DIRECTORY ${output_directory}
        DEPENDS
            ${QT_CMAKE_EXPORT_NAMESPACE}::${generator}
            ${proto_files}
        COMMENT "Generating QtProtobuf ${target} sources for ${generator}..."
        COMMAND_EXPAND_LISTS
        VERBATIM
    )
    add_custom_target(${deps_target} DEPENDS ${generated_files})
    set_property(TARGET ${target} APPEND PROPERTY
        AUTOGEN_TARGET_DEPENDS "${deps_target}")
    set_property(TARGET ${target} PROPERTY _qt_${generator}_deps_num "${num_deps}")
    set_source_files_properties(${generated_files} PROPERTIES
        GENERATED TRUE
    )

    target_include_directories(${target} PUBLIC "$<BUILD_INTERFACE:${output_directory}>")
endfunction()

# The function looks for the enum and message definitions inside provided PROTO_FILES and returns
# list of the absolute .proto file paths, protobuf include paths and files that are expected to be
# generated by qtprotobufgen.
# Optional arguments:
#   GENERATE_PACKAGE_SUBFOLDERS - generated files will be located in package-base subdirectories.
#
# Multi-value arguments:
#   PROTO_FILES - input list of the proto files. May contain either absolute or relative paths.
function(_qt_internal_protobuf_preparse_proto_files
    out_proto_files out_proto_includes out_generated_files base_dir)

    cmake_parse_arguments(arg "GENERATE_PACKAGE_SUBFOLDERS" "" "PROTO_FILES" ${ARGN})

    unset(proto_files)
    unset(proto_includes)
    unset(output_files)
    foreach(f IN LISTS arg_PROTO_FILES)
        if(NOT IS_ABSOLUTE "${f}")
            set(f "${base_dir}/${f}")
            get_filename_component(f "${f}" ABSOLUTE)
        endif()
        get_filename_component(f "${f}" REALPATH)
        list(APPEND proto_files "${f}")

        _qt_internal_preparse_proto_file_common(result proto_package "${f}" "message;enum")
        if(NOT result)
            message(STATUS "No messages or enums found in ${f}. Skipping.")
            return()
        endif()

        get_filename_component(proto_file_base_dir "${f}" DIRECTORY)
        list(PREPEND proto_includes "${proto_file_base_dir}")

        string(REPLACE "." "/" package_full_path "${proto_package}")
        set(folder_path "")
        if(arg_GENERATE_PACKAGE_SUBFOLDERS)
            set(folder_path "${package_full_path}/")
        endif()

        get_filename_component(basename "${f}" NAME_WLE)
        list(APPEND output_files
            "${folder_path}${basename}.qpb.h"
            "${folder_path}${basename}.qpb.cpp"
            "${folder_path}${basename}_protobuftyperegistrations.cpp"
        )
    endforeach()
    list(REMOVE_DUPLICATES proto_files)
    list(REMOVE_DUPLICATES proto_includes)
    list(REMOVE_DUPLICATES output_files)

    set(${out_proto_files} "${proto_files}" PARENT_SCOPE)
    set(${out_proto_includes} "${proto_includes}" PARENT_SCOPE)
    set(${out_generated_files} "${output_files}" PARENT_SCOPE)
endfunction()

# TODO Qt6:
#     - Collect PROTO_INCLUDES from the LINK_LIBRARIES property of TARGET
#     - Collect proto files from the source files of the ${TARGET}

# This function is currently in Technical Preview
# Its signature and behavior might change.
function(qt6_add_protobuf target)
    _qt_internal_get_protoc_common_options(protoc_option_opt protoc_single_opt protoc_multi_opt)
    _qt_internal_get_protoc_generate_arguments(protoc_option_arg protoc_single_arg protoc_multi_arg)

    set(option_args
        ${protoc_option_opt}
        ${protoc_option_arg}
    )

    set(single_args
        ${protoc_single_opt}
        ${protoc_single_arg}
    )

    set(multi_args
        ${protoc_multi_opt}
        ${protoc_multi_arg}
    )
    cmake_parse_arguments(arg "${option_args}" "${single_args}" "${multi_args}" ${ARGN})

    _qt_internal_get_protoc_options(generation_options arg
        protoc_option_opt protoc_single_opt protoc_multi_opt)

    if(arg_PROTO_FILES_BASE_DIR)
        set(base_dir "${arg_PROTO_FILES_BASE_DIR}")
    else()
        set(base_dir "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
    unset(extra_pre_parse_options)
    if(arg_GENERATE_PACKAGE_SUBFOLDERS)
        list(APPEND extra_pre_parse_options "GENERATE_PACKAGE_SUBFOLDERS")
    endif()
    _qt_internal_protobuf_preparse_proto_files(proto_files proto_includes generated_files
        "${base_dir}"
        ${extra_pre_parse_options}
        PROTO_FILES
            ${arg_PROTO_FILES}
    )
    if(arg_PROTO_INCLUDES)
        list(APPEND proto_includes ${arg_PROTO_INCLUDES})
    endif()

    if(NOT TARGET ${target})
        _qt_internal_add_library(${target})
        if(DEFINED arg_OUTPUT_TARGETS)
            list(APPEND ${arg_OUTPUT_TARGETS} "${target}")
        endif()
    endif()

    foreach(f ${proto_files})
        _qt_internal_expose_source_file_to_ide(${target} ${f})
    endforeach()

    set(is_shared FALSE)
    set(is_static FALSE)
    set(is_executable FALSE)
    get_target_property(target_type ${target} TYPE)
    if(target_type STREQUAL "SHARED_LIBRARY" OR target_type STREQUAL "MODULE_LIBRARY")
        set(is_shared TRUE)
    elseif(target_type STREQUAL "STATIC_LIBRARY")
        set(is_static TRUE)
    elseif(target_type STREQUAL "EXECUTABLE")
        set(is_executable TRUE)
    else()
        message(FATAL_ERROR "Unsupported target type '${target_type}'.")
    endif()

    if(is_static OR is_shared)
        # Add EXPORT_MACRO if the target is, or we will create, a shared library
        string(TOUPPER "${target}" target_upper)
        if (is_shared)
            list(APPEND generation_options "EXPORT_MACRO=${target_upper}")
        endif()
        # Define this so we can conditionally set the export macro
        target_compile_definitions(${target}
            PRIVATE "QT_BUILD_${target_upper}_LIB")
    endif()

    set(output_directory "${CMAKE_CURRENT_BINARY_DIR}")
    if(DEFINED arg_OUTPUT_DIRECTORY)
        set(output_directory "${arg_OUTPUT_DIRECTORY}")
    endif()

    list(TRANSFORM generated_files PREPEND "${output_directory}/")

    _qt_internal_protoc_generate(${target} qtprotobufgen "${output_directory}"
        PROTO_FILES ${proto_files}
        PROTO_INCLUDES ${proto_includes}
        GENERATED_FILES ${generated_files}
        OPTIONS ${generation_options}
    )

    # Filter generated headers
    unset(generated_headers)
    unset(generated_sources)
    unset(generated_typeregistrations)
    foreach(generated_file IN LISTS generated_files)
        get_filename_component(extension "${generated_file}" LAST_EXT)
        if(extension STREQUAL ".h")
            list(APPEND generated_headers "${generated_file}")
        elseif(generated_file MATCHES "_protobuftyperegistrations.cpp$")
            list(APPEND generated_typeregistrations "${generated_file}")
        elseif(extension STREQUAL ".cpp")
            list(APPEND generated_sources "${generated_file}")
        endif()
    endforeach()

    target_sources(${target} PRIVATE ${generated_headers} ${generated_sources})

    if(is_static OR is_shared)
        set_target_properties(${target}
            PROPERTIES
                AUTOMOC ON
        )
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target}
            PRIVATE "/Zc:__cplusplus" "/permissive-" "/bigobj")
    endif()

    target_link_libraries(${target} PRIVATE
        ${QT_CMAKE_EXPORT_NAMESPACE}::Protobuf
    )

    if(is_static OR (WIN32 AND NOT is_executable))
        if(TARGET ${target}_protobuf_registration)
            target_sources(${target}_protobuf_registration PRIVATE ${generated_typeregistrations})
        else()
            add_library(${target}_protobuf_registration OBJECT ${generated_typeregistrations})
            target_link_libraries(${target}
                INTERFACE "$<TARGET_OBJECTS:$<TARGET_NAME:${target}_protobuf_registration>>")
            add_dependencies(${target} ${target}_protobuf_registration)

            target_include_directories(${target}_protobuf_registration
                PRIVATE "$<TARGET_PROPERTY:${target},INCLUDE_DIRECTORIES>")
            target_link_libraries(${target}_protobuf_registration
                PRIVATE
                    ${QT_CMAKE_EXPORT_NAMESPACE}::Platform
                    ${QT_CMAKE_EXPORT_NAMESPACE}::Protobuf
                    $<GENEX_EVAL:$<TARGET_PROPERTY:${target},LINK_LIBRARIES>>
            )

            if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
                target_compile_options(${target}_protobuf_registration
                    PRIVATE "/Zc:__cplusplus" "/permissive-" "/bigobj")
            endif()
        endif()
        if(DEFINED arg_OUTPUT_TARGETS)
            list(APPEND ${arg_OUTPUT_TARGETS}
                "${target}_protobuf_registration")
        endif()
    else()
        target_sources(${target} PRIVATE ${generated_typeregistrations})
    endif()

    if(DEFINED arg_OUTPUT_HEADERS)
        set(${arg_OUTPUT_HEADERS} "${generated_headers}" PARENT_SCOPE)
    endif()

    if(DEFINED arg_OUTPUT_TARGETS)
        set(${arg_OUTPUT_TARGETS} "${${arg_OUTPUT_TARGETS}}" PARENT_SCOPE)
    endif()
endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    function(qt_add_protobuf)
        if(QT_DEFAULT_MAJOR_VERSION EQUAL 6)
            qt6_add_protobuf(${ARGN})
        else()
            message(FATAL_ERROR "qt6_add_protobuf() is only available in Qt 6. "
                                "Please check the protobuf documentation for alternatives.")
        endif()
    endfunction()
endif()

# The common parsing function looking for the 'lookup_keys' definitions inside the 'proto_file'.
# The function sets the 'out_result' variable to true if one of 'lookup_keys' is found. Also the
# function writes to the 'out_package' variable the package name that the .proto file belongs to.
function(_qt_internal_preparse_proto_file_common out_result out_package proto_file lookup_keys)
    if(NOT proto_file OR NOT EXISTS "${proto_file}")
        message(FATAL_ERROR "Unable to scan '${proto_file}': file doesn't exist.")
    endif()

    file(READ "${proto_file}" file_content)
    if(NOT file_content)
        message(FATAL_ERROR "Unable to read ${proto_file}, or file is empty.")
    endif()

    string(REPLACE "[" "" file_content "${file_content}")
    string(REPLACE "]" "" file_content "${file_content}")
    string(REPLACE ";" "[[;]]" file_content "${file_content}")
    string(REGEX REPLACE "([^\t \n]+[\t ]*)}" "\\1;}" file_content "${file_content}")
    string(REGEX REPLACE "{([\t ]*[^\t \n]+)" "{;\\1" file_content "${file_content}")
    string(REPLACE "\n" ";" file_content "${file_content}")
    set(proto_key_common_regex "[\t ]+([a-zA-Z0-9_]+)")

    set(unclosed_braces 0)
    set(in_message_scope FALSE)

    set(found_key FALSE)
    list(JOIN lookup_keys "|" lookup_keys_regex)
    foreach(item IN LISTS file_content)
        if(item MATCHES "^[\t ]*package[\t ]+([a-zA-Z0-9_.-]+)")
            set(proto_package "${CMAKE_MATCH_1}")
        elseif(item MATCHES "^[\t ]*(${lookup_keys_regex})${proto_key_common_regex}")
            set(found_key TRUE)
            break()
        endif()
        if(in_message_scope)
            if(item MATCHES "[^/]*\\{")
                math(EXPR unclosed_braces "${unclosed_braces} + 1")
            endif()
            if(item MATCHES "[^/]*\\}")
                math(EXPR unclosed_braces "${unclosed_braces} - 1")
                if(unclosed_braces EQUAL 0)
                    set(in_message_scope FALSE)
                endif()
            endif()
        endif()
    endforeach()

    set(${out_package} "${proto_package}" PARENT_SCOPE)
    set(${out_result} "${found_key}" PARENT_SCOPE)
endfunction()
