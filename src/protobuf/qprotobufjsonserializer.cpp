// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qprotobufjsonserializer.h"
#include "qprotobufserializer_p.h"
#include "qprotobufmessage_p.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QtGlobal>
#include <QtCore/QVariant>

#include <map>
#include <unordered_map>

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

class QProtobufJsonSerializerPrivate final
{
    Q_DISABLE_COPY_MOVE(QProtobufJsonSerializerPrivate)

public:
    using Serializer = std::function<QByteArray(const QVariant&)>;
    using Deserializer = std::function<QVariant(const QJsonValue&, bool &ok)>;

    struct SerializationHandlers {
        // serializer assigned to class
        Serializer serializer;
        // deserializer assigned to class
        Deserializer deserializer;
    };

    // TBD Replace std::unordered_map to QHash
    using SerializerRegistry = std::unordered_map<int/*metatypeid*/, SerializationHandlers>;

    static QByteArray serializeFloat(const QVariant &propertyValue)
    {
        bool ok = false;
        float value = propertyValue.toFloat(&ok);
        if (!ok || qIsNaN(value)) {
            return QByteArray("NaN");
        }
        return QByteArray::number(value, 'g', 7);
    }

    static QByteArray serializeDouble(const QVariant &propertyValue)
    {
        bool ok = false;
        double value = propertyValue.toDouble(&ok);
        if (!ok || qIsNaN(value)) {
            return QByteArray("NaN");
        }
        return QByteArray::number(value, 'g', 14);
    }

    static QByteArray serializeString(const QVariant &propertyValue)
    {
        return '"' + propertyValue.toString().toUtf8() + '"';
    }

    static QByteArray serializeBytes(const QVariant &propertyValue) {
        return QByteArray("\"") + propertyValue.toByteArray().toBase64() + "\"";
    }

    template<typename L>
    static QByteArray serializeList(const QVariant &propertyValue)
    {
        L listValue = propertyValue.value<L>();
        QByteArray result("[");
        for (auto value : listValue) {
            result += QByteArray::number(value) + ",";
        }
        //Remove trailing `,`
        if (listValue.size() > 0) {
            result.chop(1);
        }
        result += "]";
        return result;
    }

    static QByteArray serializeBoolList(const QVariant &propertyValue)
    {
        QtProtobuf::boolList listValue = propertyValue.value<QtProtobuf::boolList>();
        QByteArray result("[");
        for (auto value : listValue) {
            if (value)
                result += QByteArray("true,");
            else
                result += QByteArray("false,");
        }
        if (listValue.size() > 0) {
            result.chop(1);
        }
        result += "]";
        return result;
    }

    static QByteArray serializeFloatList(const QVariant &propertyValue)
    {
        QtProtobuf::floatList listValue = propertyValue.value<QtProtobuf::floatList>();
        QByteArray result("[");
        for (auto value : listValue) {
            if (qIsNaN(value))
                return QByteArray("NaN");
            result += QByteArray::number(value, 'g', 7) + ",";
        }
        if (listValue.size() > 0) {
            result.chop(1);
        }
        result += "]";
        return result;
    }

    static QByteArray serializeStringList(const QVariant &propertyValue)
    {
        QStringList listValue = propertyValue.value<QStringList>();
        QByteArray result("[");
        for (auto value : listValue) {
            result += QByteArray("\"") + value.toUtf8() + "\",";
        }
        if (listValue.size() > 0) {
            result.chop(1);
        }
        result += "]";
        return result;
    }

    static QByteArray serializeBytesList(const QVariant &propertyValue)
    {
        QByteArrayList listValue = propertyValue.value<QByteArrayList>();
        QByteArray result("[");
        for (auto value : listValue) {
            result += QByteArray("\"") + value.toBase64() + "\",";
        }
        if (listValue.size() > 0) {
            result.chop(1);
        }
        result += "]";
        return result;
    }

    static QByteArray serializeDoubleList(const QVariant &propertyValue)
    {
        QtProtobuf::doubleList listValue = propertyValue.value<QtProtobuf::doubleList>();
        QByteArray result("[");
        for (auto value : listValue) {
            if (qIsNaN(value))
                return QByteArray("NaN");
            result += QByteArray::number(value, 'g', 14) + ",";
        }
        if (listValue.size() > 0) {
            result.chop(1);
        }
        result += "]";
        return result;
    }

    QProtobufJsonSerializerPrivate(QProtobufJsonSerializer *q)
        : qPtr(q)
    {
        // TBD Change to assigning the result of a lambda to a static variable
        if (handlers.empty()) {
            handlers[qMetaTypeId<QtProtobuf::int32>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeInt32};
            handlers[qMetaTypeId<QtProtobuf::sfixed32>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeInt32};
            handlers[qMetaTypeId<QtProtobuf::sint32>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeInt32};
            handlers[qMetaTypeId<QtProtobuf::sint64>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeInt64};
            handlers[qMetaTypeId<QtProtobuf::int64>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeInt64};
            handlers[qMetaTypeId<QtProtobuf::sfixed64>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeInt64};
            handlers[qMetaTypeId<QtProtobuf::uint32>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeUInt32};
            handlers[qMetaTypeId<QtProtobuf::fixed32>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeUInt32};
            handlers[qMetaTypeId<QtProtobuf::uint64>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeUInt64};
            handlers[qMetaTypeId<QtProtobuf::fixed64>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeUInt64};
            handlers[qMetaTypeId<bool>()]
                    = {{}, QProtobufJsonSerializerPrivate::deserializeBool};
            handlers[QMetaType::Float] = {QProtobufJsonSerializerPrivate::serializeFloat,
                                          QProtobufJsonSerializerPrivate::deserializeFloat};
            handlers[QMetaType::Double] = {QProtobufJsonSerializerPrivate::serializeDouble,
                                           QProtobufJsonSerializerPrivate::deserializeDouble};
            handlers[QMetaType::QString]
                    = {QProtobufJsonSerializerPrivate::serializeString,
                       QProtobufJsonSerializerPrivate::deserializeString};
            handlers[QMetaType::QByteArray]
                    = {QProtobufJsonSerializerPrivate::serializeBytes,
                       QProtobufJsonSerializerPrivate::deserializeByteArray};
            handlers[qMetaTypeId<QtProtobuf::boolList>()]
                    = {QProtobufJsonSerializerPrivate::serializeBoolList,
                    QProtobufJsonSerializerPrivate::deserializeList<bool>};
            handlers[qMetaTypeId<QtProtobuf::int32List>()]
                    = {QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::int32List>,
                    QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::int32>};
            handlers[qMetaTypeId<QtProtobuf::int64List>()]
                    = {QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::int64List>,
                    QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::int64>};
            handlers[qMetaTypeId<QtProtobuf::sint32List>()]
                    = {QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::sint32List>,
                    QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::sint32>};
            handlers[qMetaTypeId<QtProtobuf::sint64List>()]
                    = {QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::sint64List>,
                    QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::sint64>};
            handlers[qMetaTypeId<QtProtobuf::uint32List>()]
                    = {QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::uint32List>,
                    QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::uint32>};
            handlers[qMetaTypeId<QtProtobuf::uint64List>()]
                    = {QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::uint64List>,
                    QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::uint64>};
            handlers[qMetaTypeId<QtProtobuf::fixed32List>()]
                    = {QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::fixed32List>,
                    QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::fixed32>};
            handlers[qMetaTypeId<QtProtobuf::fixed64List>()]
                    = {QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::fixed64List>,
                    QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::fixed64>};
            handlers[qMetaTypeId<QtProtobuf::sfixed32List>()]
                    = {QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::sfixed32List>,
                    QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::sfixed32>};
            handlers[qMetaTypeId<QtProtobuf::sfixed64List>()]
                    = {QProtobufJsonSerializerPrivate::serializeList<QtProtobuf::sfixed64List>,
                    QProtobufJsonSerializerPrivate::deserializeList<QtProtobuf::sfixed64>};
            handlers[qMetaTypeId<QtProtobuf::floatList>()]
                    = {QProtobufJsonSerializerPrivate::serializeFloatList,
                    QProtobufJsonSerializerPrivate::deserializeList<float>};
            handlers[qMetaTypeId<QtProtobuf::doubleList>()]
                    = {QProtobufJsonSerializerPrivate::serializeDoubleList,
                    QProtobufJsonSerializerPrivate::deserializeList<double>};
            handlers[qMetaTypeId<QStringList>()]
                    = {QProtobufJsonSerializerPrivate::serializeStringList,
                    QProtobufJsonSerializerPrivate::deserializeStringList};
            handlers[qMetaTypeId<QByteArrayList>()]
                    = {QProtobufJsonSerializerPrivate::serializeBytesList,
                    QProtobufJsonSerializerPrivate::deserializeList<QByteArray>};
        }
    }
    ~QProtobufJsonSerializerPrivate() = default;

    QByteArray serializeValue(const QVariant &propertyValue,
                              const QProtobufPropertyOrderingInfo &fieldInfo)
    {
        QByteArray buffer;
        QMetaType metaType = propertyValue.metaType();
        auto userType = propertyValue.userType();

        if (metaType.id() == QMetaType::UnknownType) {
            return {};
        }
        auto handler = QtProtobufPrivate::findHandler(metaType);
        if (handler.serializer) {
            handler.serializer(qPtr, propertyValue, fieldInfo, buffer);
        } else {
            auto handler = handlers.find(userType);
            if (handler != handlers.end() && handler->second.serializer) {
                buffer += handler->second.serializer(propertyValue);
            } else {
                buffer += propertyValue.toString().toUtf8();
            }
        }
        return buffer;
    }

    QByteArray serializeProperty(const QVariant &propertyValue,
                                 const QProtobufPropertyOrderingInfo &fieldInfo)
    {
        return QByteArray("\"")
                + QByteArrayView(fieldInfo.getJsonName())
                + QByteArray("\":") + serializeValue(propertyValue, fieldInfo);
    }

    QByteArray serializeObject(const QProtobufMessage *message,
                               const QProtobufPropertyOrdering &ordering)
    {
        QByteArray result = "{";
        // if a message is not initialized, just return empty { }
        if (message) {
            for (int index = 0; index < ordering.fieldCount(); ++index) {
                int fieldIndex = ordering.getFieldNumber(index);
                Q_ASSERT_X(fieldIndex < 536870912 && fieldIndex > 0,
                           "",
                           "fieldIndex is out of range");
                QProtobufPropertyOrderingInfo fieldInfo(ordering, index);
                QVariant propertyValue = message->property(fieldInfo);
                result.append(serializeProperty(propertyValue, fieldInfo));
                result.append(",");
            }
            result.chop(1);
        }
        result.append("}");
        return result;
    }

    static QVariant deserializeInt32(const QJsonValue &value, bool &ok)
    {
        auto val = value.toInt();
        ok = value.isDouble();
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

    QVariant deserializeValue(const QMetaType &metaType,
                              QProtobufSelfcheckIterator &it,
                              const QJsonValue &value,
                              bool &ok)
    {
        QVariant result;
        auto handler = QtProtobufPrivate::findHandler(metaType);
        if (handler.deserializer) {
            handler.deserializer(qPtr, it, result);
            ok = result.isValid();
        } else {
            auto handler = handlers.find(metaType.id());
            if (handler != handlers.end() && handler->second.deserializer) {
                result = handler->second.deserializer(value, ok);
            } else {
                QString error = QString::fromUtf8("No deserializer is registered for value %1")
                                    .arg(QString::fromUtf8(metaType.name()));
                setDeserializationError(
                            QAbstractProtobufSerializer::NoDeserializerError,
                            QCoreApplication::translate("QtProtobuf",
                                                error.toUtf8().data()));
            }
        }
        return result;
    }

    bool deserializeObject(QProtobufMessage *message,
                           const QProtobufPropertyOrdering &ordering,
                           QByteArrayView data)
    {
        auto it = QProtobufSelfcheckIterator::fromView(data);
        Q_ASSERT(it.isValid() && it.bytesLeft() > 0);

        bool ok = false;
        // TBD Try fromJson(QBAView) instead of current variant
        QJsonDocument document
                = QJsonDocument::fromJson(QByteArray(data.data(), static_cast<size_t>(data.size())));

        if (!document.isObject())
            return false;

        std::map<QString, QProtobufPropertyOrderingInfo> msgContainer; // map<key, fieldInfo>
        for (int index = 0; index < ordering.fieldCount(); ++index) {
            int fieldIndex = ordering.getFieldNumber(index);
            Q_ASSERT_X(fieldIndex < 536870912 && fieldIndex > 0, "", "fieldIndex is out of range");
            QProtobufPropertyOrderingInfo fieldInfo(ordering, index);
            QString key = fieldInfo.getJsonName().toString();
            msgContainer.insert(std::pair<QString, QProtobufPropertyOrderingInfo>(key, fieldInfo));
        }

        QByteArray object;
        // Go through QJSON doc and find keys that are presented in msgContainer
        QJsonObject obj = document.object();
        for (auto &key : obj.keys()) {
            QJsonValue jsonValue = obj.value(key);
            auto iter = msgContainer.find(key);
            if (iter != msgContainer.end()) {
                QVariant newPropertyValue = message->property(iter->second);
                // Complex message case; move iterator to the deserializing array
                if (jsonValue.isArray()) {
                    QJsonArray array = jsonValue.toArray();
                    object = QJsonDocument(array).toJson();
                    it = QProtobufSelfcheckIterator::fromView(object);
                }
                // Complex message case; move iterator to the deserializing object
                if (jsonValue.isObject()) {
                    object = QJsonDocument(jsonValue.toObject()).toJson();
                    it = QProtobufSelfcheckIterator::fromView(object);
                }
                newPropertyValue = deserializeValue(newPropertyValue.metaType(),
                                                    it,
                                                    jsonValue,
                                                    ok);
                if (ok) {
                    message->setProperty(iter->second, std::move(newPropertyValue));
                }
            }
        }

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

    void clearError();

    QAbstractProtobufSerializer::DeserializationError deserializationError =
            QAbstractProtobufSerializer::NoDeserializerError;
    QString deserializationErrorString;
    QJsonArray activeArray;

private:
    static SerializerRegistry handlers;
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
    return d_ptr->serializeObject(message, ordering);
}

bool QProtobufJsonSerializer::deserializeMessage(QProtobufMessage *message,
                                                 const QProtobufPropertyOrdering &ordering,
                                                 QByteArrayView data) const
{
    d_ptr->clearError();
    auto it = QProtobufSelfcheckIterator::fromView(data);
    if (!d_ptr->deserializeObject(message, ordering, data))
        return false;

    if (!it.isValid())
        d_ptr->setUnexpectedEndOfStreamError();
    return it.isValid();
}

QByteArray
QProtobufJsonSerializer::serializeObject(const QProtobufMessage *message,
                                         const QProtobufPropertyOrdering &ordering,
                                         const QProtobufPropertyOrderingInfo &fieldInfo) const
{
    Q_UNUSED(fieldInfo);
    return serializeMessage(message, ordering);
}

QByteArray
QProtobufJsonSerializer::serializeListObject(const QList<const QProtobufMessage*> &messageList,
                                             const QProtobufPropertyOrdering &ordering,
                                             const QProtobufPropertyOrderingInfo &fieldInfo) const
{
    QByteArray result("[");
    for (auto message : messageList) {
        result.append(serializeObject(message, ordering, fieldInfo) + ",");
    }
    if (messageList.size() > 0) {
        result.chop(1);
    }
    result.append("]");
    return result;
}

bool QProtobufJsonSerializer::deserializeObject(QProtobufMessage *message,
                                                const QProtobufPropertyOrdering &ordering,
                                                QProtobufSelfcheckIterator &it) const
{
    return deserializeMessage(message, ordering, it.data());
}

QProtobufBaseSerializer::Status
QProtobufJsonSerializer::deserializeListObject(QProtobufMessage *message,
                                               const QProtobufPropertyOrdering &ordering,
                                               QProtobufSelfcheckIterator &it) const
{
    if (d_ptr->activeArray.empty()) {
        QJsonDocument doc = QJsonDocument::fromJson(QByteArray(it.data(), it.bytesLeft()));
        if (doc.isArray())
            d_ptr->activeArray = doc.array();
        else
            return QProtobufBaseSerializer::SerializationError;
    }

    // doc.array() is empty
    if (d_ptr->activeArray.empty())
        return QProtobufBaseSerializer::SerializationError;

    const QJsonValue &element = d_ptr->activeArray.takeAt(0);
    if (element.isObject()) {
        auto newIt
            = QProtobufSelfcheckIterator::fromView(QJsonDocument(element.toObject()).toJson());
        bool result = deserializeObject(message, ordering, newIt);

        if (!d_ptr->activeArray.empty()) {
            return QProtobufBaseSerializer::SerializationInProgress;
        } else {
            return result ? QProtobufBaseSerializer::Serialized
                          : QProtobufBaseSerializer::SerializationError;
        }
    }

    return QProtobufBaseSerializer::SerializationError;
}

QByteArray QProtobufJsonSerializer::serializeMapPair(const QList<QPair<QVariant, QVariant>> &list,
                                                     const QProtobufPropertyOrderingInfo &fieldInfo
                                                     ) const
{
    Q_UNUSED(fieldInfo);
    QByteArray result;
    for (auto element: list) {
        const QVariant &key = element.first;
        qProtoWarning() << "Map pair serialization will be done soon:" << key << element.second;
    }
    return result;
}

QT_END_NAMESPACE
