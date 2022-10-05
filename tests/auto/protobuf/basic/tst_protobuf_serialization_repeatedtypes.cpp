// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2022 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "basicmessages.qpb.h"
#include "repeatedmessages.qpb.h"

#include <QTest>
#include <QProtobufSerializer>

class QtProtobufRepeatedTypesSerializationTest : public QObject
{
    Q_OBJECT
private slots:
    void init() {
        m_serializer.reset(new QProtobufSerializer);
    }
    void RepeatedIntMessageTest();
    void RepeatedSIntMessageTest();
    void RepeatedUIntMessageTest();
    void RepeatedInt64MessageTest();
    void RepeatedSInt64MessageTest();
    void RepeatedUInt64MessageTest();
    void RepeatedFixedIntMessageTest();
    void RepeatedSFixedIntMessageTest();
    void RepeatedFixedInt64MessageTest();
    void RepeatedSFixedInt64MessageTest();
    void RepeatedStringMessageTest();
    void RepeatedFloatMessageTest();
    void RepeatedDoubleMessageTest();
    void RepeatedBytesMessageTest();
    void RepeatedComplexMessageTest();
    void RepeatedBoolMessageTest();
private:
    std::unique_ptr<QProtobufSerializer> m_serializer;
};

using namespace qtprotobufnamespace::tests;

void QtProtobufRepeatedTypesSerializationTest::RepeatedIntMessageTest()
{
    RepeatedIntMessage test;
    test.setTestRepeatedInt({0, 1, 321, -65999, 123245, -3, 3});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a1c0001c102b1fcfbffffffffffff01edc207fdffffffffffffffff0103");
    test.setTestRepeatedInt(QtProtobuf::int32List());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedSIntMessageTest()
{
    RepeatedSIntMessage test;
    test.setTestRepeatedInt({1, 321, -65999, 123245, -3, 3, 0});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a0c0282059d8708da850f050600");

    test.setTestRepeatedInt(QtProtobuf::sint32List());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedUIntMessageTest()
{
    RepeatedUIntMessage test;
    test.setTestRepeatedInt({1, 0, 321, 65999, 123245, 3});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a0b0100c102cf8304edc20703");

    test.setTestRepeatedInt(QtProtobuf::uint32List());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedInt64MessageTest()
{
    RepeatedInt64Message test;
    test.setTestRepeatedInt({1, 321, -65999, 12324523123123, -3, 0, 3});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a2001c102b1fcfbffffffffffff01b3c3cab6d8e602fdffffffffffffffff010003");

    test.setTestRepeatedInt(QtProtobuf::int64List());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedSInt64MessageTest()
{
    RepeatedSInt64Message test;
    test.setTestRepeatedInt({1, 321, -65999, 12324523123123, 0, -3, 3});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a100282059d8708e68695edb0cd05000506");

    test.setTestRepeatedInt(QtProtobuf::sint64List());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedUInt64MessageTest()
{
    RepeatedUInt64Message test;
    test.setTestRepeatedInt({1, 321, 0, 65999, 123245, 123245324235425234, 3});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a1401c10200cf8304edc207d28b9fda82dff6da0103");

    test.setTestRepeatedInt(QtProtobuf::uint64List());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedFixedIntMessageTest()
{
    RepeatedFixedIntMessage test;
    test.setTestRepeatedInt({1, 321, 65999, 12324523, 3, 3, 0});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a1c0100000041010000cf010100ab0ebc00030000000300000000000000");
    test.setTestRepeatedInt(QtProtobuf::fixed32List());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedSFixedIntMessageTest()
{
    RepeatedSFixedIntMessage test;
    test.setTestRepeatedInt({0, 1, 321, -65999, 12324523, -3, 3});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a1c00000000010000004101000031fefeffab0ebc00fdffffff03000000");

    test.setTestRepeatedInt(QtProtobuf::sfixed32List());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedFixedInt64MessageTest()
{
    RepeatedFixedInt64Message test;
    test.setTestRepeatedInt({1, 321, 65999, 123245324235425234, 3, 3, 0});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a3801000000000000004101000000000000cf01010000000000d2c5472bf8dab501030000000000000003000000000000000000000000000000");

    test.setTestRepeatedInt(QtProtobuf::fixed64List());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedSFixedInt64MessageTest()
{
    RepeatedSFixedInt64Message test;
    test.setTestRepeatedInt({1, 321, -65999, 123245324235425234, -3, 3, 0});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a380100000000000000410100000000000031fefeffffffffffd2c5472bf8dab501fdffffffffffffff03000000000000000000000000000000");

    test.setTestRepeatedInt(QtProtobuf::sfixed64List());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedStringMessageTest()
{
    RepeatedStringMessage test;
    test.setTestRepeatedString({"aaaa","bbbbb","ccc","dddddd","eeeee", ""});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a04616161610a0562626262620a036363630a066464646464640a0565656565650a00");

    test.setTestRepeatedString(QStringList());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedFloatMessageTest()
{
    RepeatedFloatMessage test;
    test.setTestRepeatedFloat({0.4f, 1.2f, 0.5f, 1.4f, 0.6f});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a14cdcccc3e9a99993f0000003f3333b33f9a99193f");

    test.setTestRepeatedFloat(QtProtobuf::floatList());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedDoubleMessageTest()
{
    RepeatedDoubleMessage test;
    test.setTestRepeatedDouble({0.1, 0.2, 0.3, 0.4, 0.5});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a289a9999999999b93f9a9999999999c93f333333333333d33f9a9999999999d93f000000000000e03f");

    test.setTestRepeatedDouble(QtProtobuf::doubleList());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedBytesMessageTest()
{
    RepeatedBytesMessage test;
    test.setTestRepeatedBytes({QByteArray::fromHex("010203040506"),
                               QByteArray::fromHex("ffffffff"),
                               QByteArray::fromHex("eaeaeaeaea"),
                               QByteArray::fromHex("010203040506")});
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a060102030405060a04ffffffff0a05eaeaeaeaea0a06010203040506");

    test.setTestRepeatedBytes(QByteArrayList());
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());

    test.setTestRepeatedBytes({QByteArray::fromHex("010203040506"),
                               QByteArray::fromHex(""),
                               QByteArray::fromHex("eaeaeaeaea"),
                               QByteArray::fromHex("010203040506")});
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex().toStdString().c_str(), "0a060102030405060a000a05eaeaeaeaea0a06010203040506");
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedComplexMessageTest()
{
    SimpleStringMessage stringMsg;
    stringMsg.setTestFieldString("qwerty");
    std::shared_ptr<ComplexMessage> msg(new ComplexMessage);
    msg->setTestFieldInt(25);
    msg->setTestComplexField(stringMsg);
    RepeatedComplexMessage test;
    test.setTestRepeatedComplex({msg, msg, msg});
    QByteArray result = test.serialize(m_serializer.get());

    QVERIFY(result == QByteArray::fromHex("0a0c0819120832067177657274790a0c0819120832067177657274790a0c081912083206717765727479")
                || result == QByteArray::fromHex("0a0c1208320671776572747908190a0c1208320671776572747908190a0c120832067177657274790819"));

    test.setTestRepeatedComplex({});
    result = test.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufRepeatedTypesSerializationTest::RepeatedBoolMessageTest()
{
    RepeatedBoolMessage boolMsg;
    boolMsg.setTestRepeatedBool({ true, true, true, false, false, false, false, false, false, false,
                                  false, false, true });
    QByteArray result = boolMsg.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "0a0d01010100000000000000000001");
}

QTEST_MAIN(QtProtobufRepeatedTypesSerializationTest)
#include "tst_protobuf_serialization_repeatedtypes.moc"
