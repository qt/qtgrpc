// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "customjsonnames.qpb.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QProtobufJsonSerializer>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
using namespace qtprotobufnamespace::tests;

class QtProtobufJsonCustomNamesTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void serialize();
    void deserialize();
};

static QJsonObject expectedJson()
{
    return {
        { u"q\"uote"_s, u"quote"_s },
        { u"7up"_s, u"leadingDigit"_s },
        { u"back\\slash"_s, u"backslash"_s },
        { u"\\x41"_s, u"escapeSequence"_s },
        { u"comment*/End"_s, u"commentEnd"_s },
        { u"comment/*Middle*/"_s, u"commentMiddle"_s },
        { u"new\nline"_s, u"newline"_s },
        { u"ä"_s, u"nonAscii"_s },
        { u"$dollar$"_s, u"dollar"_s },
    };
}

void QtProtobufJsonCustomNamesTest::serialize()
{
    CustomJsonNames message;
    message.setQuote(u"quote"_s);
    message.setLeadingDigit(u"leadingDigit"_s);
    message.setBackslash(u"backslash"_s);
    message.setEscapeSequence(u"escapeSequence"_s);
    message.setCommentEnd(u"commentEnd"_s);
    message.setCommentMiddle(u"commentMiddle"_s);
    message.setNewline(u"newline"_s);
    message.setNonAscii(u"nonAscii"_s);
    message.setDollar(u"dollar"_s);

    QProtobufJsonSerializer serializer;
    const QByteArray result = message.serialize(&serializer);
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);
    QCOMPARE(QJsonDocument::fromJson(result).object(), expectedJson());
}

void QtProtobufJsonCustomNamesTest::deserialize()
{
    QProtobufJsonSerializer serializer;
    CustomJsonNames message;
    QVERIFY(message.deserialize(&serializer, QJsonDocument(expectedJson()).toJson()));
    QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::None);

    QCOMPARE(message.quote(), u"quote"_s);
    QCOMPARE(message.leadingDigit(), u"leadingDigit"_s);
    QCOMPARE(message.backslash(), u"backslash"_s);
    QCOMPARE(message.escapeSequence(), u"escapeSequence"_s);
    QCOMPARE(message.commentEnd(), u"commentEnd"_s);
    QCOMPARE(message.commentMiddle(), u"commentMiddle"_s);
    QCOMPARE(message.newline(), u"newline"_s);
    QCOMPARE(message.nonAscii(), u"nonAscii"_s);
    QCOMPARE(message.dollar(), u"dollar"_s);
}

QTEST_MAIN(QtProtobufJsonCustomNamesTest)
#include "tst_protobuf_json_customnames.moc"
