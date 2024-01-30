// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QQMLGRPCSERIALIZATIONFORMAT_P_H
#define QQMLGRPCSERIALIZATIONFORMAT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGrpcQuick/qtgrpcquickexports.h>
#include <QtCore/qobjectdefs.h>
#include <QtQml/qqmlregistration.h>
#include <QtGrpc/qgrpcserializationformat.h>

QT_BEGIN_NAMESPACE

namespace QQmlSerializationFormat {
Q_NAMESPACE_EXPORT(Q_GRPCQUICK_EXPORT)
QML_NAMED_ELEMENT(GrpcSerializationFormat)
QML_ADDED_IN_VERSION(6, 8)

enum GrpcSerializationFormat : quint8 {
    Default = QGrpcSerializationFormat::Format::Default,
    Protobuf = QGrpcSerializationFormat::Format::Protobuf,
    Json = QGrpcSerializationFormat::Format::Json
};
Q_ENUM_NS(GrpcSerializationFormat)
};

QT_END_NAMESPACE

#endif // QQMLGRPCSERIALIZATIONFORMAT_P_H
