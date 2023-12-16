// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qprotobufjsonserializer.h"
#include "qprotobufserializer_p.h"
#include "qprotobufmessage_p.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QVariant>

#include <map>
#include <unordered_map>
#include <cmath>

QT_BEGIN_NAMESPACE

/*!
    \class QProtobufJsonSerializer
    \inmodule QtProtobuf
    \since 6.7
    \brief The QProtobufJsonSerializer class is interface that represents
           basic functions for serialization/deserialization of QProtobufMessage
           objects to Json.
    \reentrant

    The QProtobufJsonSerializer class registers serializers/deserializers for
    classes implementing a protobuf message, inheriting QProtobufMessage. These
    classes are generated automatically, based on a .proto file, using the cmake
    build macro qt6_add_protobuf or by running qtprotobufgen directly.
*/


/*!
    \fn QAbstractProtobufSerializer::DeserializationError QProtobufJsonSerializer::deserializationError() const

    Returns the last deserialization error.
*/

/*!
    \fn QString QProtobufJsonSerializer::deserializationErrorString() const

    Returns a human-readable string describing the last deserialization error.
    If there was no error, an empty string is returned.
*/

using namespace Qt::StringLiterals;
using namespace QtProtobufPrivate;

namespace {

inline bool
isOneofOrOptionalField(const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo)
{
    return fieldInfo.getFieldFlags() & QtProtobufPrivate::Oneof
        || fieldInfo.getFieldFlags() & QtProtobufPrivate::Optional;
}

}

class QProtobufJsonSerializerPrivate final
{
    Q_DISABLE_COPY_MOVE(QProtobufJsonSerializerPrivate)

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

    template<typename T,
              std::enable_if_t<std::is_same_v<T, float> || std::is_same_v<T, double>, bool> = true>
    static bool isPresent(const QVariant &value)
    {
        const T val = value.value<T>();
        return val != 0 || std::signbit(val);
    }


    // TBD Replace std::unordered_map to QHash
    using SerializerRegistry = std::unordered_map<int/*metatypeid*/, SerializationHandlers>;

    template <typename T>
    static QJsonValue serializeVarint(const QVariant &propertyValue)
    {
        return QJsonValue(qint64(propertyValue.value<T>()));
    }

    template <typename T>
    static QJsonValue serializeInt64Int(const QVariant &propertyValue)
    {
        return QJsonValue(QString::number(propertyValue.value<T>()));
    }

    static QJsonValue serializeFloat(const QVariant &propertyValue)
    {
        // TODO: this is weak approach to no have some precision loss. Not sure why this happens,
        // should be converted without the conversion to string.
        float f = propertyValue.toFloat();
        double d = QString::number(f).toDouble();
        if (d == 0 && std::signbit(f))
            d = -0.0;
        return QJsonValue(d);
    }

    static QJsonValue serializeDouble(const QVariant &propertyValue)
    {
        return QJsonValue(propertyValue.toDouble());
    }

    static QJsonValue serializeString(const QVariant &propertyValue)
    {
        return QJsonValue(propertyValue.toString());
    }

    static QJsonValue serializeBytes(const QVariant &propertyValue)
    {
        return QJsonValue(QString::fromUtf8(propertyValue.toByteArray().toBase64()));
    }

    template <typename L>
    static QJsonValue serializeList(const QVariant &propertyValue)
    {
        QJsonArray arr;
        L listValue = propertyValue.value<L>();
        for (const auto &value : listValue) {
            arr.append(QJsonValue(qint64(value)));
        }
        return QJsonValue(arr);
    }

    template <typename L>
    static QJsonValue serializeInt64bitList(const QVariant &propertyValue)
    {
        QJsonArray arr;
        L listValue = propertyValue.value<L>();
        for (const auto &value : listValue) {
            arr.append(QJsonValue(QString::number(value)));
        }
        return QJsonValue(arr);
    }

    static QJsonValue serializeBoolList(const QVariant &propertyValue)
    {
        QJsonArray arr;
        QtProtobuf::boolList listValue = propertyValue.value<QtProtobuf::boolList>();
        for (const auto &value : listValue) {
            arr.append(QJsonValue(value));
        }
        return QJsonValue(arr);
    }

    static QJsonValue serializeFloatList(const QVariant &propertyValue)
    {
        QJsonArray arr;
        QtProtobuf::floatList listValue = propertyValue.value<QtProtobuf::floatList>();

        for (const auto &value : listValue) {
            // TODO: this is weak approach to not have some precision loss. Not sure why this
            // happens, should be converted without the conversion to string.
            arr.append(QJsonValue(QString::number(value).toDouble()));
        }
        return QJsonValue(arr);
    }

    static QJsonValue serializeStringList(const QVariant &propertyValue)
    {
        QJsonArray arr;
        QStringList listValue = propertyValue.value<QStringList>();
        for (const auto &value : listValue) {
            arr.append(QJsonValue(value));
        }

        return QJsonValue(arr);
    }

    static QJsonValue serializeBytesList(const QVariant &propertyValue)
    {
        QByteArrayList listValue = propertyValue.value<QByteArrayList>();
        QJsonArray arr;
        for (const auto &value : listValue) {
            arr.append(QJsonValue(QString::fromUtf8(value.toBase64())));
        }

        return QJsonValue(arr);
    }

    static QJsonValue serializeDoubleList(const QVariant &propertyValue)
    {
        QtProtobuf::doubleList listValue = propertyValue.value<QtProtobuf::doubleList>();
        QJsonArray arr;
        for (const auto &value : listValue) {
            arr.append(QJsonValue(value));
        }
        return QJsonValue(arr);
    }

    QProtobufJsonSerializerPrivate(QProtobufJsonSerializer *q)
        : qPtr(q)
    {
        [[maybe_unused]] static bool initialized = []() -> bool {
            handlers[qMetaTypeId<QtProtobuf::int32>()] = {
                QProtobufJsonSerializerPrivate::serializeVarint<QtProtobuf::int32>,
                QProtobufJsonSerializerPrivate::deserializeInt32,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::int32>
            };
            handlers[qMetaTypeId<QtProtobuf::sfixed32>()] = {
                QProtobufJsonSerializerPrivate::serializeVarint<QtProtobuf::sfixed32>,
                QProtobufJsonSerializerPrivate::deserializeInt32,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::sfixed32>
            };
            handlers[qMetaTypeId<QtProtobuf::sint32>()] = {
                {},
                QProtobufJsonSerializerPrivate::deserializeInt32,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::sint32>
            };
            handlers[qMetaTypeId<QtProtobuf::uint32>()] = {
                {},
                QProtobufJsonSerializerPrivate::deserializeUInt32,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::uint32>
            };
            handlers[qMetaTypeId<QtProtobuf::fixed32>()] = {
                QProtobufJsonSerializerPrivate::serializeVarint<QtProtobuf::fixed32>,
                QProtobufJsonSerializerPrivate::deserializeUInt32,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::fixed32>
            };
            handlers[qMetaTypeId<QtProtobuf::sint64>()] = {
                serializeInt64Int<QtProtobuf::sint64>,
                QProtobufJsonSerializerPrivate::deserializeInt64,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::sint64>
            };
            handlers[qMetaTypeId<QtProtobuf::int64>()] = {
                serializeInt64Int<QtProtobuf::int64>,
                QProtobufJsonSerializerPrivate::deserializeInt64,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::int64>
            };
            handlers[qMetaTypeId<QtProtobuf::sfixed64>()] = {
                serializeInt64Int<QtProtobuf::sfixed64>,
                QProtobufJsonSerializerPrivate::deserializeInt64,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::sfixed64>
            };
            handlers[qMetaTypeId<QtProtobuf::uint64>()] = {
                serializeInt64Int<QtProtobuf::uint64>,
                QProtobufJsonSerializerPrivate::deserializeUInt64,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::uint64>
            };
            handlers[qMetaTypeId<QtProtobuf::fixed64>()] = {
                serializeInt64Int<QtProtobuf::fixed64>,
                QProtobufJsonSerializerPrivate::deserializeUInt64,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::fixed64>
            };
            handlers[qMetaTypeId<bool>()] = { {},
                                              QProtobufJsonSerializerPrivate::deserializeBool,
                                              QProtobufSerializerPrivate::isPresent<bool> };
            handlers[QMetaType::Float] = { QProtobufJsonSerializerPrivate::serializeFloat,
                                           QProtobufJsonSerializerPrivate::deserializeFloat,
                                           QProtobufJsonSerializerPrivate::isPresent<float> };
            handlers[QMetaType::Double] = { QProtobufJsonSerializerPrivate::serializeDouble,
                                            QProtobufJsonSerializerPrivate::deserializeDouble,
                                            QProtobufJsonSerializerPrivate::isPresent<double> };
            handlers[QMetaType::QString] = { QProtobufJsonSerializerPrivate::serializeString,
                                             QProtobufJsonSerializerPrivate::deserializeString,
                                             QProtobufSerializerPrivate::isPresent<QString> };
            handlers[QMetaType::QByteArray] = {
                QProtobufJsonSerializerPrivate::serializeBytes,
                QProtobufJsonSerializerPrivate::deserializeByteArray,
                QProtobufSerializerPrivate::isPresent<QByteArray>
            };
            handlers[qMetaTypeId<QtProtobuf::boolList>()] = {
                QProtobufJsonSerializerPrivate::serializeBoolList,
                QProtobufJsonSerializerPrivate::deserializeList<bool>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::boolList>
            };
            handlers[qMetaTypeId<QtProtobuf::int32List>()] = {
                QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::int32List>,
                QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::int32>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::int32List>
            };
            handlers[qMetaTypeId<QtProtobuf::int64List>()] = {
                QProtobufJsonSerializerPrivate::serializeInt64bitList<QtProtobuf::int64List>,
                QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::int64>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::int64List>
            };
            handlers[qMetaTypeId<QtProtobuf::sint32List>()] = {
                QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::sint32List>,
                QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::sint32>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::sint32List>
            };
            handlers[qMetaTypeId<QtProtobuf::sint64List>()] = {
                QProtobufJsonSerializerPrivate::serializeInt64bitList<QtProtobuf::sint64List>,
                QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::sint64>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::sint64List>
            };
            handlers[qMetaTypeId<QtProtobuf::uint32List>()] = {
                QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::uint32List>,
                QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::uint32>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::uint32List>
            };
            handlers[qMetaTypeId<QtProtobuf::uint64List>()] = {
                QProtobufJsonSerializerPrivate::serializeInt64bitList<QtProtobuf::uint64List>,
                QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::uint64>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::uint64List>
            };
            handlers[qMetaTypeId<QtProtobuf::fixed32List>()] = {
                QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::fixed32List>,
                QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::fixed32>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::fixed32List>
            };
            handlers[qMetaTypeId<QtProtobuf::fixed64List>()] = {
                QProtobufJsonSerializerPrivate::serializeInt64bitList<QtProtobuf::fixed64List>,
                QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::fixed64>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::fixed64List>
            };
            handlers[qMetaTypeId<QtProtobuf::sfixed32List>()] = {
                QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::sfixed32List>,
                QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::sfixed32>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::sfixed32List>
            };
            handlers[qMetaTypeId<QtProtobuf::sfixed64List>()] = {
                QProtobufJsonSerializerPrivate::serializeInt64bitList<QtProtobuf::sfixed64List>,
                QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::sfixed64>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::sfixed64List>
            };
            handlers[qMetaTypeId<QtProtobuf::floatList>()] = {
                QProtobufJsonSerializerPrivate::serializeFloatList,
                QProtobufJsonSerializerPrivate::deserializeList<float>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::floatList>
            };
            handlers[qMetaTypeId<QtProtobuf::doubleList>()] = {
                QProtobufJsonSerializerPrivate::serializeDoubleList,
                QProtobufJsonSerializerPrivate::deserializeList<double>,
                QProtobufSerializerPrivate::isPresent<QtProtobuf::doubleList>
            };
            handlers[qMetaTypeId<QStringList>()] = {
                QProtobufJsonSerializerPrivate::serializeStringList,
                QProtobufJsonSerializerPrivate::deserializeStringList,
                QProtobufSerializerPrivate::isPresent<QStringList>
            };
            handlers[qMetaTypeId<QByteArrayList>()] = {
                QProtobufJsonSerializerPrivate::serializeBytesList,
                QProtobufJsonSerializerPrivate::deserializeList<QByteArray>,
                QProtobufSerializerPrivate::isPresent<QByteArrayList>
            };
            return true;
        }();
    }
    ~QProtobufJsonSerializerPrivate() = default;

    void serializeProperty(const QVariant &propertyValue,
                           const QProtobufPropertyOrderingInfo &fieldInfo)
    {
        QMetaType metaType = propertyValue.metaType();
        auto userType = propertyValue.userType();

        if (metaType.id() == QMetaType::UnknownType || propertyValue.isNull())
            return;

        auto handler = QtProtobufPrivate::findHandler(metaType);
        if (handler.serializer) {
            handler.serializer(qPtr, propertyValue, fieldInfo);
        } else {
            QJsonObject activeObject = activeValue.toObject();
            auto handler = handlers.find(userType);
            if (handler == handlers.end())
                return;

            if (!handler->second.isPresent(propertyValue) && !isOneofOrOptionalField(fieldInfo))
                return;

            activeObject.insert(fieldInfo.getJsonName().toString(),
                                handler->second.serializer
                                    ? handler->second.serializer(propertyValue)
                                    : QJsonValue::fromVariant(propertyValue));
            activeValue = activeObject;
        }
    }

    void serializeObject(const QProtobufMessage *message, const QProtobufPropertyOrdering &ordering)
    {
        // if a message is not initialized, just return empty { }
        if (message) {
            for (int index = 0; index < ordering.fieldCount(); ++index) {
                int fieldIndex = ordering.getFieldNumber(index);
                Q_ASSERT_X(fieldIndex < 536870912 && fieldIndex > 0,
                           "",
                           "fieldIndex is out of range");
                QProtobufPropertyOrderingInfo fieldInfo(ordering, index);
                QVariant propertyValue = message->property(fieldInfo);
                serializeProperty(propertyValue, fieldInfo);
            }
        }
    }

    static QVariant deserializeInt32(const QJsonValue &value, bool &ok)
    {
        auto val = value.toVariant().toInt(&ok);
        return QVariant::fromValue(val);
    }

    static QVariant deserializeUInt32(const QJsonValue &value, bool &ok)
    {
        auto val = value.toVariant().toUInt(&ok);
        return QVariant::fromValue(val);
    }

    static QVariant deserializeInt64(const QJsonValue &value, bool &ok)
    {
        auto val = value.toVariant().toLongLong(&ok);
        return QVariant::fromValue(val);
    }

    static QVariant deserializeUInt64(const QJsonValue &value, bool &ok)
    {
        auto val = value.toVariant().toULongLong(&ok);
        return QVariant::fromValue(val);
    }

    static QVariant deserializeFloat(const QJsonValue &value, bool &ok)
    {
        QByteArray data = value.toVariant().toByteArray();
        if (data == "NaN" || data == "Infinity" || data == "-Infinity") {
            ok = true;
            return QVariant(data);
        }
        auto val = data.toFloat(&ok);
        return QVariant::fromValue(val);
    }

    static QVariant deserializeDouble(const QJsonValue &value, bool &ok)
    {
        QByteArray data = value.toVariant().toByteArray();
        if (data == "NaN" || data == "Infinity" || data == "-Infinity") {
            ok = true;
            return QVariant(data);
        }
        auto val = data.toDouble(&ok);
        return QVariant::fromValue(val);
    }

    static QVariant deserializeBool(const QJsonValue &value, bool &ok)
    {
        ok = value.isBool();
        return (value.isBool()) ? QVariant(value.toBool()) : QVariant();
    }

    static QVariant deserializeString(const QJsonValue &value, bool &ok)
    {
        ok = value.isString();
        return value.toString();
    }

    static QVariant deserializeByteArray(const QJsonValue &value, bool &ok)
    {
        QByteArray data = value.toVariant().toByteArray();
        if (value.isString()) {
            ok = true;
            return QVariant::fromValue(QByteArray::fromBase64(data));
        }
        ok = false;
        return QVariant();
    }

    template<typename T>
    static QVariant deserializeList(const QJsonValue &value, bool &ok)
    {
        if (!value.isArray()) {
            ok = false;
            return QVariant();
        }
        ok = true;
        QList<T> list;
        QJsonArray array = value.toArray();
        auto handler = handlers.find(qMetaTypeId<T>());
        if (handler == handlers.end() || !handler->second.deserializer) {
            qProtoWarning() << "Unable to deserialize simple type list."
                               "Could not find deserializer for type"
                            << QMetaType::fromType<T>().name();
            return QVariant::fromValue(list);
        }

        for (auto arrayValue : array) {
            bool valueOk = false;
            QVariant newValue = handler->second.deserializer(arrayValue, valueOk);
            list.append(newValue.value<T>());
        }
        return QVariant::fromValue(list);
    }

    static QVariant deserializeStringList(const QJsonValue &value, bool &ok)
    {
        if (!value.isArray()) {
            ok = false;
            return QVariant();
        }
        QStringList list;
        if (value.isArray()) {
            QJsonArray array = value.toArray();
            for (auto arrayValue : array) {
                QVariant newValue = deserializeString(arrayValue, ok);
                list.append(newValue.value<QString>());
            }
        }
        return QVariant::fromValue(list);
    }

    QVariant deserializeValue(QVariant propertyData, bool &ok)
    {
        auto handler = QtProtobufPrivate::findHandler(propertyData.metaType());
        if (handler.deserializer) {
            handler.deserializer(qPtr, propertyData);
            ok = propertyData.isValid();
        } else {
            int userType = propertyData.userType();
            auto handler = handlers.find(propertyData.userType());
            if (handler != handlers.end() && handler->second.deserializer) {
                propertyData = handler->second.deserializer(activeValue, ok);
                if (!ok)
                    setInvalidFormatError();
                activeValue = {};
            } else {
                setDeserializationError(QAbstractProtobufSerializer::NoDeserializerError,
                                        QCoreApplication::
                                            translate("QtProtobuf",
                                                      "No deserializer is registered for type %1").arg(userType));
            }
        }
        return propertyData;
    }

    bool deserializeObject(QProtobufMessage *message, const QProtobufPropertyOrdering &ordering)
    {
        bool ok = false;
        std::map<QString, QProtobufPropertyOrderingInfo> msgContainer; // map<key, fieldInfo>
        for (int index = 0; index < ordering.fieldCount(); ++index) {
            int fieldIndex = ordering.getFieldNumber(index);
            Q_ASSERT_X(fieldIndex < 536870912 && fieldIndex > 0, "", "fieldIndex is out of range");
            QProtobufPropertyOrderingInfo fieldInfo(ordering, index);
            QString key = fieldInfo.getJsonName().toString();
            msgContainer.insert(std::pair<QString, QProtobufPropertyOrderingInfo>(key, fieldInfo));
        }

        if (!activeValue.isObject()) {
            setInvalidFormatError();
            activeValue = {};
            return false;
        }
        QJsonObject activeObject = activeValue.toObject();
        // Go through QJSON doc and find keys that are presented in msgContainer
        for (auto &key : activeObject.keys()) {
            std::map<QString, QProtobufPropertyOrderingInfo>::iterator iter = msgContainer
                                                                                  .find(key);
            if (iter != msgContainer.end()) {
                QVariant newPropertyValue = message->property(iter->second);
                auto store = activeValue;
                activeValue = activeObject.value(key);
                while (!activeValue.isNull()) {
                    newPropertyValue = deserializeValue(newPropertyValue, ok);
                }
                activeValue = store;

                if (ok)
                    message->setProperty(iter->second, newPropertyValue);
            }
        }

        // Once all keys are deserialized we assume that activeValue is empty, nothing left
        // to deserialize
        activeValue = {};

        return ok;
    }

    void setDeserializationError(QAbstractProtobufSerializer::DeserializationError error,
                                 const QString &errorString)
    {
        deserializationError = error;
        deserializationErrorString = errorString;
    }

    void setUnexpectedEndOfStreamError()
    {
        setDeserializationError(QAbstractProtobufSerializer::UnexpectedEndOfStreamError,
                                QCoreApplication::translate("QtProtobuf",
                                                            "Unexpected end of stream"));
    }

    void setInvalidFormatError()
    {
        setDeserializationError(QAbstractProtobufSerializer::InvalidFormatError,
                                QCoreApplication::
                                    translate("QtProtobuf",
                                              "One or more fields have invalid format"));
    }

    void clearError();

    QAbstractProtobufSerializer::DeserializationError deserializationError =
            QAbstractProtobufSerializer::NoDeserializerError;
    QString deserializationErrorString;
    QJsonValue activeValue;

    static SerializerRegistry handlers;

private:
    QProtobufJsonSerializer *qPtr;
};

QProtobufJsonSerializerPrivate::SerializerRegistry QProtobufJsonSerializerPrivate::handlers = {};

void QProtobufJsonSerializerPrivate::clearError()
{
    deserializationError = QAbstractProtobufSerializer::NoError;
    deserializationErrorString.clear();
}

QProtobufJsonSerializer::QProtobufJsonSerializer() :
    d_ptr(new QProtobufJsonSerializerPrivate(this))
{
}

QProtobufJsonSerializer::~QProtobufJsonSerializer() = default;

QAbstractProtobufSerializer::DeserializationError
QProtobufJsonSerializer::deserializationError() const
{
    return d_ptr->deserializationError;
}

QString QProtobufJsonSerializer::deserializationErrorString() const
{
    return d_ptr->deserializationErrorString;
}

QByteArray
QProtobufJsonSerializer::serializeMessage(const QProtobufMessage *message,
                                          const QProtobufPropertyOrdering &ordering) const
{
    d_ptr->clearError();
    d_ptr->activeValue = QJsonObject();
    d_ptr->serializeObject(message, ordering);
    QJsonDocument doc;
    doc.setObject(d_ptr->activeValue.toObject());
    d_ptr->activeValue = QJsonObject();
    return doc.toJson(QJsonDocument::Compact);
}

bool QProtobufJsonSerializer::deserializeMessage(QProtobufMessage *message,
                                                 const QProtobufPropertyOrdering &ordering,
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

    return d_ptr->deserializeObject(message, ordering);
}

void QProtobufJsonSerializer::serializeObject(const QProtobufMessage *message,
                                              const QProtobufPropertyOrdering &ordering,
                                              const QProtobufPropertyOrderingInfo &fieldInfo) const
{
    Q_UNUSED(fieldInfo);
    auto store = d_ptr->activeValue.toObject();
    d_ptr->activeValue = QJsonObject();
    d_ptr->serializeObject(message, ordering);
    store.insert(fieldInfo.getJsonName().toString(), d_ptr->activeValue);
    d_ptr->activeValue = store;
}

void QProtobufJsonSerializer::serializeListObject(const QProtobufMessage *message,
                                                  const QProtobufPropertyOrdering &ordering,
                                                  const QProtobufPropertyOrderingInfo &fieldInfo)
    const
{
    auto fieldName = fieldInfo.getJsonName().toString();
    auto store = d_ptr->activeValue.toObject();
    QJsonArray newArrayVal = store.value(fieldName).toArray();
    d_ptr->activeValue = {};
    d_ptr->serializeObject(message, ordering);
    newArrayVal.append(d_ptr->activeValue);
    store.insert(fieldName, newArrayVal);
    d_ptr->activeValue = store;
}

bool QProtobufJsonSerializer::deserializeObject(QProtobufMessage *message,
                                                const QProtobufPropertyOrdering &ordering) const
{
    return d_ptr->deserializeObject(message, ordering);
}

bool QProtobufJsonSerializer::deserializeListObject(QProtobufMessage *message,
                                                    const QProtobufPropertyOrdering &ordering) const
{
    QJsonArray array = d_ptr->activeValue.toArray();
    if (array.isEmpty()) {
        d_ptr->activeValue = {};
        return false;
    }

    auto val = array.takeAt(0);
    bool result = false;
    if (val.isObject()) {
        d_ptr->activeValue = val;
        deserializeObject(message, ordering);
        result = true;
    }

    if (!array.isEmpty())
        d_ptr->activeValue = array;
    else
        d_ptr->activeValue = {};

    return result;
}

void QProtobufJsonSerializer::serializeMapPair(const QVariant &key, const QVariant &value,
                                               const QProtobufPropertyOrderingInfo &fieldInfo) const
{
    const QString fieldName = fieldInfo.getJsonName().toString();
    auto store = d_ptr->activeValue.toObject();
    QJsonObject mapObject = store.value(fieldName).toObject();
    d_ptr->activeValue = QJsonObject();
    d_ptr->serializeProperty(value, fieldInfo.infoForMapValue());
    mapObject.insert(key.toString(), d_ptr->activeValue.toObject().value(fieldName));
    store.insert(fieldName, mapObject);
    d_ptr->activeValue = store;
}

bool QProtobufJsonSerializer::deserializeMapPair(QVariant &key, QVariant &value) const
{
    if (!d_ptr->activeValue.isObject()) {
        d_ptr->setUnexpectedEndOfStreamError();
        return false;
    }

    QJsonObject activeObject = d_ptr->activeValue.toObject();
    if (activeObject.isEmpty()) {
        d_ptr->activeValue = {};
        return false;
    }

    QString jsonKey = activeObject.keys().at(0);
    QJsonValue jsonValue = activeObject.take(jsonKey);

    auto it = d_ptr->handlers.find(key.userType());
    if (it == d_ptr->handlers.end()) {
        d_ptr->setInvalidFormatError();
        return false;
    }

    bool ok = false;
    key = it->second.deserializer(QJsonValue(jsonKey), ok);
    if (!ok) {
        d_ptr->setInvalidFormatError();
        return false;
    }

    it = d_ptr->handlers.find(value.userType());
    if (it != d_ptr->handlers.end()) {
        ok = false;
        value = it->second.deserializer(jsonValue, ok);
        if (!ok) {
            d_ptr->setInvalidFormatError();
            return false;
        }
    } else {
        auto handler = QtProtobufPrivate::findHandler(value.metaType());
        if (handler.deserializer) {
            d_ptr->activeValue = jsonValue;
            handler.deserializer(this, value);
        } else {
            d_ptr->setInvalidFormatError();
            return false;
        }
    }

    if (!activeObject.isEmpty())
        d_ptr->activeValue = activeObject;
    else
        d_ptr->activeValue = {};

    return true;
}

void QProtobufJsonSerializer::serializeEnum(QtProtobuf::int64 value, const QMetaEnum &metaEnum,
                                            const QtProtobufPrivate::QProtobufPropertyOrderingInfo
                                                &fieldInfo) const
{
    QJsonObject activeObject = d_ptr->activeValue.toObject();
    activeObject.insert(fieldInfo.getJsonName().toString(), QString::fromUtf8(metaEnum.key(value)));
    d_ptr->activeValue = activeObject;
}

void QProtobufJsonSerializer::
    serializeEnumList(const QList<QtProtobuf::int64> &values, const QMetaEnum &metaEnum,
                      const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const
{
    QJsonArray arr;
    for (const auto value : values)
        arr.append(QString::fromUtf8(metaEnum.key(value)));

    QJsonObject activeObject = d_ptr->activeValue.toObject();
    activeObject.insert(fieldInfo.getJsonName().toString(), arr);
    d_ptr->activeValue = activeObject;
}

bool QProtobufJsonSerializer::deserializeEnum(QtProtobuf::int64 &value,
                                              const QMetaEnum &metaEnum) const
{
    QString enumKey = d_ptr->activeValue.toString();
    bool ok = false;
    value = metaEnum.keyToValue(enumKey.toUtf8().data(), &ok);
    d_ptr->activeValue = {};
    return ok;
}

bool QProtobufJsonSerializer::deserializeEnumList(QList<QtProtobuf::int64> &value,
                                                  const QMetaEnum &metaEnum) const
{
    QJsonArray arr = d_ptr->activeValue.toArray();
    bool ok = false;
    for (const auto &val : arr) {
        QString enumKey = val.toString();
        value.append(metaEnum.keyToValue(enumKey.toUtf8().data(), &ok));
        if (!ok)
            break;
    }
    d_ptr->activeValue = {};
    return ok;
}

QT_END_NAMESPACE
