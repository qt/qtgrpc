// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/qbuffer.h>
#include <QtCore/qobject.h>
#include <QtGrpc/qgrpcstatus.h>
#include <QtTest/qtest.h>

using namespace Qt::StringLiterals;

class QGrpcStatusTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void defaultConstructedIsOk() const;
    void hasSpecialMemberFunctions() const;
    void hasImplicitQVariant() const;
    void hasMemberSwap() const;
    void equalityComparable() const;
    void qHashOnlyStatusCodes() const;
    void streamsToDebug() const;
    void streamsToDataStream() const;
};

void QGrpcStatusTest::defaultConstructedIsOk() const
{
    QGrpcStatus s1;
    QVERIFY(s1.message().isEmpty());
    QCOMPARE_EQ(s1.code(), QGrpcStatus::Ok);

    QGrpcStatus s2(QGrpcStatus::Ok);
    QCOMPARE_EQ(s1.code(), s2.code());
    QCOMPARE_EQ(s1.message(), s2.message());
}

void QGrpcStatusTest::hasSpecialMemberFunctions() const
{
    const auto check = [](const QGrpcStatus &s1, const QGrpcStatus &s2) {
        QCOMPARE_EQ(s1.code(), s2.code());
        QCOMPARE_EQ(s1.message(), s2.message());
    };

    QString msg = u"(◕д◕✿)😀"_s;
    QGrpcStatus s1 = { QGrpcStatus::PermissionDenied, msg };
    QCOMPARE_EQ(s1.code(), QGrpcStatus::PermissionDenied);
    QCOMPARE_EQ(s1.message(), msg);

    QGrpcStatus s2(s1);
    check(s1, s2);
    QGrpcStatus s3 = s2;
    check(s2, s3);

    QGrpcStatus s4(std::move(s3));
    check(s4, s1);
    QGrpcStatus s5 = std::move(s4);
    check(s5, s1);
}

void QGrpcStatusTest::hasImplicitQVariant() const
{
    QGrpcStatus s1(QGrpcStatus::DataLoss, "testcase");
    QVariant var1 = s1;
    QVERIFY(var1.isValid());
    const auto s2 = var1.value<QGrpcStatus>();
    QCOMPARE_EQ(s1.code(), s2.code());
    QCOMPARE_EQ(s1.message(), s2.message());
}

void QGrpcStatusTest::hasMemberSwap() const
{
    QGrpcStatus s1(QGrpcStatus::DataLoss, "testcase");
    QGrpcStatus s2(s1);
    QGrpcStatus s3 = {};
    s1.swap(s3);
    QCOMPARE_EQ(s1.code(), QGrpcStatus::Ok);
    QVERIFY(s1.message().isEmpty());
    QCOMPARE_EQ(s3.code(), s2.code());
    QCOMPARE_EQ(s3.message(), s2.message());
}

void QGrpcStatusTest::equalityComparable() const
{
    QGrpcStatus s1;
    QGrpcStatus s2(QGrpcStatus::Internal);
    QCOMPARE_NE(s1, s2);
    s1 = s2;
    QCOMPARE_EQ(s1, s2);
    s1 = { QGrpcStatus::Internal, "message is ignored" };
    QCOMPARE_EQ(s1, s2);
}

void QGrpcStatusTest::qHashOnlyStatusCodes() const
{
    QGrpcStatus s1;
    QGrpcStatus s2(QGrpcStatus::Internal);
    const auto hash1 = qHash(s1);
    const auto hash2 = qHash(s2);
    QCOMPARE_NE(hash1, hash2);
    QGrpcStatus s3(QGrpcStatus::Internal, "ignored");
    const auto hash3 = qHash(s3);
    QCOMPARE_EQ(hash2, hash3);
    QCOMPARE_NE(hash1, hash3);
}

void QGrpcStatusTest::streamsToDebug() const
{
    QGrpcStatus s1;
    QGrpcStatus s2 = { QGrpcStatus::OutOfRange, "test" };

    QByteArray storage;
    QBuffer buf(&storage);
    QDebug dbg(&buf);

    QVERIFY(buf.data().isEmpty());
    buf.open(QIODevice::WriteOnly);
    dbg << s1 << s2;
    buf.close();
    QVERIFY(!buf.data().isEmpty());
}

void QGrpcStatusTest::streamsToDataStream() const
{
    QGrpcStatus s1;
    QGrpcStatus s2 = { QGrpcStatus::OutOfRange };
    QGrpcStatus s3 = { QGrpcStatus::Unimplemented, "test" };

    QByteArray storage;
    QBuffer buf(&storage);

    // QDataStream
    QDataStream ds(&buf);
    QVERIFY(buf.data().isEmpty());
    buf.open(QIODevice::ReadWrite);
    ds << s1 << s2 << s3;

    buf.seek(0);
    QGrpcStatus os1, os2, os3;
    ds >> os1 >> os2 >> os3;
    QCOMPARE_EQ(s1, os1);
    QCOMPARE_EQ(s1.message(), os1.message());
    QCOMPARE_EQ(s2, os2);
    QCOMPARE_EQ(s2.message(), os2.message());
    QCOMPARE_EQ(s3, os3);
    QCOMPARE_EQ(s3.message(), os3.message());

    buf.close();
    QVERIFY(!buf.data().isEmpty());
}

QTEST_MAIN(QGrpcStatusTest)

#include "tst_qgrpcstatus.moc"
