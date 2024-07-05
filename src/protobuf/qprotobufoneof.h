// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFONEOF_H
#define QPROTOBUFONEOF_H

#if 0
#  pragma qt_class(QProtobufOneof)
#endif

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/qmetatype.h>
#include <QtCore/qvariant.h>

#include <type_traits>

QT_BEGIN_NAMESPACE

namespace QtProtobufPrivate {

class QProtobufOneofPrivate;
class QProtobufOneof final
{
    template<typename T>
    using IsProtobufMessageType =
            typename std::enable_if_t<!std::is_pointer_v<T>
                                              && std::is_base_of_v<QProtobufMessage, T>
                                              && has_q_protobuf_object_macro_v<T>,
                                      int>;

    template <typename T>
    using IsNonMessageProtobufType = std::enable_if_t<
        std::disjunction_v<std::is_enum<T>, QtProtobuf::is_protobuf_scalar_value_type<T>>, bool>;

public:
    Q_PROTOBUF_EXPORT  QProtobufOneof();
    Q_PROTOBUF_EXPORT  ~QProtobufOneof();
    Q_PROTOBUF_EXPORT  QProtobufOneof(const QProtobufOneof &other);
    Q_PROTOBUF_EXPORT  QProtobufOneof &operator=(const QProtobufOneof &other);
    QProtobufOneof(QProtobufOneof &&other) noexcept : d_ptr(std::exchange(other.d_ptr, {})) { }
    QProtobufOneof &operator=(QProtobufOneof &&other) noexcept
    {
        qt_ptr_swap(d_ptr, other.d_ptr);
        return *this;
    }

    template<typename T>
    void setValue(const T &value, int fieldNumber)
    {
        setValue(QVariant::fromValue<T>(value), fieldNumber);
    }

    template <typename T, IsNonMessageProtobufType<T> = true>
    T value() const
    {
        Q_ASSERT(QMetaType::fromType<T>() == rawValue().metaType());
        return rawValue().value<T>();
    }

    template<typename T, IsProtobufMessageType<T> = 0>
    T *value()
    {
        if (rawValue().isNull() || QMetaType::fromType<T>() != rawValue().metaType())
            rawValue().setValue(T());
        return reinterpret_cast<T *>(rawValue().data());
    }

    template<typename T, IsProtobufMessageType<T> = 0>
    const T *value() const
    {
        Q_ASSERT(QMetaType::fromType<T>() == rawValue().metaType());
        return reinterpret_cast<const T *>(rawValue().data());
    }

    template <typename T, IsNonMessageProtobufType<T> = true>
    bool isEqual(const T &otherValue, int fieldNumber) const
    {
        return this->fieldNumber() == fieldNumber
                && QMetaType::fromType<T>() == rawValue().metaType() && value<T>() == otherValue;
    }

    template<typename T, IsProtobufMessageType<T> = 0>
    bool isEqual(const T &otherValue, int fieldNumber) const
    {
        return this->fieldNumber() == fieldNumber
                && QMetaType::fromType<T>() == rawValue().metaType() && value<T>()
                && *(value<T>()) == otherValue;
    }

    Q_PROTOBUF_EXPORT int fieldNumber() const;
    Q_PROTOBUF_EXPORT bool holdsField(int fieldNumber) const;

private:
    friend Q_PROTOBUF_EXPORT bool comparesEqual(const QProtobufOneof &lhs,
                                                const QProtobufOneof &rhs) noexcept;
    Q_DECLARE_EQUALITY_COMPARABLE(QProtobufOneof)

    friend class QT_PREPEND_NAMESPACE(QProtobufMessage);

    Q_PROTOBUF_EXPORT void setValue(const QVariant &value, int fieldNumber);
    Q_PROTOBUF_EXPORT const QVariant &rawValue() const;
    Q_PROTOBUF_EXPORT QVariant &rawValue();

    QProtobufOneofPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QProtobufOneof)
};
} // namespace QtProtobufPrivate

QT_END_NAMESPACE

#endif // QPROTOBUFONEOF_H
