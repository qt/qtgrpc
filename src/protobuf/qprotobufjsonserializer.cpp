// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtProtobuf/qprotobufjsonserializer.h>

#include <QtProtobuf/private/qprotobufregistration_p.h>
#include <QtProtobuf/private/qprotobufserializer_p.h>
#include <QtProtobuf/private/qtprotobufdefs_p.h>
#include <QtProtobuf/private/qtprotobufserializerhelpers_p.h>

#include <QtCore/qcoreapplication.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qhash.h>
#include <QtCore/private/qnumeric_p.h>

#include <cmath>
#include <limits>
#include <map>
#include <type_traits>

QT_BEGIN_NAMESPACE

/*!
    \class QProtobufJsonSerializer
    \inmodule QtProtobuf
    \since 6.7
    \brief The QProtobufJsonSerializer class is an interface that represents
           basic functions for serialization/deserialization of QProtobufMessage
           objects to JSON.

    The QProtobufJsonSerializer class registers serializers/deserializers for
    classes implementing a protobuf message, inheriting \l QProtobufMessage. These
    classes are generated automatically, based on a \c{.proto} file, using the CMake
    function \l qt_add_protobuf or by running
    \l {The qtprotobufgen Tool} {qtprotobufgen} directly.
*/

using namespace Qt::StringLiterals;
using namespace QtProtobufPrivate;

namespace {

inline QString convertJsonKeyToJsonName(QStringView name)
{
    QString result;
    result.reserve(name.size());
    bool nextUpperCase = false;
    for (const auto &c : name) {
        if (c == QChar::fromLatin1('_')) {
            nextUpperCase = true;
            continue;
        }
        result.append(nextUpperCase ? c.toUpper() : c);
        nextUpperCase = false;
    }
    return result;
}

}

class QProtobufJsonSerializerPrivate final
{
    Q_DISABLE_COPY_MOVE(QProtobufJsonSerializerPrivate)

    // Tests if V is JSON compatible integer
    // int32 | int64 | uint32 | sint32 | sint64 | fixed32 | sfixed32 | sfixed64
    template <typename V>
    struct IsJsonInt
        : std::integral_constant<
              bool,
              std::is_same_v<V, QtProtobuf::int32> || std::is_same_v<V, QtProtobuf::int64>
                  || std::is_same_v<V, QtProtobuf::uint32> || std::is_same_v<V, QtProtobuf::sint32>
                  || std::is_same_v<V, QtProtobuf::sint64> || std::is_same_v<V, QtProtobuf::fixed32>
                  || std::is_same_v<V, QtProtobuf::sfixed32>
                  || std::is_same_v<V, QtProtobuf::sfixed64>>
    {
    };

    // Tests if V is JSON incompatible 64-bit unsigned integer
    // uint64 | fixed64
    template <typename V>
    struct IsJsonInt64
        : std::integral_constant<
              bool, std::is_same_v<V, QtProtobuf::uint64> || std::is_same_v<V, QtProtobuf::fixed64>>
    {
    };

    // Tests if V is JSON compatible floating point number
    // float | double
    template <typename V>
    struct IsJsonFloatingPoint
        : std::integral_constant<bool, std::is_same_v<V, float> || std::is_same_v<V, double>>
    {
    };

public:
    using Serializer = std::function<QJsonValue(const QVariant &)>;
    using Deserializer = std::function<QVariant(const QJsonValue&, bool &ok)>;

    struct SerializationHandlers {
        // serializer assigned to class
        Serializer serializer;
        // deserializer assigned to class
        Deserializer deserializer;
        QProtobufSerializerPrivate::IsPresentChecker isPresent;
    };

    template <typename T>
    static SerializationHandlers createCommonHandler()
    {
        return { QProtobufJsonSerializerPrivate::serializeCommon<T>,
                 QProtobufJsonSerializerPrivate::deserializeCommon<T>,
                 QProtobufSerializerPrivate::isPresent<T> };
    }

    template <typename L, typename T>
    static SerializationHandlers createCommonListHandler()
    {
        return { QProtobufJsonSerializerPrivate::serializeList<L>,
                 QProtobufJsonSerializerPrivate::deserializeList<L, T>,
                 QProtobufSerializerPrivate::isPresent<L> };
    }

    template<typename T,
              std::enable_if_t<std::is_same_v<T, float> || std::is_same_v<T, double>, bool> = true>
    static bool isPresent(const QVariant &value)
    {
        const T val = value.value<T>();
        return val != 0 || std::signbit(val);
    }

    using SerializerRegistry = QHash<int/*metatypeid*/, SerializationHandlers>;

    template <typename T>
    static QJsonValue serializeCommon(const QVariant &propertyValue)
    {
        return serialize(propertyValue.value<T>());
    }

    template <typename T, std::enable_if_t<IsJsonInt<T>::value, bool> = true>
    static QJsonValue serialize(T propertyValue)
    {
        return QJsonValue(qint64(propertyValue));
    }

    template <typename T, std::enable_if_t<IsJsonInt64<T>::value, bool> = true>
    static QJsonValue serialize(T propertyValue)
    {
        return QJsonValue(QString::number(propertyValue));
    }

    template <typename T, std::enable_if_t<IsJsonFloatingPoint<T>::value, bool> = true>
    static QJsonValue serialize(T propertyValue)
    {
        if (propertyValue == -std::numeric_limits<T>::infinity())
            return QJsonValue("-infinity"_L1);

        if (propertyValue == std::numeric_limits<T>::infinity())
            return QJsonValue("infinity"_L1);

        if (propertyValue != propertyValue)
            return QJsonValue("nan"_L1);

        return QJsonValue(propertyValue);
    }

    static QJsonValue serialize(bool propertyValue) { return QJsonValue(propertyValue); }

    static QJsonValue serialize(const QString &propertyValue) { return QJsonValue(propertyValue); }

    static QJsonValue serialize(const QByteArray &propertyValue)
    {
        return QJsonValue(QString::fromUtf8(propertyValue.toBase64()));
    }

    template <typename L>
    static QJsonValue serializeList(const QVariant &propertyValue)
    {
        QJsonArray arr;
        L listValue = propertyValue.value<L>();
        for (const auto &value : listValue) {
            arr.append(serialize(value));
        }
        return QJsonValue(arr);
    }

    QProtobufJsonSerializerPrivate()
    {
        [[maybe_unused]] static bool initialized = []() -> bool {
            handlers[qMetaTypeId<QtProtobuf::int32>()] = createCommonHandler<QtProtobuf::int32>();
            handlers[qMetaTypeId<QtProtobuf::sfixed32>()] = createCommonHandler<
                QtProtobuf::sfixed32>();
            handlers[qMetaTypeId<QtProtobuf::sint32>()] = createCommonHandler<QtProtobuf::sint32>();
            handlers[qMetaTypeId<QtProtobuf::uint32>()] = createCommonHandler<QtProtobuf::uint32>();
            handlers[qMetaTypeId<QtProtobuf::fixed32>()] = createCommonHandler<
                QtProtobuf::fixed32>();
            handlers[qMetaTypeId<QtProtobuf::sint64>()] = createCommonHandler<QtProtobuf::sint64>();
            handlers[qMetaTypeId<QtProtobuf::int64>()] = createCommonHandler<QtProtobuf::int64>();
            handlers[qMetaTypeId<QtProtobuf::sfixed64>()] = createCommonHandler<
                QtProtobuf::sfixed64>();
            handlers[qMetaTypeId<QtProtobuf::uint64>()] = createCommonHandler<QtProtobuf::uint64>();
            handlers[qMetaTypeId<QtProtobuf::fixed64>()] = createCommonHandler<
                QtProtobuf::fixed64>();
            handlers[qMetaTypeId<bool>()] = createCommonHandler<bool>();
            handlers[QMetaType::QString] = createCommonHandler<QString>();
            handlers[QMetaType::QByteArray] = createCommonHandler<QByteArray>();
            handlers[QMetaType::Float] = { QProtobufJsonSerializerPrivate::serializeCommon<float>,
                                           QProtobufJsonSerializerPrivate::deserializeCommon<float>,
                                           QProtobufJsonSerializerPrivate::isPresent<float> };
            handlers[QMetaType::Double] = {
                QProtobufJsonSerializerPrivate::serializeCommon<double>,
                QProtobufJsonSerializerPrivate::deserializeCommon<double>,
                QProtobufJsonSerializerPrivate::isPresent<double>
            };

            handlers[qMetaTypeId<QtProtobuf::boolList>()] = createCommonListHandler<
                QtProtobuf::boolList, bool>();
            handlers[qMetaTypeId<QtProtobuf::int32List>()] = createCommonListHandler<
                QtProtobuf::int32List, QtProtobuf::int32>();
            handlers[qMetaTypeId<QtProtobuf::int64List>()] = createCommonListHandler<
                QtProtobuf::int64List, QtProtobuf::int64>();
            handlers[qMetaTypeId<QtProtobuf::sint32List>()] = createCommonListHandler<
                QtProtobuf::sint32List, QtProtobuf::sint32>();
            handlers[qMetaTypeId<QtProtobuf::sint64List>()] = createCommonListHandler<
                QtProtobuf::sint64List, QtProtobuf::sint64>();
            handlers[qMetaTypeId<QtProtobuf::uint32List>()] = createCommonListHandler<
                QtProtobuf::uint32List, QtProtobuf::uint32>();
            handlers[qMetaTypeId<QtProtobuf::uint64List>()] = createCommonListHandler<
                QtProtobuf::uint64List, QtProtobuf::uint64>();
            handlers[qMetaTypeId<QtProtobuf::fixed32List>()] = createCommonListHandler<
                QtProtobuf::fixed32List, QtProtobuf::fixed32>();
            handlers[qMetaTypeId<QtProtobuf::fixed64List>()] = createCommonListHandler<
                QtProtobuf::fixed64List, QtProtobuf::fixed64>();
            handlers[qMetaTypeId<QtProtobuf::sfixed32List>()] = createCommonListHandler<
                QtProtobuf::sfixed32List, QtProtobuf::sfixed32>();
            handlers[qMetaTypeId<QtProtobuf::sfixed64List>()] = createCommonListHandler<
                QtProtobuf::sfixed64List, QtProtobuf::sfixed64>();
            handlers[qMetaTypeId<QtProtobuf::floatList>()] = createCommonListHandler<
                QtProtobuf::floatList, float>();
            handlers[qMetaTypeId<QtProtobuf::doubleList>()] = createCommonListHandler<
                QtProtobuf::doubleList, double>();
            handlers[qMetaTypeId<QStringList>()] = createCommonListHandler<QStringList, QString>();
            handlers[qMetaTypeId<QByteArrayList>()] = createCommonListHandler<QByteArrayList,
                                                                              QByteArray>();
            return true;
        }();
    }
    ~QProtobufJsonSerializerPrivate() = default;

    [[nodiscard]] static QMetaEnum getMetaEnum(QMetaType enumMetaType)
    {
        const auto *metaObject = enumMetaType.metaObject();
        Q_ASSERT(metaObject);
        for (int i = 0; i < metaObject->enumeratorCount(); ++i) {
            if (metaObject->enumerator(i).metaType() == enumMetaType)
                return metaObject->enumerator(i);
        }
        return {};
    }

    void serializeProperty(QVariant propertyValue, const QProtobufFieldInfo &fieldInfo)
    {
        QMetaType metaType = propertyValue.metaType();
        auto userType = propertyValue.userType();

        if (metaType.id() == QMetaType::UnknownType || propertyValue.isNull())
            return;

        if (metaType.flags() & QMetaType::IsPointer) {
            auto store = activeValue.toObject();
            activeValue = QJsonObject();
            auto *messageProperty = propertyValue.value<QProtobufMessage *>();
            serializeObjectImpl(messageProperty);
            store.insert(fieldInfo.jsonName().toString(), activeValue);
            activeValue = store;
            return;
        }

        const auto fieldFlags = fieldInfo.fieldFlags();

        if (propertyValue.canView(QMetaType::fromType<QProtobufRepeatedIterator>())) {
            QProtobufRepeatedIterator propertyIt = propertyValue.view<QProtobufRepeatedIterator>();
            if (fieldFlags & QtProtobufPrivate::FieldFlag::Repeated
                && !(fieldFlags & QtProtobufPrivate::FieldFlag::Enum)) {
                const auto fieldName = fieldInfo.jsonName().toString();
                QJsonObject activeObject = activeValue.toObject();
                activeValue = activeObject.value(fieldName).toArray();
                while (propertyIt.hasNext())
                    serializeObject(propertyIt.next(), fieldInfo);
                if (!activeValue.toArray().empty())
                    activeObject.insert(fieldName, activeValue);
                activeValue = activeObject;
            } else {
                while (propertyIt.hasNext())
                    serializeObject(propertyIt.next(), fieldInfo);
            }
            return;
        }

        if (fieldFlags & QtProtobufPrivate::FieldFlag::Enum) {
            QJsonObject activeObject = activeValue.toObject();
            if (fieldFlags & QtProtobufPrivate::FieldFlag::Repeated) {
                const auto stringList = propertyValue.value<QStringList>();
                if (!stringList.isEmpty()) {
                    activeObject.insert(fieldInfo.jsonName().toString(),
                                        QProtobufJsonSerializerPrivate::serializeList<
                                            QStringList>(stringList));
                }
            } else {
                const auto metaEnum = getMetaEnum(metaType);
                Q_ASSERT(metaEnum.isValid());
                activeObject
                    .insert(fieldInfo.jsonName().toString(),
                            QString::fromUtf8(metaEnum.key(propertyValue.value<int32_t>())));
            }
            activeValue = activeObject;
            return;
        }

        auto handler = QtProtobufPrivate::findHandler(metaType);
        if (handler.serializer) {
            handler.serializer([this](const QProtobufMessage *message,
                                      const QProtobufFieldInfo &
                                          fieldInfo) { this->serializeObject(message, fieldInfo); },
                               propertyValue.constData(), fieldInfo);
        } else {
            QJsonObject activeObject = activeValue.toObject();
            auto iter = handlers.constFind(userType);
            if (iter == handlers.constEnd())
                return;
            const auto &handler = iter.value();
            if (!handler.isPresent(propertyValue) && !isOneofOrOptionalField(fieldInfo))
                return;

            activeObject.insert(fieldInfo.jsonName().toString(),
                                handler.serializer
                                    ? handler.serializer(propertyValue)
                                    : QJsonValue::fromVariant(propertyValue));
            activeValue = activeObject;
        }
    }

    void serializeObject(const QProtobufMessage *message, const QProtobufFieldInfo &fieldInfo);
    void serializeObjectImpl(const QProtobufMessage *message);

    template <typename T>
    static QVariant deserializeCommon(const QJsonValue &value, bool &ok)
    {
        ok = false;
        return QVariant::fromValue<T>(deserialize<T>(value, ok));
    }

    template <typename T, std::enable_if_t<IsJsonInt<T>::value, bool> = true>
    static T deserialize(const QJsonValue &value, bool &ok)
    {
        auto variantValue = value.toVariant();
        qint64 raw = 0;
        switch (variantValue.metaType().id()) {
        case QMetaType::QString: // TODO: check if string has prepending/appending whitespaces.
        case QMetaType::LongLong:
            raw = variantValue.toLongLong(&ok);
            break;
        case QMetaType::Double: {
            double d = value.toDouble();
            ok = convertDoubleTo(d, &raw) && double(raw) == d;
        } break;
        default:
            break;
        }

        // For types that "smaller" than qint64 we need to check if the value fits its limits range
        if constexpr (sizeof(T) != sizeof(qint64)) {
            if (ok) {
                if constexpr (std::is_same_v<T, QtProtobuf::sfixed32>
                              || std::is_same_v<T, QtProtobuf::int32>) {
                    using limits = std::numeric_limits<qint32>;
                    ok = raw >= limits::min() && raw <= limits::max();
                } else if constexpr (std::is_same_v<T, QtProtobuf::fixed32>) {
                    using limits = std::numeric_limits<quint32>;
                    ok = raw >= limits::min() && raw <= limits::max();
                } else {
                    using limits = std::numeric_limits<T>;
                    ok = raw >= limits::min() && raw <= limits::max();
                }
            }
        }

        return T(raw);
    }

    template <typename T, std::enable_if_t<IsJsonInt64<T>::value, bool> = true>
    static T deserialize(const QJsonValue &value, bool &ok)
    {
        quint64 raw = 0;
        auto variantValue = value.toVariant();
        switch (variantValue.metaType().id()) {
        case QMetaType::QString:
        case QMetaType::LongLong:
            // Here we attempt converting the value to ULongLong
            raw = variantValue.toULongLong(&ok);
            break;
        case QMetaType::Double: {
            double d = value.toDouble();
            ok = convertDoubleTo(d, &raw) && double(raw) == d;
        } break;
        default:
            break;
        }
        return T(raw);
    }

    template <typename T, std::enable_if_t<IsJsonFloatingPoint<T>::value, bool> = true>
    static T deserialize(const QJsonValue &value, bool &ok)
    {
        ok = true;
        QByteArray data = value.toVariant().toByteArray();
        if (data.toLower() == "-infinity"_ba)
            return -std::numeric_limits<T>::infinity();

        if (data.toLower() == "infinity"_ba)
            return std::numeric_limits<T>::infinity();

        if (data.toLower() == "nan"_ba)
            return T(NAN);

        if constexpr (std::is_same_v<T, float>)
            return data.toFloat(&ok);
        else
            return data.toDouble(&ok);
    }

    template <typename T, std::enable_if_t<std::is_same_v<T, bool>, bool> = true>
    static bool deserialize(const QJsonValue &value, bool &ok)
    {
        if (value.isBool()) {
            ok = true;
            return value.toBool();
        } else if (value.isString()) {
            if (value.toString() == "true"_L1) {
                ok = true;
                return true;
            } else if (value.toString() == "false"_L1) {
                ok = true;
                return false;
            }
        }
        return false;
    }

    template <typename T, std::enable_if_t<std::is_same_v<T, QString>, bool> = true>
    static QString deserialize(const QJsonValue &value, bool &ok)
    {
        ok = value.isString();
        return value.toString();
    }

    template <typename T, std::enable_if_t<std::is_same_v<T, QByteArray>, bool> = true>
    static QByteArray deserialize(const QJsonValue &value, bool &ok)
    {
        QByteArray data = value.toVariant().toByteArray();
        if (value.isString()) {
            ok = true;
            return QByteArray::fromBase64(data);
        }
        return {};
    }

    template <typename L /*list*/, typename T /*element*/>
    static QVariant deserializeList(const QJsonValue &value, bool &ok)
    {
        if (!value.isArray()) {
            ok = false;
            return {};
        }

        L list;
        QJsonArray array = value.toArray();
        for (auto arrayValue : array) {
            ok = false;
            T value = deserialize<T>(arrayValue, ok);
            if (!ok)
                break;
            list.append(value);
        }
        return QVariant::fromValue(list);
    }

    static QtProtobuf::int64 deserializeEnum(const QJsonValue &value, const QMetaEnum &metaEnum,
                                             bool &ok)
    {
        QtProtobuf::int64 result = 0;
        if (value.isString()) {
            QString enumKey = value.toString();
            result = metaEnum.keyToValue(enumKey.toUtf8().data(), &ok);
        }
        if (ok)
            return result;

        result = deserialize<QtProtobuf::int64>(value, ok);
        if (ok) {
            ok = false;
            // Make sure that it's the known enum value
            for (int i = 0; i < metaEnum.keyCount(); ++i) {
                if (metaEnum.value(i) == result) {
                    ok = true;
                    break;
                }
            }
        }

        return result;
    }

    QVariant deserializeValue(QVariant propertyData, bool &ok)
    {
        ok = false;
        auto metaType = propertyData.metaType();
        if (metaType.flags() & QMetaType::IsPointer) {
            auto *messageProperty = propertyData.value<QProtobufMessage *>();
            Q_ASSERT(messageProperty != nullptr);
            ok = deserializeObject(messageProperty);
            return propertyData;
        }

        if (propertyData.canView(QMetaType::fromType<QProtobufRepeatedIterator>())) {
            QProtobufRepeatedIterator propertyIt = propertyData.view<QProtobufRepeatedIterator>();
            if (activeValue.isArray()) {
                QJsonArray array = activeValue.toArray();
                if (array.isEmpty()) {
                    ok = true;
                    activeValue = {};
                    return propertyData;
                }

                while (!array.isEmpty()
                       && lastError == QAbstractProtobufSerializer::Error::None) {
                    activeValue = array.takeAt(0);
                    if (deserializeObject(propertyIt.addNext()))
                        propertyIt.push();
                }
                ok = propertyData.isValid();
            } else {
                while (!activeValue.isNull()
                       && lastError == QAbstractProtobufSerializer::Error::None) {
                    if (deserializeObject(propertyIt.addNext()))
                        propertyIt.push();
                }
            }
            ok = lastError == QAbstractProtobufSerializer::Error::None;
            return propertyData;
        }

        auto handler = QtProtobufPrivate::findHandler(metaType);
        if (handler.deserializer) {
            while (!activeValue.isNull()
                   && lastError == QAbstractProtobufSerializer::Error::None) {
                handler
                    .deserializer([this](QProtobufMessage
                                             *message) { return this->deserializeObject(message); },
                                  propertyData.data());
            }
            ok = propertyData.isValid();
        } else {
            int userType = propertyData.userType();
            auto handler = handlers.constFind(userType);
            if (handler != handlers.constEnd() && handler.value().deserializer) {
                propertyData = handler.value().deserializer(activeValue, ok);
                if (!ok)
                    setInvalidFormatError();
            } else {
                setDeserializationError(QAbstractProtobufSerializer::Error::UnknownType,
                                        QCoreApplication::
                                            translate("QtProtobuf",
                                                      "No deserializer is registered for type %1")
                                                .arg(userType));
            }
        }
        return propertyData;
    }

    bool deserializeObject(QProtobufMessage *message)
    {
        Q_ASSERT(message != nullptr);

        auto restoreOnReturn = qScopeGuard([prevCachedPropertyValue = cachedPropertyValue,
                                            prevCachedIndex = cachedIndex, this]() {
            cachedPropertyValue = prevCachedPropertyValue;
            cachedIndex = prevCachedIndex;
        });

        cachedPropertyValue.clear();
        cachedIndex = -1;

        auto ordering = message->propertyOrdering();
        Q_ASSERT(ordering != nullptr);

        std::map<QString, QProtobufFieldInfo> msgContainer; // map<key, fieldInfo>
        for (int index = 0; index < ordering->fieldCount(); ++index) {
            int fieldIndex = ordering->fieldNumber(index);
            Q_ASSERT_X(fieldIndex <= ProtobufFieldNumMax && fieldIndex >= ProtobufFieldNumMin, "",
                       "fieldIndex is out of range");
            QProtobufFieldInfo fieldInfo(*ordering, index);
            QString key = fieldInfo.jsonName().toString();
            msgContainer.insert(std::pair<QString, QProtobufFieldInfo>(key, fieldInfo));
        }

        if (!activeValue.isObject()) {
            setInvalidFormatError();
            activeValue = {};
            return false;
        }
        QJsonObject activeObject = activeValue.toObject();
        // Go through QJSON doc and find keys that are presented in msgContainer
        for (auto &key : activeObject.keys()) {
            if (activeObject.value(key).isNull())
                continue;

            std::map<QString, QProtobufFieldInfo>::iterator iter = msgContainer
                                                                                  .find(key);
            if (iter == msgContainer.end())
                iter = msgContainer.find(convertJsonKeyToJsonName(key));

            if (iter != msgContainer.end()) {
                auto store = activeValue;
                activeValue = activeObject.value(key);

                if (auto index = ordering->indexOfFieldNumber(iter->second.fieldNumber());
                    index != cachedIndex) {
                    if (!storeCachedValue(message))
                        return false;
                    cachedPropertyValue = QtProtobufSerializerHelpers::messageProperty(message,
                                                                                       iter->second,
                                                                                       true);
                    cachedIndex = index;
                }

                bool ok = false;

                const auto fieldFlags = iter->second.fieldFlags();
                if (fieldFlags & QtProtobufPrivate::FieldFlag::Enum) {
                    if (fieldFlags & QtProtobufPrivate::FieldFlag::Repeated) {
                        QMetaType originalMetatype = cachedPropertyValue.metaType();
                        cachedPropertyValue
                            .setValue(deserializeList<QStringList, QString>(activeValue, ok));
                        if (ok)
                            ok = cachedPropertyValue.convert(originalMetatype);
                    } else {
                        const auto metaEnum = getMetaEnum(cachedPropertyValue.metaType());
                        Q_ASSERT(metaEnum.isValid());
                        cachedPropertyValue.setValue(deserializeEnum(activeValue, metaEnum, ok));
                    }
                    if (!ok)
                        setInvalidFormatError();
                } else if (fieldFlags & QtProtobufPrivate::FieldFlag::Map) {
                    auto activeValueObj = activeValue.toObject();
                    for (auto it = activeValueObj.begin(); it != activeValueObj.end(); ++it) {
                        auto mapObj = QJsonObject{};
                        mapObj.insert("key"_L1, it.key());
                        mapObj.insert("value"_L1, it.value());
                        activeValue = mapObj;
                        cachedPropertyValue = deserializeValue(cachedPropertyValue, ok);
                    }
                    activeValue = store;
                } else {
                    cachedPropertyValue = deserializeValue(cachedPropertyValue, ok);
                    activeValue = store;
                }
                if (!ok) {
                    cachedPropertyValue.clear();
                    cachedIndex = -1;
                }
            }
        }

        // Once all keys are deserialized we assume that activeValue is empty, nothing left
        // to deserialize
        activeValue = {};

        return storeCachedValue(message);
    }

    void setDeserializationError(QAbstractProtobufSerializer::Error error,
                                 const QString &errorString)
    {
        lastError = error;
        lastErrorString = errorString;
    }

    void setUnexpectedEndOfStreamError()
    {
        setDeserializationError(QAbstractProtobufSerializer::Error::UnexpectedEndOfStream,
                                QCoreApplication::translate("QtProtobuf",
                                                            "JSON: Unexpected end of stream"));
    }

    void setInvalidFormatError()
    {
        setDeserializationError(QAbstractProtobufSerializer::Error::InvalidFormat,
                                QCoreApplication::
                                    translate("QtProtobuf",
                                              "JSON: One or more fields have invalid format"));
    }

    void clearError();

    QAbstractProtobufSerializer::Error lastError = QAbstractProtobufSerializer::Error::None;
    QString lastErrorString;
    QJsonValue activeValue;

    static SerializerRegistry handlers;

    [[nodiscard]] bool storeCachedValue(QProtobufMessage *message);

    QVariant cachedPropertyValue;
    int cachedIndex = -1;
};

bool QProtobufJsonSerializerPrivate::storeCachedValue(QProtobufMessage *message)
{
    bool ok = true;
    if (cachedIndex >= 0 && !cachedPropertyValue.isNull()) {
        const auto *ordering = message->propertyOrdering();
        QProtobufFieldInfo fieldInfo(*ordering, cachedIndex);
        ok = QtProtobufSerializerHelpers::setMessageProperty(message, fieldInfo,
                                                             cachedPropertyValue);
        cachedPropertyValue.clear();
        cachedIndex = -1;
    }
    return ok;
}

QProtobufJsonSerializerPrivate::SerializerRegistry QProtobufJsonSerializerPrivate::handlers = {};

void QProtobufJsonSerializerPrivate::serializeObjectImpl(const QProtobufMessage *message)
{
    // if a message is not initialized, just return empty { }
    if (message) {
        auto ordering = message->propertyOrdering();
        Q_ASSERT(ordering != nullptr);
        for (int index = 0; index < ordering->fieldCount(); ++index) {
            int fieldIndex = ordering->fieldNumber(index);
            Q_ASSERT_X(fieldIndex <= ProtobufFieldNumMax && fieldIndex >= ProtobufFieldNumMin, "",
                       "fieldIndex is out of range");
            QProtobufFieldInfo fieldInfo(*ordering, index);
            QVariant propertyValue = QtProtobufSerializerHelpers::messageProperty(message,
                                                                                  fieldInfo);
            serializeProperty(propertyValue, fieldInfo);
        }
    }
}

void QProtobufJsonSerializerPrivate::clearError()
{
    lastError = QAbstractProtobufSerializer::Error::None;
    lastErrorString.clear();
}

QProtobufJsonSerializer::QProtobufJsonSerializer() : d_ptr(new QProtobufJsonSerializerPrivate)
{
}

QProtobufJsonSerializer::~QProtobufJsonSerializer() = default;

/*!
   Returns the last deserialization error for the serializer instance.
   \sa lastErrorString()
*/
QAbstractProtobufSerializer::Error QProtobufJsonSerializer::lastError() const
{
    return d_ptr->lastError;
}

/*!
   Returns the last deserialization error string for the serializer instance.
   \sa lastError()
*/
QString QProtobufJsonSerializer::lastErrorString() const
{
    return d_ptr->lastErrorString;
}

QByteArray QProtobufJsonSerializer::serializeMessage(const QProtobufMessage *message) const
{
    d_ptr->clearError();
    d_ptr->activeValue = QJsonObject();
    d_ptr->serializeObjectImpl(message);
    QJsonDocument doc;
    doc.setObject(d_ptr->activeValue.toObject());
    d_ptr->activeValue = QJsonObject();
    return doc.toJson(QJsonDocument::Compact);
}

bool QProtobufJsonSerializer::deserializeMessage(QProtobufMessage *message,
                                                 QByteArrayView data) const
{
    d_ptr->clearError();
    QJsonParseError err;
    auto document = QJsonDocument::fromJson(data.toByteArray(), &err);
    if (err.error != QJsonParseError::NoError) {
        d_ptr->setUnexpectedEndOfStreamError();
        return false;
    }

    if (!document.isObject()) {
        d_ptr->setInvalidFormatError();
        return false;
    }
    d_ptr->activeValue = document.object();

    d_ptr->cachedPropertyValue.clear();
    d_ptr->cachedIndex = -1;
    return d_ptr->deserializeObject(message);
}

void QProtobufJsonSerializerPrivate::serializeObject(const QProtobufMessage *message,
                                                     const QProtobufFieldInfo &fieldInfo)
{
    if (activeValue.isArray()) {
        auto store = activeValue.toArray();
        activeValue = QJsonObject();
        serializeObjectImpl(message);
        store.append(activeValue);
        activeValue = store;
    } else {
        auto store = activeValue.toObject();
        const QString fieldName = fieldInfo.jsonName().toString();
        activeValue = QJsonObject();
        if (fieldInfo.fieldFlags() & QtProtobufPrivate::FieldFlag::Map) {
            QJsonObject mapObject = store.value(fieldName).toObject();
            serializeObjectImpl(message);
            mapObject.insert(message->property("key").toString(),
                             activeValue.toObject().value("value"_L1));
            store.insert(fieldName, mapObject);
        } else {
            serializeObjectImpl(message);
            store.insert(fieldName, activeValue);
        }
        activeValue = store;
    }
}

QT_END_NAMESPACE
