// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QABSTRACTPROTOBUFSERIALIZER_H
#define QABSTRACTPROTOBUFSERIALIZER_H

#include <QtCore/qbytearray.h>
#include <QtCore/qbytearrayview.h>
#include <QtCore/qstring.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qvariant.h>

#include <QtProtobuf/qtprotobufglobal.h>
#include <QtProtobuf/qtprotobuftypes.h>
#include <QtProtobuf/qprotobufmessage.h>

QT_BEGIN_NAMESPACE

namespace QtProtobuf {
class Any;
}

class Q_PROTOBUF_EXPORT QAbstractProtobufSerializer
{
public:
    enum DeserializationError {
        NoError,
        InvalidHeaderError,
        NoDeserializerError,
        UnexpectedEndOfStreamError,
        InvalidFormatError,
    };

    QByteArray serialize(const QProtobufMessage *message) const;
    bool deserialize(QProtobufMessage *message, QByteArrayView data) const;

    virtual ~QAbstractProtobufSerializer();

    virtual QAbstractProtobufSerializer::DeserializationError deserializationError() const = 0;
    virtual QString deserializationErrorString() const = 0;

    virtual void
    serializeObject(const QProtobufMessage *message,
                    const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const = 0;
    virtual bool deserializeObject(QProtobufMessage *message) const = 0;

    virtual void serializeListObject(const QProtobufMessage *message,
                                     const QtProtobufPrivate::QProtobufPropertyOrderingInfo
                                         &fieldInfo) const = 0;
    virtual bool deserializeListObject(QProtobufMessage *message) const = 0;

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
protected:
    virtual QByteArray serializeMessage(const QProtobufMessage *message) const = 0;
    virtual bool deserializeMessage(QProtobufMessage *message, QByteArrayView data) const = 0;

    friend class QtProtobuf::Any;
};

QT_END_NAMESPACE
#endif // QABSTRACTPROTOBUFSERIALIZER_H
