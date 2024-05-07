// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCDEFS_H
#define QGRPCDEFS_H

#include <QtGrpc/qtgrpcglobal.h>

#include <chrono>
#include <unordered_map>

#if 0
// Create a forwarding header
#  pragma qt_class(QGrpcMetadata)
#  pragma qt_class(QGrpcDuration)
#  pragma qt_sync_stop_processing
#endif

QT_BEGIN_NAMESPACE

class QDebug;

using QGrpcMetadata = std::unordered_multimap<QByteArray, QByteArray>;
using QGrpcDuration = std::chrono::milliseconds;

#ifndef QT_NO_DEBUG_STREAM
Q_GRPC_EXPORT QDebug operator<<(QDebug debug, const QGrpcMetadata &metadata);
#endif

QT_END_NAMESPACE

#endif // QGRPCDEFS_H
