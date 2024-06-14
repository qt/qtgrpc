// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFJSONSERIALIZER_H
#define QPROTOBUFJSONSERIALIZER_H

#include <QtProtobuf/qtprotobufglobal.h>

#include <QtProtobuf/qabstractprotobufserializer.h>
#include <QtProtobuf/qtprotobuftypes.h>

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

private:
    std::unique_ptr<QProtobufJsonSerializerPrivate> d_ptr;
};

QT_END_NAMESPACE

#endif // QPROTOBUFJSONSERIALIZER_H

