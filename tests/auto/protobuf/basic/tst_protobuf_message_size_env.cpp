// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "basicmessages.qpb.h"

#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/qtenvironmentvariables.h>

#include <QtTest/qtest.h>

class tst_protobuf_message_size_env : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void environmentVariable();
};

void tst_protobuf_message_size_env::environmentVariable()
{
    constexpr char EnvName[] = "QT_PROTOBUF_MAXIMUM_INPUT_SIZE";
    const QByteArray data(100, 'x');

    // QT_PROTOBUF_MAXIMUM_INPUT_SIZE is read only once per process, the first time
    // the limit is needed.
    qputenv(EnvName, "64");
    {
        QProtobufSerializer serializer;
        qtprotobufnamespace::tests::SimpleStringMessage msg;
        QVERIFY(!msg.deserialize(&serializer, data));
        QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);
    }

    // The value is already cached, so changing the variable now has no effect.
    qputenv(EnvName, "128");
    {
        QProtobufSerializer serializer;
        qtprotobufnamespace::tests::SimpleStringMessage msg;
        QVERIFY(!msg.deserialize(&serializer, data));
        QCOMPARE(serializer.lastError(), QAbstractProtobufSerializer::Error::InvalidFormat);
    }
}

QTEST_MAIN(tst_protobuf_message_size_env)
#include "tst_protobuf_message_size_env.moc"
