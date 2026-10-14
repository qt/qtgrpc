// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

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

    // QT_PROTOBUF_MAXIMUM_INPUT_SIZE is read only once per process, the first time
    // the limit is needed.
    qputenv(EnvName, "64");
    {
        QProtobufSerializer serializer;
        QCOMPARE(serializer.maximumInputSize(), 64);
    }

    // The value is already cached, so changing the variable now has no effect.
    qputenv(EnvName, "128");
    {
        QProtobufSerializer serializer;
        QCOMPARE(serializer.maximumInputSize(), 64);
    }
}

QTEST_MAIN(tst_protobuf_message_size_env)
#include "tst_protobuf_message_size_env.moc"
