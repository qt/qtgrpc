// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QtProtobuf/qprotobuflazymessagepointer.h>

#include <QtTest/qtest.h>

// Unfortunately this test does not test the QML-ownership parts of the code
class tst_QProtobufLazyMessagePointer : public QObject
{
    Q_OBJECT
private slots:
    void ctor();
    void reset();
    void get();
};

void tst_QProtobufLazyMessagePointer::ctor()
{
    QtProtobufPrivate::QProtobufLazyMessagePointer<QObject> ptr;
    QVERIFY(!ptr);
    QtProtobufPrivate::QProtobufLazyMessagePointer<QObject> ptr2(nullptr);
    QVERIFY(!ptr2);
    QObject *obj = new QObject;
    QtProtobufPrivate::QProtobufLazyMessagePointer<QObject> ptr3(obj);
    QVERIFY(ptr3);
}

void tst_QProtobufLazyMessagePointer::reset()
{
    QObject *obj = new QObject;
    QtProtobufPrivate::QProtobufLazyMessagePointer<QObject> ptr(obj);
    QPointer<QObject> objPtr = obj;
    ptr.reset();
    QVERIFY(objPtr.isNull());
    ptr.reset();
    QVERIFY(objPtr.isNull());
    obj = new QObject;
    ptr.reset(obj);
    QCOMPARE(ptr.get(), obj);
}

void tst_QProtobufLazyMessagePointer::get()
{
    QObject *obj = new QObject;
    obj->setObjectName("obj");
    QtProtobufPrivate::QProtobufLazyMessagePointer<QObject> ptr(obj);
    QCOMPARE(ptr.get(), obj);
    QCOMPARE(&(*ptr), obj);
    QCOMPARE(ptr->objectName(), obj->objectName());
    obj->setObjectName("obj2");
    QCOMPARE(ptr->objectName(), obj->objectName());
}

QTEST_APPLESS_MAIN(tst_QProtobufLazyMessagePointer)
#include "tst_qprotobuflazymessagepointer.moc"
