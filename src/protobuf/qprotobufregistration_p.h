// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFREGISTRATION_P_H
#define QPROTOBUFREGISTRATION_P_H

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

#include <QtProtobuf/qprotobufregistration.h>
#include <QtProtobuf/qtprotobufexports.h>

QT_BEGIN_NAMESPACE

namespace QtProtobufPrivate {
extern Q_PROTOBUF_EXPORT SerializationHandler findHandler(QMetaType type);
}

QT_END_NAMESPACE

#endif // QPROTOBUFREGISTRATION_P_H
