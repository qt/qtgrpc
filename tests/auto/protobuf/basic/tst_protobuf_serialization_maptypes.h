// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2022 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef SERIALIZATIONMAPTYPES_H
#define SERIALIZATIONMAPTYPES_H

#include <QObject>
#include <QProtobufSerializer>

class QtProtobufMapTypesSerializationTest : public QObject
{
    Q_OBJECT
private slots:
    void init() {
        m_serializer.reset(new QProtobufSerializer);
    }
    void SimpleFixed32StringMapSerializeTest();
    void SimpleSFixed32StringMapSerializeTest();
    void SimpleInt32StringMapSerializeTest();
    void SimpleSInt32StringMapSerializeTest();
    void SimpleUInt32StringMapSerializeTest();
    void SimpleFixed64StringMapSerializeTest();
    void SimpleSFixed64StringMapSerializeTest();
    void SimpleInt64StringMapSerializeTest();
    void SimpleSInt64StringMapSerializeTest();
    void SimpleUInt64StringMapSerializeTest();
    void SimpleStringStringMapSerializeTest();
    void SimpleFixed32ComplexMapSerializeTest();
    void SimpleSFixed32ComplexMapSerializeTest();
    void SimpleInt32ComplexMapSerializeTest();
    void SimpleSInt32ComplexMapSerializeTest();
    void SimpleUInt32ComplexMapSerializeTest();
    void SimpleFixed64ComplexMapSerializeTest();
    void SimpleSFixed64ComplexMapSerializeTest();
    void SimpleInt64ComplexMapSerializeTest();
    void SimpleSInt64ComplexMapSerializeTest();
    void SimpleUInt64ComplexMapSerializeTest();
    void SimpleStringComplexMapSerializeTest();
private:
    std::unique_ptr<QProtobufSerializer> m_serializer;
};
#endif // SERIALIZATIONMAPTYPES_H
