// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2022 Alexey Edelev <semlanik@gmail.com>, Viktor Kopp <vifactor@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef DESERIALIZATIONMAPTYPES_H
#define DESERIALIZATIONMAPTYPES_H

#include "basicmessages.qpb.h"
#include "mapmessages.qpb.h"

#include <QObject>

#include <QTest>
#include <QProtobufSerializer>

#include <limits>

class QtProtobufMapTypesDeserializationTest : public QObject
{
    Q_OBJECT
private slots:
    void init()
    {
        serializer.reset(new QProtobufSerializer);
    }

    void SimpleFixed32StringMapDeserializeTest();
    void SimpleSFixed32StringMapDeserializeTest();
    void SimpleInt32StringMapDeserializeTest();
    void SimpleSInt32StringMapDeserializeTest();
    void SimpleUInt32StringMapDeserializeTest();
    void SimpleFixed64StringMapDeserializeTest();
    void SimpleSFixed64StringMapDeserializeTest();
    void SimpleInt64StringMapDeserializeTest();
    void SimpleSInt64StringMapDeserializeTest();
    void SimpleUInt64StringMapDeserializeTest();
    void SimpleStringStringMapDeserializeTest();

    void SimpleFixed32ComplexMapDeserializeTest();
    void SimpleSFixed32ComplexMapDeserializeTest();
    void SimpleInt32ComplexMapDeserializeTest();
    void SimpleSInt32ComplexMapDeserializeTest();
    void SimpleUInt32ComplexMapDeserializeTest();
    void SimpleFixed64ComplexMapDeserializeTest();
    void SimpleSFixed64ComplexMapDeserializeTest();
    void SimpleInt64ComplexMapDeserializeTest();
    void SimpleSInt64ComplexMapDeserializeTest();
    void SimpleUInt64ComplexMapDeserializeTest();
    void SimpleStringComplexMapDeserializeTest();
    void SimpleUInt64ComplexMapInvalidLengthDeserializeTest();
    void SimpleStringComplexMapInvalidLengthDeserializeTest();
    void SimpleUInt64ComplexMapCorruptedDeserializeTest();
private:
    std::unique_ptr<QProtobufSerializer> serializer;
};
#endif // DESERIALIZATIONMAPTYPES_H
