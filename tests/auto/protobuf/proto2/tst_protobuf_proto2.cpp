// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QObject>
#include <QTest>

#include <QtProtobuf/qprotobufjsonserializer.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <proto2messages.qpb.h>

using namespace Qt::Literals::StringLiterals;

class QtProtobufProto2Test : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void serializeRequiredDefaultValues();
    void serializeRequiredExplicitDefaultValues();
    void serializeRequiredDefaultValuesJson();
    void serializeRequiredExplicitDefaultValuesJson();
    void serializeNestedRequiredDefaultValues();
    void serializeNestedRequiredDefaultValuesJson();
    void serializeRequiredCycle();
    void deserializeRejectsMissingRequiredFields_data();
    void deserializeRejectsMissingRequiredFields();
    void deserializeAcceptsCompletePayload();
    void deserializeJsonStaysLenient();
    void deserializeChecksNestedRequiredFields();
};

// A proto2 required field has no implicit presence: it must appear on the wire even
// when it holds the type's default value, or a conformant parser rejects the message.
void QtProtobufProto2Test::serializeRequiredDefaultValues()
{
    QProtobufSerializer serializer;
    qtprotobufnamespace::proto2::tests::RequiredMessage msg;

    QCOMPARE(msg.serialize(&serializer).toHex(), "080010001a00"_ba);
}

void QtProtobufProto2Test::serializeRequiredExplicitDefaultValues()
{
    QProtobufSerializer serializer;
    qtprotobufnamespace::proto2::tests::RequiredMessage msg;
    msg.setTestFieldInt(0);
    msg.setTestFieldBool(false);
    msg.setTestFieldString(QString());

    QCOMPARE(msg.serialize(&serializer).toHex(), "080010001a00"_ba);
}

void QtProtobufProto2Test::serializeRequiredDefaultValuesJson()
{
    QProtobufJsonSerializer serializer;
    qtprotobufnamespace::proto2::tests::RequiredMessage msg;

    QCOMPARE(msg.serialize(&serializer),
             R"({"testFieldBool":false,"testFieldInt":0,"testFieldString":""})"_ba);
}

void QtProtobufProto2Test::serializeRequiredExplicitDefaultValuesJson()
{
    QProtobufJsonSerializer serializer;
    qtprotobufnamespace::proto2::tests::RequiredMessage msg;
    msg.setTestFieldInt(0);
    msg.setTestFieldBool(false);
    msg.setTestFieldString(QString());

    QCOMPARE(msg.serialize(&serializer),
             R"({"testFieldBool":false,"testFieldInt":0,"testFieldString":""})"_ba);
}

void QtProtobufProto2Test::serializeNestedRequiredDefaultValues()
{
    QProtobufSerializer serializer;
    qtprotobufnamespace::proto2::tests::NestedRequiredMessage msg;

    const QByteArray payload = msg.serialize(&serializer);
    QCOMPARE(payload.toHex(), "0a06080010001a00"_ba);
    QVERIFY(!msg.hasNested());

    qtprotobufnamespace::proto2::tests::NestedRequiredMessage roundTripped;
    QVERIFY(serializer.deserialize(&roundTripped, payload));
}

void QtProtobufProto2Test::serializeNestedRequiredDefaultValuesJson()
{
    QProtobufJsonSerializer serializer;
    qtprotobufnamespace::proto2::tests::NestedRequiredMessage msg;

    QCOMPARE(msg.serialize(&serializer),
             R"({"nested":{"testFieldBool":false,"testFieldInt":0,"testFieldString":""}})"_ba);
}

// No finite message satisfies a cycle of required fields, so the default
// submessage is written once instead of recursing without end.
void QtProtobufProto2Test::serializeRequiredCycle()
{
    QProtobufSerializer serializer;
    qtprotobufnamespace::proto2::tests::RequiredCycle msg;

    QCOMPARE(msg.serialize(&serializer).toHex(), "0a00"_ba);
}

void QtProtobufProto2Test::deserializeRejectsMissingRequiredFields_data()
{
    QTest::addColumn<QByteArray>("payload");
    QTest::addColumn<QString>("missing");

    QTest::newRow("all missing") << ""_ba << u"testFieldInt, testFieldBool, testFieldString"_s;
    QTest::newRow("int present") << "0800"_ba << u"testFieldBool, testFieldString"_s;
    QTest::newRow("string missing") << "08001000"_ba << u"testFieldString"_s;
    // A scalar may repeat on the wire; counting occurrences would hide the missing field.
    QTest::newRow("int repeated") << "08000800"_ba
                                  << u"testFieldBool, testFieldString"_s;
}

void QtProtobufProto2Test::deserializeRejectsMissingRequiredFields()
{
    QFETCH(const QByteArray, payload);
    QFETCH(const QString, missing);

    QProtobufSerializer serializer;
    qtprotobufnamespace::proto2::tests::RequiredMessage msg;

    QVERIFY(!serializer.deserialize(&msg, QByteArray::fromHex(payload)));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);
    QVERIFY2(serializer.lastErrorString().contains(missing),
             qPrintable(serializer.lastErrorString()));
}

void QtProtobufProto2Test::deserializeAcceptsCompletePayload()
{
    QProtobufSerializer serializer;
    qtprotobufnamespace::proto2::tests::RequiredMessage msg;

    QVERIFY(serializer.deserialize(&msg, QByteArray::fromHex("080010001a00"_ba)));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
}

// The reference implementation enforces required fields when parsing the binary
// format but not when parsing JSON. Being stricter would fail conformance.
void QtProtobufProto2Test::deserializeJsonStaysLenient()
{
    QProtobufJsonSerializer serializer;
    qtprotobufnamespace::proto2::tests::RequiredMessage msg;

    QVERIFY(serializer.deserialize(&msg, "{}"_ba));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
}

void QtProtobufProto2Test::deserializeChecksNestedRequiredFields()
{
    QProtobufSerializer serializer;
    qtprotobufnamespace::proto2::tests::NestedRequiredMessage msg;

    // Field 1, length 2: a RequiredMessage carrying only testFieldInt.
    QVERIFY(!serializer.deserialize(&msg, QByteArray::fromHex("0a020800"_ba)));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);
    QVERIFY2(serializer.lastErrorString().contains(u"testFieldBool"_s),
             qPrintable(serializer.lastErrorString()));

    QVERIFY(serializer.deserialize(&msg, QByteArray::fromHex("0a06080010001a00"_ba)));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
}

QTEST_MAIN(QtProtobufProto2Test)

#include "tst_protobuf_proto2.moc"
