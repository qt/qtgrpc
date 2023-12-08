// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFJSONSERIALIZER_H
#define QPROTOBUFJSONSERIALIZER_H

#include <QtProtobuf/qtprotobufglobal.h>
#include <QtProtobuf/qtprotobuftypes.h>
#include <QtProtobuf/qprotobufbaseserializer.h>

#include <QtCore/QMetaObject>

#include <memory>

QT_BEGIN_NAMESPACE

class QProtobufJsonSerializerPrivate;
class Q_PROTOBUF_EXPORT QProtobufJsonSerializer : public QProtobufBaseSerializer
{
    Q_DISABLE_COPY_MOVE(QProtobufJsonSerializer)

public:
    QProtobufJsonSerializer();
    ~QProtobufJsonSerializer() override;

    QAbstractProtobufSerializer::DeserializationError deserializationError() const override;
    QString deserializationErrorString() const override;

private:
    QByteArray serializeMessage(const QProtobufMessage *message,
                                const QtProtobufPrivate::QProtobufPropertyOrdering &ordering
                                ) const override;

    bool deserializeMessage(QProtobufMessage *message,
                            const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                            QByteArrayView data) const override;

    void serializeObject(const QProtobufMessage *message,
                         const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                         const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo)
        const override;
    bool
    deserializeObject(QProtobufMessage *message,
                      const QtProtobufPrivate::QProtobufPropertyOrdering &ordering) const override;

    void serializeListObject(const QProtobufMessage *message,
                             const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                             const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo)
        const override;

    bool deserializeListObject(QProtobufMessage *message,
                               const QtProtobufPrivate::QProtobufPropertyOrdering &ordering)
        const override;

    void serializeMapPair(const QVariant &key, const QVariant &value,
                          const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo)
        const override;

    bool deserializeMapPair(QVariant &key, QVariant &value) const override
    {
        Q_UNUSED(key);
        Q_UNUSED(value);
        return false;
    }

    void
    serializeEnum(QtProtobuf::int64 value,
                  const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const override
    {
        Q_UNUSED(value);
        Q_UNUSED(fieldInfo);
    }

    void serializeEnumList(const QList<QtProtobuf::int64> &value,
                           const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo)
        const override
    {
        Q_UNUSED(value);
        Q_UNUSED(fieldInfo);
    }

    bool deserializeEnum(QtProtobuf::int64 &value) const override
    {
        Q_UNUSED(value);
        return false;
    }

    bool deserializeEnumList(QList<QtProtobuf::int64> &value) const override
    {
        Q_UNUSED(value);
        return false;
    }

private:
    std::unique_ptr<QProtobufJsonSerializerPrivate> d_ptr;
};

QT_END_NAMESPACE

#endif // QPROTOBUFJSONSERIALIZER_H

