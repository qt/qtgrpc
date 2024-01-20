// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFSBASEERIALIZER_H
#define QPROTOBUFSBASEERIALIZER_H

#include <QtProtobuf/qabstractprotobufserializer.h>

#include <QtCore/QList>
#include <QtCore/QMetaEnum>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

class Q_PROTOBUF_EXPORT QProtobufBaseSerializer: public QAbstractProtobufSerializer
{
public:
    virtual void
    serializeObject(const QProtobufMessage *message,
                    const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                    const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const = 0;
    virtual bool
    deserializeObject(QProtobufMessage *message,
                      const QtProtobufPrivate::QProtobufPropertyOrdering &ordering) const = 0;

    virtual void serializeListObject(const QProtobufMessage *message,
                                     const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                                     const QtProtobufPrivate::QProtobufPropertyOrderingInfo
                                         &fieldInfo) const = 0;
    virtual bool
    deserializeListObject(QProtobufMessage *message,
                          const QtProtobufPrivate::QProtobufPropertyOrdering &ordering) const = 0;

    virtual void
    serializeMapPair(const QVariant &key, const QVariant &value,
                     const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const = 0;
    virtual bool deserializeMapPair(QVariant &key, QVariant &value) const = 0;

    virtual void
    serializeEnum(QtProtobuf::int64 value, const QMetaEnum &metaEnum,
                  const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const = 0;
    virtual void
    serializeEnumList(const QList<QtProtobuf::int64> &value, const QMetaEnum &metaEnum,
                      const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const = 0;

    virtual bool deserializeEnum(QtProtobuf::int64 &value, const QMetaEnum &metaEnum) const = 0;
    virtual bool deserializeEnumList(QList<QtProtobuf::int64> &value,
                                     const QMetaEnum &metaEnum) const = 0;
};

QT_END_NAMESPACE
#endif // QPROTOBUFSBASEERIALIZER_H
