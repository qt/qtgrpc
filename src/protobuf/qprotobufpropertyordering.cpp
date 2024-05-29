// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qprotobufpropertyordering.h"
#include "qtprotobuflogging_p.h"
#include "qprotobufregistration.h"


#include <QtCore/qhash.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qpair.h>
#include <QtCore/qreadwritelock.h>
#include <QtCore/qstring.h>

#include <limits>

namespace {
/*!
    \internal
    The ProtobufOrderingRegistry stores the mapping between
    QtProtobufPrivate::QProtobufPropertyOrdering and QMetaType.
*/
struct ProtobufOrderingRegistry
{
    using ProtobufOrderingRegistryRecord =
        QPair<QMetaType, QtProtobufPrivate::QProtobufPropertyOrdering>;

    void registerOrdering(QMetaType type, QtProtobufPrivate::QProtobufPropertyOrdering ordering)
    {
        QWriteLocker locker(&m_lock);
        m_registry[ordering.getMessageFullName().toString()] =
            ProtobufOrderingRegistryRecord(type, ordering);
    }

    QtProtobufPrivate::QProtobufPropertyOrdering getOrdering(QMetaType type) const
    {
        QtProtobufPrivate::QProtobufPropertyOrdering ordering{ nullptr };

        QReadLocker locker(&m_lock);
        for (auto it = m_registry.constBegin(); it != m_registry.constEnd(); ++it) {
            if (type == it.value().first) {
                ordering = it.value().second;
                break;
            }
        }

        return ordering;
    }

    ProtobufOrderingRegistryRecord
    findMessageByName(const QString &messageName) const
    {
        ProtobufOrderingRegistryRecord record = {
            QMetaType(QMetaType::UnknownType), { nullptr }
        };

        QReadLocker locker(&m_lock);
        auto it = m_registry.constFind(messageName);
        if (it != m_registry.constEnd())
            record = it.value();
        return record;
    }

private:
    mutable QReadWriteLock m_lock;
    QHash<QString, ProtobufOrderingRegistryRecord> m_registry;
};

Q_GLOBAL_STATIC(ProtobufOrderingRegistry, orderingRegistry)
}

QT_BEGIN_NAMESPACE

namespace QtProtobufPrivate {
constexpr uint jsonNameOffsetsOffset = 0;
// Use this constant to make the +/- 1 more easily readable
constexpr int NullTerminator = 1;
QUtf8StringView QProtobufPropertyOrdering::getMessageFullName() const
{
    Q_ASSERT(data);
    const char *name = char_data();
    return { name, qsizetype(data->fullPackageNameSize) };
}

QUtf8StringView QProtobufPropertyOrdering::getJsonName(int index) const
{
    Q_ASSERT(data);
    if (Q_UNLIKELY(index < 0 || index >= fieldCount()))
        return {}; // Invalid index (out of bounds)
    const uint stringOffset = uint_dataForIndex(index, jsonNameOffsetsOffset);
    const char *name = char_data() + stringOffset;
    // This is fine because we store an extra offset for end-of-string
    const uint nameLength =
        uint_dataForIndex(index + 1, jsonNameOffsetsOffset) - NullTerminator - stringOffset;
    return { name, qsizetype(nameLength) };
}

int QProtobufPropertyOrdering::getFieldNumber(int index) const
{
    Q_ASSERT(data);
    if (Q_UNLIKELY(index < 0 || index >= fieldCount()))
        return -1; // Invalid index (out of bounds)
    uint fieldNumber = uint_dataForIndex(index, data->fieldNumberOffset);
    if (Q_UNLIKELY(fieldNumber > uint(std::numeric_limits<int>::max())))
        return -1;
    return int(fieldNumber);
}

int QProtobufPropertyOrdering::getPropertyIndex(int index) const
{
    Q_ASSERT(data);
    if (Q_UNLIKELY(index < 0 || index >= fieldCount()))
        return -1; // Invalid index (out of bounds)
    uint propertyIndex = uint_dataForIndex(index, data->propertyIndexOffset);
    if (Q_UNLIKELY(propertyIndex > uint(std::numeric_limits<int>::max())))
        return -1;
    return int(propertyIndex);
}

int QProtobufPropertyOrdering::indexOfFieldNumber(int fieldNumber) const
{
    Q_ASSERT(data);
    if (Q_LIKELY(fieldNumber > 0)) {
        for (int i = 0; i < fieldCount(); ++i) {
            if (uint_dataForIndex(i, data->fieldNumberOffset) == static_cast<uint>(fieldNumber))
                return i;
        }
    }
    return -1;
}

FieldFlags QProtobufPropertyOrdering::getFieldFlags(int index) const
{
    Q_ASSERT(data);
    if (Q_UNLIKELY(index < 0 || index >= fieldCount()))
        return {}; // Invalid index (out of bounds)
    return FieldFlags { int(uint_dataForIndex(index, data->flagsOffset)) };
}

const uint *QProtobufPropertyOrdering::uint_data() const
{
    Q_ASSERT(data);
    Q_ASSERT(data->version == 0);
    quintptr dataPtr = quintptr(data);
    dataPtr += sizeof(Data);
#if 0 // if Data is ever not just a bunch of uints, remove the #if 0
        if (dataPtr % alignof(uint) != 0)
            dataPtr += alignof(uint) - (dataPtr % alignof(uint));
#else
    static_assert(alignof(Data) == alignof(uint));
#endif
    return reinterpret_cast<const uint *>(dataPtr);
}

const char *QProtobufPropertyOrdering::char_data() const
{
    Q_ASSERT(data);
    const uint LastOffset = data->flagsOffset;
    const uint *u_data = uint_data() + LastOffset + data->numFields;
    quintptr uptr_data = quintptr(u_data);
    if (size_t(uptr_data) % alignof(char) != 0)
        uptr_data += alignof(char) - size_t(uptr_data) % alignof(char);
    const char *c_data = reinterpret_cast<const char *>(uptr_data);
    return c_data;
}

const uint &QProtobufPropertyOrdering::uint_dataForIndex(int index, uint offset) const
{
    Q_ASSERT(data);
    Q_ASSERT(index >= 0);
    Q_ASSERT(uint(index) < data->numFields
             || (offset == jsonNameOffsetsOffset && uint(index) == data->numFields));
    return *(uint_data() + offset + index);
}

void registerOrdering(QMetaType type, QProtobufPropertyOrdering ordering)
{
    orderingRegistry->registerOrdering(type, ordering);
}

QProtobufPropertyOrdering getOrderingByMetaType(QMetaType type)
{
    return orderingRegistry->getOrdering(type);
}

QProtobufMessagePointer constructMessageByName(const QString &messageType)
{
    qRegisterProtobufTypes();
    ProtobufOrderingRegistry::ProtobufOrderingRegistryRecord messageOrderingRecord =
        orderingRegistry->findMessageByName(messageType);
    QMetaType type = messageOrderingRecord.first;
    QProtobufMessagePointer pointer;
    if (type.id() != QMetaType::UnknownType) {
        void *msg = type.create();
        pointer.reset(reinterpret_cast<QProtobufMessage *>(msg));
        return pointer;
    }
    qProtoWarning() << "Unable to find protobuf message with name" << messageType
                    << ". Message is not registered.";
    return pointer;
}

}

QT_END_NAMESPACE
