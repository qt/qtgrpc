// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QObject>
#include <QTest>

#include <QtProtobuf/qprotobufserializer.h>

#include <proto2messages.qpb.h>
#include <proto2messages.pb.h>

#include <string>

using namespace Qt::Literals::StringLiterals;

// qtprotobufgen puts its output in QtProtobufInterop, so the two implementations
// of the same .proto can be compared side by side in one translation unit.
using QtMessage = QtProtobufInterop::qtprotobufnamespace::proto2::tests::RequiredMessage;
using ReferenceMessage = qtprotobufnamespace::proto2::tests::RequiredMessage;
using QtNestedMessage = QtProtobufInterop::qtprotobufnamespace::proto2::tests::
    NestedRequiredMessage;
using ReferenceNestedMessage = qtprotobufnamespace::proto2::tests::NestedRequiredMessage;

class QtProtobufInteropTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void requiredMessage_data();
    void requiredMessage();
    void nestedRequiredMessage();
    void nestedRequiredMessageIncomplete();
};

void QtProtobufInteropTest::requiredMessage_data()
{
    QTest::addColumn<qint32>("intValue");
    QTest::addColumn<bool>("boolValue");
    QTest::addColumn<QByteArray>("stringValue");

    QTest::newRow("all defaults") << 0 << false << ""_ba;
    QTest::newRow("int set") << 42 << false << ""_ba;
    QTest::newRow("bool set") << 0 << true << ""_ba;
    QTest::newRow("string set") << 0 << false << "qwerty"_ba;
    QTest::newRow("all set") << -7 << true << "qwerty"_ba;
}

void QtProtobufInteropTest::requiredMessage()
{
    QFETCH(const qint32, intValue);
    QFETCH(const bool, boolValue);
    QFETCH(const QByteArray, stringValue);

    QProtobufSerializer serializer;

    QtMessage qtMessage;
    qtMessage.setTestFieldInt(intValue);
    qtMessage.setTestFieldBool(boolValue);
    qtMessage.setTestFieldString(QString::fromUtf8(stringValue));
    const QByteArray qtBytes = qtMessage.serialize(&serializer);

    ReferenceMessage reference;
    reference.set_testfieldint(intValue);
    reference.set_testfieldbool(boolValue);
    reference.set_testfieldstring(stringValue.toStdString());
    std::string referenceBytes;
    QVERIFY(reference.SerializeToString(&referenceBytes));

    QCOMPARE(qtBytes.toHex(), QByteArrayView(referenceBytes).toByteArray().toHex());

    // Completeness is checked separately so a missing required field is reported as
    // such instead of as a generic parse failure.
    ReferenceMessage parsed;
    QVERIFY(parsed.ParsePartialFromArray(qtBytes.data(), int(qtBytes.size())));
    QVERIFY2(parsed.IsInitialized(),
             qPrintable(QString::fromStdString(parsed.InitializationErrorString())));
    QCOMPARE(parsed.testfieldint(), intValue);
    QCOMPARE(parsed.testfieldbool(), boolValue);
    QCOMPARE(QByteArray::fromStdString(parsed.testfieldstring()), stringValue);

    QtMessage roundTripped;
    QVERIFY(serializer.deserialize(&roundTripped, QByteArrayView(referenceBytes)));
    QCOMPARE(roundTripped.testFieldInt(), intValue);
    QCOMPARE(roundTripped.testFieldBool(), boolValue);
    QCOMPARE(roundTripped.testFieldString().toUtf8(), stringValue);
}

void QtProtobufInteropTest::nestedRequiredMessage()
{
    QProtobufSerializer serializer;
    const QByteArray qtBytes = QtNestedMessage().serialize(&serializer);

    ReferenceNestedMessage reference;
    ReferenceMessage *nested = reference.mutable_nested();
    nested->set_testfieldint(0);
    nested->set_testfieldbool(false);
    nested->set_testfieldstring(std::string());
    std::string referenceBytes;
    QVERIFY(reference.SerializeToString(&referenceBytes));

    QCOMPARE(qtBytes.toHex(), QByteArrayView(referenceBytes).toByteArray().toHex());

    ReferenceNestedMessage parsed;
    QVERIFY(parsed.ParsePartialFromArray(qtBytes.data(), int(qtBytes.size())));
    QVERIFY2(parsed.IsInitialized(),
             qPrintable(QString::fromStdString(parsed.InitializationErrorString())));
}

void QtProtobufInteropTest::nestedRequiredMessageIncomplete()
{
    ReferenceNestedMessage reference;
    reference.mutable_nested()->set_testfieldint(42);
    QVERIFY(!reference.IsInitialized());
    std::string referenceBytes;
    QVERIFY(reference.SerializePartialToString(&referenceBytes));

    QProtobufSerializer serializer;
    QtNestedMessage qtMessage;
    QVERIFY(!serializer.deserialize(&qtMessage, QByteArrayView(referenceBytes)));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);
}

QTEST_MAIN(QtProtobufInteropTest)

#include "tst_protobuf_interop.moc"
