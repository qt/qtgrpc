# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_internal_add_module(Grpc
    SOURCES
        qgrpcoperation.h qgrpcoperation.cpp
        qgrpccallreply.h qgrpccallreply.cpp
        qgrpcstream.h qgrpcstream.cpp
        qgrpcstatus.h qgrpcstatus.cpp
        qabstractgrpcchannel.h qabstractgrpcchannel.cpp
        qgrpchttp2channel.h qgrpchttp2channel.cpp
        qabstractgrpcclient.h qabstractgrpcclient.cpp
        qabstractgrpccredentials.h qabstractgrpccredentials.cpp
        qgrpccredentials.h qgrpccredentials.cpp
        qgrpcinsecurecredentials.h qgrpcinsecurecredentials.cpp
        qgrpcuserpasswordcredentials.h qgrpcuserpasswordcredentials.cpp
        qtgrpcglobal.h qtgrpcglobal_p.h  qtgrpcglobal.cpp
    GENERATE_CPP_EXPORTS
    LIBRARIES
        Qt6::CorePrivate
    PUBLIC_LIBRARIES
        Qt6::Core
        Qt6::Protobuf
        Qt6::Network
)

qt_internal_extend_target(Grpc CONDITION QT_FEATURE_ssl
    SOURCES
        qgrpcsslcredentials.h  qgrpcsslcredentials.cpp
)

qt_internal_extend_target(Grpc CONDITION QT_FEATURE_native_grpc
    SOURCES
        qgrpcchannel.h qgrpcchannel_p.h qgrpcchannel.cpp
        qabstractgrpcservice.h qabstractgrpcservice.cpp
    PUBLIC_LIBRARIES
        WrapgRPC::WrapLibgRPC
)

qt_internal_add_docs(Grpc
    doc/qtgrpc.qdocconf
)
