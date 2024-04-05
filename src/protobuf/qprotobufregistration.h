// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFREGISTRATION_H
#define QPROTOBUFREGISTRATION_H

#if 0
#  pragma qt_sync_skip_header_check
#  pragma qt_sync_stop_processing
#endif

#include <QtCore/qmetatype.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qhash.h>

#include <QtProtobuf/qtprotobufglobal.h>
#include <QtProtobuf/qtprotobuftypes.h>
#include <QtProtobuf/qabstractprotobufserializer.h>
#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufpropertyordering.h>

#include <memory>

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
                            const QProtobufPropertyOrderingInfo &);
using Deserializer = void (*)(const QAbstractProtobufSerializer *, QVariant &);

struct SerializationHandler
{
    Serializer serializer = nullptr; /*!< serializer assigned to class */
    Deserializer deserializer = nullptr; /*!< deserializer assigned to class */
};

extern Q_PROTOBUF_EXPORT SerializationHandler findHandler(QMetaType type);
extern Q_PROTOBUF_EXPORT void registerHandler(QMetaType type, const SerializationHandler &handlers);

template <typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void serializeList(const QAbstractProtobufSerializer *serializer, const QVariant &listValue,
                   const QProtobufPropertyOrderingInfo &fieldInfo)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufSerializer", "Serializer is null");
    for (const auto &value : listValue.value<QList<V>>()) {
        serializer->serializeListObject(&value, fieldInfo);
    }
}

template <typename K, typename V,
          typename std::enable_if_t<!std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void serializeMap(const QAbstractProtobufSerializer *serializer, const QVariant &value,
                  const QProtobufPropertyOrderingInfo &fieldInfo)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufSerializer", "Serializer is null");
    for (const auto &[k, v] : value.value<QHash<K, V>>().asKeyValueRange()) {
        serializer->serializeMapPair(QVariant::fromValue<K>(k), QVariant::fromValue<V>(v),
                                     fieldInfo);
    }
}

template <typename K, typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void serializeMap(const QAbstractProtobufSerializer *serializer, const QVariant &value,
                  const QProtobufPropertyOrderingInfo &fieldInfo)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufSerializer", "Serializer is null");
    for (const auto &[k, v] : value.value<QHash<K, V>>().asKeyValueRange()) {
        serializer->serializeMapPair(QVariant::fromValue<K>(k), QVariant::fromValue<V *>(&v),
                                     fieldInfo);
    }
}

template <typename T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
void serializeEnum(const QAbstractProtobufSerializer *serializer, const QVariant &value,
                   const QProtobufPropertyOrderingInfo &fieldInfo)
{
    Q_ASSERT_X(serializer != nullptr, "QAbstractProtobufSerializer", "Serializer is null");
    static const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
    serializer->serializeEnum(QtProtobuf::int64(value.value<T>()), metaEnum,
                              fieldInfo);
}

template <typename T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
void serializeEnumList(const QAbstractProtobufSerializer *serializer, const QVariant &value,
                       const QProtobufPropertyOrderingInfo &fieldInfo)
{
    Q_ASSERT_X(serializer != nullptr, "QAbstractProtobufSerializer", "Serializer is null");
    static const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
    QList<QtProtobuf::int64> intList;
    for (auto enumValue : value.value<QList<T>>()) {
        intList.append(QtProtobuf::int64(enumValue));
    }
    serializer->serializeEnumList(intList, metaEnum, fieldInfo);
}

template <typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void deserializeList(const QAbstractProtobufSerializer *serializer, QVariant &previous)
{
    Q_ASSERT_X(serializer != nullptr, "QAbstractProtobufSerializer", "Serializer is null");

    Q_ASSERT_X(previous.metaType() == QMetaType::fromType<QList<V>>() && previous.data(),
               "QAbstractProtobufSerializer::deserializeList",
               "Previous value metatype doesn't match the list metatype");
    V newValue;
    if (serializer->deserializeListObject(&newValue)) {
        QList<V> *list = reinterpret_cast<QList<V> *>(previous.data());
        list->append(newValue);
    }
}

template <typename K, typename V,
          typename std::enable_if_t<!std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void deserializeMap(const QAbstractProtobufSerializer *serializer, QVariant &previous)
{
    Q_ASSERT_X(serializer != nullptr, "QAbstractProtobufSerializer", "Serializer is null");
    Q_ASSERT_X((previous.metaType() == QMetaType::fromType<QHash<K, V>>()) && previous.data(),
               "QAbstractProtobufSerializer::deserializeMap",
               "Previous value metatype doesn't match the map metatype");

    QVariant key = QVariant::fromValue<K>(K());
    QVariant value = QVariant::fromValue<V>(V());

    if (serializer->deserializeMapPair(key, value)) {
        QHash<K, V> *out = reinterpret_cast<QHash<K, V> *>(previous.data());
        out->insert(key.value<K>(), value.value<V>());
    }
}

template <typename K, typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void deserializeMap(const QAbstractProtobufSerializer *serializer, QVariant &previous)
{
    Q_ASSERT_X(serializer != nullptr, "QAbstractProtobufSerializer", "Serializer is null");
    Q_ASSERT_X((previous.metaType() == QMetaType::fromType<QHash<K, V>>()) && previous.data(),
               "QAbstractProtobufSerializer::deserializeMap",
               "Previous value metatype doesn't match the map metatype");

    std::unique_ptr<V> valuePtr = std::make_unique<V>();
    QVariant key = QVariant::fromValue<K>(K());
    QVariant value = QVariant::fromValue<V *>(valuePtr.get());

    if (serializer->deserializeMapPair(key, value)) {
        QHash<K, V> *out = reinterpret_cast<QHash<K, V> *>(previous.data());
        out->insert(key.value<K>(), *(valuePtr.get()));
    }
}

template <typename T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
void deserializeEnum(const QAbstractProtobufSerializer *serializer, QVariant &to)
{
    Q_ASSERT_X(serializer != nullptr, "QAbstractProtobufSerializer", "Serializer is null");
    static const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
    QtProtobuf::int64 intValue;
    if (serializer->deserializeEnum(intValue, metaEnum))
        to = QVariant::fromValue<T>(static_cast<T>(intValue._t));
}

template <typename T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
void deserializeEnumList(const QAbstractProtobufSerializer *serializer, QVariant &previous)
{
    Q_ASSERT_X(serializer != nullptr, "QAbstractProtobufSerializer", "Serializer is null");
    static const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
    QList<QtProtobuf::int64> intList;
    if (!serializer->deserializeEnumList(intList, metaEnum))
        return;
    QList<T> enumList = previous.value<QList<T>>();
    for (auto intValue : intList)
        enumList.append(static_cast<T>(intValue._t));
    previous = QVariant::fromValue<QList<T>>(enumList);
}
} // namespace QtProtobufPrivate

Q_PROTOBUF_EXPORT void qRegisterProtobufTypes();

template<typename T>
inline void qRegisterProtobufType()
{
    T::registerTypes();
    QtProtobufPrivate::registerOrdering(QMetaType::fromType<T>(), T::staticPropertyOrdering);
    QtProtobufPrivate::registerHandler(
        QMetaType::fromType<QList<T>>(),
        { QtProtobufPrivate::serializeList<T>, QtProtobufPrivate::deserializeList<T> });
}

template<typename K, typename V>
inline void qRegisterProtobufMapType()
{
    QtProtobufPrivate::registerHandler(
        QMetaType::fromType<QHash<K, V>>(),
        { QtProtobufPrivate::serializeMap<K, V>, QtProtobufPrivate::deserializeMap<K, V> });
}

#ifdef Q_QDOC
template<typename T>
inline void qRegisterProtobufEnumType();
#else // !Q_QDOC
template<typename T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
inline void qRegisterProtobufEnumType()
{
    QtProtobufPrivate::registerHandler(
        QMetaType::fromType<T>(),
        { QtProtobufPrivate::serializeEnum<T>, QtProtobufPrivate::deserializeEnum<T> });
    QtProtobufPrivate::registerHandler(
        QMetaType::fromType<QList<T>>(),
        { QtProtobufPrivate::serializeEnumList<T>, QtProtobufPrivate::deserializeEnumList<T> });
}
#endif // Q_QDOC

QT_END_NAMESPACE

#endif // QPROTOBUFREGISTRATION_H
