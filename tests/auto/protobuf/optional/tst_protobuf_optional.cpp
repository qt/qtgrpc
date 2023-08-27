// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QTest>
#include <QObject>

#include <QtProtobuf/qprotobufserializer.h>

#include <optional.qpb.h>

#include <QDebug>

using namespace Qt::Literals::StringLiterals;

class QtProtobufOptionalTest : public QObject
{
    Q_OBJECT

private slots:
    void init() { serializer.reset(new QProtobufSerializer); }

    void SerializeOptionalInt();
    void SerializeOptionalBool();
    void SerializeOptionalString();
    void SerializeOptionalBytes();
    void SerializeOptionalMessage();

    void DeserializeOptionalInt();
    void DeserializeOptionalBool();
    void DeserializeOptionalString();
    void DeserializeOptionalBytes();
    void DeserializeOptionalMessage();

private:
    std::unique_ptr<QProtobufSerializer> serializer;
};

void QtProtobufOptionalTest::SerializeOptionalInt()
{
    qtprotobufnamespace::optional::tests::OptionalMessage msg;
    QCOMPARE(msg.serialize(serializer.get()).toHex(), ""_ba);
    msg.setTestFieldOpt(0);
    QCOMPARE(msg.serialize(serializer.get()).toHex(), "1000"_ba);
    msg.setTestFieldOpt(84);
    QCOMPARE(msg.serialize(serializer.get()).toHex(), "10a801"_ba);
    msg.clearTestFieldOpt();
    QCOMPARE(msg.serialize(serializer.get()).toHex(), ""_ba);
}

void QtProtobufOptionalTest::SerializeOptionalBool()
{
    qtprotobufnamespace::optional::tests::OptionalMessage msg;
    msg.setTestFieldBoolOpt(false);
    QCOMPARE(msg.serialize(serializer.get()).toHex(), "2000"_ba);
    msg.setTestFieldBoolOpt(true);
    QCOMPARE(msg.serialize(serializer.get()).toHex(), "2001"_ba);
    msg.clearTestFieldBoolOpt();
    QCOMPARE(msg.serialize(serializer.get()).toHex(), ""_ba);
}

void QtProtobufOptionalTest::SerializeOptionalString()
{
    qtprotobufnamespace::optional::tests::OptionalMessage msg;
    msg.setTestFieldStringOpt(""_L1);
    QCOMPARE(msg.serialize(serializer.get()).toHex(), "4200"_ba);
    msg.setTestFieldStringOpt("qwerty"_L1);
    QCOMPARE(msg.serialize(serializer.get()).toHex(), "4206717765727479"_ba);
    msg.clearTestFieldStringOpt();
    QCOMPARE(msg.serialize(serializer.get()).toHex(), ""_ba);
}

void QtProtobufOptionalTest::SerializeOptionalBytes()
{
    qtprotobufnamespace::optional::tests::OptionalMessage msg;
    msg.setTestFieldBytesOpt(""_ba);
    QCOMPARE(msg.serialize(serializer.get()).toHex(), "3200"_ba);
    msg.setTestFieldBytesOpt("qwerty"_ba);
    QCOMPARE(msg.serialize(serializer.get()).toHex(), "3206717765727479"_ba);
    msg.clearTestFieldBytesOpt();
    QCOMPARE(msg.serialize(serializer.get()).toHex(), ""_ba);
}

void QtProtobufOptionalTest::SerializeOptionalMessage()
{
    qtprotobufnamespace::optional::tests::OptionalMessage msg;
    msg.setTestFieldMessageOpt({});
    QCOMPARE(msg.serialize(serializer.get()).toHex(), "5200"_ba);
    msg.clearTestFieldMessageOpt();
    QCOMPARE(msg.serialize(serializer.get()).toHex(), ""_ba);

    // Accessing the field of message type initializes it.
    msg.testFieldMessageOpt();
    QCOMPARE(msg.serialize(serializer.get()).toHex(), "5200"_ba);
    msg.clearTestFieldMessageOpt();
    QCOMPARE(msg.serialize(serializer.get()).toHex(), ""_ba);
}

void QtProtobufOptionalTest::DeserializeOptionalInt()
{
    qtprotobufnamespace::optional::tests::OptionalMessage msg;
    msg.deserialize(serializer.get(), ""_ba);
    QVERIFY(!msg.hasTestFieldOpt());
    msg.deserialize(serializer.get(), QByteArray::fromHex("1000"_ba));
    QVERIFY(msg.hasTestFieldOpt());
    QCOMPARE(msg.testFieldOpt(), 0);

    msg.deserialize(serializer.get(), ""_ba);
    QVERIFY(!msg.hasTestFieldOpt());
    msg.deserialize(serializer.get(), QByteArray::fromHex("10a801"_ba));
    QVERIFY(msg.hasTestFieldOpt());
    QCOMPARE(msg.testFieldOpt(), 84);
}

void QtProtobufOptionalTest::DeserializeOptionalBool()
{
    qtprotobufnamespace::optional::tests::OptionalMessage msg;
    msg.deserialize(serializer.get(), ""_ba);
    QVERIFY(!msg.hasTestFieldBoolOpt());
    msg.deserialize(serializer.get(), QByteArray::fromHex("2000"_ba));
    QVERIFY(msg.hasTestFieldBoolOpt());
    QCOMPARE(msg.testFieldBoolOpt(), false);

    msg.deserialize(serializer.get(), ""_ba);
    QVERIFY(!msg.hasTestFieldBoolOpt());
    msg.deserialize(serializer.get(), QByteArray::fromHex("2001"_ba));
    QVERIFY(msg.hasTestFieldBoolOpt());
    QCOMPARE(msg.testFieldBoolOpt(), true);
}

void QtProtobufOptionalTest::DeserializeOptionalString()
{
    qtprotobufnamespace::optional::tests::OptionalMessage msg;
    msg.deserialize(serializer.get(), ""_ba);
    QVERIFY(!msg.hasTestFieldStringOpt());
    msg.deserialize(serializer.get(), QByteArray::fromHex("4200"_ba));
    QVERIFY(msg.hasTestFieldStringOpt());
    QCOMPARE(msg.testFieldStringOpt(), ""_L1);

    msg.deserialize(serializer.get(), ""_ba);
    QVERIFY(!msg.hasTestFieldStringOpt());
    msg.deserialize(serializer.get(), QByteArray::fromHex("4206717765727479"_ba));
    QVERIFY(msg.hasTestFieldStringOpt());
    QCOMPARE(msg.testFieldStringOpt(), "qwerty"_L1);
}

void QtProtobufOptionalTest::DeserializeOptionalBytes()
{
    qtprotobufnamespace::optional::tests::OptionalMessage msg;
    msg.deserialize(serializer.get(), ""_ba);
    QVERIFY(!msg.hasTestFieldBytesOpt());
    msg.deserialize(serializer.get(), QByteArray::fromHex("3200"_ba));
    QVERIFY(msg.hasTestFieldBytesOpt());
    QCOMPARE(msg.testFieldBytesOpt(), ""_ba);

    msg.deserialize(serializer.get(), ""_ba);
    QVERIFY(!msg.hasTestFieldBytesOpt());
    msg.deserialize(serializer.get(), QByteArray::fromHex("3206717765727479"_ba));
    QVERIFY(msg.hasTestFieldBytesOpt());
    QCOMPARE(msg.testFieldBytesOpt(), "qwerty"_ba);
}

void QtProtobufOptionalTest::DeserializeOptionalMessage()
{
    qtprotobufnamespace::optional::tests::OptionalMessage msg;
    msg.deserialize(serializer.get(), ""_ba);
    QVERIFY(!msg.hasTestFieldMessageOpt());
    msg.deserialize(serializer.get(), QByteArray::fromHex("5200"_ba));
    QVERIFY(msg.hasTestFieldMessageOpt());
    QCOMPARE(msg.testFieldMessageOpt(), qtprotobufnamespace::optional::tests::TestStringMessage());
}

QTEST_MAIN(QtProtobufOptionalTest)

#include "tst_protobuf_optional.moc"
