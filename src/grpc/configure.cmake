# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_find_package(WrapgRPC PROVIDED_TARGETS WrapgRPC::WrapLibgRPC MODULE_NAME
                global)

qt_feature("grpc" PUBLIC
  SECTION "Utilities"
  LABEL "gRPC support"
  PURPOSE "Provides support for serializing and deserializing gRPC services using Qt."
  CONDITION TARGET Qt6::Network)

qt_feature("native_grpc" PUBLIC
    SECTION "Utilities"
    LABEL "Native gRPC support"
    PURPOSE "Provides native implementation for gRPC channels and client and service code generation."
    CONDITION TARGET WrapgRPC::WrapLibgRPC
)

qt_configure_add_summary_section(NAME "QtGrpc")
qt_configure_add_summary_entry(ARGS "grpc")
qt_configure_add_summary_entry(ARGS "native_grpc")
qt_configure_end_summary_section()
