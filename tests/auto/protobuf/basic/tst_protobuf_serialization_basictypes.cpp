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
    QCOMPARE(QLatin1StringView(result.toHex()), "0d0f000000"_L1);

    test.setTestFieldFixedInt32(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d2c010000"_L1);

    test.setTestFieldFixedInt32(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d09000100"_L1);

    test.setTestFieldFixedInt32(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d00000000"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<uint8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d00010000"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<uint16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d00000100"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<uint8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0dff000000"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<uint16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0dffff0000"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<uint32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0dffffffff"_L1);
}

void QtProtobufTypesSerializationTest::FixedInt64MessageSerializeTest()
{
    constexpr int Fixed64MessageSize = 9;
    SimpleFixedInt64Message test;
    test.setTestFieldFixedInt64(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090f00000000000000"_L1);

    test.setTestFieldFixedInt64(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "092c01000000000000"_L1);

    test.setTestFieldFixedInt64(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090900010000000000"_L1);

    test.setTestFieldFixedInt64(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090000000000000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<uint8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090001000000000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<uint16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090000010000000000"_L1);

    test.setTestFieldFixedInt64((unsigned long long)(std::numeric_limits<uint32_t>::max()) + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090000000001000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<uint8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "09ff00000000000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<uint16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "09ffff000000000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<uint32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "09ffffffff00000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<uint64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "09ffffffffffffffff"_L1);
}

void QtProtobufTypesSerializationTest::SFixedInt32MessageSerializeTest()
{
    constexpr int Fixed32MessageSize = 5;
    SimpleSFixedInt32Message test;
    test.setTestFieldFixedInt32(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d0f000000"_L1);

    test.setTestFieldFixedInt32(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d2c010000"_L1);

    test.setTestFieldFixedInt32(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d09000100"_L1);

    test.setTestFieldFixedInt32(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d00000000"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d80000000"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d00800000"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d7f000000"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0dff7f0000"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d7fffffff"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0dff7fffff"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d80ffffff"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d0080ffff"_L1);

    test.setTestFieldFixedInt32(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed32MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0d00000080"_L1);
}

void QtProtobufTypesSerializationTest::SFixedInt64MessageSerializeTest()
{
    constexpr int Fixed64MessageSize = 9;
    SimpleSFixedInt64Message test;
    test.setTestFieldFixedInt64(15);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090f00000000000000"_L1);

    test.setTestFieldFixedInt64(300);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "092c01000000000000"_L1);

    test.setTestFieldFixedInt64(65545);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090900010000000000"_L1);

    test.setTestFieldFixedInt64(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090000000000000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "098000000000000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::max() + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090080000000000000"_L1);

    test.setTestFieldFixedInt64((unsigned long long)(std::numeric_limits<int32_t>::max()) + 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090000008000000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "097f00000000000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "09ff7f000000000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int32_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "09ffffff7f00000000"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int64_t>::max());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "09ffffffffffffff7f"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "097fffffffffffffff"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "09ff7fffffffffffff"_L1);

    test.setTestFieldFixedInt64((qlonglong)std::numeric_limits<int32_t>::min() - 1);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "09ffffff7fffffffff"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int8_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0980ffffffffffffff"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int16_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090080ffffffffffff"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int32_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "0900000080ffffffff"_L1);

    test.setTestFieldFixedInt64(std::numeric_limits<int64_t>::min());
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), Fixed64MessageSize);
    QCOMPARE(QLatin1StringView(result.toHex()), "090000000000000080"_L1);
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
    QCOMPARE(result.size(), FloatMessageSize);
    QCOMPARE(result.toHex(), QStringLiteral("3d00000080"));
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
    QCOMPARE(result.size(), DoubleMessageSize);
    QCOMPARE(result.toHex(), QStringLiteral("410000000000000000"));
}

void QtProtobufTypesSerializationTest::StringMessageSerializeTest()
{
    SimpleStringMessage test;
    test.setTestFieldString("qwerty");
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(QLatin1StringView(result.toHex()), "3206717765727479"_L1);

    test.setTestFieldString("oepSNLIVG08UJpk2W7JtTkkBxyK06X0lQ6ML7IMd55K8XC1Tpsc1kDWym5v8z68b4FQu"
                            "p9O95QSgAvjHIA15OX6Bu68esbQFT9LPzSADJ6qSGBTYBHX5QSZg32trCdHMj80XuDHq"
                            "yBgM4uf6RKq2mgWb8Ovxxr0NwLxjHOfhJ8Mrfd2R7hbUgjespbYoQhbgHEj2gKEV3Qvn"
                            "umYmrVXe1BkCzZhKVXodDhj0OfAE67viAy4i3Oag1hr1z4Azo8O5Xq68POEZ1CsZPo2D"
                            "XNNR8ebVCdYOz0Q6JLPSl5jasLCFrQN7EiVNjQmCrSsZHRgLNylvgoEFxGYxXJ9gmK4m"
                            "r0OGdZcGJORRGZOQCpQMhXmhezFalNIJXMPPXaRVXiRhRAPCNUEie8DtaCWAMqz4nNUx"
                            "RMZ5UcXBXsXPshygzkyyXnNWTIDojFlrcsnKqSkQ1G6E85gSZbtIYBh7sqO6GDXHjOrX"
                            "VaVCVCUubjcJKThlyslt29zHuIs5JGppXxX1");
    result = test.serialize(m_serializer.get());
    QCOMPARE(QLatin1StringView(result.toHex()), "3280046f6570534e4c4956473038554a706b3257374a74546"
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
                                                "73354a47707058785831"_L1);
}


void QtProtobufTypesSerializationTest::ComplexTypeSerializeTest()
{
    SimpleStringMessage stringMsg;
    stringMsg.setTestFieldString("qwerty");

    ComplexMessage test;
    QByteArray result = test.serialize(m_serializer.get());
    QEXPECT_FAIL("", "The result of serialization can be either "
                     "an empty message or a message containing empty fields.", Continue);
    QCOMPARE(result.toHex(), "");

    test.setTestFieldInt(42);
    test.setTestComplexField(stringMsg);

    result = test.serialize(m_serializer.get());
    QCOMPARE(QLatin1StringView(result.toHex()), "082a12083206717765727479"_L1);

    stringMsg.setTestFieldString("YVRfJvjxqbgvFwS1YvOZXgtj5ffGLS7AiNHz9oZIoKbm7z"
                                 "8H79xBuyPkpQXvGoO09OY9xRawx3eOAs9xjoTA1xJhrw28"
                                 "TAcq1CebYlC9WUfQC6hIantaNdyHiKToffi0Zt7la42SRx"
                                 "XZSP4GuxbcZIp53pJnyCwfCy1qdFczT0dmn7h8fpyAdemE"
                                 "avwFeda4d0PApGfSU2jLt39X8kYUBxNM2WgALRBgHdVde8"
                                 "7q6Pi5U69TjhMd28W1SFD1DxyogCCrqOct2ZPICoLnrqdF"
                                 "3OdNzjRVLfeyvQ8LgLvRNFR9WfWAyAz79nKgBamd8Ntlvt"
                                 "4Mg35E5gVS2g7AQ7rkm72cBdnW9sCEyGabeXAuH5j4GRbu"
                                 "LT7qBZWDcFLF4SsCdS3WfFGdNHfwaijzykByo71PvFVlTX"
                                 "H2WJWoFvR5FALjBTn7bCdP0pAiSbLCY8Xz2Msc3dBb5Ff9"
                                 "GISPbUpNmUvBdMZMHQvqOmTNXEPpN0b74MDOMQfWJShOo3NkAvMjs");
    test.setTestFieldInt(42);
    test.setTestComplexField(stringMsg);

    result = test.serialize(m_serializer.get());
    QCOMPARE(QLatin1StringView(result.toHex()), "082a128404328104595652664a766a78716267764"
                                                "677533159764f5a5867746a356666474c53374169"
                                                "4e487a396f5a496f4b626d377a384837397842757"
                                                "9506b70515876476f4f30394f5939785261777833"
                                                "654f417339786a6f544131784a687277323854416"
                                                "37131436562596c43395755665143366849616e74"
                                                "614e647948694b546f666669305a74376c6134325"
                                                "35278585a53503447757862635a49703533704a6e"
                                                "79437766437931716446637a5430646d6e3768386"
                                                "670794164656d4561767746656461346430504170"
                                                "47665355326a4c74333958386b595542784e4d325"
                                                "767414c5242674864566465383771365069355536"
                                                "39546a684d6432385731534644314478796f67434"
                                                "372714f6374325a5049436f4c6e72716446334f64"
                                                "4e7a6a52564c6665797651384c674c76524e46523"
                                                "95766574179417a37396e4b6742616d64384e746c"
                                                "7674344d6733354535675653326737415137726b6"
                                                "d37326342646e5739734345794761626558417548"
                                                "356a34475262754c543771425a574463464c46345"
                                                "3734364533357664647644e48667761696a7a796b"
                                                "42796f3731507646566c54584832574a576f46765"
                                                "23546414c6a42546e37624364503070416953624c"
                                                "435938587a324d736333644262354666394749535"
                                                "06255704e6d557642644d5a4d485176714f6d544e"
                                                "584550704e306237344d444f4d5166574a53684f6"
                                                "f334e6b41764d6a73"_L1);


    stringMsg.setTestFieldString("qwerty");
    test.setTestFieldInt(-45);
    test.setTestComplexField(stringMsg);

    result = test.serialize(m_serializer.get());
    QCOMPARE(QLatin1StringView(result.toHex()), "08d3ffffffffffffffff0112083206717765727479"_L1);
}

void QtProtobufTypesSerializationTest::EmptyBytesMessageTest()
{
    SimpleBytesMessage msg;

    QByteArray result = msg.serialize(m_serializer.get());
    QEXPECT_FAIL("", "The result of serialization can be either an empty "
                     "message or a message of size zero.", Continue);
    QVERIFY(result.isEmpty());
}

void QtProtobufTypesSerializationTest::EmptyStringMessageTest()
{
    SimpleStringMessage msg;

    QByteArray result = msg.serialize(m_serializer.get());
    QEXPECT_FAIL("", "The result of serialization can be either an empty "
                     "message or a message of size zero.", Continue);
    QVERIFY(result.isEmpty());
}

void QtProtobufTypesSerializationTest::FieldIndexRangeTest()
{
    FieldIndexTest1Message msg1;
    msg1.setTestField(1);
    QByteArray result = msg1.serialize(m_serializer.get());
    QCOMPARE(QLatin1StringView(result.toHex()), "f80102"_L1);

    FieldIndexTest2Message msg2;
    msg2.setTestField(1);
    result = msg2.serialize(m_serializer.get());
    QCOMPARE(QLatin1StringView(result.toHex()), "f8ff0302"_L1);

    FieldIndexTest3Message msg3;
    msg3.setTestField(1);
    result = msg3.serialize(m_serializer.get());
    QCOMPARE(QLatin1StringView(result.toHex()), "f8ffff0702"_L1);

    FieldIndexTest4Message msg4;
    msg4.setTestField(1);
    result = msg4.serialize(m_serializer.get());
    QCOMPARE(QLatin1StringView(result.toHex()), "f8ffffff0f02"_L1);
}

QTEST_MAIN(QtProtobufTypesSerializationTest)
#include "tst_protobuf_serialization_basictypes.moc"
