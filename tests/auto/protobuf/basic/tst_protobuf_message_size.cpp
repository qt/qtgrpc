// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "basicmessages.qpb.h"

#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qregularexpression.h>

#include <QtTest/qtest.h>

#include <limits>

using namespace Qt::Literals::StringLiterals;

class tst_protobuf_message_size : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void deserializeDefaultLimit();
    void serializeIgnoresDeserializeLimit();
    void deserializeLimit();
    void deserializeAcceptsLargeInputByDefault();
    void deserializeOtherErrorsAreNotTooLarge();
    void deserializeAfterRejection();
    void deserializeZeroLimitAllowsOnlyEmptyInput();
    void deserializeLimitAboveProtobufLimitIsClamped();

private:
    static constexpr quint64 ProtobufLimit = (2ull * 1024 * 1024 * 1024) - 1;

    static qtprotobufnamespace::tests::SimpleStringMessage message(qsizetype payload)
    {
        qtprotobufnamespace::tests::SimpleStringMessage msg;
        msg.setTestFieldString(QString(payload, u'a'));
        return msg;
    }
};

void tst_protobuf_message_size::deserializeDefaultLimit()
{
    QProtobufSerializer serializer;
    QCOMPARE(serializer.maximumInputSize(), ProtobufLimit);
}

void tst_protobuf_message_size::serializeIgnoresDeserializeLimit()
{
    // The configurable limit applies only to deserialize(). serialize() is
    // limited by the fixed protobuf limit only.
    QProtobufSerializer serializer;
    const auto msg = message(20);
    const QByteArray expected = msg.serialize(&serializer);
    QVERIFY(!expected.isEmpty());

    serializer.setMaximumInputSize(1);
    QCOMPARE(msg.serialize(&serializer), expected);
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
}

void tst_protobuf_message_size::deserializeLimit()
{
    QProtobufSerializer serializer;
    const auto msg = message(20);
    const QByteArray data = msg.serialize(&serializer);

    serializer.setMaximumInputSize(quint32(data.size()));
    qtprotobufnamespace::tests::SimpleStringMessage accepted;
    QVERIFY(accepted.deserialize(&serializer, data));
    QCOMPARE(accepted.testFieldString(), msg.testFieldString());

    const quint32 smallerSize = quint32(data.size() - 1);
    serializer.setMaximumInputSize(smallerSize);
    QCOMPARE(serializer.maximumInputSize(), smallerSize);
    qtprotobufnamespace::tests::SimpleStringMessage rejected;
    QVERIFY(!rejected.deserialize(&serializer, data));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::MessageTooLarge);
    // The input is rejected before parsing.
    QVERIFY(rejected.testFieldString().isEmpty());
}

void tst_protobuf_message_size::deserializeAcceptsLargeInputByDefault()
{
    // Without any setting, input far above a small limit is still accepted.
    QProtobufSerializer serializer;
    const auto msg = message(100000);
    const QByteArray data = msg.serialize(&serializer);
    QVERIFY(data.size() > 100000);

    qtprotobufnamespace::tests::SimpleStringMessage result;
    QVERIFY(result.deserialize(&serializer, data));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
    QCOMPARE(result.testFieldString(), msg.testFieldString());
}

void tst_protobuf_message_size::deserializeOtherErrorsAreNotTooLarge()
{
    // Input that is within the limit but broken must fail with another error.
    QProtobufSerializer serializer;
    const QByteArray data = message(20).serialize(&serializer);
    serializer.setMaximumInputSize(quint32(data.size()));

    qtprotobufnamespace::tests::SimpleStringMessage result;
    QVERIFY(!result.deserialize(&serializer, data.first(data.size() - 5)));
    QCOMPARE_NE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
    QCOMPARE_NE(serializer.lastError(), QAbstractProtobufSerializer::Error::MessageTooLarge);
}

void tst_protobuf_message_size::deserializeAfterRejection()
{
    // A rejected input does not break the serializer. The same serializer
    // accepts smaller input afterwards.
    QProtobufSerializer serializer;
    const QByteArray small = message(5).serialize(&serializer);
    const QByteArray large = message(50).serialize(&serializer);
    serializer.setMaximumInputSize(quint32(small.size()));

    qtprotobufnamespace::tests::SimpleStringMessage rejected;
    QVERIFY(!rejected.deserialize(&serializer, large));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::MessageTooLarge);

    qtprotobufnamespace::tests::SimpleStringMessage accepted;
    QVERIFY(accepted.deserialize(&serializer, small));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
    QCOMPARE(accepted.testFieldString(), QString(5, u'a'));
}

void tst_protobuf_message_size::deserializeZeroLimitAllowsOnlyEmptyInput()
{
    QProtobufSerializer serializer;
    serializer.setMaximumInputSize(0);
    QCOMPARE(serializer.maximumInputSize(), 0);

    // Any input with a payload is rejected.
    qtprotobufnamespace::tests::SimpleStringMessage unused;
    QVERIFY(!unused.deserialize(&serializer, QByteArray("x")));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::MessageTooLarge);

    // An empty message is still accepted.
    qtprotobufnamespace::tests::SimpleStringMessage empty;
    QVERIFY(empty.serialize(&serializer).isEmpty());
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
    QVERIFY(empty.deserialize(&serializer, QByteArray()));
}

void tst_protobuf_message_size::deserializeLimitAboveProtobufLimitIsClamped()
{
    QProtobufSerializer serializer;
    QTest::ignoreMessage(QtWarningMsg,
                         QRegularExpression(u"exceeds the protobuf limit"_s));
    serializer.setMaximumInputSize(ProtobufLimit + 1);
    QCOMPARE(serializer.maximumInputSize(), ProtobufLimit);

    // 4 GiB + 1 must not wrap around to 1 byte.
    QTest::ignoreMessage(QtWarningMsg,
                         QRegularExpression(u"exceeds the protobuf limit"_s));
    serializer.setMaximumInputSize((quint64(1) << 32) + 1);
    QCOMPARE(serializer.maximumInputSize(), ProtobufLimit);

    QTest::ignoreMessage(QtWarningMsg,
                         QRegularExpression(u"exceeds the protobuf limit"_s));
    serializer.setMaximumInputSize((std::numeric_limits<quint64>::max)());
    QCOMPARE(serializer.maximumInputSize(), ProtobufLimit);
}

QTEST_MAIN(tst_protobuf_message_size)
#include "tst_protobuf_message_size.moc"
