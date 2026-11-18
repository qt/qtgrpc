// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/qregularexpression.h>
#include <QtCore/qtenvironmentvariables.h>

#include <QtTest/qtest.h>

using namespace Qt::StringLiterals;

class tst_protobuf_recursion_limit_env_invalid : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void invalidValueIsIgnored();
};

void tst_protobuf_recursion_limit_env_invalid::invalidValueIsIgnored()
{
    // The variable is read only once per process, so this value is used for
    // the whole test. The warning is printed once, when the value is read.
    qputenv("QT_PROTOBUF_NESTING_LIMIT", "-5");
    auto guard = qScopeGuard([]() {
        qunsetenv("QT_PROTOBUF_NESTING_LIMIT");
    });

    QProtobufSerializer serializer;
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(u"invalid value"_s));
    QCOMPARE(serializer.deserializationDepthLimit(), 100);
    QCOMPARE(serializer.deserializationDepthLimit(), 100);
}

QTEST_MAIN(tst_protobuf_recursion_limit_env_invalid)
#include "tst_protobuf_recursion_limit_env_invalid.moc"
