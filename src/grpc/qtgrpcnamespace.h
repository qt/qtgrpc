// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QTGRPCNAMESPACE_H
#define QTGRPCNAMESPACE_H

#if 0
#pragma qt_class(QtGrpcNamespace)
#endif

#include <QtGrpc/qtgrpcexports.h>

#include <QtCore/qtmetamacros.h>

QT_BEGIN_NAMESPACE

struct QMetaObject;

namespace QtGrpc {
Q_NAMESPACE_EXPORT(Q_GRPC_EXPORT)

enum class SerializationFormat : quint8 {
    Default,
    Protobuf,
    Json,
};
Q_ENUM_NS(SerializationFormat)

Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
} // namespace QtGrpc

QT_END_NAMESPACE

#endif // QTGRPCNAMESPACE_H
