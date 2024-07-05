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

using Serializer = void (*)(const QAbstractProtobufSerializer *, const QVariant &,
                            const QProtobufFieldInfo &);
using Deserializer = void (*)(const QAbstractProtobufSerializer *, QVariant &);

struct SerializationHandler
{
    Serializer serializer = nullptr; /*!< serializer assigned to class */
    Deserializer deserializer = nullptr; /*!< deserializer assigned to class */
};

extern Q_PROTOBUF_EXPORT SerializationHandler findHandler(QMetaType type);
extern Q_PROTOBUF_EXPORT void registerHandler(QMetaType type, Serializer serializer,
                                              Deserializer deserializer);

inline void ensureSerializer(const QAbstractProtobufSerializer *serializer)
{
    Q_ASSERT_X(serializer != nullptr, "QAbstractProtobufSerializer", "Serializer is null");
}

template <typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void serializeList(const QAbstractProtobufSerializer *serializer, const QVariant &listValue,
                   const QProtobufFieldInfo &fieldInfo)
{
    ensureSerializer(serializer);

    for (const auto &value : listValue.value<QList<V>>()) {
        serializer->serializeObject(&value, fieldInfo);
    }
}

template <typename K, typename V>
void serializeMap(const QAbstractProtobufSerializer *serializer, const QVariant &value,
                  const QProtobufFieldInfo &fieldInfo)
{
    ensureSerializer(serializer);

    using QProtobufMapEntry = QProtobufMapEntry<K, V>;
    static_assert(!std::is_pointer_v<typename QProtobufMapEntry::KeyType>,
                  "Map key must not be message");
    QProtobufMapEntry el;
    for (const auto &[k, v] : value.value<QHash<K, V>>().asKeyValueRange()) {
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
void deserializeList(const QAbstractProtobufSerializer *serializer, QVariant &previous)
{
    ensureSerializer(serializer);

    Q_ASSERT_X(previous.metaType() == QMetaType::fromType<QList<V>>() && previous.data(),
               "QAbstractProtobufSerializer::deserializeList",
               "Previous value metatype doesn't match the list metatype");
    V newValue;
    if (serializer->deserializeObject(&newValue)) {
        QList<V> *list = reinterpret_cast<QList<V> *>(previous.data());
        list->append(newValue);
    }
}

template <typename K, typename V>
void deserializeMap(const QAbstractProtobufSerializer *serializer, QVariant &previous)
{
    ensureSerializer(serializer);

    Q_ASSERT_X((previous.metaType() == QMetaType::fromType<QHash<K, V>>()) && previous.data(),
               "QAbstractProtobufSerializer::deserializeMap",
               "Previous value metatype doesn't match the map metatype");

    using QProtobufMapEntry = QProtobufMapEntry<K, V>;
    static_assert(!std::is_pointer_v<typename QProtobufMapEntry::KeyType>,
                  "Map key must not be message");
    QProtobufMapEntry el;
    if (serializer->deserializeObject(&el)) {
        QHash<K, V> *out = reinterpret_cast<QHash<K, V> *>(previous.data());
        auto it = out->emplace(std::move(el).key());

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

#ifdef Q_QDOC
template<typename T>
inline void qRegisterProtobufEnumType();
#else // !Q_QDOC
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
#endif // Q_QDOC

QT_END_NAMESPACE

#endif // QPROTOBUFREGISTRATION_H
