// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QtQuickTest>
#include <QtProtobuf/qtprotobuftypes.h>

QT_BEGIN_NAMESPACE
class QQmlEngine;
QT_END_NAMESPACE

class TestSetup : public QObject {
    Q_OBJECT

public slots:
    void qmlEngineAvailable(QQmlEngine *)
    {
        qRegisterProtobufTypes();
    }
};

QUICK_TEST_MAIN_WITH_SETUP(tst_protobuf_basictypes_qml, TestSetup)
#include "tst_protobuf_basictypes_qml.moc"
