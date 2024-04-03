// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef Q_PROTOBUF_ANYSUPPORT_H
#define Q_PROTOBUF_ANYSUPPORT_H

#include <QtProtobufWellKnownTypes/qtprotobufwellknowntypesglobal.h>

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/qanystringview.h>

#include <utility>
#include <optional>
#include <type_traits>

QT_BEGIN_NAMESPACE

class QAbstractProtobufSerializer;

namespace QtProtobuf {
class AnyPrivate;
class Q_PROTOBUFWELLKNOWNTYPES_EXPORT Any : public QProtobufMessage
{
    Q_GADGET
    Q_PROPERTY(QString typeUrl READ typeUrl WRITE setTypeUrl SCRIPTABLE true)
    Q_PROPERTY(QByteArray value READ value WRITE setValue SCRIPTABLE true)
public:
    static void registerTypes();

    Any();
    ~Any();
    Any(const Any &other);
    Any &operator=(const Any &other);
    Any(Any &&other) noexcept
        : QProtobufMessage(std::move(other)), d_ptr(std::exchange(other.d_ptr, {}))
    {
    }
    Any &operator=(Any &&other) noexcept
    {
        qt_ptr_swap(d_ptr, other.d_ptr);
        QProtobufMessage::operator=(std::move(other));
        return *this;
    }

    QString typeUrl() const;
    QByteArray value() const;
    void setTypeUrl(const QString &typeUrl);
    void setValue(const QByteArray &value);

    template <typename T>
    std::optional<T> unpack(QAbstractProtobufSerializer *serializer) const
    {
        if constexpr (std::is_same_v<T, Any>) {
            return unpackAnyImpl(serializer);
        } else {
            static_assert(QtProtobufPrivate::HasProtobufPropertyOrdering<T>,
                          "T must have the Q_PROTOBUF_OBJECT macro");
            T obj;
            if (unpackImpl(serializer, &obj))
                return { std::move(obj) };
        }
        return std::nullopt;
    }

    template <typename T>
    static Any fromMessage(QAbstractProtobufSerializer *serializer, const T &message,
                           QAnyStringView typeUrlPrefix = defaultUrlPrefix())
    {
        if constexpr (std::is_same_v<T, Any>)
            return fromAnyMessageImpl(serializer, &message, typeUrlPrefix);

        static_assert(QtProtobufPrivate::HasProtobufPropertyOrdering<T>,
                      "T must have the Q_PROTOBUF_OBJECT macro");
        return fromMessageImpl(serializer, &message, typeUrlPrefix);
    }

private:
    AnyPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Any)

    bool unpackImpl(QAbstractProtobufSerializer *serializer, QProtobufMessage *message) const;
    std::optional<Any> unpackAnyImpl(QAbstractProtobufSerializer *serializer) const;
    static Any fromMessageImpl(QAbstractProtobufSerializer *serializer,
                               const QProtobufMessage *message, QAnyStringView typeUrlPrefix);
    static Any fromAnyMessageImpl(QAbstractProtobufSerializer *serializer,
                                  const Any *message, QAnyStringView typeUrlPrefix);

    static QAnyStringView defaultUrlPrefix();

    friend Q_PROTOBUFWELLKNOWNTYPES_EXPORT bool
    comparesEqual(const Any &lhs, const Any &rhs) noexcept;
    friend bool operator==(const Any &lhs, const Any &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const Any &lhs, const Any &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
};
} // namespace QtProtobuf

QT_END_NAMESPACE

#endif // Q_PROTOBUF_ANYSUPPORT_H
