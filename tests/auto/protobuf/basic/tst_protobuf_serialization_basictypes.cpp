// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2022 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "basicmessages.qpb.h"
#include "fieldindexrange.qpb.h"

#include <QTest>
#include <QProtobufSerializer>

#include <limits>

using namespace Qt::Literals::StringLiterals;

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
    QCOMPARE(result.toHex(), "0801"_ba);

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
    QCOMPARE(result.toHex(), "080f"_ba);

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08ac02"_ba);

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08898004"_ba);

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "088001"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08808002"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), "087f"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08ffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "08ffffffff07"_ba);

    test.setTestFieldInt(-1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "08ffffffffffffffffff01"_ba);

    test.setTestFieldInt(-462);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "08b2fcffffffffffffff01"_ba);

    test.setTestFieldInt(-63585);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "089f8ffcffffffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "0880ffffffffffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "088080feffffffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "0880808080f8ffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "08fffeffffffffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "08fffffdffffffffffff01"_ba);
}

void QtProtobufTypesSerializationTest::UIntMessageSerializeTest()
{
    SimpleUIntMessage test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), "080f"_ba);

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08ac02"_ba);

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08898004"_ba);

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<uint8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "088002"_ba);

    test.setTestFieldInt(std::numeric_limits<uint16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08808004"_ba);

    test.setTestFieldInt(std::numeric_limits<uint8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08ff01"_ba);

    test.setTestFieldInt(std::numeric_limits<uint16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08ffff03"_ba);

    test.setTestFieldInt(std::numeric_limits<uint32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "08ffffffff0f"_ba);
}

void QtProtobufTypesSerializationTest::SIntMessageSerializeTest()
{
    SimpleSIntMessage test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), "081e"_ba);

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08d804"_ba);

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08928008"_ba);

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "088002"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08808004"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08fe01"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08feff03"_ba);

    test.setTestFieldInt(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "08feffffff0f"_ba);

    test.setTestFieldInt(-1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), "0801"_ba);

    test.setTestFieldInt(-462);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "089b07"_ba);

    test.setTestFieldInt(-63585);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08c1e107"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08ff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08ffff03"_ba);

    test.setTestFieldInt(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "08ffffffff0f"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "088102"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08818004"_ba);
}


void QtProtobufTypesSerializationTest::Int64MessageSerializeTest()
{
    SimpleInt64Message test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), "080f"_ba);

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08ac02"_ba);

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08898004"_ba);

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "088001"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08808002"_ba);

    test.setTestFieldInt((qlonglong)(std::numeric_limits<int32_t>::max()) + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "088080808008"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), "087f"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08ffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "08ffffffff07"_ba);

    test.setTestFieldInt(std::numeric_limits<int64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 10);
    QCOMPARE(result.toHex(), "08ffffffffffffffff7f"_ba);

    test.setTestFieldInt(-1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "08ffffffffffffffffff01"_ba);

    test.setTestFieldInt(-462);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "08b2fcffffffffffffff01"_ba);

    test.setTestFieldInt(-63585);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "089f8ffcffffffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "0880ffffffffffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "088080feffffffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "0880808080f8ffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "08fffeffffffffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "08fffffdffffffffffff01"_ba);

    test.setTestFieldInt((qlonglong)std::numeric_limits<int32_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "08fffffffff7ffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int64_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "0880808080808080808001"_ba);
}

void QtProtobufTypesSerializationTest::UInt64MessageSerializeTest()
{
    SimpleUInt64Message test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), "080f"_ba);

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08ac02"_ba);

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08898004"_ba);

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<uint8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "088002"_ba);

    test.setTestFieldInt(std::numeric_limits<uint16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08808004"_ba);

    test.setTestFieldInt(((uint64_t)std::numeric_limits<uint32_t>::max()) + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "088080808010"_ba);

    test.setTestFieldInt(std::numeric_limits<uint8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08ff01"_ba);

    test.setTestFieldInt(std::numeric_limits<uint16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08ffff03"_ba);

    test.setTestFieldInt(std::numeric_limits<uint32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "08ffffffff0f"_ba);

    test.setTestFieldInt(std::numeric_limits<uint64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "08ffffffffffffffffff01"_ba);
}

void QtProtobufTypesSerializationTest::SInt64MessageSerializeTest()
{
    SimpleSInt64Message test;
    test.setTestFieldInt(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), "081e"_ba);

    test.setTestFieldInt(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08d804"_ba);

    test.setTestFieldInt(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08928008"_ba);

    test.setTestFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 0);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "088002"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08808004"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08fe01"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08feff03"_ba);

    test.setTestFieldInt(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "08feffffff0f"_ba);

    test.setTestFieldInt(std::numeric_limits<int64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "08feffffffffffffffff01"_ba);

    test.setTestFieldInt(-1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.toHex(), "0801"_ba);

    test.setTestFieldInt(-462);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "089b07"_ba);

    test.setTestFieldInt(-63585);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08c1e107"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "08ff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08ffff03"_ba);

    test.setTestFieldInt(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "08ffffffff0f"_ba);

    test.setTestFieldInt(std::numeric_limits<int64_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 11);
    QCOMPARE(result.toHex(), "08ffffffffffffffffff01"_ba);

    test.setTestFieldInt(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.toHex(), "088102"_ba);

    test.setTestFieldInt(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 4);
    QCOMPARE(result.toHex(), "08818004"_ba);

    test.setTestFieldInt((qlonglong)std::numeric_limits<int32_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.toHex(), "088180808010"_ba);
}

void QtProtobufTypesSerializationTest::FixedInt32MessageSerializeTest()
{
    constexpr int Fixed32MessageSize = 5;
    SimpleFixedInt32Message test;
    test.setTestFieldFixedInt32(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d0f000000"_ba);

    test.setTestFieldFixedInt32(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d2c010000"_ba);

    test.setTestFieldFixedInt32(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d09000100"_ba);

    test.setTestFieldFixedInt32(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d00000000"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<uint8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d00010000"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<uint16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d00000100"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<uint8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0dff000000"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<uint16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0dffff0000"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<uint32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0dffffffff"_ba);
}

void QtProtobufTypesSerializationTest::FixedInt64MessageSerializeTest()
{
    constexpr int Fixed64MessageSize = 9;
    SimpleFixedInt64Message test;
    test.setTestFieldFixedInt64(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090f00000000000000"_ba);

    test.setTestFieldFixedInt64(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "092c01000000000000"_ba);

    test.setTestFieldFixedInt64(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090900010000000000"_ba);

    test.setTestFieldFixedInt64(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090000000000000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<uint8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090001000000000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<uint16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090000010000000000"_ba);

    test.setTestFieldFixedInt64((unsigned long long)(std::numeric_limits<uint32_t>::max()) + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090000000001000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<uint8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "09ff00000000000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<uint16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "09ffff000000000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<uint32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "09ffffffff00000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<uint64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "09ffffffffffffffff"_ba);
}

void QtProtobufTypesSerializationTest::SFixedInt32MessageSerializeTest()
{
    constexpr int Fixed32MessageSize = 5;
    SimpleSFixedInt32Message test;
    test.setTestFieldFixedInt32(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d0f000000"_ba);

    test.setTestFieldFixedInt32(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d2c010000"_ba);

    test.setTestFieldFixedInt32(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d09000100"_ba);

    test.setTestFieldFixedInt32(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d00000000"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d80000000"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d00800000"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d7f000000"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0dff7f0000"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d7fffffff"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0dff7fffff"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d80ffffff"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d0080ffff"_ba);

    test.setTestFieldFixedInt32(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(result.toHex(), "0d00000080"_ba);
}

void QtProtobufTypesSerializationTest::SFixedInt64MessageSerializeTest()
{
    constexpr int Fixed64MessageSize = 9;
    SimpleSFixedInt64Message test;
    test.setTestFieldFixedInt64(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090f00000000000000"_ba);

    test.setTestFieldFixedInt64(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "092c01000000000000"_ba);

    test.setTestFieldFixedInt64(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090900010000000000"_ba);

    test.setTestFieldFixedInt64(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090000000000000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "098000000000000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090080000000000000"_ba);

    test.setTestFieldFixedInt64((unsigned long long)(std::numeric_limits<int32_t>::max()) + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090000008000000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "097f00000000000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "09ff7f000000000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "09ffffff7f00000000"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "09ffffffffffffff7f"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "097fffffffffffffff"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "09ff7fffffffffffff"_ba);

    test.setTestFieldFixedInt64((qlonglong)std::numeric_limits<int32_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "09ffffff7fffffffff"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "0980ffffffffffffff"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090080ffffffffffff"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "0900000080ffffffff"_ba);

    test.setTestFieldFixedInt64(std::numeric_limits<int64_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(result.toHex(), "090000000000000080"_ba);
}

void QtProtobufTypesSerializationTest::FloatMessageSerializeTest()
{
    constexpr int FloatMessageSize = 5;
    SimpleFloatMessage test;
    test.setTestFieldFloat(0.1f);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), FloatMessageSize);
    QCOMPARE(result.toHex(), "3dcdcccc3d"_ba);

    test.setTestFieldFloat(std::numeric_limits<float>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), FloatMessageSize);
    QCOMPARE(result.toHex(), "3d00008000"_ba);

    test.setTestFieldFloat(std::numeric_limits<float>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), FloatMessageSize);
    QCOMPARE(result.at(0), 0x3d);
    QCOMPARE(result.toHex(), "3dffff7f7f"_ba);

    test.setTestFieldFloat(-4.2f);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), FloatMessageSize);
    QCOMPARE(result.at(0), 0x3d);
    QCOMPARE(result.toHex(), "3d666686c0"_ba);

    test.setTestFieldFloat(-0.0f);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), FloatMessageSize);
    QCOMPARE(result.toHex(), "3d00000080"_ba);
}

void QtProtobufTypesSerializationTest::DoubleMessageSerializeTest()
{
    constexpr int DoubleMessageSize = 9;
    SimpleDoubleMessage test;
    test.setTestFieldDouble(0.1);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), DoubleMessageSize);
    QCOMPARE(result.toHex(), "419a9999999999b93f"_ba);

    test.setTestFieldDouble(std::numeric_limits<double>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), DoubleMessageSize);
    QCOMPARE(result.toHex(), "410000000000001000"_ba);

    test.setTestFieldDouble(std::numeric_limits<double>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), DoubleMessageSize);
    QCOMPARE(result.toHex(), "41ffffffffffffef7f"_ba);

    test.setTestFieldDouble(-4.2);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), DoubleMessageSize);
    QCOMPARE(result.toHex(), "41cdcccccccccc10c0"_ba);

    test.setTestFieldDouble(0.0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), DoubleMessageSize);
    QCOMPARE(result.toHex(), "410000000000000000"_ba);
}

void QtProtobufTypesSerializationTest::StringMessageSerializeTest()
{
    SimpleStringMessage test;
    test.setTestFieldString("qwerty");
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "3206717765727479"_ba);

    test.setTestFieldString("oepSNLIVG08UJpk2W7JtTkkBxyK06X0lQ6ML7IMd55K8XC1Tpsc1kDWym5v8z68b4FQup9O95QSgAvjHIA15OX6Bu68esbQFT9LPzSADJ6qSGBTYBHX5QSZg32trCdHMj80XuDHqyBgM4uf6RKq2mgWb8Ovxxr0NwLxjHOfhJ8Mrfd2R7hbUgjespbYoQhbgHEj2gKEV3QvnumYmrVXe1BkCzZhKVXodDhj0OfAE67viAy4i3Oag1hr1z4Azo8O5Xq68POEZ1CsZPo2DXNNR8ebVCdYOz0Q6JLPSl5jasLCFrQN7EiVNjQmCrSsZHRgLNylvgoEFxGYxXJ9gmK4mr0OGdZcGJORRGZOQCpQMhXmhezFalNIJXMPPXaRVXiRhRAPCNUEie8DtaCWAMqz4nNUxRMZ5UcXBXsXPshygzkyyXnNWTIDojFlrcsnKqSkQ1G6E85gSZbtIYBh7sqO6GDXHjOrXVaVCVCUubjcJKThlyslt29zHuIs5JGppXxX1");
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(),
             "3280046f6570534e4c4956473038554a706b3257374a74546"
             "b6b4278794b303658306c51364d4c37494d6435354b385843"
             "3154707363316b4457796d3576387a3638623446517570394"
             "f393551536741766a48494131354f58364275363865736251"
             "4654394c507a5341444a367153474254594248583551535a6"
             "7333274724364484d6a383058754448717942674d34756636"
             "524b71326d675762384f76787872304e774c786a484f66684"
             "a384d726664325237686255676a65737062596f5168626748"
             "456a32674b45563351766e756d596d7256586531426b437a5"
             "a684b56586f6444686a304f6641453637766941793469334f"
             "6167316872317a34417a6f384f3558713638504f455a31437"
             "35a506f3244584e4e52386562564364594f7a3051364a4c50"
             "536c356a61734c434672514e374569564e6a516d437253735"
             "a4852674c4e796c76676f454678475978584a39676d4b346d"
             "72304f47645a63474a4f5252475a4f514370514d68586d686"
             "57a46616c4e494a584d50505861525658695268524150434e"
             "55456965384474614357414d717a346e4e5578524d5a35556"
             "3584258735850736879677a6b7979586e4e575449446f6a46"
             "6c7263736e4b71536b5131473645383567535a62744959426"
             "83773714f36474458486a4f72585661564356435575626a63"
             "4a4b54686c79736c7432397a487549"
             "73354a47707058785831"_ba);
}


void QtProtobufTypesSerializationTest::ComplexTypeSerializeTest()
{
    SimpleStringMessage stringMsg;
    stringMsg.setTestFieldString("qwerty");

    ComplexMessage test;
    QByteArray result = test.serialize(m_serializer.get());
    QEXPECT_FAIL("", "The result of serialization can be either an empty message or a message containing empty fields.", Continue);
    QCOMPARE(result.toHex(), "");

    test.setTestFieldInt(42);
    test.setTestComplexField(stringMsg);
    result = test.serialize(m_serializer.get());
    QVERIFY(result.toHex() == "12083206717765727479082a"_ba
            || result.toHex() == "082a12083206717765727479"_ba);

    stringMsg.setTestFieldString("YVRfJvjxqbgvFwS1YvOZXgtj5ffGLS7AiNHz9oZIoKbm7z8H79xBuyPkpQXvGoO09OY9xRawx3eOAs9xjoTA1xJhrw28TAcq1CebYlC9WUfQC6hIantaNdyHiKToffi0Zt7la42SRxXZSP4GuxbcZIp53pJnyCwfCy1qdFczT0dmn7h8fpyAdemEavwFeda4d0PApGfSU2jLt39X8kYUBxNM2WgALRBgHdVde87q6Pi5U69TjhMd28W1SFD1DxyogCCrqOct2ZPICoLnrqdF3OdNzjRVLfeyvQ8LgLvRNFR9WfWAyAz79nKgBamd8Ntlvt4Mg35E5gVS2g7AQ7rkm72cBdnW9sCEyGabeXAuH5j4GRbuLT7qBZWDcFLF4SsCdS3WfFGdNHfwaijzykByo71PvFVlTXH2WJWoFvR5FALjBTn7bCdP0pAiSbLCY8Xz2Msc3dBb5Ff9GISPbUpNmUvBdMZMHQvqOmTNXEPpN0b74MDOMQfWJShOo3NkAvMjs");
    test.setTestFieldInt(42);
    test.setTestComplexField(stringMsg);

    result = test.serialize(m_serializer.get());
    QVERIFY(result.toHex()
                    == "128404328104595652664a766a78716267764677533159764f5a5867746a356666474c"
                       "533741694e487a396f5a496f4b626d377a3848373978427579506b70515876476f4f30"
                       "394f5939785261777833654f417339786a6f544131784a687277323854416371314365"
                       "62596c43395755665143366849616e74614e647948694b546f666669305a74376c6134"
                       "32535278585a53503447757862635a49703533704a6e79437766437931716446637a54"
                       "30646d6e3768386670794164656d456176774665646134643050417047665355326a4c"
                       "74333958386b595542784e4d325767414c524267486456646538377136506935553639"
                       "546a684d6432385731534644314478796f67434372714f6374325a5049436f4c6e7271"
                       "6446334f644e7a6a52564c6665797651384c674c76524e4652395766574179417a3739"
                       "6e4b6742616d64384e746c7674344d6733354535675653326737415137726b6d373263"
                       "42646e5739734345794761626558417548356a34475262754c543771425a574463464c"
                       "463453734364533357664647644e48667761696a7a796b42796f3731507646566c5458"
                       "4832574a576f4676523546414c6a42546e37624364503070416953624c435938587a32"
                       "4d73633364426235466639474953506255704e6d557642644d5a4d485176714f6d544e"
                       "584550704e306237344d444f4d5166574a53684f6f334e6b41764d6a73082a"_ba
            || result.toHex()
                    == "082a128404328104595652664a766a78716267764677533159764f5a5867746a356666"
                       "474c533741694e487a396f5a496f4b626d377a3848373978427579506b70515876476f"
                       "4f30394f5939785261777833654f417339786a6f544131784a68727732385441637131"
                       "436562596c43395755665143366849616e74614e647948694b546f666669305a74376c"
                       "613432535278585a53503447757862635a49703533704a6e7943776643793171644663"
                       "7a5430646d6e3768386670794164656d45617677466564613464305041704766535532"
                       "6a4c74333958386b595542784e4d325767414c52426748645664653837713650693555"
                       "3639546a684d6432385731534644314478796f67434372714f6374325a5049436f4c6e"
                       "72716446334f644e7a6a52564c6665797651384c674c76524e4652395766574179417a"
                       "37396e4b6742616d64384e746c7674344d6733354535675653326737415137726b6d37"
                       "326342646e5739734345794761626558417548356a34475262754c543771425a574463"
                       "464c463453734364533357664647644e48667761696a7a796b42796f3731507646566c"
                       "54584832574a576f4676523546414c6a42546e37624364503070416953624c43593858"
                       "7a324d73633364426235466639474953506255704e6d557642644d5a4d485176714f6d"
                       "544e584550704e306237344d444f4d5166574a53684f6f334e6b41764d6a73"_ba);

    stringMsg.setTestFieldString("qwerty");
    test.setTestFieldInt(-45);
    test.setTestComplexField(stringMsg);

    result = test.serialize(m_serializer.get());
    QVERIFY(result.toHex() == "1208320671776572747908d3ffffffffffffffff01"_ba
            || result.toHex() == "08d3ffffffffffffffff0112083206717765727479"_ba);
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
    QCOMPARE(result.toHex(), "f80102"_ba);

    FieldIndexTest2Message msg2;
    msg2.setTestField(1);
    result = msg2.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "f8ff0302"_ba);

    FieldIndexTest3Message msg3;
    msg3.setTestField(1);
    result = msg3.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "f8ffff0702"_ba);

    FieldIndexTest4Message msg4;
    msg4.setTestField(1);
    result = msg4.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "f8ffffff0f02"_ba);
}

QTEST_MAIN(QtProtobufTypesSerializationTest)
#include "tst_protobuf_serialization_basictypes.moc"
