# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(__qt_protobuf_macros_module_base_dir "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")

# TODO Qt6:
#     - Collect PROTO_INCLUDES from the LINK_LIBRARIES property of TARGET
#     - Collect proto files from the source files of the ${TARGET}

# This function is currently in Technical Preview
# Its signature and behavior might change.
function(qt6_add_protobuf target)
    set(options COPY_COMMENTS GENERATE_PACKAGE_SUBFOLDERS)
    set(oneValueArgs
        OUTPUT_DIRECTORY
        EXTRA_NAMESPACE
        PROTO_FILES_BASE_DIR
        OUTPUT_HEADERS
        OUTPUT_TARGETS
    )
    set(multiValueArgs PROTO_FILES PROTO_INCLUDES)
    cmake_parse_arguments(arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    unset(proto_files)
    foreach(proto_file IN LISTS arg_PROTO_FILES)
        if(NOT IS_ABSOLUTE "${proto_file}")
            if(arg_PROTO_FILES_BASE_DIR)
                set(proto_file "${arg_PROTO_FILES_BASE_DIR}/${proto_file}")
            endif()
            get_filename_component(proto_file "${proto_file}" ABSOLUTE)
        endif()
        get_filename_component(proto_file "${proto_file}" REALPATH)
        list(APPEND proto_files "${proto_file}")
    endforeach()
    list(REMOVE_DUPLICATES proto_files)

    if(DEFINED arg_OUTPUT_TARGETS)
        set(${arg_OUTPUT_TARGETS} "")
    endif()

    if(DEFINED arg_OUTPUT_HEADERS)
        set(${arg_OUTPUT_HEADERS} "")
    endif()

    if("${proto_files}" STREQUAL "")
        message(FATAL_ERROR "PROTO_FILES list is empty.")
    endif()

    string(TOUPPER "${target}" target_upper)

    set(generation_options "")
    set(extra_pre_parse_options "")

    if(arg_COPY_COMMENTS)
        list(APPEND generation_options "COPY_COMMENTS")
    endif()

    if(arg_GENERATE_PACKAGE_SUBFOLDERS)
        list(APPEND generation_options "GENERATE_PACKAGE_SUBFOLDERS")
        list(APPEND extra_pre_parse_options "GENERATE_PACKAGE_SUBFOLDERS")
    endif()

    if(arg_EXTRA_NAMESPACE)
        list(APPEND generation_options "EXTRA_NAMESPACE=${arg_EXTRA_NAMESPACE}")
    endif()

    if(TARGET ${target})
        get_target_property(target_type ${target} TYPE)
    elseif(BUILD_SHARED_LIBS)
        set(target_type "SHARED_LIBRARY")
    else()
        set(target_type "STATIC_LIBRARY")
    endif()
    # Add EXPORT_MACRO if the target is, or we will create, a shared library
    if (target_type STREQUAL SHARED_LIBRARY)
        list(APPEND generation_options "EXPORT_MACRO=${target_upper}")
    endif()

    set(proto_includes "")
    foreach(proto_include IN LISTS arg_PROTO_INCLUDES)
        get_filename_component(proto_include "${proto_include}" REALPATH)
        list(APPEND proto_includes "${proto_include}")
    endforeach()

    unset(generated_files)
    foreach(proto_file IN LISTS proto_files)
        get_filename_component(proto_file_base_dir "${proto_file}" DIRECTORY)
        list(PREPEND proto_includes "-I${proto_file_base_dir}")
        _qt_internal_preparse_proto_file(preparsed_files "${proto_file}" ${extra_pre_parse_options})
        list(APPEND generated_files ${preparsed_files})
    endforeach()

    if(NOT generated_files)
        message(FATAL_ERROR
            "Unable to get the list of generated sources for target '${target}'")
    endif()

    list(REMOVE_DUPLICATES generated_files)

    if(NOT DEFINED arg_OUTPUT_DIRECTORY OR "${arg_OUTPUT_DIRECTORY}" STREQUAL "")
        set(out_dir ${CMAKE_CURRENT_BINARY_DIR})
    else()
        set(out_dir ${arg_OUTPUT_DIRECTORY})
    endif()

    get_filename_component(out_dir "${out_dir}" REALPATH)

    file(MAKE_DIRECTORY ${out_dir})

    list(TRANSFORM generated_files PREPEND "${out_dir}/")

    set_source_files_properties(${generated_files} PROPERTIES
        GENERATED TRUE
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

    unset(num_deps)
    if(TARGET ${target})
        get_target_property(num_deps ${target} _qt_protobufgen_deps_num)
    endif()
    if(NOT num_deps)
        set(num_deps 0)
    endif()
    set(deps_target ${target}_protobufgen_deps_${num_deps})
    math(EXPR num_deps "${num_deps} + 1")

    set(qtprotobufgen_file $<TARGET_FILE:${QT_CMAKE_EXPORT_NAMESPACE}::qtprotobufgen>)

    list(REMOVE_DUPLICATES proto_includes)
    list(JOIN proto_files "\\$<SEMICOLON>"  proto_files_string)
    list(JOIN proto_includes "\\$<SEMICOLON>"  proto_includes_string)
    list(JOIN generation_options "\\\\$<SEMICOLON>" generation_options_string)
    string(JOIN "\\$<SEMICOLON>" protoc_arguments
        "--plugin=protoc-gen-qtprotobufgen=${qtprotobufgen_file}"
        "--qtprotobufgen_out=${out_dir}"
        "--qtprotobufgen_opt=${generation_options_string}"
        "${proto_files_string}"
        "${proto_includes_string}"
    )
    add_custom_command(OUTPUT ${generated_files}
        COMMAND ${CMAKE_COMMAND}
            -DPROTOC_EXECUTABLE=$<TARGET_FILE:WrapProtoc::WrapProtoc>
            -DPROTOC_ARGS=${protoc_arguments}
            -DWORKING_DIRECTORY=${out_dir}
            -P
            ${__qt_protobuf_macros_module_base_dir}/QtProtocCommandWrapper.cmake
        WORKING_DIRECTORY ${out_dir}
        DEPENDS
            ${QT_CMAKE_EXPORT_NAMESPACE}::qtprotobufgen
            ${proto_files}
        COMMENT "Generating QtProtobuf ${target} sources..."
        COMMAND_EXPAND_LISTS
        VERBATIM
    )
    add_custom_target(${deps_target} DEPENDS ${generated_files})

    if(TARGET ${target})
        target_sources(${target} PRIVATE ${generated_headers} ${generated_sources})
    else()
        if(DEFINED arg_OUTPUT_TARGETS)
            list(APPEND arg_OUTPUT_TARGETS "${target}")
        endif()
        _qt_internal_add_library(${target} ${generated_headers} ${generated_sources})
    endif()
    set_property(TARGET ${target} APPEND PROPERTY
        AUTOGEN_TARGET_DEPENDS "${deps_target}")

    set_property(TARGET ${target} PROPERTY _qt_protobufgen_deps_num "${num_deps}")

    set_target_properties(${target}
        PROPERTIES
            PUBLIC_HEADER "${generated_headers}"
            AUTOMOC ON
    )
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target}
            PRIVATE "/Zc:__cplusplus" "/permissive-" "/bigobj")
    endif()
    if(NOT target_type STREQUAL "EXECUTABLE")
        # Define this so we can conditionally set the export macro
        target_compile_definitions(${target}
            PRIVATE "QT_BUILD_${target_upper}_LIB")
    endif()


    target_link_libraries(${target} PRIVATE
        ${QT_CMAKE_EXPORT_NAMESPACE}::Protobuf
    )

    if(target_type STREQUAL "STATIC_LIBRARY" OR (WIN32 AND NOT target_type STREQUAL "EXECUTABLE"))
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
                    $<TARGET_PROPERTY:${target},LINK_LIBRARIES>
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

    target_include_directories(${target} PUBLIC "$<BUILD_INTERFACE:${out_dir}>")

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

function(_qt_internal_preparse_proto_file out_generated_files proto_file)
    cmake_parse_arguments(arg "GENERATE_PACKAGE_SUBFOLDERS" "" "" ${ARGN})

    if(NOT proto_file OR NOT EXISTS "${proto_file}")
        message(FATAL_ERROR "Unable to scan '${proto_file}': file doesn't exist.")
    endif()

    file(READ "${proto_file}" file_content)
    if(NOT file_content)
        message(FATAL_ERROR "Unable to read ${proto_file}, or file is empty.")
    endif()

    string(REPLACE ";" "[[;]]" file_content "${file_content}")
    string(REGEX REPLACE "([^\t \n]+[\t ]*)}" "\\1;}" file_content "${file_content}")
    string(REGEX REPLACE "{([\t ]*[^\t \n]+)" "{;\\1" file_content "${file_content}")
    string(REPLACE "\n" ";" file_content "${file_content}")
    set(proto_key_common_regex "[\t ]+([a-zA-Z0-9_]+)")

    set(unclosed_braces 0)
    set(in_message_scope FALSE)

    foreach(item IN LISTS file_content)
        if(item MATCHES "^[\t ]*package[\t ]+([a-zA-Z0-9_.-]+)")
            set(proto_package "${CMAKE_MATCH_1}")
        elseif(item MATCHES "^[\t ]*message${proto_key_common_regex}")
            # Skip adding nested messages to the list.
            if(unclosed_braces EQUAL 0)
                list(APPEND proto_messages "${CMAKE_MATCH_1}")
                set(in_message_scope TRUE)
            endif()
        elseif(item MATCHES "^[\t ]*service${proto_key_common_regex}")
            list(APPEND proto_services "${CMAKE_MATCH_1}")
        elseif(item MATCHES "^[\t ]*enum${proto_key_common_regex}")
            list(APPEND proto_enums "${CMAKE_MATCH_1}")
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

    unset(output_files)
    string(REPLACE "." "/" package_full_path "${proto_package}")
    get_filename_component(basename "${proto_file}" NAME_WLE)

    set(folder_path "")
    if(arg_GENERATE_PACKAGE_SUBFOLDERS)
        set(folder_path "${package_full_path}/")
    endif()
    if(proto_messages OR proto_enums)
        list(APPEND output_files
            "${folder_path}${basename}.qpb.h"
            "${folder_path}${basename}.qpb.cpp"
            "${folder_path}${basename}_protobuftyperegistrations.cpp"
        )
    endif()
    if(proto_services)
        if(QT_FEATURE_native_grpc)
            list(APPEND output_files
                "${folder_path}${basename}_service.grpc.qpb.h"
            )
        endif()
        list(APPEND output_files
            "${folder_path}${basename}_client.grpc.qpb.h"
            "${folder_path}${basename}_client.grpc.qpb.cpp"
        )
    endif()
    set(${out_generated_files} "${output_files}" PARENT_SCOPE)
endfunction()
