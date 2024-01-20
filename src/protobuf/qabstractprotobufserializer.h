// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QABSTRACTPROTOBUFSERIALIZER_H
#define QABSTRACTPROTOBUFSERIALIZER_H

#include <QtCore/qbytearray.h>
#include <QtCore/qbytearrayview.h>

#include <QtProtobuf/qtprotobufglobal.h>
#include <QtProtobuf/qtprotobuftypes.h>
#include <QtProtobuf/qprotobufmessage.h>

#include <type_traits>

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

protected:
    virtual QByteArray
    serializeMessage(const QProtobufMessage *message,
                     const QtProtobufPrivate::QProtobufPropertyOrdering &ordering) const = 0;
    virtual bool deserializeMessage(QProtobufMessage *message,
                                    const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                                    QByteArrayView data) const = 0;

    friend class QtProtobuf::Any;
};

QT_END_NAMESPACE
#endif // QABSTRACTPROTOBUFSERIALIZER_H
