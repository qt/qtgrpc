// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/qtest.h>

#include <QtProtobuf/qprotobufserializer.h>

#include "bench.qpb.h"

class tst_ProtoDeserialize : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void deserialize_data();
    void deserialize();
};

std::array<std::pair<QString, QByteArray>, 3> data = {
    std::make_pair("qtbench.SimpleBoolMessage", QByteArray::fromHex("0801")),
    std::make_pair("qtbench.SimpleBytesMessage", QByteArray::fromHex("0a0c48656c6c6f20776f726c6421")),
    std::make_pair("qtbench.RecursiveMessage", QByteArray::fromHex("080112120802120e0803120a08041206080512020806")),
};
// This one can be repeated, so we will use it specially
auto listOfObject = std::make_pair("qtbench.ListOfObjects", QByteArray::fromHex("0a020801"));

void tst_ProtoDeserialize::deserialize_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QByteArray>("data");

    for (auto &&[name, value] : data)
        QTest::newRow(qPrintable(name)) << name << value;

    for (auto i : {1, 10, 100, 1000}) {
        QTest::addRow("qtbench.ListOfObjects x %d", i)
            << listOfObject.first << listOfObject.second.repeated(i);
    }
}

void tst_ProtoDeserialize::deserialize()
{
    QFETCH(QString, name);
    QFETCH(QByteArray, data);

    QProtobufSerializer serializer;
    QBENCHMARK {
        QProtobufMessagePointer res = QProtobufMessage::constructByName(name);
        QVERIFY(res);
        QVERIFY(res->deserialize(&serializer, data));
    }
}

QTEST_MAIN(tst_ProtoDeserialize)

#include "tst_bench_deserialize_protobuf.moc"
