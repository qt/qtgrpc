// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFSERIALIZER_H
#define QPROTOBUFSERIALIZER_H

#include <QtProtobuf/qtprotobufglobal.h>
#include <QtProtobuf/qprotobufbaseserializer.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <functional>
#include <memory>

QT_BEGIN_NAMESPACE

namespace QtProtobufPrivate {
class QProtobufSelfcheckIterator;
} // namespace QtProtobufPrivate

class QProtobufMessage;
class QProtobufSerializerPrivate;
class Q_PROTOBUF_EXPORT QProtobufSerializer : public QProtobufBaseSerializer
{
    Q_DISABLE_COPY_MOVE(QProtobufSerializer)
public:
    QProtobufSerializer();
    ~QProtobufSerializer() override;

    QProtobufSerializer::DeserializationError deserializationError() const override;
    QString deserializationErrorString() const override;

    QByteArray
    serializeMessage(const QProtobufMessage *message,
                     const QtProtobufPrivate::QProtobufPropertyOrdering &ordering
                     ) const override;
    bool deserializeMessage(QProtobufMessage *message,
                            const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                            QByteArrayView data) const override;

    QByteArray
    serializeObject(const QProtobufMessage *message,
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
    QProtobufBaseSerializer::Status deserializeListObject(QProtobufMessage *message,
                                                          const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                                                          QtProtobufPrivate::QProtobufSelfcheckIterator &it
                                                          ) const override;

    QByteArray
    serializeMapPair(const QList<QPair<QVariant, QVariant> > &list,
                     const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo
                     ) const override;
    QProtobufBaseSerializer::Status deserializeMapPair(QVariant &key, QVariant &value,
                            QtProtobufPrivate::QProtobufSelfcheckIterator &it
                            ) const override;

    QByteArray
    serializeEnum(QtProtobuf::int64 value,
                  const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo
                  ) const override;
    QByteArray
    serializeEnumList(const QList<QtProtobuf::int64> &value,
                      const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo
                      ) const override;

    Q_REQUIRED_RESULT
    bool deserializeEnum(QtProtobuf::int64 &value,
                         QtProtobufPrivate::QProtobufSelfcheckIterator &it) const override;
    Q_REQUIRED_RESULT bool
    deserializeEnumList(QList<QtProtobuf::int64> &value,
                        QtProtobufPrivate::QProtobufSelfcheckIterator &it) const override;

    void shouldPreserveUnknownFields(bool preserveUnknownFields);
private:
    std::unique_ptr<QProtobufSerializerPrivate> d_ptr;
};

QT_END_NAMESPACE
#endif // QPROTOBUFSERIALIZER_H
