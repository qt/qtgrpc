// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFREGISTRATION_H
#define QPROTOBUFREGISTRATION_H

#if 0
#  pragma qt_sync_skip_header_check
#  pragma qt_sync_stop_processing
#endif

#include <QtProtobuf/qtprotobufexports.h>

#include <QtProtobuf/qprotobufpropertyordering.h>
#include <QtProtobuf/qprotobufrepeatediterator.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/qhash.h>
#include <QtCore/qlist.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qmetatype.h>

QT_BEGIN_NAMESPACE

class QProtobufMessage;

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
        intList.append(QtProtobuf::int64(qToUnderlying(enumValue)));

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
        stringList.append(QString::fromUtf8(metaEnum.valueToKey(qToUnderlying(enumValue))));

    return stringList;
}

class Q_PROTOBUF_EXPORT AbstractRepeatedIterator
{
public:
    virtual ~AbstractRepeatedIterator() noexcept;

    virtual bool hasNext() const noexcept = 0;
    virtual QProtobufMessage &next() const noexcept = 0;

    virtual QProtobufMessage &addNext() noexcept = 0;
    virtual void push() noexcept = 0;
};

template <typename T, QtProtobuf::if_protobuf_message<T> = true>
class ListIterator : public AbstractRepeatedIterator
{
public:
    ~ListIterator() noexcept override = default;

    bool hasNext() const noexcept override { return m_it != m_list->end(); }
    QProtobufMessage &next() const noexcept override
    {
        Q_ASSERT(m_it != m_list->end());
        return *m_it++;
    }

    QProtobufMessage &addNext() noexcept override { return m_list->emplace_back(T()); }
    /* protobuf allows having elements in the repeated fields that are failed to deserialize */
    void push() noexcept override { }

    static QProtobufRepeatedIterator fromList(QList<T> &list) noexcept
    {
        return QProtobufRepeatedIterator(new ListIterator<T>(list));
    }

private:
    Q_DISABLE_COPY_MOVE(ListIterator)

    explicit ListIterator(QList<T> &list) : m_list(&list), m_it(m_list->begin()) { }
    QList<T> *m_list = nullptr;
    mutable typename QList<T>::Iterator m_it;
};

template <typename K, typename V, QtProtobuf::if_protobuf_map<K, V> = true>
class MapIterator : public AbstractRepeatedIterator
{
    using MapEntry = QProtobufMapEntry<K, V>;
    static_assert(!std::is_pointer_v<typename MapEntry::KeyType>, "Map key must not be message");

public:
    ~MapIterator() noexcept override = default;

    bool hasNext() const noexcept override { return m_it != m_hash->end(); }
    QProtobufMessage &next() const noexcept override
    {
        Q_ASSERT(m_it != m_hash->end());
        m_mapEntry.setKey(m_it.key());
        if constexpr (std::is_pointer_v<typename MapEntry::ValueType>)
            m_mapEntry.setValue(&m_it.value());
        else
            m_mapEntry.setValue(m_it.value());
        m_it++;
        return m_mapEntry;
    }

    QProtobufMessage &addNext() noexcept override
    {
        m_mapEntry.setKey({});
        m_mapEntry.setValue({});
        return m_mapEntry;
    }
    void push() noexcept override
    {
        auto it = m_hash->emplace(m_mapEntry.key());
        if constexpr (std::is_pointer_v<typename MapEntry::ValueType>)
            *it = std::move(*m_mapEntry.value());
        else
            *it = std::move(m_mapEntry).value();
    }

    static QProtobufRepeatedIterator fromHash(QHash<K, V> &hash) noexcept
    {
        return QProtobufRepeatedIterator(new MapIterator<K, V>(hash));
    }

private:
    Q_DISABLE_COPY_MOVE(MapIterator)

    explicit MapIterator(QHash<K, V> &hash) : m_hash(&hash), m_it(m_hash->begin()) { }
    QHash<K, V> *m_hash = nullptr;
    mutable typename QHash<K, V>::Iterator m_it;
    mutable MapEntry m_mapEntry;
};

} // namespace QtProtobufPrivate

Q_PROTOBUF_EXPORT void qRegisterProtobufTypes();

template<typename T, QtProtobuf::if_protobuf_message<T> = true>
inline void qRegisterProtobufType()
{
    T::registerTypes();
    QMetaType::registerMutableView<
        QList<T>, QProtobufRepeatedIterator>(&QtProtobufPrivate::ListIterator<T>::fromList);
    QtProtobufPrivate::registerOrdering(QMetaType::fromType<T>(), T::staticPropertyOrdering);
}

template <typename K, typename V, QtProtobuf::if_protobuf_map<K, V> = true>
inline void qRegisterProtobufMapType()
{
    QMetaType::registerMutableView<
        QHash<K, V>, QProtobufRepeatedIterator>(&QtProtobufPrivate::MapIterator<K, V>::fromHash);
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
