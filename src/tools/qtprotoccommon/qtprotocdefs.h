// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef QTPROTOCDEFS_H
#define QTPROTOCDEFS_H

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION >= 3012000
// Idicates the major libprotoc API update that happened in version 3.12
#  define HAVE_PROTOBUF_SYNC_PIPER
#endif
#include <google/protobuf/port_undef.inc>

#endif // QTPROTOCDEFS_H
