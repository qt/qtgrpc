// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtProtobuf/qprotobufjsonserializer.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/qtenvironmentvariables.h>

#include <QtTest/qtest.h>

class tst_protobuf_recursion_limit_env : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void environmentVariable();
    void environmentVariableIsCached();
    void setterOverridesEnvironmentVariable();
};

void tst_protobuf_recursion_limit_env::initTestCase()
{
    // The variable is read only once per process, the first time the limit is
    // needed. Set it here, so every test can run alone and in any order.
    qputenv("QT_PROTOBUF_NESTING_LIMIT", "3");
}

void tst_protobuf_recursion_limit_env::cleanupTestCase()
{
    qunsetenv("QT_PROTOBUF_NESTING_LIMIT");
}

void tst_protobuf_recursion_limit_env::environmentVariable()
{
    QProtobufSerializer binary;
    QCOMPARE(binary.deserializationDepthLimit(), 3);
    QProtobufJsonSerializer json;
    QCOMPARE(json.deserializationDepthLimit(), 3);
}

void tst_protobuf_recursion_limit_env::environmentVariableIsCached()
{
    QProtobufSerializer serializer;
    QCOMPARE(serializer.deserializationDepthLimit(), 3);

    // The value is already cached, so changing the variable now has no effect.
    qputenv("QT_PROTOBUF_NESTING_LIMIT", "50");
    QCOMPARE(serializer.deserializationDepthLimit(), 3);
    QProtobufSerializer other;
    QCOMPARE(other.deserializationDepthLimit(), 3);
}

void tst_protobuf_recursion_limit_env::setterOverridesEnvironmentVariable()
{
    QProtobufSerializer serializer;
    QCOMPARE(serializer.deserializationDepthLimit(), 3);
    serializer.setDeserializationDepthLimit(200);
    QCOMPARE(serializer.deserializationDepthLimit(), 200);

    // Other instances still use the environment variable.
    QProtobufJsonSerializer other;
    QCOMPARE(other.deserializationDepthLimit(), 3);
}

QTEST_MAIN(tst_protobuf_recursion_limit_env)
#include "tst_protobuf_recursion_limit_env.moc"
