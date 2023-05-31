# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# The function generates Qt classes from the .proto schema of the well-know types passed in 'ARGN'
# and adds the generated sources to the 'target'
function(qt_internal_add_protobuf_wellknown_types target)
    set(lookup_dirs "")
    if(TARGET WrapProtobuf::WrapLibProtobuf)
        get_target_property(lookup_dirs WrapProtobuf::WrapLibProtobuf
            _qt_internal_proto_include_dirs)
    endif()

    # It's not possible to generate all the well-known types in one chunk, because of complicated
    # dependencies between the .proto files. So we use separate qt_add_protobuf(protoc) calls for
    # them.
    set(generated_headers "")
    set(generated_targets "")
    foreach(type IN LISTS ARGN)
        set(proto_file_name "google/protobuf/${type}.proto")
        unset(proto_file_dir)
        find_path(proto_file_dir NAMES "${proto_file_name}" PATHS ${lookup_dirs} NO_CACHE)
        if(NOT proto_file_dir)
            message(AUTHOR_WARNING "The .proto schema ${proto_file_name} is not found. This can"
                " happen because an incompatible protobuf version is used. Please file a bug on"
                " https://bugreports.qt.io/, so we can adjust the implementation accordingly.")
            continue()
        endif()

        message(DEBUG "Adding well-known type ${proto_file_name}")

        set(proto_file "${proto_file_dir}/${proto_file_name}")
        qt6_add_protobuf(${target}
            PROTO_FILES "${proto_file}"
            PROTO_INCLUDES "${proto_file_dir}"
            OUTPUT_HEADERS type_generated_headers
            OUTPUT_TARGETS type_generated_targets
            GENERATE_PACKAGE_SUBFOLDERS
        )
        list(APPEND generated_headers ${type_generated_headers})
        list(APPEND generated_targets ${type_generated_targets})

        get_target_property(known_proto_includes ${target} QT_PROTO_INCLUDES)
        if(NOT known_proto_includes)
            set(known_proto_includes "")
        endif()
        list(APPEND known_proto_includes "${proto_file_dir}")
        list(REMOVE_DUPLICATES known_proto_includes)
        set_target_properties(${target} PROPERTIES
            QT_PROTO_INCLUDES "${known_proto_includes}")
    endforeach()

    if(generated_headers)
        # The generated header files need to be accessible by both the ${target} module include
        # path and the expected protobuf import path 'google/protobuf'. Instead of making copies,
        # let's create the simple aliases.
        qt_internal_module_info(module ${target})
        set(alias_headers "")
        foreach(header IN LISTS generated_headers)
            get_filename_component(file_name "${header}" NAME)
            set(alias_header "${module_build_interface_include_dir}/google/protobuf/${file_name}")
            qt_internal_generate_wellknown_header_alias(
                "${module_build_interface_include_dir}/${file_name}" "${alias_header}")
            list(APPEND alias_headers "${alias_header}")
        endforeach()
        qt_install(FILES ${alias_headers}
            DESTINATION "${module_install_interface_include_dir}/google/protobuf")
    endif()

    if(generated_targets)
        list(REMOVE_DUPLICATES generated_targets)
        qt_install(TARGETS ${generated_targets}
            EXPORT "${INSTALL_CMAKE_NAMESPACE}${target}Targets"
            DESTINATION "${INSTALL_LIBDIR}"
        )

        qt_internal_add_targets_to_additional_targets_export_file(
            TARGETS ${generated_targets}
            EXPORT_NAME_PREFIX "${INSTALL_CMAKE_NAMESPACE}${target}"
        )
    endif()

    set_property(TARGET ${target} APPEND PROPERTY EXPORT_PROPERTIES QT_PROTO_INCLUDES)
endfunction()

# The function generates the header 'alias_file' containing the include of the original
# 'header_file' by the relative file path.
function(qt_internal_generate_wellknown_header_alias header_file alias_file)
    get_filename_component(alias_directory "${alias_file}" DIRECTORY)
    get_filename_component(header_name "${header_file}" NAME)
    file(RELATIVE_PATH rel_path ${alias_directory} "${header_file}")
    set(content "#include \"${rel_path}\"")
    qt_configure_file(OUTPUT "${alias_file}" CONTENT "${content}")
endfunction()