// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#define private public
#define protected public
#include "basicmessages.qpb.h"
#undef private
#undef protected

#include <QTest>

using namespace qtprotobufnamespace::tests;

class QtProtobufInternalsTest : public QObject
{
    Q_OBJECT
public:
    QtProtobufInternalsTest() = default;

private slots:
    void nullPointerMessageTest();
    void nullPointerGetterMessageTest();
};

void QtProtobufInternalsTest::nullPointerMessageTest()
{
    SimpleStringMessage stringMsg;
    stringMsg.setTestFieldString({ "not null" });
    ComplexMessage msg;
    msg.setTestFieldInt(0);
    msg.setTestComplexField(stringMsg);

    msg.setTestComplexField_p(nullptr);
    QVERIFY(msg.testComplexField().testFieldString().isEmpty());
    QVERIFY(msg.testComplexField_p() != nullptr);
}

void QtProtobufInternalsTest::nullPointerGetterMessageTest()
{
    ComplexMessage msg;
    QVERIFY(!msg.hasTestComplexField());
    QVERIFY(msg.testComplexField_p() != nullptr);
    msg.setTestComplexField_p(nullptr);
    QVERIFY(msg.testComplexField().testFieldString().isEmpty());
    QVERIFY(msg.testComplexField_p() != nullptr);
}

QTEST_MAIN(QtProtobufInternalsTest)
#include "tst_protobuf_internals.moc"
