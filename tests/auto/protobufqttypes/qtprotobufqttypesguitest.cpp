// Copyright (C) 2023 The Qt Company Ltd.
// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "qtguitypes.qpb.h"

#include <qtprotobuftestscommon.h>
#include <QProtobufSerializer>

#include <QObject>
#include <QtGui/qrgb.h>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtGui/QRgba64>
#include <QtTest/QtTest>

#include <memory>

const char *conversionErrorMessage = "Qt Proto Type conversion error.";
const char *invalidHex = "0";

class QtProtobufQtTypesQtGuiTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        QtProtobuf::qRegisterProtobufQtGuiTypes();
    }

    void qRgba64();
    void qColor();
    void qMatrix4x4();
    void qVector2D();
    void qVector3D();
    void qVector4D();
    void qTransform();
    void qQuaternion();
    void qImage();

private:
    QProtobufSerializer serializer;
};

using namespace qtprotobufnamespace::qttypes::tests;

void QtProtobufQtTypesQtGuiTest::qRgba64()
{
    qProtobufAssertMessagePropertyRegistered<QRgba64Message,
            QRgba64>(1, "QRgba64", "testField");

    QRgba64Message msg;
    const QRgba64 rgba64 = QRgba64::fromRgba(128, 64, 32, 255);

    msg.setTestField(rgba64);
    auto result = msg.serialize(&serializer);
    const char *rgba64Hex = "0a0b08808182828484c8ffff01";
    QCOMPARE(QByteArray::fromHex(rgba64Hex), result);

    msg.setTestField({});
    msg.deserialize(&serializer, QByteArray::fromHex(rgba64Hex));
    QCOMPARE(rgba64, msg.testField());
}

void QtProtobufQtTypesQtGuiTest::qColor()
{
    qProtobufAssertMessagePropertyRegistered<QColorMessage,
            QColor>(1, "QColor", "testField");

    QColor color64(QRgba64::fromRgba(128, 64, 32, 255));
    const char *colorHex = "0a0d0a0b08808182828484c8ffff01";

    QColorMessage msg;
    msg.setTestField(color64);
    QCOMPARE(QByteArray::fromHex(colorHex), msg.serialize(&serializer));

    msg.setTestField({});
    msg.deserialize(&serializer, QByteArray::fromHex(colorHex));
    QCOMPARE(color64, msg.testField());

    QColor color32(qRgba(128, 64, 32, 255));
    msg.setTestField(color32);
    QCOMPARE(QByteArray::fromHex(colorHex), msg.serialize(&serializer));

    msg.setTestField({});
    msg.deserialize(&serializer, QByteArray::fromHex(colorHex));
    QCOMPARE(color32, msg.testField());

    QColor invalidColor;
    msg.setTestField(invalidColor);

    QTest::ignoreMessage(QtWarningMsg, conversionErrorMessage);
    msg.serialize(&serializer); // Error message is generated
    QVERIFY(!msg.testField().isValid());

    msg.setTestField({});
    QVERIFY(!msg.deserialize(&serializer, QByteArray::fromHex(invalidHex)));
    QVERIFY(!msg.testField().isValid());
}

void QtProtobufQtTypesQtGuiTest::qMatrix4x4()
{
    qProtobufAssertMessagePropertyRegistered<QMatrix4x4Message,
            QMatrix4x4>(1, "QMatrix4x4", "testField");

    QMatrix4x4Message msg;
    const QMatrix4x4 matrix = QMatrix4x4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    msg.setTestField(matrix);
    auto result = msg.serialize(&serializer);
    const char *matrixHex = "0a420a40000000000000803f0000004000004040000080400000a040"
                            "0000c0400000e0400000004100001041000020410000304100004041"
                            "000050410000604100007041";

    QCOMPARE(QByteArray::fromHex(matrixHex), result);
    msg.setTestField({});
    msg.deserialize(&serializer, QByteArray::fromHex(matrixHex));

    QCOMPARE(matrix, msg.testField());
}

void QtProtobufQtTypesQtGuiTest::qVector2D()
{
    qProtobufAssertMessagePropertyRegistered<QVector2DMessage,
            QVector2D>(1, "QVector2D", "testField");

    QVector2DMessage msg;
    QVector2D vector(42, 24);
    msg.setTestField(vector);
    auto result = msg.serialize(&serializer);

    QCOMPARE(QByteArray::fromHex("0a0a0d00002842150000c041"), result);

    msg.setTestField({});
    msg.deserialize(&serializer, QByteArray::fromHex("0a0a0d0000c0411500002842"));

    QCOMPARE(QVector2D(vector.y(), vector.x()), msg.testField());
}

void QtProtobufQtTypesQtGuiTest::qVector3D()
{
    qProtobufAssertMessagePropertyRegistered<QVector3DMessage,
            QVector3D>(1, "QVector3D", "testField");

    QVector3DMessage msg;
    QVector3D vector(42, 24, 11);
    msg.setTestField(vector);
    auto result = msg.serialize(&serializer);

    QCOMPARE(QByteArray::fromHex("0a0f0d00002842150000c0411d00003041"), result);

    msg.setTestField({});
    msg.deserialize(&serializer, QByteArray::fromHex("0a0f0d0000c04115000030411d00002842"));

    QCOMPARE(QVector3D(vector.y(), vector.z(), vector.x()), msg.testField());
}

void QtProtobufQtTypesQtGuiTest::qVector4D()
{
    qProtobufAssertMessagePropertyRegistered<QVector4DMessage,
            QVector4D>(1, "QVector4D", "testField");

    QVector4DMessage msg;
    const QVector4D vector({24, 11, 42, 0});
    msg.setTestField(vector);
    auto result = msg.serialize(&serializer);

    QCOMPARE(QByteArray::fromHex("0a0f0d0000c04115000030411d00002842"), result);

    msg.setTestField({});
    msg.deserialize(&serializer, QByteArray::fromHex("0a0f0d0000c04115000030411d00002842"));

    QCOMPARE(vector, msg.testField());
}

void QtProtobufQtTypesQtGuiTest::qTransform()
{
    qProtobufAssertMessagePropertyRegistered<QTransformMessage,
            QTransform>(1, "QTransform", "testField");

    QTransformMessage msg;
    msg.setTestField(QTransform(0, 1, 2, 3, 4, 5, 6, 7, 8));
    auto result = msg.serialize(&serializer);
    const char * transformHex = "0a4a0a480000000000000000000000000000f03f0000000000000040000"
                                "00000000008400000000000001040000000000000144000000000000018"
                                "400000000000001c400000000000002040";
    QCOMPARE(QByteArray::fromHex(transformHex), result);
    msg.setTestField({});
    msg.deserialize(&serializer,
                    QByteArray::fromHex("0a4a0a4800000000000020400000000000001c40000000000"
                                        "0001840000000000000144000000000000010400000000000"
                                        "0008400000000000000040000000000000f03f0000000000000000"));
    QCOMPARE(QTransform(8, 7, 6, 5, 4, 3, 2, 1, 0), msg.testField());
}

void QtProtobufQtTypesQtGuiTest::qQuaternion()
{
    qProtobufAssertMessagePropertyRegistered<QQuaternionMessage, QQuaternion>(1, "QQuaternion",
                                                                              "testField");

    QQuaternionMessage msg;
    const QQuaternion quater(14, 10, 24, 22);
    msg.setTestField(quater);
    auto result = msg.serialize(&serializer);
    const char *hexValue = "0a140d0000604115000020411d0000c041250000b041";
    QCOMPARE(QByteArray::fromHex(hexValue), result);

    msg.setTestField({});
    msg.deserialize(&serializer,
                    QByteArray::fromHex(hexValue));

    QCOMPARE(quater, msg.testField());
}

void fillTestImage(QImage &testImage)
{
    testImage.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&testImage);
    QColor color_1(255, 0, 0, 255);
    QColor color_2(0, 255, 0, 255);
    QRect rect_1(0, 0, 100, 100);
    QRect rect_2(50, 50, 10, 50);
    painter.fillRect(rect_1, color_1);
    painter.fillRect(rect_2, color_2);
    painter.end();
}

void QtProtobufQtTypesQtGuiTest::qImage()
{
    qProtobufAssertMessagePropertyRegistered<QImageMessage,
            QImage>(1, "QImage", "testField");

    QImageMessage msg;
    QSize imgSize(200, 200);
    QImage initialImage(imgSize, QImage::Format_RGBA64);
    fillTestImage(initialImage);

    msg.setTestField(initialImage);
    QVERIFY(!msg.testField().isNull());

    auto result = msg.serialize(&serializer);
    msg.setTestField({});
    msg.deserialize(&serializer, result);
    QCOMPARE(initialImage, msg.testField());
}

QTEST_MAIN(QtProtobufQtTypesQtGuiTest)
#include "qtprotobufqttypesguitest.moc"
