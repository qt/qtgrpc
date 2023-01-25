// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef Q_PROTOBUF_MESSAGE_H
#define Q_PROTOBUF_MESSAGE_H

#include <QtProtobuf/qtprotobufglobal.h>
#include <QtCore/qtconfigmacros.h>
#include <QtCore/qtmetamacros.h>
#include <QtCore/qmetatype.h>

QT_BEGIN_NAMESPACE

class QProtobufMessagePrivate;
class Q_PROTOBUF_EXPORT QProtobufMessage
{
    Q_GADGET
public:
    virtual ~QProtobufMessage();

    QVariant property(QAnyStringView propertyName) const;
    bool setProperty(QAnyStringView propertyName, const QVariant &value);

    Q_REQUIRED_RESULT static QProtobufMessage *constructByName(const QString &messageType);

protected:
    explicit QProtobufMessage(const QMetaObject *metaObject);
    QProtobufMessage(const QProtobufMessage &other);
    QProtobufMessage &operator=(const QProtobufMessage &other);
    QProtobufMessage(QProtobufMessage &&other) noexcept : d_ptr(std::exchange(other.d_ptr, {})) { }
    QProtobufMessage &operator=(QProtobufMessage &&other) noexcept
    {
        qt_ptr_swap(d_ptr, other.d_ptr);
        return *this;
    }

    static bool isEqual(const QProtobufMessage &lhs, const QProtobufMessage &rhs) noexcept;

private:
    const QMetaObject *metaObject() const;

    friend class QProtobufSerializer;
    friend class QAbstractProtobufSerializer;
    friend class QProtobufSerializerPrivate;
    friend class QAbstractProtobufSerializer;

    QProtobufMessagePrivate *d_ptr;
    Q_DECLARE_PRIVATE(QProtobufMessage)
};

QT_END_NAMESPACE

#endif // Q_PROTOBUF_MESSAGE_H
