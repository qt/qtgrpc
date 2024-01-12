// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QObject>
#include <QtTest/QTest>

#include <QtGrpc/qgrpcserializationformat.h>
#include <QtProtobuf/qprotobufjsonserializer.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <memory>

class QGrpcSerializationFormatTest : public QObject
{
    Q_OBJECT
public:
    QGrpcSerializationFormatTest() { }

private slots:
    void ConstructEmbedded();
    void ConstructCustom();
    void CopyMove();
};

void QGrpcSerializationFormatTest::ConstructEmbedded()
{
    QGrpcSerializationFormat defaultFormat(QGrpcSerializationFormat::Format::Default);
    QCOMPARE(defaultFormat.suffix(), "");
    QVERIFY(dynamic_cast<QProtobufSerializer *>(defaultFormat.serializer().get()) != nullptr);

    QGrpcSerializationFormat jsonFormat(QGrpcSerializationFormat::Format::Json);
    QCOMPARE(jsonFormat.suffix(), "json");
    QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(jsonFormat.serializer().get()) != nullptr);

    QGrpcSerializationFormat protobufFormat(QGrpcSerializationFormat::Format::Protobuf);
    QCOMPARE(protobufFormat.suffix(), "proto");
    QVERIFY(dynamic_cast<QProtobufSerializer *>(protobufFormat.serializer().get()) != nullptr);
}

void QGrpcSerializationFormatTest::ConstructCustom()
{
    QGrpcSerializationFormat customFormat("test", std::make_shared<QProtobufJsonSerializer>());
    QCOMPARE(customFormat.suffix(), "test");
    QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(customFormat.serializer().get()) != nullptr);
}

void QGrpcSerializationFormatTest::CopyMove()
{
    QGrpcSerializationFormat f1(QGrpcSerializationFormat::Format::Json);

    {
        QGrpcSerializationFormat f2(f1);
        QCOMPARE(f2.suffix(), "json");
        QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(f2.serializer().get()) != nullptr);
        QVERIFY(f2.serializer() == f1.serializer());

        QGrpcSerializationFormat f3(std::move(f2));
        QCOMPARE(f3.suffix(), "json");
        QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(f3.serializer().get()) != nullptr);
        QVERIFY(f3.serializer() == f1.serializer());
    }

    {
        QGrpcSerializationFormat f2;
        f2 = f1;
        QCOMPARE(f2.suffix(), "json");
        QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(f2.serializer().get()) != nullptr);
        QVERIFY(f2.serializer() == f1.serializer());

        QGrpcSerializationFormat f3;
        f3 = std::move(f2);
        QCOMPARE(f3.suffix(), "json");
        QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(f3.serializer().get()) != nullptr);
        QVERIFY(f3.serializer() == f1.serializer());
    }
}

QTEST_MAIN(QGrpcSerializationFormatTest)

#include "tst_qgrpcserializationformat.moc"
