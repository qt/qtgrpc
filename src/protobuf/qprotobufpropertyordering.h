// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFPROPERTYORDERING_H
#define QPROTOBUFPROPERTYORDERING_H

#if 0
#  pragma qt_sync_skip_header_check
#  pragma qt_sync_stop_processing
#endif

#include <QtProtobuf/qtprotobufglobal.h>

#include <QtCore/qutf8stringview.h>

#include <QtCore/qxptype_traits.h>

QT_BEGIN_NAMESPACE

namespace QtProtobufPrivate {

enum FieldFlag : uint {
    NoFlags = 0x0,
    NonPacked = 0x1,
    Oneof = 0x02,
    Optional = 0x04,
    ExplicitPresence = 0x08,
    Message = 0x10,
    Enum = 0x20,
    Repeated = 0x40,
    Map = 0x80,
};
Q_DECLARE_FLAGS(FieldFlags, FieldFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(FieldFlags)

class QProtobufPropertyOrderingBuilder;
struct QProtobufPropertyOrdering
{
    const struct Data
    {
        uint version;
        uint numFields;
        uint fieldNumberOffset;
        uint propertyIndexOffset;
        uint flagsOffset;
        uint fullPackageNameSize;
    } *data;

    Q_PROTOBUF_EXPORT QUtf8StringView getMessageFullName() const;
    Q_PROTOBUF_EXPORT QUtf8StringView getJsonName(int index) const;
    Q_PROTOBUF_EXPORT int getFieldNumber(int index) const;
    Q_PROTOBUF_EXPORT int getPropertyIndex(int index) const;
    Q_PROTOBUF_EXPORT FieldFlags getFieldFlags(int index) const;
    Q_PROTOBUF_EXPORT int indexOfFieldNumber(int fieldNumber) const;
    int fieldCount() const { return int(data->numFields); }

private:
    friend class QProtobufPropertyOrderingBuilder;
    struct NonConstTag {};
    uint *uint_data(NonConstTag) const;
    char *char_data(NonConstTag) const;
    const uint *uint_data() const;
    const char *char_data() const;
    const uint &uint_dataForIndex(int index, uint offset) const;
};
static_assert(std::is_trivially_destructible_v<QProtobufPropertyOrdering>);

// Convenience structure to hold a reference to a single field information
struct QProtobufFieldInfo
{
    QProtobufFieldInfo(QProtobufPropertyOrdering ord, int ind)
        : ordering(ord), index(ind)
    {
        Q_ASSERT(index >= 0);
    }

    QUtf8StringView getJsonName() const { return ordering.getJsonName(index); }
    int getFieldNumber() const { return ordering.getFieldNumber(index); }
    int getPropertyIndex() const { return ordering.getPropertyIndex(index); }
    FieldFlags getFieldFlags() const { return ordering.getFieldFlags(index); }

private:
    const QProtobufPropertyOrdering ordering;
    const int index;
};

template<typename T>
using HasProtobufStaticPropertyOrdering = decltype(T::staticPropertyOrdering);

template <typename T>
using has_q_protobuf_object_macro = qxp::is_detected<HasProtobufStaticPropertyOrdering, T>;

template <typename T>
constexpr bool has_q_protobuf_object_macro_v = has_q_protobuf_object_macro<T>::value;

} // namespace QtProtobufPrivate

QT_END_NAMESPACE

#endif // QPROTOBUFPROPERTYORDERING_H
