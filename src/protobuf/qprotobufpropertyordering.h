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

#include <type_traits>

QT_BEGIN_NAMESPACE

namespace QtProtobufPrivate {

enum FieldFlag : uint {
    NoFlags = 0x0,
    NonPacked = 0x1,
    Oneof = 0x02,
    Optional = 0x04,
    ExplicitPresence = 0x08
};

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
    Q_PROTOBUF_EXPORT uint getFieldFlags(int index) const;
    Q_PROTOBUF_EXPORT int indexOfFieldNumber(int fieldNumber) const;
    int fieldCount() const { return int(data->numFields); }

private:
    const uint *uint_data() const;
    const char *char_data() const;
    const uint &uint_dataForIndex(int index, uint offset) const;
};
static_assert(std::is_trivially_destructible_v<QProtobufPropertyOrdering>);

// Convenience structure to hold a reference to a single entry
struct QProtobufPropertyOrderingInfo
{
    QProtobufPropertyOrderingInfo(QProtobufPropertyOrdering ord, int ind)
        : ordering(ord), index(ind)
    {
        Q_ASSERT(index >= 0);
    }

    QUtf8StringView getJsonName() const { return ordering.getJsonName(index); }
    int getFieldNumber() const { return ordering.getFieldNumber(index); }
    int getPropertyIndex() const { return ordering.getPropertyIndex(index); }
    uint getFieldFlags() const { return ordering.getFieldFlags(index); }

private:
    const QProtobufPropertyOrdering ordering;
    const int index;
};

template<typename>
using sfinae = void;
template<typename T, typename = void>
[[maybe_unused]] static constexpr bool HasProtobufPropertyOrdering = false;
template<typename T>
[[maybe_unused]] static constexpr bool
    HasProtobufPropertyOrdering<T, sfinae<decltype(T::staticPropertyOrdering)>> = true;
} // namespace QtProtobufPrivate

QT_END_NAMESPACE

#endif // QPROTOBUFPROPERTYORDERING_H
