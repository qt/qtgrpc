// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFREGISTRATION_H
#define QPROTOBUFREGISTRATION_H

#if 0
#  pragma qt_sync_skip_header_check
#  pragma qt_sync_stop_processing
#endif

#include <QtProtobuf/qtprotobufglobal.h>

#include <QtProtobuf/qabstractprotobufserializer.h>
#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufpropertyordering.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/qmetatype.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qhash.h>

QT_BEGIN_NAMESPACE

namespace QtProtobuf {
using RegisterFunction = void (*)();
// This struct is used for type registrations in generated code
struct ProtoTypeRegistrar
{
    Q_PROTOBUF_EXPORT explicit ProtoTypeRegistrar(QtProtobuf::RegisterFunction initializer);
};
}


namespace QtProtobufPrivate {
extern Q_PROTOBUF_EXPORT void registerOrdering(QMetaType type, QProtobufPropertyOrdering ordering);

using Serializer = void (*)(const QAbstractProtobufSerializer *, const void *,
                            const QProtobufFieldInfo &);
using Deserializer = void (*)(const QAbstractProtobufSerializer *, void *);

struct SerializationHandler
{
    Serializer serializer = nullptr; /*!< serializer assigned to class */
    Deserializer deserializer = nullptr; /*!< deserializer assigned to class */
};

extern Q_PROTOBUF_EXPORT void registerHandler(QMetaType type, Serializer serializer,
                                              Deserializer deserializer);

inline void ensureSerializer(const QAbstractProtobufSerializer *serializer)
{
    Q_ASSERT_X(serializer != nullptr, "QAbstractProtobufSerializer", "Serializer is null");
}

inline void ensureValue(const void *valuePtr)
{
    Q_ASSERT_X(valuePtr != nullptr, "QAbstractProtobufSerializer", "Value is nullptr");
}

template <typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void serializeList(const QAbstractProtobufSerializer *serializer, const void *valuePtr,
                   const QProtobufFieldInfo &fieldInfo)
{
    ensureSerializer(serializer);
    ensureValue(valuePtr);

    for (const auto &value : *static_cast<const QList<V> *>(valuePtr))
        serializer->serializeObject(&value, fieldInfo);
}

template <typename K, typename V>
void serializeMap(const QAbstractProtobufSerializer *serializer, const void *valuePtr,
                  const QProtobufFieldInfo &fieldInfo)
{
    ensureSerializer(serializer);
    ensureValue(valuePtr);

    using QProtobufMapEntry = QProtobufMapEntry<K, const V>;
    static_assert(!std::is_pointer_v<typename QProtobufMapEntry::KeyType>,
                  "Map key must not be message");
    QProtobufMapEntry el;
    const auto *mapPtr = static_cast<const QHash<K, V> *>(valuePtr);
    for (const auto &[k, v] : mapPtr->asKeyValueRange()) {
        el.setKey(k);

        if constexpr (std::is_pointer_v<typename QProtobufMapEntry::ValueType>)
            el.setValue(&v);
        else
            el.setValue(v);

        serializer->serializeObject(&el, fieldInfo);
    }
}

template <typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void deserializeList(const QAbstractProtobufSerializer *serializer, void *valuePtr)
{
    ensureSerializer(serializer);
    ensureValue(valuePtr);

    auto *listPtr = static_cast<QList<V> *>(valuePtr);
    if (V item; serializer->deserializeObject(&item))
        listPtr->append(std::move(item));
}

template <typename K, typename V>
void deserializeMap(const QAbstractProtobufSerializer *serializer, void *valuePtr)
{
    ensureSerializer(serializer);

    using QProtobufMapEntry = QProtobufMapEntry<K, V>;
    static_assert(!std::is_pointer_v<typename QProtobufMapEntry::KeyType>,
                  "Map key must not be message");
    auto *mapPtr = static_cast<QHash<K, V> *>(valuePtr);
    if (QProtobufMapEntry el; serializer->deserializeObject(&el)) {
        auto it = mapPtr->emplace(std::move(el).key());

        if constexpr (std::is_pointer_v<typename QProtobufMapEntry::ValueType>)
            *it = std::move(*el.value());
        else
            *it = std::move(el).value();
    }
}

template <typename T, typename std::enable_if_t<std::is_enum<T>::value, bool> = true>
static std::optional<QList<T>> intToEnumList(QList<QtProtobuf::int64> v)
{
    QList<T> enumList;
    for (const auto &intValue : v)
        enumList.append(static_cast<T>(intValue.t));

    return enumList;
}

template <typename T, typename std::enable_if_t<std::is_enum<T>::value, bool> = true>
static QList<QtProtobuf::int64> enumToIntList(QList<T> v)
{
    QList<QtProtobuf::int64> intList;
    for (const auto enumValue : v)
        intList.append(QtProtobuf::int64(enumValue));

    return intList;
}

template <typename T, typename std::enable_if_t<std::is_enum<T>::value, bool> = true>
static std::optional<QList<T>> stringToEnumList(QStringList v)
{
    static const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
    QList<T> enumList;
    bool ok = false;
    for (const auto &stringValue : v) {
        T enumV = T(metaEnum.keyToValue(stringValue.toUtf8().data(), &ok));
        if (!ok)
            return std::nullopt;

        enumList.append(enumV);
    }

    return enumList;
}

template <typename T, typename std::enable_if_t<std::is_enum<T>::value, bool> = true>
static QStringList enumToStringList(QList<T> v)
{
    static const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
    QStringList stringList;
    for (const auto enumValue : v)
        stringList.append(QString::fromUtf8(metaEnum.valueToKey(enumValue)));

    return stringList;
}
} // namespace QtProtobufPrivate

Q_PROTOBUF_EXPORT void qRegisterProtobufTypes();

template<typename T>
inline void qRegisterProtobufType()
{
    T::registerTypes();
    QtProtobufPrivate::registerOrdering(QMetaType::fromType<T>(), T::staticPropertyOrdering);
    QtProtobufPrivate::registerHandler(QMetaType::fromType<QList<T>>(),
                                       QtProtobufPrivate::serializeList<T>,
                                       QtProtobufPrivate::deserializeList<T>);
}

template <typename K, typename V>
inline void qRegisterProtobufMapType()
{
    QtProtobufPrivate::registerHandler(QMetaType::fromType<QHash<K, V>>(),
                                       QtProtobufPrivate::serializeMap<K, V>,
                                       QtProtobufPrivate::deserializeMap<K, V>);
}

template <typename T, typename std::enable_if_t<std::is_enum<T>::value, bool> = true>
inline void qRegisterProtobufEnumType()
{
    QMetaType::registerConverter<QList<T>,
                                 QList<QtProtobuf::int64>>(QtProtobufPrivate::enumToIntList<T>);
    QMetaType::registerConverter<QList<QtProtobuf::int64>,
                                 QList<T>>(QtProtobufPrivate::intToEnumList<T>);
    QMetaType::registerConverter<QList<T>, QStringList>(QtProtobufPrivate::enumToStringList<T>);
    QMetaType::registerConverter<QStringList, QList<T>>(QtProtobufPrivate::stringToEnumList<T>);
}

QT_END_NAMESPACE

#endif // QPROTOBUFREGISTRATION_H
