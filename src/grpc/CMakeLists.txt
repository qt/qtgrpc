# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Handle the conditional finding of either system zlib or qt zlib.
if(NOT QT_FEATURE_system_zlib)
    find_package(Qt6 COMPONENTS ZlibPrivate)
elseif(NOT TARGET WrapZLIB::WrapZLIB)
    # ZLIB::ZLIB could already come from another directory scope (e.g. Protobuf) so
    # avoid promoting it to GLOBAL here, which CMake forbids outside that scope.
    if(TARGET ZLIB::ZLIB)
        qt_internal_disable_find_package_global_promotion(ZLIB::ZLIB)
    endif()
    qt_find_package(WrapZLIB PROVIDED_TARGETS WrapZLIB::WrapZLIB)
endif()

qt_internal_add_module(Grpc
    SOURCES
        qgrpcoperation.h qgrpcoperation_p.h qgrpcoperation.cpp
        qgrpcoperationcontext.h qgrpcoperationcontext_p.h qgrpcoperationcontext.cpp
        qgrpccallreply.h qgrpccallreply.cpp
        qgrpcstream.h qgrpcstream.cpp
        qgrpcstatus.h qgrpcstatus.cpp
        qabstractgrpcchannel.h qabstractgrpcchannel_p.h qabstractgrpcchannel.cpp
        qgrpchttp2channel.h qgrpchttp2channel.cpp
        qgrpcclientbase.h qgrpcclientbase.cpp
        qgrpccommonoptions_p.h qgrpccommonoptions.cpp
        qgrpccalloptions.h qgrpccalloptions.cpp
        qgrpcchanneloptions.h qgrpcchanneloptions.cpp
        qtgrpcglobal.h
        qtgrpclogging_p.h qtgrpclogging.cpp
        qtgrpcnamespace.h qtgrpcnamespace.cpp
        qgrpcserializationformat.h qgrpcserializationformat.cpp
        qgrpcinterceptor.h qgrpcinterceptor.cpp
        qgrpcinterceptorchain.h qgrpcinterceptorchain_p.h qgrpcinterceptorchain.cpp
        qgrpcinterceptorengine_p.h
        compat/removed_api.cpp
    LIBRARIES
        Qt::CorePrivate
        Qt::NetworkPrivate
        Qt::ProtobufPrivate
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Protobuf
        Qt::Network
    NO_PCH_SOURCES
        compat/removed_api.cpp
    QT_LICENSE_ID QT_COMMERCIAL_OR_GPL3
)

qt_internal_extend_target(Grpc CONDITION QT_FEATURE_system_zlib
    LIBRARIES
        WrapZLIB::WrapZLIB
)

qt_internal_extend_target(Grpc CONDITION NOT QT_FEATURE_system_zlib
    LIBRARIES
        Qt::ZlibPrivate
)

qt_internal_add_docs(Grpc
    doc/qtgrpc.qdocconf
)

# TODO: Remove this and fix it in qtbase's qt_internal_find_tool directly
if(QT_HOST_PATH AND Qt6::qtgrpcgen IN_LIST Qt6GrpcTools_TARGETS AND NOT QT_FORCE_BUILD_TOOLS)
    qt_record_extra_qt_main_tools_package_dependency(Grpc GrpcTools "${PROJECT_VERSION}")
endif()
