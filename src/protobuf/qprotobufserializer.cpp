// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>, Viktor Kopp <vifactor@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtProtobuf/qprotobufserializer.h>

#include <QtProtobuf/private/qprotobufmessage_p.h>
#include <QtProtobuf/private/qprotobufserializer_p.h>
#include <QtProtobuf/private/qprotobufregistration_p.h>
#include <QtProtobuf/private/qtprotobufdefs_p.h>
#include <QtProtobuf/private/qtprotobufserializerhelpers_p.h>
#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/qmetatype.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qreadwritelock.h>

QT_BEGIN_NAMESPACE

/*!
    \class QProtobufSerializer
    \inmodule QtProtobuf
    \since 6.5
    \brief The QProtobufSerializer class is interface that represents
           basic functions for serialization/deserialization.

    The QProtobufSerializer class registers serializers/deserializers for
    classes implementing a protobuf message, inheriting \l QProtobufMessage. These
    classes are generated automatically, based on a \c{.proto} file, using the CMake
    function \l qt_add_protobuf or by running
    \l {The qtprotobufgen Tool} {qtprotobufgen} directly.
*/

using namespace Qt::StringLiterals;
using namespace QtProtobufPrivate;

template<std::size_t N>
using SerializerRegistryType =
        std::array<QProtobufSerializerPrivate::ProtobufSerializationHandler, N>;

namespace {

#define QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(Type, WireType)          \
  {                                                                          \
    QMetaType::fromType<Type>(),                                             \
            QProtobufSerializerPrivate::serializeWrapper<                    \
                    Type, QProtobufSerializerPrivate::serializeBasic<Type>>, \
            QProtobufSerializerPrivate::deserializeBasic<Type>,              \
            QProtobufSerializerPrivate::isPresent<Type>, WireType        \
  }
#define QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(ListType, Type)            \
  {                                                                                 \
    QMetaType::fromType<ListType>(),                                                \
            QProtobufSerializerPrivate::serializeWrapper<                           \
                    ListType, QProtobufSerializerPrivate::serializeListType<Type>>, \
            QProtobufSerializerPrivate::deserializeList<Type>,                      \
            QProtobufSerializerPrivate::isPresent<ListType>,                    \
            QtProtobuf::WireTypes::LengthDelimited                                  \
  }

#define QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(ListType, Type, WireType) \
{                                                                                           \
QMetaType::fromType<ListType>(),                                                          \
        QProtobufSerializerPrivate::serializeNonPackedWrapper<                            \
                ListType, QProtobufSerializerPrivate::serializeNonPackedList<Type>>,      \
        QProtobufSerializerPrivate::deserializeNonPackedList<Type>,                       \
        QProtobufSerializerPrivate::isPresent<ListType>, WireType                     \
}

constexpr SerializerRegistryType<30> IntegratedTypesSerializers = { {
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(float, QtProtobuf::WireTypes::Fixed32),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(double, QtProtobuf::WireTypes::Fixed64),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::int32,
                                                    QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::int64,
                                                    QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::uint32,
                                                    QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::uint64,
                                                    QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::sint32,
                                                    QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::sint64,
                                                    QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::fixed32,
                                                    QtProtobuf::WireTypes::Fixed32),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::fixed64,
                                                    QtProtobuf::WireTypes::Fixed64),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::sfixed32,
                                                    QtProtobuf::WireTypes::Fixed32),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::sfixed64,
                                                    QtProtobuf::WireTypes::Fixed64),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QtProtobuf::boolean,
                                                    QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QString,
                                                    QtProtobuf::WireTypes::LengthDelimited),
        QT_CONSTRUCT_PROTOBUF_SERIALIZATION_HANDLER(QByteArray,
                                                    QtProtobuf::WireTypes::LengthDelimited),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::floatList, float),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::doubleList, double),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::int32List, QtProtobuf::int32),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::int64List, QtProtobuf::int64),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::uint32List,
                                                         QtProtobuf::uint32),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::uint64List,
                                                         QtProtobuf::uint64),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::sint32List,
                                                         QtProtobuf::sint32),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::sint64List,
                                                         QtProtobuf::sint64),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::fixed32List,
                                                         QtProtobuf::fixed32),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::fixed64List,
                                                         QtProtobuf::fixed64),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::sfixed32List,
                                                         QtProtobuf::sfixed32),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::sfixed64List,
                                                         QtProtobuf::sfixed64),
        QT_CONSTRUCT_PROTOBUF_LIST_SERIALIZATION_HANDLER(QtProtobuf::boolList, QtProtobuf::boolean),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QStringList, QString, QtProtobuf::WireTypes::LengthDelimited),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QByteArrayList, QByteArray, QtProtobuf::WireTypes::LengthDelimited),
} };

constexpr SerializerRegistryType<13> IntegratedNonPackedSerializers = { {
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(QtProtobuf::floatList, float,
                                                                    QtProtobuf::WireTypes::Fixed32),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(QtProtobuf::doubleList, double,
                                                                    QtProtobuf::WireTypes::Fixed64),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::int32List, QtProtobuf::int32, QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::int64List, QtProtobuf::int64, QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::uint32List, QtProtobuf::uint32, QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::uint64List, QtProtobuf::uint64, QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::sint32List, QtProtobuf::sint32, QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::sint64List, QtProtobuf::sint64, QtProtobuf::WireTypes::Varint),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::fixed32List, QtProtobuf::fixed32, QtProtobuf::WireTypes::Fixed32),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::fixed64List, QtProtobuf::fixed64, QtProtobuf::WireTypes::Fixed64),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::sfixed32List, QtProtobuf::sfixed32, QtProtobuf::WireTypes::Fixed32),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::sfixed64List, QtProtobuf::sfixed64, QtProtobuf::WireTypes::Fixed64),
        QT_CONSTRUCT_PROTOBUF_NON_PACKED_LIST_SERIALIZATION_HANDLER(
                QtProtobuf::boolList, QtProtobuf::boolean, QtProtobuf::WireTypes::Varint),
} };

template<std::size_t N>
std::optional<QProtobufSerializerPrivate::ProtobufSerializationHandler>
findIntegratedTypeHandlerImpl(QMetaType metaType, const SerializerRegistryType<N> &registry)
{
    typename SerializerRegistryType<N>::const_iterator it = std::find_if(
            registry.begin(), registry.end(),
            [&metaType](const QProtobufSerializerPrivate::ProtobufSerializationHandler &handler) {
                return handler.metaType == metaType;
            });
    if (it == registry.end())
        return std::nullopt;
    return { *it };
}

std::optional<QProtobufSerializerPrivate::ProtobufSerializationHandler>
findIntegratedTypeHandler(QMetaType metaType, bool nonPacked)
{
    if (nonPacked)
        return findIntegratedTypeHandlerImpl(metaType, IntegratedNonPackedSerializers);

    return findIntegratedTypeHandlerImpl(metaType, IntegratedTypesSerializers);
}
}

/*!
    Constructs a new serializer instance.
*/
QProtobufSerializer::QProtobufSerializer() : d_ptr(new QProtobufSerializerPrivate)
{
}

/*!
    Destroys the serializer instance.
*/
QProtobufSerializer::~QProtobufSerializer() = default;

QByteArray QProtobufSerializer::serializeMessage(const QProtobufMessage *message) const
{
    d_ptr->clearError();
    d_ptr->result = {};
    d_ptr->serializeMessage(message);
    return d_ptr->result;
}

void QProtobufSerializerPrivate::serializeMessage(const QProtobufMessage *message)
{
    Q_ASSERT(message != nullptr);

    auto ordering = message->propertyOrdering();
    Q_ASSERT(ordering != nullptr);

    for (int index = 0; index < ordering->fieldCount(); ++index) {
        int fieldIndex = ordering->fieldNumber(index);
        Q_ASSERT_X(fieldIndex <= ProtobufFieldNumMax && fieldIndex >= ProtobufFieldNumMin, "",
                   "fieldIndex is out of range");
        QProtobufFieldInfo fieldInfo(*ordering, index);
        QVariant propertyValue = QtProtobufSerializerHelpers::messageProperty(message, fieldInfo);
        serializeProperty(propertyValue, fieldInfo);
    }

    if (preserveUnknownFields) {
        // Restore any unknown fields we have stored away:
        const QProtobufMessagePrivate *messagePrivate = QProtobufMessagePrivate::get(message);
        for (const auto &fields : messagePrivate->unknownEntries)
            result += fields.join();
    }
}

void QProtobufSerializerPrivate::setUnexpectedEndOfStreamError()
{
    setDeserializationError(QAbstractProtobufSerializer::Error::UnexpectedEndOfStream,
                            QCoreApplication::translate("QtProtobuf", "Unexpected end of stream"));
}

void QProtobufSerializerPrivate::clearError()
{
    lastError = QAbstractProtobufSerializer::Error::None;
    lastErrorString.clear();
}

bool QProtobufSerializer::deserializeMessage(QProtobufMessage *message, QByteArrayView data) const
{
    d_ptr->clearCachedValue();
    d_ptr->clearError();
    d_ptr->it = QProtobufSelfcheckIterator::fromView(data);

    bool ok = true;
    while (d_ptr->it.isValid() && d_ptr->it != data.end()) {
        if (!d_ptr->deserializeProperty(message)) {
            ok = false;
            break;
        }
    }

    if (!d_ptr->storeCachedValue(message) || !ok)
        return false;

    if (!d_ptr->it.isValid())
        d_ptr->setUnexpectedEndOfStreamError();
    return d_ptr->it.isValid();
}

void QProtobufSerializerPrivate::serializeObject(const QProtobufMessage *message,
                                          const QProtobufFieldInfo &fieldInfo)
{
    auto store = result;
    result = {};
    serializeMessage(message);
    store.append(QProtobufSerializerPrivate::encodeHeader(fieldInfo.fieldNumber(),
                                                          QtProtobuf::WireTypes::LengthDelimited));
    store.append(QProtobufSerializerPrivate::serializeVarintCommon<uint32_t>(result.size()));
    store.append(result);
    result = store;
}

bool QProtobufSerializerPrivate::deserializeObject(QProtobufMessage *message)
{
    if (it.bytesLeft() == 0) {
        setUnexpectedEndOfStreamError();
        return false;
    }
    std::optional<QByteArray>
        array = QProtobufSerializerPrivate::deserializeLengthDelimited(it);
    if (!array) {
        setUnexpectedEndOfStreamError();
        return false;
    }

    auto prevCachedRepeatedIterator = std::move(cachedRepeatedIterator);
    auto restoreOnReturn = qScopeGuard([prevIt = it, prevCachedPropertyValue = cachedPropertyValue,
                                        prevCachedIndex = cachedIndex, &prevCachedRepeatedIterator,
                                        this]() {
        it = prevIt;
        cachedPropertyValue = prevCachedPropertyValue;
        cachedIndex = prevCachedIndex;
        cachedRepeatedIterator = std::move(prevCachedRepeatedIterator);
    });
    clearCachedValue();

    QByteArrayView data = *array;
    clearError();
    it = QProtobufSelfcheckIterator::fromView(data);
    bool ok = true;
    while (it.isValid() && it != data.end()) {
        if (!deserializeProperty(message)) {
            ok = false;
            break;
        }
    }

    if (!storeCachedValue(message) || !ok)
        return false;

    if (!it.isValid())
        setUnexpectedEndOfStreamError();
    return it.isValid();
}

void QProtobufSerializerPrivate::serializeEnumList(const QList<QtProtobuf::int64> &value,
                                                   const QProtobufFieldInfo &fieldInfo)
{
    if (value.isEmpty())
        return;

    auto header = QProtobufSerializerPrivate::encodeHeader(fieldInfo.fieldNumber(),
                                                           QtProtobuf::WireTypes::LengthDelimited);

    if (fieldInfo.fieldFlags().testFlag(QtProtobufPrivate::FieldFlag::NonPacked))
        result
            .append(QProtobufSerializerPrivate::serializeNonPackedList<QtProtobuf::int64>(value,
                                                                                          header));
    else
        result.append(header
                      + QProtobufSerializerPrivate::serializeListType<QtProtobuf::int64>(value));
}

bool QProtobufSerializerPrivate::deserializeEnumList(QList<QtProtobuf::int64> &value)
{
    QVariant variantValue;
    if (!QProtobufSerializerPrivate::deserializeList<QtProtobuf::int64>(it, variantValue)) {
        setUnexpectedEndOfStreamError();
        return false;
    }
    value = variantValue.value<QList<QtProtobuf::int64>>();
    return true;
}

/*!
    \internal
    Encode a property field index and its type into output bytes.

    Header byte
    Meaning    |  Field index  |  Type
    ---------- | ------------- | --------
    bit number | 7  6  5  4  3 | 2  1  0

    fieldIndex: The index of a property in parent object
    wireType: Serialization type used for the property with index @p fieldIndex

    Returns a varint-encoded fieldIndex and wireType
 */
QByteArray QProtobufSerializerPrivate::encodeHeader(int fieldIndex,
                                                           QtProtobuf::WireTypes wireType)
{
    uint32_t header = (fieldIndex << 3) | int(wireType);
    return serializeVarintCommon<uint32_t>(header);
}

/*!
    \internal
    Decode a property field index and its serialization type from input bytes

    Iterator: that points to header with encoded field index and serialization type
    fieldIndex: Decoded index of a property in parent object
    wireType: Decoded serialization type used for the property with index
    Return true if both decoded wireType and fieldIndex have "allowed" values and false, otherwise
 */
bool QProtobufSerializerPrivate::decodeHeader(QProtobufSelfcheckIterator &it,
                                                     int &fieldIndex,
                                                     QtProtobuf::WireTypes &wireType)
{
    if (it.bytesLeft() == 0)
        return false;
    auto opt = deserializeVarintCommon<uint32_t>(it);
    if (!opt)
        return false;
    uint32_t header = opt.value();
    wireType = static_cast<QtProtobuf::WireTypes>(header & 0b00000111);
    fieldIndex = header >> 3;

    constexpr int maxFieldIndex = (1 << 29) - 1;
    return fieldIndex <= maxFieldIndex && fieldIndex > 0
            && (wireType == QtProtobuf::WireTypes::Varint
                || wireType == QtProtobuf::WireTypes::Fixed64
                || wireType == QtProtobuf::WireTypes::Fixed32
                || wireType == QtProtobuf::WireTypes::LengthDelimited);
}

void QProtobufSerializerPrivate::skipVarint(QProtobufSelfcheckIterator &it)
{
    while ((*it) & 0x80)
        ++it;
    ++it;
}

void QProtobufSerializerPrivate::skipLengthDelimited(QProtobufSelfcheckIterator &it)
{
    //Get length of length-delimited field
    auto opt = QProtobufSerializerPrivate::deserializeVarintCommon<QtProtobuf::uint64>(it);
    if (!opt) {
        it += it.bytesLeft() + 1;
        return;
    }
    QtProtobuf::uint64 length = opt.value();
    it += length;
}

qsizetype QProtobufSerializerPrivate::skipSerializedFieldBytes(QProtobufSelfcheckIterator &it, QtProtobuf::WireTypes type)
{
    const auto *initialIt = QByteArray::const_iterator(it);
    switch (type) {
    case QtProtobuf::WireTypes::Varint:
        skipVarint(it);
        break;
    case QtProtobuf::WireTypes::Fixed32:
        it += sizeof(decltype(QtProtobuf::fixed32::t));
        break;
    case QtProtobuf::WireTypes::Fixed64:
        it += sizeof(decltype(QtProtobuf::fixed64::t));
        break;
    case QtProtobuf::WireTypes::LengthDelimited:
        skipLengthDelimited(it);
        break;
    case QtProtobuf::WireTypes::Unknown:
    default:
        Q_UNREACHABLE();
        return 0;
    }

    return std::distance(initialIt, QByteArray::const_iterator(it));
}

void QProtobufSerializerPrivate::serializeProperty(QVariant propertyValue,
                                                   const QProtobufFieldInfo &fieldInfo)
{
    QMetaType metaType = propertyValue.metaType();

    qProtoDebug() << "propertyValue" << propertyValue << "fieldIndex" << fieldInfo.fieldNumber()
                  << "metaType" << metaType.name();

    if (metaType.id() == QMetaType::UnknownType || propertyValue.isNull()) {
        // Empty value
        return;
    }

    if (metaType.flags() & QMetaType::IsPointer) {
        auto *messageProperty = propertyValue.value<QProtobufMessage *>();
        auto store = result;
        result = {};
        serializeMessage(messageProperty);
        store.append(QProtobufSerializerPrivate::
                         encodeHeader(fieldInfo.fieldNumber(),
                                      QtProtobuf::WireTypes::LengthDelimited));
        store.append(QProtobufSerializerPrivate::serializeVarintCommon<uint32_t>(result.size()));
        store.append(result);
        result = store;
        return;
    }

    const auto fieldFlags = fieldInfo.fieldFlags();
    if (fieldFlags.testFlag(QtProtobufPrivate::FieldFlag::Enum)) {
        if (fieldFlags.testFlag(QtProtobufPrivate::FieldFlag::Repeated)) {
            auto value = propertyValue.value<QList<QtProtobuf::int64>>();
            serializeEnumList(value, fieldInfo);
        } else {
            auto value = propertyValue.value<int64_t>();
            if (value == 0 && !isOneofOrOptionalField(fieldInfo))
                return;
            result.append(QProtobufSerializerPrivate::encodeHeader(fieldInfo.fieldNumber(),
                                                                   QtProtobuf::WireTypes::Varint));
            result.append(QProtobufSerializerPrivate::serializeBasic<
                          QtProtobuf::int64>(propertyValue.value<int64_t>()));
        }
        return;
    }

    if (propertyValue.canView(QMetaType::fromType<QProtobufRepeatedIterator>())) {
        QProtobufRepeatedIterator propertyIt = propertyValue.view<QProtobufRepeatedIterator>();
        while (propertyIt.hasNext())
            serializeObject(propertyIt.next(), fieldInfo);
        return;
    }

    auto basicHandler = findIntegratedTypeHandler(metaType,
                                                  fieldFlags
                                                      .testFlag(QtProtobufPrivate::FieldFlag::NonPacked));
    if (basicHandler) {
        bool serializeUninitialized = isOneofOrOptionalField(fieldInfo);
        if (!basicHandler->isPresent(propertyValue) && !serializeUninitialized) {
            return;
        }

        QByteArray header = QProtobufSerializerPrivate::encodeHeader(fieldInfo.fieldNumber(),
                                                                     basicHandler->wireType);
        result.append(basicHandler->serializer(propertyValue, header));
        return;
    }

    auto handler = QtProtobufPrivate::findHandler(metaType);
    if (!handler.serializer) {
        qProtoWarning() << "No serializer for type" << propertyValue.typeName();
        return;
    }

    handler.serializer([this](const QProtobufMessage *message,
                              const QProtobufFieldInfo
                                  &fieldInfo) { this->serializeObject(message, fieldInfo); },
                       propertyValue.constData(), fieldInfo);
}

bool QProtobufSerializerPrivate::deserializeProperty(QProtobufMessage *message)
{
    Q_ASSERT(message != nullptr);
    Q_ASSERT(it.isValid() && it.bytesLeft() > 0);
    //Each iteration we expect iterator is setup to beginning of next chunk
    int fieldNumber = QtProtobuf::InvalidFieldNumber;
    QtProtobuf::WireTypes wireType = QtProtobuf::WireTypes::Unknown;
    const QProtobufSelfcheckIterator itBeforeHeader = it; // copy this, we may need it later
    if (!QProtobufSerializerPrivate::decodeHeader(it, fieldNumber, wireType)) {
        setDeserializationError(
                QAbstractProtobufSerializer::Error::InvalidHeader,
                QCoreApplication::translate("QtProtobuf",
                                     "Message received doesn't contain valid header byte."));
        return false;
    }

    auto ordering = message->propertyOrdering();
    Q_ASSERT(ordering != nullptr);

    int index = ordering->indexOfFieldNumber(fieldNumber);
    if (index == -1) {
        // This is an unknown field, it may have been added in a later revision
        // of the Message we are currently deserializing. We must store the
        // bytes for this field and re-emit them later if this message is
        // serialized again.
        qsizetype length = std::distance(itBeforeHeader, it); // size of header
        length += QProtobufSerializerPrivate::skipSerializedFieldBytes(it, wireType);

        if (!it.isValid()) {
            setUnexpectedEndOfStreamError();
            return false;
        }

        if (preserveUnknownFields) {
            QProtobufMessagePrivate::storeUnknownEntry(message,
                                                       QByteArrayView(itBeforeHeader.data(),
                                                                      length),
                                                       fieldNumber);
        }
        return true;
    }

    QProtobufFieldInfo fieldInfo(*ordering, index);
    if (cachedIndex != index) {
        if (!storeCachedValue(message))
            return false;

        cachedPropertyValue = QtProtobufSerializerHelpers::messageProperty(message, fieldInfo,
                                                                           true);
        cachedIndex = index;
    }
    QMetaType metaType = cachedPropertyValue.metaType();

    qProtoDebug() << "wireType:" << wireType << "metaType:" << metaType.name()
                  << "currentByte:" << QString::number((*it), 16);

    if (metaType.flags() & QMetaType::IsPointer) {
        auto *messageProperty = cachedPropertyValue.value<QProtobufMessage *>();
        Q_ASSERT(messageProperty != nullptr);
        return deserializeObject(messageProperty);
    }

    const auto fieldFlags = fieldInfo.fieldFlags();
    if (fieldFlags.testFlag(QtProtobufPrivate::FieldFlag::Enum)) {
        if (fieldFlags.testFlag(QtProtobufPrivate::FieldFlag::Repeated)) {
            auto intList = cachedPropertyValue.value<QList<QtProtobuf::int64>>();
            if (deserializeEnumList(intList)) {
                cachedPropertyValue.setValue(intList);
                return true;
            }
            return false;
        } else {
            if (deserializeBasic<QtProtobuf::int64>(it, cachedPropertyValue))
                return true;
            return false;
        }
    }

    bool isNonPacked = fieldFlags.testFlag(QtProtobufPrivate::FieldFlag::NonPacked);
    auto basicHandler = findIntegratedTypeHandler(metaType, isNonPacked);

    if (basicHandler) {
        if (basicHandler->wireType != wireType) {
            // If the handler wiretype mismatches the wiretype received from the
            // wire that most probably means that we received the list in wrong
            // format. This can happen because of mismatch of the field packed
            // option in the protobuf schema on the wire ends. Invert the
            // isNonPacked flag and try to find the handler one more time to make
            // sure that we cover this exceptional case.
            // See the conformance tests
            // Required.Proto3.ProtobufInput.ValidDataRepeated.*.UnpackedInput
            // for details.
            basicHandler = findIntegratedTypeHandler(metaType, !isNonPacked);
            if (!basicHandler || basicHandler->wireType != wireType) {
                setDeserializationError(
                        QAbstractProtobufSerializer::Error::InvalidHeader,
                        QCoreApplication::translate("QtProtobuf",
                                                    "Message received has invalid wiretype for the "
                                                    "field number %1. Expected %2, received %3")
                                .arg(fieldNumber)
                                .arg(basicHandler ? static_cast<int>(basicHandler->wireType) : -1)
                                .arg(static_cast<int>(wireType)));
                return false;
            }
        }

        if (!basicHandler->deserializer(it, cachedPropertyValue)) {
            setUnexpectedEndOfStreamError();
            return false;
        }
        return true;
    }

    if (cachedPropertyValue.canView(QMetaType::fromType<QProtobufRepeatedIterator>())) {
        if (!cachedRepeatedIterator.isValid()) {
            cachedRepeatedIterator = cachedPropertyValue.view<QProtobufRepeatedIterator>();
        }
        if (deserializeObject(cachedRepeatedIterator.addNext())) {
            cachedRepeatedIterator.push();
            return true;
        }
        return false;
    }

    auto handler = QtProtobufPrivate::findHandler(metaType);
    if (!handler.deserializer) {
        qProtoWarning() << "No deserializer for type" << metaType.name();
        QString error = QString::fromUtf8("No deserializer is registered for type %1")
                            .arg(QString::fromUtf8(metaType.name()));
        setDeserializationError(QAbstractProtobufSerializer::Error::UnknownType,
                                QCoreApplication::translate("QtProtobuf", error.toUtf8().data()));
        return false;
    }
    handler.deserializer([this](QProtobufMessage
                                    *message) { return this->deserializeObject(message); },
                         cachedPropertyValue.data());

    return true;
}

bool QProtobufSerializerPrivate::storeCachedValue(QProtobufMessage *message)
{
    bool ok = true;
    if (cachedIndex >= 0 && !cachedPropertyValue.isNull()) {
        const auto *ordering = message->propertyOrdering();
        QProtobufFieldInfo fieldInfo(*ordering, cachedIndex);
        ok = QtProtobufSerializerHelpers::setMessageProperty(message, fieldInfo,
                                                             cachedPropertyValue);

        clearCachedValue();
    }
    return ok;
}

void QProtobufSerializerPrivate::clearCachedValue()
{
    cachedPropertyValue.clear();
    cachedIndex = -1;
    cachedRepeatedIterator = QProtobufRepeatedIterator();
}

/*!
   Returns the last deserialization error for the serializer instance.
   \sa lastErrorString()
*/
QAbstractProtobufSerializer::Error QProtobufSerializer::lastError() const
{
    return d_ptr->lastError;
}

/*!
   Returns the last deserialization error string for the serializer instance.
   \sa lastError()
*/
QString QProtobufSerializer::lastErrorString() const
{
    return d_ptr->lastErrorString;
}

void QProtobufSerializerPrivate::setDeserializationError(
        QAbstractProtobufSerializer::Error error, const QString &errorString)
{
    lastError = error;
    lastErrorString = errorString;
}

/*!
    Controls whether the unknown fields received from the wire should be
    stored in the resulting message or if it should be omitted, based
    on \a preserveUnknownFields.
    \since 6.7
*/
void QProtobufSerializer::shouldPreserveUnknownFields(bool preserveUnknownFields)
{
    d_ptr->preserveUnknownFields = preserveUnknownFields;
}

QT_END_NAMESPACE
