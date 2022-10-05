# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# The include is needed by Qt::Protobuf tests that use qt6_add_protobuf call to
# generate the code of protobuf messages for testing.
include("${CMAKE_CURRENT_SOURCE_DIR}/${QT_CMAKE_EXPORT_NAMESPACE}ProtobufToolsMacros.cmake")

qt_get_tool_target_name(target_name qtprotobufgen)
qt_internal_add_tool(${target_name}
    TARGET_DESCRIPTION "Qt protobuf generator"
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    TOOLS_TARGET Protobuf
    CORE_LIBRARY None
    SOURCES
        utils.cpp utils.h
        descriptorprinterbase.h
        main.cpp
        generatorcommon.cpp generatorcommon.h
        options.cpp options.h
        templates.cpp templates.h
        generatorbase.cpp generatorbase.h
        singlefilegenerator.cpp singlefilegenerator.h
        baseprinter.cpp baseprinter.h
        messagedeclarationprinter.cpp messagedeclarationprinter.h
        messagedefinitionprinter.cpp messagedefinitionprinter.h
        enumdeclarationprinter.cpp enumdeclarationprinter.h
        enumdefinitionprinter.cpp enumdefinitionprinter.h
    EXTRA_CMAKE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/${QT_CMAKE_EXPORT_NAMESPACE}ProtobufToolsMacros.cmake"
        "${CMAKE_CURRENT_SOURCE_DIR}/QtProtocCommandWrapper.cmake"
    EXTRA_CMAKE_INCLUDES
        "${QT_CMAKE_EXPORT_NAMESPACE}ProtobufToolsMacros.cmake"
    LIBRARIES
        WrapProtobuf::WrapLibProtobuf
        WrapProtobuf::WrapLibProtoc
)

qt_internal_return_unless_building_tools()

qt_record_extra_package_dependency(${target_name} WrapProtoc "")

if(MSVC)
    # Disable C4251:
    # `class 'type1' needs to have dll-interface to be used by clients of class 'type2'`
    # which can be generated from some of the protobuf headers on msvc.
    # This warning is also disabled globally in Qt in QtCore/qcompilerdetection.h
    target_compile_options(${target_name}
        PRIVATE "/wd4251"
    )
    target_compile_definitions(${target_name}
        PRIVATE "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
    )
endif()
