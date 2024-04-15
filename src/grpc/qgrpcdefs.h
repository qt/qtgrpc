// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCDEFS_H
#define QGRPCDEFS_H

#include <QtGrpc/qtgrpcglobal.h>

#include <unordered_map>
#include <chrono>

#if 0
// Create a forwarding header
#pragma qt_class(QGrpcMetadata)
#pragma qt_class(QGrpcDuration)
#pragma qt_sync_stop_processing
#endif

QT_BEGIN_NAMESPACE

using QGrpcMetadata = std::unordered_multimap<QByteArray, QByteArray>;
using QGrpcDuration = std::chrono::milliseconds;

QT_END_NAMESPACE

#endif // QGRPCDEFS_H
