// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef Q_PROTOBUF_MESSAGE_H
#define Q_PROTOBUF_MESSAGE_H

#include <QtProtobuf/qtprotobufglobal.h>
#include <QtProtobuf/qprotobufpropertyordering.h>

#include <QtCore/qtconfigmacros.h>
#include <QtCore/qtmetamacros.h>
#include <QtCore/qmetaobject.h>


QT_BEGIN_NAMESPACE

class QProtobufMessage;
struct QProtobufMessageDeleter {
    Q_PROTOBUF_EXPORT void operator()(QProtobufMessage *ptr) noexcept;
};
using QProtobufMessagePointer = std::unique_ptr<QProtobufMessage, QProtobufMessageDeleter>;

class QAbstractProtobufSerializer;
class QProtobufMessagePrivate;
class QProtobufMessage
{
    Q_GADGET_EXPORT(Q_PROTOBUF_EXPORT)
public:
    Q_PROTOBUF_EXPORT QVariant property(QAnyStringView propertyName) const;
    Q_PROTOBUF_EXPORT bool setProperty(QAnyStringView propertyName, const QVariant &value);
    Q_PROTOBUF_EXPORT bool setProperty(QAnyStringView propertyName, QVariant &&value);

    [[nodiscard]]
    Q_PROTOBUF_EXPORT static QProtobufMessagePointer constructByName(const QString &messageType);

    Q_PROTOBUF_EXPORT QList<qint32> unknownFieldNumbers() const;
    Q_PROTOBUF_EXPORT QList<QByteArray> unknownFieldData(qint32 field) const;

    Q_PROTOBUF_EXPORT QByteArray serialize(QAbstractProtobufSerializer *serializer) const;
    Q_PROTOBUF_EXPORT bool deserialize(QAbstractProtobufSerializer *serializer,
                                       QByteArrayView data);

    Q_PROTOBUF_EXPORT const QtProtobufPrivate::QProtobufPropertyOrdering *propertyOrdering() const;

protected:
    Q_PROTOBUF_EXPORT explicit QProtobufMessage(const QMetaObject *metaObject,
                                                const QtProtobufPrivate::QProtobufPropertyOrdering
                                                    *ordering);
    Q_PROTOBUF_EXPORT ~QProtobufMessage();
    Q_PROTOBUF_EXPORT QProtobufMessage(const QProtobufMessage &other);
    Q_PROTOBUF_EXPORT QProtobufMessage &operator=(const QProtobufMessage &other);
    QProtobufMessage(QProtobufMessage &&other) noexcept : d_ptr(std::exchange(other.d_ptr, {})) { }
    QProtobufMessage &operator=(QProtobufMessage &&other) noexcept
    {
        qt_ptr_swap(d_ptr, other.d_ptr);
        return *this;
    }

    QVariant property(const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo,
                      bool allowInitialize = false) const;
    bool setProperty(const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo,
                     const QVariant &value);
    bool setProperty(const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo,
                     QVariant &&value);

private:
    friend Q_PROTOBUF_EXPORT bool comparesEqual(const QProtobufMessage &lhs,
                                                const QProtobufMessage &rhs) noexcept;
    Q_DECLARE_EQUALITY_COMPARABLE(QProtobufMessage);

    void detachPrivate(); // Call before editing the private!

    const QMetaObject *metaObject() const;

    friend class QProtobufSerializer;
    friend class QAbstractProtobufSerializer;
    friend class QProtobufSerializerPrivate;
    friend class QProtobufJsonSerializerPrivate;
    friend struct QProtobufMessageDeleter;

    QProtobufMessagePrivate *d_ptr;
    Q_DECLARE_PRIVATE(QProtobufMessage)
};

QT_END_NAMESPACE

#endif // Q_PROTOBUF_MESSAGE_H
