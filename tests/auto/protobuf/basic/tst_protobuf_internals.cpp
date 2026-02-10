// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "basicmessages.qpb.h"

#include <QTest>

using namespace qtprotobufnamespace::tests;

class QtProtobufInternalsTest : public QObject
{
    Q_OBJECT
public:
    QtProtobufInternalsTest() = default;

private Q_SLOTS:
    void nullPointerMessageTest();
    void nullPointerGetterMessageTest();
    void qprotobufmessageCastTest();
};

void QtProtobufInternalsTest::nullPointerMessageTest()
{
    SimpleStringMessage stringMsg;
    stringMsg.setTestFieldString({ "not null" });
    ComplexMessage msg;
    msg.setTestFieldInt(0);
    msg.setTestComplexField(stringMsg);

    msg.setProperty("testComplexField_p",
                    QVariant::fromValue(static_cast<SimpleStringMessage *>(nullptr)));
    QVERIFY(msg.testComplexField().testFieldString().isEmpty());
    QVERIFY(msg.property("testComplexField_p").value<SimpleStringMessage *>() != nullptr);
}

void QtProtobufInternalsTest::nullPointerGetterMessageTest()
{
    ComplexMessage msg;
    QVERIFY(!msg.hasTestComplexField());
    QVERIFY(msg.property("testComplexField_p").value<SimpleStringMessage *>() != nullptr);
    msg.setProperty("testComplexField_p",
                    QVariant::fromValue(static_cast<SimpleStringMessage *>(nullptr)));
    QVERIFY(msg.testComplexField().testFieldString().isEmpty());
    QVERIFY(msg.property("testComplexField_p").value<SimpleStringMessage *>() != nullptr);
}

class DerivedMessage : public SimpleIntMessage
{
    Q_PROTOBUF_OBJECT
public:
    QByteArray data;
};

void QtProtobufInternalsTest::qprotobufmessageCastTest()
{
    auto *original = new SimpleStringMessage();
    QProtobufMessage *base = original;

    QVERIFY(original);
    // Case 1: Successful downcast
    auto *castToSelf = qprotobufmessage_cast<SimpleStringMessage *>(base);
    QCOMPARE_EQ(castToSelf, original);
    // Case 2: Failed downcast to an unrelated type
    auto *castToOther = qprotobufmessage_cast<SimpleIntMessage *>(base);
    QCOMPARE_EQ(castToOther, nullptr);
    auto *castToOtherClone = qprotobufmessage_cast<SimpleStringMessageClone *>(base);
    QCOMPARE_EQ(castToOtherClone, nullptr);

    // Case 3: Casting a null pointer
    QProtobufMessage *nullBase = nullptr;
    auto *castFromNull = qprotobufmessage_cast<SimpleStringMessage *>(nullBase);
    QCOMPARE_EQ(castFromNull, nullptr);

    // Case 4: Const-correct casting
    const QProtobufMessage *constBase = base;
    const auto *constCast = qprotobufmessage_cast<const SimpleStringMessage *>(constBase);
    QCOMPARE_EQ(constCast, original);
    // Case 5: Failed const-correct casting
    const auto *failedConstCast = qprotobufmessage_cast<const SimpleIntMessage *>(constBase);
    QCOMPARE_EQ(failedConstCast, nullptr);

    // Case 5: Derived msg casts
    {
        auto *derived = new DerivedMessage();
        base = derived;

        // We cannot cast to user-derived types because the QProtobufMessage ctor
        // stores the parent's metaObject (SimpleIntMessage), not DerivedMessage's.
        // There is currently no way to provide this for user-derived types.
        QEXPECT_FAIL("", "User-derived classes not detectable", Continue);
        QCOMPARE_EQ(qprotobufmessage_cast<DerivedMessage *>(base), derived);

        QCOMPARE_EQ(qprotobufmessage_cast<SimpleIntMessage *>(base), derived);
        QCOMPARE_EQ(qprotobufmessage_cast<SimpleStringMessage *>(base), nullptr);
        delete derived;

        auto *baseMsg = new SimpleIntMessage();
        base = baseMsg;
        // Correctly rejects invalid upcast
        QCOMPARE_EQ(qprotobufmessage_cast<DerivedMessage *>(base), nullptr);
        delete baseMsg;
    }

    delete original;
}

QTEST_MAIN(QtProtobufInternalsTest)
#include "tst_protobuf_internals.moc"
