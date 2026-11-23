// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtProtobuf/qprotobufjsonserializer.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/qtenvironmentvariables.h>

#include <QtTest/qtest.h>

#include "recursionlimit.qpb.h"

using namespace qtprotobufnamespace::tests::recursionlimit;

namespace {
constexpr int EnvLimit = 3;

// A message with `levels` nested messages in field 2.
QByteArray nested(int levels)
{
    QByteArray data;
    for (int i = 0; i < levels; ++i)
        data = QByteArray("\x12") + char(data.size()) + data;
    return data;
}

QByteArray nestedJson(int levels)
{
    QByteArray data = "{}";
    for (int i = 0; i < levels; ++i)
        data = "{\"child\":" + data + "}";
    return data;
}
} // namespace

class tst_protobuf_recursion_limit_env : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void environmentVariable();
    void environmentVariableIsCached();
};

void tst_protobuf_recursion_limit_env::initTestCase()
{
    // The variable is read only once per process, the first time the limit is
    // needed. Set it here, so every test can run alone and in any order.
    qputenv("QT_PROTOBUF_NESTING_LIMIT", QByteArray::number(EnvLimit));
}

void tst_protobuf_recursion_limit_env::cleanupTestCase()
{
    qunsetenv("QT_PROTOBUF_NESTING_LIMIT");
}

void tst_protobuf_recursion_limit_env::environmentVariable()
{
    QProtobufSerializer binary;
    Node node;
    QVERIFY(binary.deserialize(&node, nested(EnvLimit)));
    QCOMPARE(binary.lastError(), QAbstractProtobufSerializer::Error::None);
    QVERIFY(!binary.deserialize(&node, nested(EnvLimit + 1)));
    QCOMPARE(binary.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);

    QProtobufJsonSerializer json;
    QVERIFY(json.deserialize(&node, nestedJson(EnvLimit)));
    QCOMPARE(json.lastError(), QAbstractProtobufSerializer::Error::None);
    QVERIFY(!json.deserialize(&node, nestedJson(EnvLimit + 1)));
    QCOMPARE(json.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);
}

void tst_protobuf_recursion_limit_env::environmentVariableIsCached()
{
    QProtobufSerializer serializer;
    Node node;
    QVERIFY(!serializer.deserialize(&node, nested(EnvLimit + 1)));

    // The value is already cached, so changing the variable now has no effect.
    qputenv("QT_PROTOBUF_NESTING_LIMIT", "50");
    QVERIFY(!serializer.deserialize(&node, nested(EnvLimit + 1)));
    QProtobufSerializer other;
    QVERIFY(!other.deserialize(&node, nested(EnvLimit + 1)));
    QVERIFY(other.deserialize(&node, nested(EnvLimit)));
}

QTEST_MAIN(tst_protobuf_recursion_limit_env)
#include "tst_protobuf_recursion_limit_env.moc"
