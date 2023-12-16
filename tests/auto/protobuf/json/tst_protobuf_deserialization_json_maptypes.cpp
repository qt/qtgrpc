// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "mapmessages.qpb.h"

#include <QObject>

#include <QProtobufJsonSerializer>
#include <QTest>

#include <limits>

using namespace qtprotobufnamespace::tests;
using namespace Qt::Literals::StringLiterals;

class QtProtobufJsonMapTypesDeserializationTest : public QObject
{
    Q_OBJECT
private slots:
    void init() { serializer.reset(new QProtobufJsonSerializer); }

    void SimpleFixed32StringMapDeserializeTest();
    void SimpleSFixed32StringMapDeserializeTest();
    void SimpleInt32StringMapDeserializeTest();
    void SimpleSInt32StringMapDeserializeTest();
    void SimpleUInt32StringMapDeserializeTest();
    void SimpleFixed64StringMapDeserializeTest();
    void SimpleSFixed64StringMapDeserializeTest();
    void SimpleInt64StringMapDeserializeTest();
    void SimpleSInt64StringMapDeserializeTest();
    void SimpleUInt64StringMapDeserializeTest();
    void SimpleStringStringMapDeserializeTest();
    void SimpleFixed32ComplexMapDeserializeTest();
    void BoolBoolMapDeserializeTest();

private:
    std::unique_ptr<QProtobufJsonSerializer> serializer;
};

void QtProtobufJsonMapTypesDeserializationTest::SimpleFixed32StringMapDeserializeTest()
{
    SimpleFixed32StringMapMessage test;
    test.deserialize(
        serializer.get(),
        "{\"mapField\":{\"10\":\"ten\",\"15\":\"fifteen\",\"42\":\"fourty two\",\"0\":\"\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleFixed32StringMapMessage::MapFieldEntry({ { 10, { "ten" } },
                                                            { 42, { "fourty two" } },
                                                            { 15, { "fifteen" } },
                                                            { 0, { "" } } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleSFixed32StringMapDeserializeTest()
{
    SimpleSFixed32StringMapMessage test;
    test.deserialize(
        serializer.get(),
        "{\"mapField\":{\"-42\":\"minus fourty two\",\"10\":\"ten\",\"15\":\"fifteen\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleSFixed32StringMapMessage::MapFieldEntry({ { 10, { "ten" } },
                                                             { -42, { "minus fourty two" } },
                                                             { 15, { "fifteen" } } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleInt32StringMapDeserializeTest()
{
    SimpleInt32StringMapMessage test;
    test.deserialize(
        serializer.get(),
        "{\"mapField\":{\"-10\":\"minus ten\",\"15\":\"fifteen\",\"42\":\"fourty two\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleInt32StringMapMessage::MapFieldEntry({ { -10, { "minus ten" } },
                                                          { 42, { "fourty two" } },
                                                          { 15, { "fifteen" } } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleSInt32StringMapDeserializeTest()
{
    SimpleSInt32StringMapMessage test;
    test.deserialize(
        serializer.get(),
        "{\"mapField\":{\"-42\":\"minus fourty two\",\"10\":\"ten\",\"15\":\"fifteen\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleSInt32StringMapMessage::MapFieldEntry({ { 10, { "ten" } },
                                                           { -42, { "minus fourty two" } },
                                                           { 15, { "fifteen" } } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleUInt32StringMapDeserializeTest()
{
    SimpleUInt32StringMapMessage test;
    test.deserialize(serializer.get(),
                     "{\"mapField\":{\"10\":\"ten\",\"15\":\"fifteen\",\"42\":\"fourty two\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleUInt32StringMapMessage::MapFieldEntry({ { 10, { "ten" } },
                                                           { 42, { "fourty two" } },
                                                           { 15, { "fifteen" } } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleFixed64StringMapDeserializeTest()
{
    SimpleFixed64StringMapMessage test;
    test.deserialize(serializer.get(),
                     "{\"mapField\":{\"10\":\"ten\",\"15\":\"fifteen\",\"42\":\"fourty two\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleFixed64StringMapMessage::MapFieldEntry({ { 10, { "ten" } },
                                                            { 42, { "fourty two" } },
                                                            { 15, { "fifteen" } } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleSFixed64StringMapDeserializeTest()
{
    SimpleSFixed64StringMapMessage test;
    test.deserialize(
        serializer.get(),
        "{\"mapField\":{\"-42\":\"minus fourty two\",\"10\":\"ten\",\"15\":\"fifteen\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleSFixed64StringMapMessage::MapFieldEntry({ { 10, { "ten" } },
                                                             { -42, { "minus fourty two" } },
                                                             { 15, { "fifteen" } } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleInt64StringMapDeserializeTest()
{
    SimpleInt64StringMapMessage test;
    test.deserialize(
        serializer.get(),
        "{\"mapField\":{\"-10\":\"minus ten\",\"15\":\"fifteen\",\"42\":\"fourty two\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleInt64StringMapMessage::MapFieldEntry({ { -10, { "minus ten" } },
                                                          { 42, { "fourty two" } },
                                                          { 15, { "fifteen" } } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleSInt64StringMapDeserializeTest()
{
    SimpleSInt64StringMapMessage test;
    test.deserialize(
        serializer.get(),
        "{\"mapField\":{\"-42\":\"minus fourty two\",\"10\":\"ten\",\"15\":\"fifteen\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleSInt64StringMapMessage::MapFieldEntry({ { 10, { "ten" } },
                                                           { -42, { "minus fourty two" } },
                                                           { 15, { "fifteen" } } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleUInt64StringMapDeserializeTest()
{
    SimpleUInt64StringMapMessage test;
    test.deserialize(serializer.get(),
                     "{\"mapField\":{\"10\":\"ten\",\"15\":\"fifteen\",\"42\":\"fourty two\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleUInt64StringMapMessage::MapFieldEntry({ { 10, { "ten" } },
                                                           { 42, { "fourty two" } },
                                                           { 15, { "fifteen" } } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleStringStringMapDeserializeTest()
{
    SimpleStringStringMapMessage test;
    test.deserialize(serializer.get(),
                     "{\"mapField\":{\"ben\":\"ten\",\"sweet\":\"fifteen\","
                     "\"what is the answer?\":\"fourty two\"}}"_ba);
    QCOMPARE(test.mapField(),
             SimpleStringStringMapMessage::MapFieldEntry({ { "ben", "ten" },
                                                           { "what is the answer?", "fourty two" },
                                                           { "sweet", "fifteen" } }));
}

void QtProtobufJsonMapTypesDeserializationTest::SimpleFixed32ComplexMapDeserializeTest()
{
    SimpleFixed32ComplexMessageMapMessage test;
    test.deserialize(
        serializer.get(),
        "{\"mapField\":{\"10\":{\"testComplexField\":{\"testFieldString\":\"ten sixteen\"},"
        "\"testFieldInt\":16},\"42\":{\"testComplexField\":{\"testFieldString\":"
        "\"fourty two ten sixteen\"},\"testFieldInt\":10},\"65555\":{\"testComplexField\":"
        "{\"testFieldString\":\"WUT?\"},\"testFieldInt\":10}}}"_ba);
    QCOMPARE(QAbstractProtobufSerializer::NoError, serializer->deserializationError());

    qtprotobufnamespace::tests::SimpleStringMessage stringMsg;

    stringMsg.setTestFieldString({ "ten sixteen" });
    ComplexMessage expected1;
    expected1.setTestFieldInt(16);
    expected1.setTestComplexField(stringMsg);

    stringMsg.setTestFieldString({ "fourty two ten sixteen" });
    ComplexMessage expected2;
    expected2.setTestFieldInt(10);
    expected2.setTestComplexField(stringMsg);

    stringMsg.setTestFieldString({ "WUT?" });
    ComplexMessage expected3;
    expected3.setTestFieldInt(10);
    expected3.setTestComplexField(stringMsg);

    SimpleFixed32ComplexMessageMapMessage expected;
    expected.setMapField({ { 10, expected1 }, { 42, expected2 }, { 65555, expected3 } });

    QCOMPARE(test.mapField()[10], expected1);
    QCOMPARE(test.mapField()[42], expected2);
    QCOMPARE(test.mapField()[65555], expected3);
}

void QtProtobufJsonMapTypesDeserializationTest::BoolBoolMapDeserializeTest()
{
    BoolBoolMessageMapMessage test;
    test.deserialize(serializer.get(), "{\"mapField\":{\"true\":\"false\",\"false\":\"true\"}}");
    QCOMPARE(QAbstractProtobufSerializer::NoError, serializer->deserializationError());

    QCOMPARE(test.mapField()[true], false);
    QCOMPARE(test.mapField()[false], true);
}

QTEST_MAIN(QtProtobufJsonMapTypesDeserializationTest)
#include "tst_protobuf_deserialization_json_maptypes.moc"
