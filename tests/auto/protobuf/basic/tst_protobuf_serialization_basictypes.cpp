// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2022 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "basicmessages.qpb.h"
#include "fieldindexrange.qpb.h"

#include <QTest>
#include <QProtobufSerializer>

#include <limits>

class QtProtobufTypesSerializationTest : public QObject
{
    Q_OBJECT
private slots:
    void init() {
        m_serializer.reset(new QProtobufSerializer);
    }
    void BoolMessageSerializeTest();
    void IntMessageSerializeTest();
    void UIntMessageSerializeTest();
    void SIntMessageSerializeTest();
    void Int64MessageSerializeTest();
    void UInt64MessageSerializeTest();
    void SInt64MessageSerializeTest();
    void FixedInt32MessageSerializeTest();
    void FixedInt64MessageSerializeTest();
    void SFixedInt32MessageSerializeTest();
    void SFixedInt64MessageSerializeTest();
    void FloatMessageSerializeTest();
    void DoubleMessageSerializeTest();
    void StringMessageSerializeTest();
    void ComplexTypeSerializeTest();
    void EmptyBytesMessageTest();
    void EmptyStringMessageTest();
    void FieldIndexRangeTest();
private:
    std::unique_ptr<QProtobufSerializer> m_serializer;
};

using namespace qtprotobufnamespace::tests;

void QtProtobufTypesSerializationTest::BoolMessageSerializeTest()
{
    SimpleBoolMessage test;
    test.setTestFieldBool(true);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("0801"));

    test.setTestFieldBool(false);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);
    QCOMPARE(result.toHex(), QByteArray());
}

void QtProtobufTypesSerializationTest::IntMessageSerializeTest()
{
    qtprotobufnamespace::tests::SimpleIntMessage test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("080f"));

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08ac02"));

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08898004"));

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("088001"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08808002"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("087f"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08ffff01"));

    test.setTestFieldInt(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffff07"));

    test.setTestFieldInt(-1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffffffffffffff01"));

    test.setTestFieldInt(-462);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), QStringLiteral("08b2fcffffffffffffff01"));

    test.setTestFieldInt(-63585);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), QStringLiteral("089f8ffcffffffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), QStringLiteral("0880ffffffffffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), QStringLiteral("088080feffffffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), QStringLiteral("0880808080f8ffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), QStringLiteral("08fffeffffffffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), QStringLiteral("08fffffdffffffffffff01"));
}

void QtProtobufTypesSerializationTest::UIntMessageSerializeTest()
{
    SimpleUIntMessage test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("080f"));

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08ac02"));

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08898004"));

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<uint8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("088002"));

    test.setTestFieldInt(std::numeric_limits<uint16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08808004"));

    test.setTestFieldInt(std::numeric_limits<uint8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08ff01"));

    test.setTestFieldInt(std::numeric_limits<uint16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08ffff03"));

    test.setTestFieldInt(std::numeric_limits<uint32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffff0f"));
}

void QtProtobufTypesSerializationTest::SIntMessageSerializeTest()
{
    SimpleSIntMessage test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("081e"));

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08d804"));

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08928008"));

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("088002"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08808004"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08fe01"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08feff03"));

    test.setTestFieldInt(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("08feffffff0f"));

    test.setTestFieldInt(-1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("0801"));

    test.setTestFieldInt(-462);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("089b07"));

    test.setTestFieldInt(-63585);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08c1e107"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08ff01"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08ffff03"));

    test.setTestFieldInt(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffff0f"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("088102"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08818004"));
}


void QtProtobufTypesSerializationTest::Int64MessageSerializeTest()
{
    SimpleInt64Message test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("080f"));

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08ac02"));

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08898004"));

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("088001"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08808002"));

    test.setTestFieldInt((qlonglong)(std::numeric_limits<int32_t>::max()) + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("088080808008"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("087f"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08ffff01"));

    test.setTestFieldInt(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffff07"));

    test.setTestFieldInt(std::numeric_limits<int64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 10);
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffffffffffff7f"));

    test.setTestFieldInt(-1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffffffffffffff01"));

    test.setTestFieldInt(-462);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("08b2fcffffffffffffff01"));

    test.setTestFieldInt(-63585);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("089f8ffcffffffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("0880ffffffffffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("088080feffffffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("0880808080f8ffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("08fffeffffffffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("08fffffdffffffffffff01"));

    test.setTestFieldInt((qlonglong)std::numeric_limits<int32_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("08fffffffff7ffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int64_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("0880808080808080808001"));
}

void QtProtobufTypesSerializationTest::UInt64MessageSerializeTest()
{
    SimpleUInt64Message test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("080f"));

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08ac02"));

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08898004"));

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<uint8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("088002"));

    test.setTestFieldInt(std::numeric_limits<uint16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08808004"));

    test.setTestFieldInt(((uint64_t)std::numeric_limits<uint32_t>::max()) + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("088080808010"));

    test.setTestFieldInt(std::numeric_limits<uint8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08ff01"));

    test.setTestFieldInt(std::numeric_limits<uint16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08ffff03"));

    test.setTestFieldInt(std::numeric_limits<uint32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffff0f"));

    test.setTestFieldInt(std::numeric_limits<uint64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffffffffffffff01"));
}

void QtProtobufTypesSerializationTest::SInt64MessageSerializeTest()
{
    SimpleSInt64Message test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("081e"));

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08d804"));

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08928008"));

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("088002"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08808004"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08fe01"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08feff03"));

    test.setTestFieldInt(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("08feffffff0f"));

    test.setTestFieldInt(std::numeric_limits<int64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("08feffffffffffffffff01"));

    test.setTestFieldInt(-1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), QStringLiteral("0801"));

    test.setTestFieldInt(-462);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("089b07"));

    test.setTestFieldInt(-63585);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08c1e107"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("08ff01"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08ffff03"));

    test.setTestFieldInt(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffff0f"));

    test.setTestFieldInt(std::numeric_limits<int64_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), QStringLiteral("08ffffffffffffffffff01"));

    test.setTestFieldInt(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), QStringLiteral("088102"));

    test.setTestFieldInt(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), QStringLiteral("08818004"));

    test.setTestFieldInt((qlonglong)std::numeric_limits<int32_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), QStringLiteral("088180808010"));
}

void QtProtobufTypesSerializationTest::FixedInt32MessageSerializeTest()
{
    constexpr int Fixed32MessageSize = 5;
    SimpleFixedInt32Message test;
    test.setTestFieldFixedInt32(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d0f000000"));

    test.setTestFieldFixedInt32(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d2c010000"));

    test.setTestFieldFixedInt32(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d09000100"));

    test.setTestFieldFixedInt32(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldFixedInt32(std::numeric_limits<uint8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d00010000"));

    test.setTestFieldFixedInt32(std::numeric_limits<uint16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d00000100"));

    test.setTestFieldFixedInt32(std::numeric_limits<uint8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0dff000000"));

    test.setTestFieldFixedInt32(std::numeric_limits<uint16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0dffff0000"));

    test.setTestFieldFixedInt32(std::numeric_limits<uint32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0dffffffff"));
}

void QtProtobufTypesSerializationTest::FixedInt64MessageSerializeTest()
{
    constexpr int Fixed64MessageSize = 9;
    SimpleFixedInt64Message test;
    test.setTestFieldFixedInt64(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090f00000000000000"));

    test.setTestFieldFixedInt64(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("092c01000000000000"));

    test.setTestFieldFixedInt64(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090900010000000000"));

    test.setTestFieldFixedInt64(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldFixedInt64(std::numeric_limits<uint8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090001000000000000"));

    test.setTestFieldFixedInt64(std::numeric_limits<uint16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090000010000000000"));

    test.setTestFieldFixedInt64((unsigned long long)(std::numeric_limits<uint32_t>::max()) + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090000000001000000"));

    test.setTestFieldFixedInt64(std::numeric_limits<uint8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("09ff00000000000000"));

    test.setTestFieldFixedInt64(std::numeric_limits<uint16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("09ffff000000000000"));

    test.setTestFieldFixedInt64(std::numeric_limits<uint32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("09ffffffff00000000"));

    test.setTestFieldFixedInt64(std::numeric_limits<uint64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("09ffffffffffffffff"));
}

void QtProtobufTypesSerializationTest::SFixedInt32MessageSerializeTest()
{
    constexpr int Fixed32MessageSize = 5;
    SimpleSFixedInt32Message test;
    test.setTestFieldFixedInt32(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d0f000000"));

    test.setTestFieldFixedInt32(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d2c010000"));

    test.setTestFieldFixedInt32(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d09000100"));

    test.setTestFieldFixedInt32(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d80000000"));

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d00800000"));

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d7f000000"));

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0dff7f0000"));

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d7fffffff"));

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0dff7fffff"));

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d80ffffff"));

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d0080ffff"));

    test.setTestFieldFixedInt32(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0d00000080"));
}

void QtProtobufTypesSerializationTest::SFixedInt64MessageSerializeTest()
{
    constexpr int Fixed64MessageSize = 9;
    SimpleSFixedInt64Message test;
    test.setTestFieldFixedInt64(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090f00000000000000"));

    test.setTestFieldFixedInt64(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("092c01000000000000"));

    test.setTestFieldFixedInt64(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090900010000000000"));

    test.setTestFieldFixedInt64(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("098000000000000000"));

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090080000000000000"));

    test.setTestFieldFixedInt64((unsigned long long)(std::numeric_limits<int32_t>::max()) + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090000008000000000"));

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("097f00000000000000"));

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("09ff7f000000000000"));

    test.setTestFieldFixedInt64(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("09ffffff7f00000000"));

    test.setTestFieldFixedInt64(std::numeric_limits<int64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("09ffffffffffffff7f"));

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("097fffffffffffffff"));

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("09ff7fffffffffffff"));

    test.setTestFieldFixedInt64((qlonglong)std::numeric_limits<int32_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("09ffffff7fffffffff"));

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0980ffffffffffffff"));

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090080ffffffffffff"));

    test.setTestFieldFixedInt64(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("0900000080ffffffff"));

    test.setTestFieldFixedInt64(std::numeric_limits<int64_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result, QByteArray::fromHex("090000000000000080"));
}

void QtProtobufTypesSerializationTest::FloatMessageSerializeTest()
{
    constexpr int FloatMessageSize = 5;
    SimpleFloatMessage test;
    test.setTestFieldFloat(0.1f);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), FloatMessageSize);
    QCOMPARE(result.toHex(), QStringLiteral("3dcdcccc3d"));

    test.setTestFieldFloat(std::numeric_limits<float>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), FloatMessageSize);
    QCOMPARE(result.toHex(), QStringLiteral("3d00008000"));

    test.setTestFieldFloat(std::numeric_limits<float>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), FloatMessageSize);
    QCOMPARE(result.at(0), 0x3d);
    QCOMPARE(result.toHex(), QStringLiteral("3dffff7f7f"));

    test.setTestFieldFloat(-4.2f);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), FloatMessageSize);
    QCOMPARE(result.at(0), 0x3d);
    QCOMPARE(result.toHex(), QStringLiteral("3d666686c0"));

    test.setTestFieldFloat(-0.0f);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);
}

void QtProtobufTypesSerializationTest::DoubleMessageSerializeTest()
{
    constexpr int DoubleMessageSize = 9;
    SimpleDoubleMessage test;
    test.setTestFieldDouble(0.1);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), DoubleMessageSize);
    QCOMPARE(result.toHex(), QStringLiteral("419a9999999999b93f"));

    test.setTestFieldDouble(std::numeric_limits<double>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), DoubleMessageSize);
    QCOMPARE(result.toHex(), QStringLiteral("410000000000001000"));

    test.setTestFieldDouble(std::numeric_limits<double>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), DoubleMessageSize);
    QCOMPARE(result.toHex(), QStringLiteral("41ffffffffffffef7f"));

    test.setTestFieldDouble(-4.2);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), DoubleMessageSize);
    QCOMPARE(result.toHex(), QStringLiteral("41cdcccccccccc10c0"));

    test.setTestFieldDouble(0.0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);
}

void QtProtobufTypesSerializationTest::StringMessageSerializeTest()
{
    SimpleStringMessage test;
    test.setTestFieldString("qwerty");
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), QStringLiteral("3206717765727479"));

    test.setTestFieldString("oepSNLIVG08UJpk2W7JtTkkBxyK06X0lQ6ML7IMd55K8XC1Tpsc1kDWym5v8z68b4FQup9O95QSgAvjHIA15OX6Bu68esbQFT9LPzSADJ6qSGBTYBHX5QSZg32trCdHMj80XuDHqyBgM4uf6RKq2mgWb8Ovxxr0NwLxjHOfhJ8Mrfd2R7hbUgjespbYoQhbgHEj2gKEV3QvnumYmrVXe1BkCzZhKVXodDhj0OfAE67viAy4i3Oag1hr1z4Azo8O5Xq68POEZ1CsZPo2DXNNR8ebVCdYOz0Q6JLPSl5jasLCFrQN7EiVNjQmCrSsZHRgLNylvgoEFxGYxXJ9gmK4mr0OGdZcGJORRGZOQCpQMhXmhezFalNIJXMPPXaRVXiRhRAPCNUEie8DtaCWAMqz4nNUxRMZ5UcXBXsXPshygzkyyXnNWTIDojFlrcsnKqSkQ1G6E85gSZbtIYBh7sqO6GDXHjOrXVaVCVCUubjcJKThlyslt29zHuIs5JGppXxX1");
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), QStringLiteral("3280046f6570534e4c4956473038554a706b3257374a74546b6b4278794b303658306c51364d4c37494d6435354b3858433154707363316b4457796d3576387a3638623446517570394f393551536741766a48494131354f583642753638657362514654394c507a5341444a367153474254594248583551535a67333274724364484d6a383058754448717942674d34756636524b71326d675762384f76787872304e774c786a484f66684a384d726664325237686255676a65737062596f5168626748456a32674b45563351766e756d596d7256586531426b437a5a684b56586f6444686a304f6641453637766941793469334f6167316872317a34417a6f384f3558713638504f455a3143735a506f3244584e4e52386562564364594f7a3051364a4c50536c356a61734c434672514e374569564e6a516d437253735a4852674c4e796c76676f454678475978584a39676d4b346d72304f47645a63474a4f5252475a4f514370514d68586d68657a46616c4e494a584d50505861525658695268524150434e55456965384474614357414d717a346e4e5578524d5a355563584258735850736879677a6b7979586e4e575449446f6a466c7263736e4b71536b5131473645383567535a6274495942683773714f36474458486a4f72585661564356435575626a634a4b54686c79736c7432397a48754973354a47707058785831"));
}


void QtProtobufTypesSerializationTest::ComplexTypeSerializeTest()
{
    SimpleStringMessage stringMsg;
    stringMsg.setTestFieldString("qwerty");

    ComplexMessage test;
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "");

    test.setTestFieldInt(42);
    test.setTestComplexField(stringMsg);

    result = test.serialize(m_serializer.get());
    QVERIFY(result == QByteArray::fromHex("12083206717765727479082a")
                || result == QByteArray::fromHex("082a12083206717765727479"));

    stringMsg.setTestFieldString("YVRfJvjxqbgvFwS1YvOZXgtj5ffGLS7AiNHz9oZIoKbm7z8H79xBuyPkpQXvGoO09OY9xRawx3eOAs9xjoTA1xJhrw28TAcq1CebYlC9WUfQC6hIantaNdyHiKToffi0Zt7la42SRxXZSP4GuxbcZIp53pJnyCwfCy1qdFczT0dmn7h8fpyAdemEavwFeda4d0PApGfSU2jLt39X8kYUBxNM2WgALRBgHdVde87q6Pi5U69TjhMd28W1SFD1DxyogCCrqOct2ZPICoLnrqdF3OdNzjRVLfeyvQ8LgLvRNFR9WfWAyAz79nKgBamd8Ntlvt4Mg35E5gVS2g7AQ7rkm72cBdnW9sCEyGabeXAuH5j4GRbuLT7qBZWDcFLF4SsCdS3WfFGdNHfwaijzykByo71PvFVlTXH2WJWoFvR5FALjBTn7bCdP0pAiSbLCY8Xz2Msc3dBb5Ff9GISPbUpNmUvBdMZMHQvqOmTNXEPpN0b74MDOMQfWJShOo3NkAvMjs");
    test.setTestFieldInt(42);
    test.setTestComplexField(stringMsg);

    result = test.serialize(m_serializer.get());
    QVERIFY(result == QByteArray::fromHex("128404328104595652664a766a78716267764677533159764f5a5867746a356666474c533741694e487a396f5a496f4b626d377a3848373978427579506b70515876476f4f30394f5939785261777833654f417339786a6f544131784a68727732385441637131436562596c43395755665143366849616e74614e647948694b546f666669305a74376c613432535278585a53503447757862635a49703533704a6e79437766437931716446637a5430646d6e3768386670794164656d456176774665646134643050417047665355326a4c74333958386b595542784e4d325767414c524267486456646538377136506935553639546a684d6432385731534644314478796f67434372714f6374325a5049436f4c6e72716446334f644e7a6a52564c6665797651384c674c76524e4652395766574179417a37396e4b6742616d64384e746c7674344d6733354535675653326737415137726b6d37326342646e5739734345794761626558417548356a34475262754c543771425a574463464c463453734364533357664647644e48667761696a7a796b42796f3731507646566c54584832574a576f4676523546414c6a42546e37624364503070416953624c435938587a324d73633364426235466639474953506255704e6d557642644d5a4d485176714f6d544e584550704e306237344d444f4d5166574a53684f6f334e6b41764d6a73082a")
                ||result == QByteArray::fromHex("082a128404328104595652664a766a78716267764677533159764f5a5867746a356666474c533741694e487a396f5a496f4b626d377a3848373978427579506b70515876476f4f30394f5939785261777833654f417339786a6f544131784a68727732385441637131436562596c43395755665143366849616e74614e647948694b546f666669305a74376c613432535278585a53503447757862635a49703533704a6e79437766437931716446637a5430646d6e3768386670794164656d456176774665646134643050417047665355326a4c74333958386b595542784e4d325767414c524267486456646538377136506935553639546a684d6432385731534644314478796f67434372714f6374325a5049436f4c6e72716446334f644e7a6a52564c6665797651384c674c76524e4652395766574179417a37396e4b6742616d64384e746c7674344d6733354535675653326737415137726b6d37326342646e5739734345794761626558417548356a34475262754c543771425a574463464c463453734364533357664647644e48667761696a7a796b42796f3731507646566c54584832574a576f4676523546414c6a42546e37624364503070416953624c435938587a324d73633364426235466639474953506255704e6d557642644d5a4d485176714f6d544e584550704e306237344d444f4d5166574a53684f6f334e6b41764d6a73"));


    stringMsg.setTestFieldString("qwerty");
    test.setTestFieldInt(-45);
    test.setTestComplexField(stringMsg);

    result = test.serialize(m_serializer.get());

    QVERIFY(result == QByteArray::fromHex("1208320671776572747908d3ffffffffffffffff01")
                || result == QByteArray::fromHex("08d3ffffffffffffffff0112083206717765727479"));
}

void QtProtobufTypesSerializationTest::EmptyBytesMessageTest()
{
    SimpleBytesMessage msg;

    QByteArray result = msg.serialize(m_serializer.get());
    QEXPECT_FAIL("", "The result of serialization can be either an empty message or a message of size zero.", Continue);
    QVERIFY(result.isEmpty());
}

void QtProtobufTypesSerializationTest::EmptyStringMessageTest()
{
    SimpleStringMessage msg;

    QByteArray result = msg.serialize(m_serializer.get());
    QEXPECT_FAIL("", "The result of serialization can be either an empty message or a message of size zero.", Continue);
    QVERIFY(result.isEmpty());
}

void QtProtobufTypesSerializationTest::FieldIndexRangeTest()
{
    FieldIndexTest1Message msg1;
    msg1.setTestField(1);
    QByteArray result = msg1.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(),
                 "f80102");

    FieldIndexTest2Message msg2;
    msg2.setTestField(1);
    result = msg2.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(),
                 "f8ff0302");

    FieldIndexTest3Message msg3;
    msg3.setTestField(1);
    result = msg3.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(),
                 "f8ffff0702");

    FieldIndexTest4Message msg4;
    msg4.setTestField(1);
    result = msg4.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(),
                 "f8ffffff0f02");
}

QTEST_MAIN(QtProtobufTypesSerializationTest)
#include "tst_protobuf_serialization_basictypes.moc"
