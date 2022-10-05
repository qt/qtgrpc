# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

function(qt_internal_add_protobuf_module target)
    qt_internal_get_internal_add_module_keywords(module_option_args module_single_args
        module_multi_args)

    set(protobuf_module_single_args TYPE_REGISTRATION_FUNCTION)

    set(option_args ${module_option_args})
    set(single_args ${module_single_args} ${protobuf_module_single_args})
    set(multi_args ${module_multi_args})

    qt_parse_all_arguments(arg "qt_internal_add_protobuf_module"
        "${option_args}"
        "${single_args}"
        "${multi_args}"
        ${ARGN}
    )

    qt_remove_args(module_args
        ARGS_TO_REMOVE
            ${protobuf_module_single_args}
        ALL_ARGS
            ${option_args}
            ${single_args}
            ${multi_args}
        ARGS
            ${ARGN}
    )

    qt_internal_add_module(${target} ${module_args})
    get_target_property(target_type ${target} TYPE)
    if(arg_TYPE_REGISTRATION_FUNCTION)
        set(registration_file
            "${CMAKE_CURRENT_BINARY_DIR}/${target}_protobuf_module_registration.cpp")
        set(content "#include <QtProtobuf/qtprotobufglobal.h>\n\
QT_BEGIN_NAMESPACE\n\
extern Q_DECL_IMPORT void ${arg_TYPE_REGISTRATION_FUNCTION}();\n\
Q_CONSTRUCTOR_FUNCTION(${arg_TYPE_REGISTRATION_FUNCTION})\n\
QT_END_NAMESPACE\n")
        qt_internal_get_main_cmake_configuration(main_config)
        file(GENERATE
            OUTPUT "${registration_file}"
            CONTENT "${content}"
            CONDITION "$<CONFIG:${main_config}>")
        if(target_type STREQUAL "STATIC_LIBRARY"
            OR (WIN32 AND NOT target_type STREQUAL "EXECUTABLE"))
            set(object_library ${target}_protobuf_module_registration)

            add_library(${object_library} OBJECT ${registration_file})
            qt_internal_link_internal_platform_for_object_library(${object_library})
            _qt_internal_copy_dependency_properties(${object_library}
                ${target} PRIVATE_ONLY)

            target_link_libraries(${target}
                INTERFACE "$<TARGET_OBJECTS:$<TARGET_NAME:${object_library}>>")
            add_dependencies(${target} ${object_library})

            qt_install(TARGETS ${object_library}
                EXPORT "${INSTALL_CMAKE_NAMESPACE}${target}Targets"
                DESTINATION "${INSTALL_LIBDIR}"
            )

            qt_internal_add_targets_to_additional_targets_export_file(
                TARGETS ${object_library}
                EXPORT_NAME_PREFIX "${INSTALL_CMAKE_NAMESPACE}${target}"
            )
        else()
            target_sources(${target} PRIVATE "${registration_file}")
        endif()
    endif()
endfunction()
