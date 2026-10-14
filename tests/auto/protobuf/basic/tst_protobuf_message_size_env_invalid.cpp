// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "basicmessages.qpb.h"

#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/qregularexpression.h>
#include <QtCore/qtenvironmentvariables.h>

#include <QtTest/qtest.h>

using namespace Qt::StringLiterals;

class tst_protobuf_message_size_env_invalid : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void nonNumericValueIsIgnored();
};

void tst_protobuf_message_size_env_invalid::nonNumericValueIsIgnored()
{
    // The variable is read only once per process, so this value is used for
    // the whole test.
    qputenv("QT_PROTOBUF_MAXIMUM_INPUT_SIZE", "64MB");

    // The invalid value is ignored, so the default limit applies.
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(u"not an integer"_s));
    QProtobufSerializer serializer;
    qtprotobufnamespace::tests::SimpleStringMessage msg;
    msg.setTestFieldString(QString(100, u'a'));
    const QByteArray data = msg.serialize(&serializer);
    qtprotobufnamespace::tests::SimpleStringMessage result;
    QVERIFY(result.deserialize(&serializer, data));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
    QCOMPARE(result.testFieldString(), msg.testFieldString());
}

QTEST_MAIN(tst_protobuf_message_size_env_invalid)
#include "tst_protobuf_message_size_env_invalid.moc"
