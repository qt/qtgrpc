// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "recursionlimit.qpb.h"
#ifdef HAS_PROTOBUF_WELLKNOWN_TYPES
#  include "recursionlimitwellknown.qpb.h"
#endif

#include <QtProtobuf/qprotobufjsonserializer.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <QtTest/qtest.h>

using namespace Qt::StringLiterals;
using namespace qtprotobufnamespace::tests::recursionlimit;

namespace {
constexpr int DefaultLimit = 100;

QByteArray varint(quint32 value)
{
    QByteArray result;
    while (value >= 0x80) {
        result.append(char((value & 0x7f) | 0x80));
        value >>= 7;
    }
    result.append(char(value));
    return result;
}

// A length-delimited field with the given tag byte.
QByteArray wrap(const QByteArray &inner, char tag = '\x12')
{
    return QByteArray(1, tag) + varint(quint32(inner.size())) + inner;
}

// A message with `levels` nested messages in field 2 (Node, RepeatedNode,
// OneofNode, MutualA/MutualB).
QByteArray nested(int levels)
{
    QByteArray data;
    for (int i = 0; i < levels; ++i)
        data = wrap(data);
    return data;
}

// Each hop is two levels of nesting: the map entry and its value.
QByteArray nestedMap(int hops)
{
    QByteArray data;
    for (int i = 0; i < hops; ++i)
        data = wrap(QByteArray("\x08\x01", 2) + wrap(data));
    return data;
}

QByteArray nestedJson(int levels)
{
    QByteArray data = "{}";
    for (int i = 0; i < levels; ++i)
        data = "{\"child\":" + data + "}";
    return data;
}

Node makeNode(int levels)
{
    Node node;
    for (int i = 0; i < levels; ++i) {
        Node parent;
        parent.setChild(node);
        node = parent;
    }
    return node;
}
} // namespace

class tst_protobuf_recursion_limit : public QObject
{
    Q_OBJECT
private Q_SLOTS:

    void binary_data();
    void binary();
    void binaryAfterError();
    void binaryMap_data();
    void binaryMap();

    void json_data();
    void json();
    void jsonAfterError();

    void serializeIsNotLimited();
#ifdef HAS_PROTOBUF_WELLKNOWN_TYPES
    void wellKnownTypes_data();
    void wellKnownTypes();
    void anyHandler_data();
    void anyHandler();
    void jsonCustomDeserializer_data();
    void jsonCustomDeserializer();
#endif
};

void tst_protobuf_recursion_limit::binary_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<int>("type"); // 0 Node, 1 RepeatedNode, 2 OneofNode, 3 MutualA
    QTest::addColumn<int>("limit");
    QTest::addColumn<bool>("ok");

    const char *names[] = { "Node", "RepeatedNode", "OneofNode", "MutualA" };
    for (int type = 0; type < 4; ++type) {
        for (int limit : { DefaultLimit }) {
            const auto name = [&](const char *what) {
                return QByteArray(names[type] + "-limit"_ba + QByteArray::number(limit) + '-'
                                  + what);
            };
            QTest::newRow(name("at-limit").constData())
                    << nested(limit) << type << limit << true;
            QTest::newRow(name("over-limit").constData())
                    << nested(limit + 1) << type << limit << false;
            QTest::newRow(name("far-over-limit").constData())
                    << nested(limit + 1000) << type << limit << false;
        }
    }
}

void tst_protobuf_recursion_limit::binary()
{
    QFETCH(const QByteArray, data);
    QFETCH(const int, type);
    QFETCH(const int, limit);
    QFETCH(const bool, ok);

    QProtobufSerializer serializer;

    bool result = false;
    switch (type) {
    case 0: { Node m; result = serializer.deserialize(&m, data); break; }
    case 1: { RepeatedNode m; result = serializer.deserialize(&m, data); break; }
    case 2: { OneofNode m; result = serializer.deserialize(&m, data); break; }
    case 3: { MutualA m; result = serializer.deserialize(&m, data); break; }
    }

    QCOMPARE(result, ok);
    if (ok) {
        QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
    } else {
        QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);
        QVERIFY(serializer.lastErrorString().contains(u"recursion limit of %1"_s.arg(limit)));
    }
}

void tst_protobuf_recursion_limit::binaryAfterError()
{
    // The nesting depth must not leak from a failed deserialization.
    QProtobufSerializer serializer;

    Node failed;
    QVERIFY(!serializer.deserialize(&failed, nested(DefaultLimit + 10)));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);

    for (int i : { 0, 3, DefaultLimit }) {
        Node ok;
        QVERIFY(serializer.deserialize(&ok, nested(i)));
        QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
    }
}

void tst_protobuf_recursion_limit::binaryMap_data()
{
    QTest::addColumn<int>("hops");
    QTest::addColumn<bool>("ok");

    QTest::newRow("at-limit") << DefaultLimit / 2 << true;
    QTest::newRow("over-limit") << DefaultLimit / 2 + 1 << false;
    QTest::newRow("deep") << DefaultLimit << false;
}

void tst_protobuf_recursion_limit::binaryMap()
{
    QFETCH(const int, hops);
    QFETCH(const bool, ok);

    QProtobufSerializer serializer;
    MapNode m;
    QCOMPARE(serializer.deserialize(&m, nestedMap(hops)), ok);
    QCOMPARE(serializer.lastError(),
             ok ? QAbstractProtobufSerializer::Error::None
                : QAbstractProtobufSerializer::Error::InvalidFormat);
}

void tst_protobuf_recursion_limit::json_data()
{
    QTest::addColumn<int>("levels");
    QTest::addColumn<bool>("ok");

    QTest::newRow("at-limit") << DefaultLimit << true;
    QTest::newRow("over-limit") << DefaultLimit + 1 << false;
}

void tst_protobuf_recursion_limit::json()
{
    QFETCH(const int, levels);
    QFETCH(const bool, ok);

    QProtobufJsonSerializer serializer;
    Node m;
    QCOMPARE(serializer.deserialize(&m, nestedJson(levels)), ok);
    QCOMPARE(serializer.lastError(),
             ok ? QAbstractProtobufSerializer::Error::None
                : QAbstractProtobufSerializer::Error::InvalidFormat);
}

void tst_protobuf_recursion_limit::jsonAfterError()
{
    QProtobufJsonSerializer serializer;

    Node failed;
    QVERIFY(!serializer.deserialize(&failed, nestedJson(DefaultLimit + 10)));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);

    Node ok;
    QVERIFY(serializer.deserialize(&ok, nestedJson(DefaultLimit)));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
}

void tst_protobuf_recursion_limit::serializeIsNotLimited()
{
    // The recursion limit applies to deserialization only, as in the upstream
    // C++ implementation.
    constexpr int Levels = DefaultLimit * 3;
    const Node node = makeNode(Levels);

    QProtobufSerializer binary;
    const QByteArray binaryData = binary.serialize(&node);
    QCOMPARE(binary.lastError(), QAbstractProtobufSerializer::Error::None);
    QCOMPARE(binaryData, nested(Levels));

    QProtobufJsonSerializer json;
    const QByteArray jsonData = json.serialize(&node);
    QCOMPARE(json.lastError(), QAbstractProtobufSerializer::Error::None);
    QCOMPARE(jsonData, nestedJson(Levels));

    // Deserializing the result fails, because it exceeds the default limit.
    Node out;
    QVERIFY(!binary.deserialize(&out, binaryData));
    QCOMPARE(binary.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);
}

#ifdef HAS_PROTOBUF_WELLKNOWN_TYPES
void tst_protobuf_recursion_limit::wellKnownTypes_data()
{
    QTest::addColumn<int>("levels");
    QTest::addColumn<bool>("ok");

    QTest::newRow("at-limit") << DefaultLimit << true;
    QTest::newRow("over-limit") << DefaultLimit + 1 << false;
}

void tst_protobuf_recursion_limit::wellKnownTypes()
{
    QFETCH(const int, levels);
    QFETCH(const bool, ok);

    // AnyNode.child is field 1.
    QByteArray data;
    for (int i = 0; i < levels; ++i)
        data = wrap(data, '\x0a');

    QProtobufSerializer serializer;
    AnyNode m;
    QCOMPARE(serializer.deserialize(&m, data), ok);
    QCOMPARE(serializer.lastError(),
             ok ? QAbstractProtobufSerializer::Error::None
                : QAbstractProtobufSerializer::Error::InvalidFormat);
}

void tst_protobuf_recursion_limit::anyHandler_data()
{
    QTest::addColumn<int>("levels");
    QTest::addColumn<int>("limit");
    QTest::addColumn<bool>("ok");

    // google.protobuf.Any is deserialized by a registered type handler, which
    // does not report failures itself. The Any in the innermost AnyNode is one
    // more nesting level than `levels`.
    QTest::newRow("any-default-at-limit") << DefaultLimit - 1 << DefaultLimit << true;
    QTest::newRow("any-default-over-limit") << DefaultLimit << DefaultLimit << false;
}

void tst_protobuf_recursion_limit::anyHandler()
{
    QFETCH(const int, levels);
    QFETCH(const int, limit);
    QFETCH(const bool, ok);

    // AnyNode.payload (field 2) is an Any with type_url = "x".
    QByteArray data = wrap(wrap("x", '\x0a'), '\x12');
    // AnyNode.child is field 1.
    for (int i = 0; i < levels; ++i)
        data = wrap(data, '\x0a');

    QProtobufSerializer serializer;
    AnyNode m;
    QCOMPARE(serializer.deserialize(&m, data), ok);
    if (ok) {
        QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
    } else {
        QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);
        QVERIFY(serializer.lastErrorString().contains(u"recursion limit of %1"_s.arg(limit)));
    }

    // The serializer is still usable after the error.
    AnyNode simple;
    QVERIFY(serializer.deserialize(&simple, wrap(wrap("x", '\x0a'), '\x12')));
}

void tst_protobuf_recursion_limit::jsonCustomDeserializer_data()
{
    QTest::addColumn<int>("levels");
    QTest::addColumn<int>("limit");
    QTest::addColumn<bool>("ok");

    // A wrapper type is read from a JSON scalar by a custom deserializer. It
    // is one more nesting level than `levels`.
    QTest::newRow("default-at-limit") << DefaultLimit - 1 << DefaultLimit << true;
    QTest::newRow("default-over-limit") << DefaultLimit << DefaultLimit << false;
}

void tst_protobuf_recursion_limit::jsonCustomDeserializer()
{
    QFETCH(const int, levels);
    QFETCH(const int, limit);
    QFETCH(const bool, ok);

    QByteArray data = "{\"number\":42}";
    for (int i = 0; i < levels; ++i)
        data = "{\"child\":" + data + "}";

    QProtobufJsonSerializer serializer;
    AnyNode m;
    QCOMPARE(serializer.deserialize(&m, data), ok);
    if (ok) {
        QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
    } else {
        QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);
        QVERIFY(serializer.lastErrorString().contains(u"recursion limit of %1"_s.arg(limit)));
    }

    // The serializer is still usable after the error.
    AnyNode simple;
    QVERIFY(serializer.deserialize(&simple, QByteArray("{\"number\":42}")));
}
#endif

QTEST_MAIN(tst_protobuf_recursion_limit)
#include "tst_protobuf_recursion_limit.moc"
