// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "basicmessages.qpb.h"
#include "fieldindexrange.qpb.h"

#include <QtTest/QtTest>
#include <QProtobufSerializer>

#include <limits>

class QtProtobufRawSerializersTest : public QObject
{
    Q_OBJECT
private slots:
    void init() { m_serializer.reset(new QProtobufSerializer); }
    void ComplexMessageSerializeTest();
    void ComplexMessageDeserializeTest();
private:
    std::unique_ptr<QProtobufSerializer> m_serializer;
};

class NonMessageQObject : public QObject
{
    Q_OBJECT
};

void QtProtobufRawSerializersTest::ComplexMessageSerializeTest()
{
    QProtobufMessage *rawMessage =
            QProtobufMessage::constructByName("qtprotobufnamespace.tests.ComplexMessage");
    m_serializer->deserializeRawMessage(
                rawMessage, QByteArray::fromHex("1208320671776572747908d3ffffffffffffffff01"));
    auto *message = reinterpret_cast<qtprotobufnamespace::tests::ComplexMessage *>(rawMessage);
    QCOMPARE(message->testFieldInt(), -45);
    QCOMPARE(message->testComplexField().testFieldString(), QLatin1String("qwerty"));
    delete rawMessage;
}

void QtProtobufRawSerializersTest::ComplexMessageDeserializeTest()
{
    QProtobufMessage *rawMessage =
            QProtobufMessage::constructByName("qtprotobufnamespace.tests.ComplexMessage");
    auto *message = reinterpret_cast<qtprotobufnamespace::tests::ComplexMessage *>(rawMessage);
    message->setTestFieldInt(-45);
    message->testComplexField().setTestFieldString(QLatin1String("qwerty"));

    QByteArray buffer = m_serializer->serializeRawMessage(rawMessage);
    QCOMPARE(buffer.toHex(), "08d3ffffffffffffffff0112083206717765727479");
    delete rawMessage;
}

QTEST_MAIN(QtProtobufRawSerializersTest)
#include "tst_protobuf_raw_serializers.moc"
