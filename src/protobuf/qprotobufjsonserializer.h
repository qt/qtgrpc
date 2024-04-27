// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFJSONSERIALIZER_H
#define QPROTOBUFJSONSERIALIZER_H

#include <QtProtobuf/qtprotobufglobal.h>
#include <QtProtobuf/qtprotobuftypes.h>
#include <QtProtobuf/qabstractprotobufserializer.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QProtobufJsonSerializerPrivate;
class Q_PROTOBUF_EXPORT QProtobufJsonSerializer final : public QAbstractProtobufSerializer
{
    Q_DISABLE_COPY_MOVE(QProtobufJsonSerializer)

public:
    QProtobufJsonSerializer();
    ~QProtobufJsonSerializer() override;

    QAbstractProtobufSerializer::DeserializationError deserializationError() const override;
    QString deserializationErrorString() const override;

private:
    QByteArray serializeMessage(const QProtobufMessage *message) const override;
    bool deserializeMessage(QProtobufMessage *message, QByteArrayView data) const override;

    void serializeObject(const QProtobufMessage *message,
                         const QtProtobufPrivate::QProtobufFieldInfo &fieldInfo)
        const override;
    bool deserializeObject(QProtobufMessage *message) const override;

    void serializeMapPair(const QVariant &key, const QVariant &value,
                          const QtProtobufPrivate::QProtobufFieldInfo &fieldInfo)
        const override;
    bool deserializeMapPair(QVariant &key, QVariant &value) const override;

    void serializeEnumList(const QList<QtProtobuf::int64> &value, const QMetaEnum &metaEnum,
                           const QtProtobufPrivate::QProtobufFieldInfo &fieldInfo)
        const override;

    bool deserializeEnumList(QList<QtProtobuf::int64> &value,
                             const QMetaEnum &metaEnum) const override;

private:
    std::unique_ptr<QProtobufJsonSerializerPrivate> d_ptr;
};

QT_END_NAMESPACE

#endif // QPROTOBUFJSONSERIALIZER_H

