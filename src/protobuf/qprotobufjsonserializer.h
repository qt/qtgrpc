// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFJSONSERIALIZER_H
#define QPROTOBUFJSONSERIALIZER_H

#include <QtProtobuf/qtprotobufglobal.h>
#include <QtProtobuf/qtprotobuftypes.h>
#include <QtProtobuf/qprotobufbaseserializer.h>

#include <QtCore/QMetaObject>

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

    QByteArray serializeMessage(const QProtobufMessage *message,
                                const QtProtobufPrivate::QProtobufPropertyOrdering &ordering
                                ) const override;

    bool deserializeMessage(QProtobufMessage *message,
                            const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                            QByteArrayView data) const override;

    QByteArray serializeObject(const QProtobufMessage *message,
                               const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                               const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo
                               ) const override;
    bool deserializeObject(QProtobufMessage *message,
                           const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                           QtProtobufPrivate::QProtobufSelfcheckIterator &it) const override;

    QByteArray
    serializeListObject(const QList<const QProtobufMessage *> &messageList,
                        const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                        const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo
                        ) const override;

    QProtobufBaseSerializer::Status
    deserializeListObject(QProtobufMessage *message,
                          const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                          QtProtobufPrivate::QProtobufSelfcheckIterator &it) const override;

    QByteArray
    serializeMapPair(const QList<QPair<QVariant, QVariant>> &list,
                     const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo
                     ) const override;

    QProtobufBaseSerializer::Status
    deserializeMapPair(QVariant &key, QVariant &value,
                       QtProtobufPrivate::QProtobufSelfcheckIterator &it) const override
    {
        Q_UNUSED(key);
        Q_UNUSED(value);
        Q_UNUSED(it);
        return QProtobufBaseSerializer::SerializationError;
    }

    QByteArray
    serializeEnum(QtProtobuf::int64 value,
                  const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const override
    {
        Q_UNUSED(value);
        Q_UNUSED(fieldInfo);
        return QByteArray();
    }

    QByteArray
    serializeEnumList(const QList<QtProtobuf::int64> &value,
                      const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo
                      ) const override
    {
        Q_UNUSED(value);
        Q_UNUSED(fieldInfo);
        return QByteArray();
    }

    Q_REQUIRED_RESULT
    bool deserializeEnum(QtProtobuf::int64 &value,
                         QtProtobufPrivate::QProtobufSelfcheckIterator &it) const override
    {
        Q_UNUSED(value);
        Q_UNUSED(it);
        return false;
    }

    Q_REQUIRED_RESULT bool
    deserializeEnumList(QList<QtProtobuf::int64> &value,
                        QtProtobufPrivate::QProtobufSelfcheckIterator &it) const override
    {
        Q_UNUSED(value);
        Q_UNUSED(it);
        return false;
    }

private:
    std::unique_ptr<QProtobufJsonSerializerPrivate> d_ptr;
};

QT_END_NAMESPACE

#endif // QPROTOBUFJSONSERIALIZER_H

