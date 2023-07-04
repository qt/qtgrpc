# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_find_package(WrapgRPC
    PROVIDED_TARGETS WrapgRPC::WrapLibgRPC
    MODULE_NAME global
)

qt_feature("grpc" PUBLIC
    SECTION "Utilities"
    LABEL "gRPC support"
    PURPOSE "Provides support for serializing and deserializing gRPC services using Qt."
    CONDITION TARGET Qt6::Network
)

qt_feature("qtgrpcgen" PRIVATE
    SECTION "Utilities"
    LABEL "Qt GRPC generator"
    PURPOSE "Provides support for generating Qt-based gRPC services."
    DISABLE NOT QT_FEATURE_grpc
    CONDITION TARGET WrapProtobuf::WrapLibProtoc AND TARGET WrapProtobuf::WrapLibProtobuf AND
        TARGET WrapProtoc::WrapProtoc AND TEST_libprotobuf AND TEST_libprotoc
)

# If CMAKE_CPP_LINK_PIE_SUPPORTED is set we build Qt libraries with -fPIE enabled. In this case
# we need to have libgrpc++ and libprotobuf built with the -fPIE as well. Disabling the native_grpc
# feature in this case to avoid linker issues.
# TODO: Re-enable once QTBUG-111485 is fixed.
qt_feature("native_grpc" PUBLIC
    SECTION "Utilities"
    LABEL "Native gRPC support"
    DISABLE NOT QT_FEATURE_grpc
    PURPOSE "Provides native implementation for gRPC channels and client and service code generation."
    CONDITION TARGET WrapgRPC::WrapLibgRPC AND NOT CMAKE_CPP_LINK_PIE_SUPPORTED
)

qt_configure_add_summary_section(NAME "Qt GRPC")
qt_configure_add_summary_entry(ARGS "grpc")
qt_configure_add_summary_entry(ARGS "native_grpc")
qt_configure_end_summary_section()
qt_configure_add_summary_section(NAME "Qt GRPC tools")
qt_configure_add_summary_entry(ARGS "qtgrpcgen")
qt_configure_end_summary_section()
