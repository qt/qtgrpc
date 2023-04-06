// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QtQuickTest>

class TestSetup : public QObject
{
    Q_OBJECT
};

QUICK_TEST_MAIN_WITH_SETUP(tst_protobuf_enumtypes_qml, TestSetup)
#include "tst_protobuf_enumtypes_qml.moc"

