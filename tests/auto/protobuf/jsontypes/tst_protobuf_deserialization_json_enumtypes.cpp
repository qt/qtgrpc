// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "enummessages.qpb.h"

#include <QProtobufJsonSerializer>
#include <QTest>

class QtProtobufEnumTypesDeserializationTest : public QObject
{
    Q_OBJECT
private slots:
    void init() { m_serializer.reset(new QProtobufJsonSerializer); }
    void SimpleEnumMessageDeserializeTest();
    void RepeatedEnumMessageTest();

private:
    std::unique_ptr<QProtobufJsonSerializer> m_serializer;
};

using namespace qtprotobufnamespace::tests;

void QtProtobufEnumTypesDeserializationTest::SimpleEnumMessageDeserializeTest()
{
    SimpleEnumMessage test;
    test.deserialize(m_serializer.get(), "{\"localEnum\":\"LOCAL_ENUM_VALUE2\"}");
    QCOMPARE(test.localEnum(), SimpleEnumMessage::LocalEnum::LOCAL_ENUM_VALUE2);
}

void QtProtobufEnumTypesDeserializationTest::RepeatedEnumMessageTest()
{
    RepeatedEnumMessage msg;

    msg.deserialize(m_serializer.get(), QByteArray());
    QVERIFY(msg.localEnumList().isEmpty());

    msg.deserialize(m_serializer.get(),
                    "{\"localEnumList\":[\"LOCAL_ENUM_VALUE0\","
                    "\"LOCAL_ENUM_VALUE1\",\"LOCAL_ENUM_VALUE2\","
                    "\"LOCAL_ENUM_VALUE1\",\"LOCAL_ENUM_VALUE2\","
                    "\"LOCAL_ENUM_VALUE3\"]}");
    QVERIFY((msg.localEnumList()
             == RepeatedEnumMessage::LocalEnumRepeated{
                 RepeatedEnumMessage::LocalEnum::LOCAL_ENUM_VALUE0,
                 RepeatedEnumMessage::LocalEnum::LOCAL_ENUM_VALUE1,
                 RepeatedEnumMessage::LocalEnum::LOCAL_ENUM_VALUE2,
                 RepeatedEnumMessage::LocalEnum::LOCAL_ENUM_VALUE1,
                 RepeatedEnumMessage::LocalEnum::LOCAL_ENUM_VALUE2,
                 RepeatedEnumMessage::LocalEnum::LOCAL_ENUM_VALUE3 }));
}

QTEST_MAIN(QtProtobufEnumTypesDeserializationTest)
#include "tst_protobuf_deserialization_json_enumtypes.moc"
