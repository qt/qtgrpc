// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFSBASEERIALIZER_H
#define QPROTOBUFSBASEERIALIZER_H

#include <QtProtobuf/qabstractprotobufserializer.h>
#include <QtProtobuf/qprotobufselfcheckiterator.h>

#include <QtCore/QList>
#include <QtCore/QMetaObject>
#include <QtCore/QPair>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

class Q_PROTOBUF_EXPORT QProtobufBaseSerializer: public QAbstractProtobufSerializer
{

public:
    enum Status {
        Serialized = 0,
        SerializationInProgress,
        SerializationError
    };

    virtual ~QProtobufBaseSerializer();

    virtual QByteArray
    serializeObject(const QProtobufMessage *message,
                    const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                    const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const = 0;
    virtual bool deserializeObject(QProtobufMessage *message,
                                   const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                                   QtProtobufPrivate::QProtobufSelfcheckIterator &it) const = 0;

    virtual QByteArray
    serializeListObject(const QList<const QProtobufMessage*> &messageList,
                        const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                        const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const = 0;
    virtual Status
    deserializeListObject(QProtobufMessage *message,
                          const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
                          QtProtobufPrivate::QProtobufSelfcheckIterator &it) const = 0;

    virtual QByteArray
    serializeMapPair(const QList<QPair<QVariant, QVariant>> &list,
                     const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const = 0;
    virtual Status deserializeMapPair(QVariant &key, QVariant &value,
                                      QtProtobufPrivate::QProtobufSelfcheckIterator &it) const = 0;

    virtual QByteArray
    serializeEnum(QtProtobuf::int64 value,
                  const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const = 0;
    virtual QByteArray
    serializeEnumList(const QList<QtProtobuf::int64> &value,
                      const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const = 0;
    virtual bool deserializeEnum(QtProtobuf::int64 &value,
                                 QtProtobufPrivate::QProtobufSelfcheckIterator &it) const = 0;
    virtual bool
    deserializeEnumList(QList<QtProtobuf::int64> &value,
                        QtProtobufPrivate::QProtobufSelfcheckIterator &it) const = 0;
};

namespace QtProtobufPrivate {

using Serializer = void (*)(const QProtobufBaseSerializer *, const QVariant &,
                            const QProtobufPropertyOrderingInfo &, QByteArray &);
using Deserializer = void (*)(const QProtobufBaseSerializer *, QProtobufSelfcheckIterator &,
                              QVariant &);

/*!
 \private
 \brief SerializationHandlers contains set of objects that required for class
 serialization/deserialization
 */
struct SerializationHandler
{
    Serializer serializer = nullptr; /*!< serializer assigned to class */
    Deserializer deserializer = nullptr; /*!< deserializer assigned to class */
};

extern Q_PROTOBUF_EXPORT SerializationHandler findHandler(QMetaType type);
extern Q_PROTOBUF_EXPORT void registerHandler(QMetaType type, const SerializationHandler &handlers);

/*!
 \private
 \brief default serializer template for type T that inherits from QProtobufMessage
 */
template<typename T,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, T>::value, int> = 0>
void serializeObject(const QProtobufBaseSerializer *serializer, const QVariant &value,
                     const QProtobufPropertyOrderingInfo &fieldInfo, QByteArray &buffer)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");
    buffer.append(serializer->serializeObject(value.value<T *>(), T::propertyOrdering, fieldInfo));
}

/*!
 \private
 \brief default serializer template for list of type T objects that inherits from QProtobufMessage
 */
template<typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void serializeList(const QProtobufBaseSerializer *serializer, const QVariant &listValue,
                   const QProtobufPropertyOrderingInfo &fieldInfo, QByteArray &buffer)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");
    QList<V> objList = listValue.value<QList<V>>();
    QList<const QProtobufMessage*> messageList;
    for (const auto &value : objList)
        messageList.append(&value);
    buffer.append(serializer->serializeListObject(messageList, V::propertyOrdering, fieldInfo));
}

/*!
 \private
 \brief default serializer template for map of key K, value V
 */
template<typename K, typename V,
          typename std::enable_if_t<!std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void serializeMap(const QProtobufBaseSerializer *serializer, const QVariant &value,
                  const QProtobufPropertyOrderingInfo &fieldInfo, QByteArray &buffer)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");
    QHash<K, V> original = value.value<QHash<K, V>>();
    QList<QPair<QVariant, QVariant>> variantContainer;
    for (const auto &[k, v] : original.asKeyValueRange()) {
        variantContainer.append(qMakePair(QVariant::fromValue<K>(k),
                                           QVariant::fromValue<V>(v)));
    }
    buffer.append(serializer->serializeMapPair(variantContainer, fieldInfo));

}

/*!
 \private
 \brief default serializer template for map of type key K, value V. Specialization for V that
 inherits from QProtobufMessage
 */
template<typename K, typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void serializeMap(const QProtobufBaseSerializer *serializer, const QVariant &value,
                  const QProtobufPropertyOrderingInfo &fieldInfo, QByteArray &buffer)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");
    QHash<K, V> original = value.value<QHash<K, V>>();
    QList<QPair<QVariant, QVariant>> variantContainer;
    for (const auto &[k, v] : original.asKeyValueRange()) {
        variantContainer.append(qMakePair(QVariant::fromValue<K>(k),
                                           QVariant::fromValue<V *>(&v)));

    }
    buffer.append(serializer->serializeMapPair(variantContainer, fieldInfo));
}

/*!
 \private
 \brief default serializer template for enum types
 */
template<typename T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
void serializeEnum(const QProtobufBaseSerializer *serializer, const QVariant &value,
                   const QProtobufPropertyOrderingInfo &fieldInfo, QByteArray &buffer)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");
    buffer.append(serializer->serializeEnum(QtProtobuf::int64(value.value<T>()), fieldInfo));
}

/*!
 \private
 \brief default serializer template for enum list types
 */
template<typename T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
void serializeEnumList(const QProtobufBaseSerializer *serializer, const QVariant &value,
                       const QProtobufPropertyOrderingInfo &fieldInfo, QByteArray &buffer)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");
    QList<QtProtobuf::int64> intList;
    for (auto enumValue : value.value<QList<T>>()) {
        intList.append(QtProtobuf::int64(enumValue));
    }
    buffer.append(serializer->serializeEnumList(intList, fieldInfo));
}

/*!
 \private
 \brief default deserializer template for type T that inherits from QProtobufMessage
 */
template<typename T,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, T>::value, int> = 0>
void deserializeObject(const QProtobufBaseSerializer *serializer, QProtobufSelfcheckIterator &it,
                       QVariant &to)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");
    Q_ASSERT_X(to.isNull() || to.metaType() == QMetaType::fromType<T *>(),
               "QProtobufBaseSerializer",
               "Property should be either uninitialized or contain a valid pointer");

    T *value = to.value<T *>();
    if (value == nullptr) {
        value = new T;
        to = QVariant::fromValue<T *>(value);
    }
    serializer->deserializeObject(value, T::propertyOrdering, it);
}

/*!
 \private
 \brief default deserializer template for list of type T objects that inherits from QProtobufMessage
 */
template<typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void deserializeList(const QProtobufBaseSerializer *serializer, QProtobufSelfcheckIterator &it,
                     QVariant &previous)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");

    V newValue;
    QProtobufBaseSerializer::Status result = QProtobufBaseSerializer::SerializationError;
    do {
        result = serializer->deserializeListObject(&newValue, V::propertyOrdering, it);
        QList<V> list = previous.value<QList<V>>();
        list.append(newValue);
        previous.setValue(list);
    } while (result == QProtobufBaseSerializer::SerializationInProgress);
}

/*!
 \private
 *
 \brief default deserializer template for map of key K, value V
 */
template<typename K, typename V,
          typename std::enable_if_t<!std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void deserializeMap(const QProtobufBaseSerializer *serializer, QProtobufSelfcheckIterator &it,
                    QVariant &previous)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");

    QHash<K, V> out = previous.value<QHash<K, V>>();
    QVariant key = QVariant::fromValue<K>(K());
    QVariant value = QVariant::fromValue<V>(V());

    if (serializer->deserializeMapPair(key, value, it) == QProtobufBaseSerializer::Serialized) {
        out[key.value<K>()] = value.value<V>();
        previous = QVariant::fromValue<QHash<K, V>>(out);
    }
}

/*!
 \private
 *
 \brief default deserializer template for map of type key K, value V. Specialization for V
        that inherits from QProtobufMessage
 */
template<typename K, typename V,
          typename std::enable_if_t<std::is_base_of<QProtobufMessage, V>::value, int> = 0>
void deserializeMap(const QProtobufBaseSerializer *serializer, QProtobufSelfcheckIterator &it,
                    QVariant &previous)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");

    auto out = previous.value<QHash<K, V>>();
    QVariant key = QVariant::fromValue<K>(K());
    QVariant value = QVariant::fromValue<V *>(nullptr);

    if (serializer->deserializeMapPair(key, value, it) == QProtobufBaseSerializer::Serialized) {
        const auto valuePtr = value.value<V *>();
        out[key.value<K>()] = valuePtr ? *valuePtr : V();
        previous = QVariant::fromValue<QHash<K, V>>(out);
    }
}

/*!
 \private
 *
 \brief default deserializer template for enum type T
 */
template<typename T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
void deserializeEnum(const QProtobufBaseSerializer *serializer, QProtobufSelfcheckIterator &it,
                     QVariant &to)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");
    QtProtobuf::int64 intValue;
    if (serializer->deserializeEnum(intValue, it))
        to = QVariant::fromValue<T>(static_cast<T>(intValue._t));
}

/*!
 \private
 *
 \brief default deserializer template for enumList type T
 */
template<typename T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
void deserializeEnumList(const QProtobufBaseSerializer *serializer, QProtobufSelfcheckIterator &it,
                         QVariant &previous)
{
    Q_ASSERT_X(serializer != nullptr, "QProtobufBaseSerializer", "Serializer is null");
    QList<QtProtobuf::int64> intList;
    if (!serializer->deserializeEnumList(intList, it))
        return;
    QList<T> enumList = previous.value<QList<T>>();
    for (auto intValue : intList)
        enumList.append(static_cast<T>(intValue._t));
    previous = QVariant::fromValue<QList<T>>(enumList);
}
} // namespace QtProtobufPrivate

template<typename T>
inline void qRegisterProtobufType()
{
    T::registerTypes();
    QtProtobufPrivate::registerOrdering(QMetaType::fromType<T>(), T::propertyOrdering);
    QtProtobufPrivate::registerHandler(
            QMetaType::fromType<T *>(),
            { QtProtobufPrivate::serializeObject<T>, QtProtobufPrivate::deserializeObject<T> });
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
#endif // QPROTOBUFSBASEERIALIZER_H
