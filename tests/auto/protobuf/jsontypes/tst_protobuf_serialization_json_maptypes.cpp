// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "mapmessages.qpb.h"

#include <QObject>
#include <QProtobufJsonSerializer>
#include <QTest>

using namespace qtprotobufnamespace::tests;
using namespace Qt::Literals::StringLiterals;

class QtProtobufJsonMapTypesSerializationTest : public QObject
{
    Q_OBJECT
private slots:
    void init() { m_serializer.reset(new QProtobufJsonSerializer); }
    void SimpleFixed32StringMapSerializeTest();
    void SimpleSFixed32StringMapSerializeTest();
    void SimpleInt32StringMapSerializeTest();
    void SimpleSInt32StringMapSerializeTest();
    void SimpleUInt32StringMapSerializeTest();
    void SimpleFixed64StringMapSerializeTest();
    void SimpleSFixed64StringMapSerializeTest();
    void SimpleInt64StringMapSerializeTest();
    void SimpleSInt64StringMapSerializeTest();
    void SimpleUInt64StringMapSerializeTest();
    void SimpleStringStringMapSerializeTest();

private:
    std::unique_ptr<QProtobufJsonSerializer> m_serializer;
};

void QtProtobufJsonMapTypesSerializationTest::SimpleFixed32StringMapSerializeTest()
{
    SimpleFixed32StringMapMessage test;
    test.setMapField({ { 10, { "ten" } }, { 42, { "fourty two" } }, { 15, { "fifteen" } } });
    QByteArray result = test.serialize(m_serializer.get());
}

void QtProtobufJsonMapTypesSerializationTest::SimpleSFixed32StringMapSerializeTest()
{
    SimpleSFixed32StringMapMessage test;
    test.setMapField({ { 10, { "ten" } }, { -42, { "minus fourty two" } }, { 15, { "fifteen" } } });
    QByteArray result = test.serialize(m_serializer.get());
}

void QtProtobufJsonMapTypesSerializationTest::SimpleInt32StringMapSerializeTest()
{
    SimpleInt32StringMapMessage test;
    test.setMapField({ { -10, { "minus ten" } }, { 42, { "fourty two" } }, { 15, { "fifteen" } } });
    QByteArray result = test.serialize(m_serializer.get());

}

void QtProtobufJsonMapTypesSerializationTest::SimpleSInt32StringMapSerializeTest()
{
    SimpleSInt32StringMapMessage test;
    test.setMapField({ { 10, { "ten" } }, { -42, { "minus fourty two" } }, { 15, { "fifteen" } } });
    QByteArray result = test.serialize(m_serializer.get());

}

void QtProtobufJsonMapTypesSerializationTest::SimpleUInt32StringMapSerializeTest()
{
    SimpleUInt32StringMapMessage test;
    test.setMapField({ { 10, { "ten" } }, { 42, { "fourty two" } }, { 15, { "fifteen" } } });
    QByteArray result = test.serialize(m_serializer.get());

}

void QtProtobufJsonMapTypesSerializationTest::SimpleFixed64StringMapSerializeTest()
{
    SimpleFixed64StringMapMessage test;
    test.setMapField({ { 10, { "ten" } }, { 42, { "fourty two" } }, { 15, { "fifteen" } } });
    QByteArray result = test.serialize(m_serializer.get());

}

void QtProtobufJsonMapTypesSerializationTest::SimpleSFixed64StringMapSerializeTest()
{
    SimpleSFixed64StringMapMessage test;
    test.setMapField({ { 10, { "ten" } }, { -42, { "minus fourty two" } }, { 15, { "fifteen" } } });
    QByteArray result = test.serialize(m_serializer.get());

}

void QtProtobufJsonMapTypesSerializationTest::SimpleInt64StringMapSerializeTest()
{
    SimpleInt64StringMapMessage test;
    test.setMapField({ { -10, { "minus ten" } }, { 42, { "fourty two" } }, { 15, { "fifteen" } } });
    QByteArray result = test.serialize(m_serializer.get());

}

void QtProtobufJsonMapTypesSerializationTest::SimpleSInt64StringMapSerializeTest()
{
    SimpleSInt64StringMapMessage test;
    test.setMapField({ { 10, { "ten" } }, { -42, { "minus fourty two" } }, { 15, { "fifteen" } } });
    QByteArray result = test.serialize(m_serializer.get());

}

void QtProtobufJsonMapTypesSerializationTest::SimpleUInt64StringMapSerializeTest()
{
    SimpleUInt64StringMapMessage test;
    test.setMapField({ { 10, { "ten" } }, { 42, { "fourty two" } }, { 15, { "fifteen" } } });
    QByteArray result = test.serialize(m_serializer.get());

}

void QtProtobufJsonMapTypesSerializationTest::SimpleStringStringMapSerializeTest()
{
    SimpleStringStringMapMessage test;
    test.setMapField({ { "ben", "ten" },
                       { "what is the answer?", "fourty two" },
                       { "sweet", "fifteen" } });
    QByteArray result = test.serialize(m_serializer.get());

}

QTEST_MAIN(QtProtobufJsonMapTypesSerializationTest)
#include "tst_protobuf_serialization_json_maptypes.moc"
