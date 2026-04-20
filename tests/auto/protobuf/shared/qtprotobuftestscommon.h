// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>, Viktor Kopp <vifactor@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QTPROTOBUFTESTSCOMMON_H
#define QTPROTOBUFTESTSCOMMON_H

#include <QtTest/qtest.h>

#include <QtCore/qbytearrayview.h>
#include <QtCore/qmetatype.h>

#include <algorithm>

template<typename MessageType, typename PropertyType>
static void qProtobufAssertMessagePropertyRegistered(int fieldIndex, const char *propertyTypeName, const char *propertyName)
{
    // TODO: there should be(?) a mapping available: PropertyType -> propertyTypeName

    int index = MessageType::staticPropertyOrdering.indexOfFieldNumber(fieldIndex);
    const int propertyNumber = MessageType::staticPropertyOrdering.propertyIndex(index);
    // TODO Qt6: Property type name check is disable because metatype system changes in Qt6.
    // Q_PROPERTY returns non-aliased type for the aliases defined using the 'using' keyword.
    // QCOMPARE(QLatin1String(propertyTypeName), QLatin1String(MessageType::staticMetaObject.property(propertyNumber).typeName()));
    Q_UNUSED(propertyTypeName)
    QCOMPARE(QMetaType::fromType<PropertyType>(),
             MessageType::staticMetaObject.property(propertyNumber).metaType());
    QCOMPARE(QLatin1String(MessageType::staticMetaObject.property(propertyNumber).name()),
             QLatin1String(propertyName));
}

[[maybe_unused]]
static bool compareSerializedChunks(QByteArrayView actual, QByteArrayView chunk1,
                                    QByteArrayView chunk2, QByteArrayView chunk3)
{
    const qsizetype p1 = actual.indexOf(chunk1);
    const qsizetype p2 = actual.indexOf(chunk2);
    const qsizetype p3 = actual.indexOf(chunk3);
    if (p1 < 0 || p2 < 0 || p3 < 0)
        return false;
    std::pair<qsizetype, qsizetype> iv[3] = {
        { p1, p1 + chunk1.size() },
        { p2, p2 + chunk2.size() },
        { p3, p3 + chunk3.size() },
    };
    std::sort(std::begin(iv), std::end(iv));
    return iv[0].first == 0 && iv[0].second == iv[1].first
        && iv[1].second == iv[2].first && iv[2].second == actual.size();
}

#endif // QTPROTOBUFTESTSCOMMON_H
