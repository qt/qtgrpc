// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>, Viktor Kopp <vifactor@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFSERIALIZER_P_H
#define QPROTOBUFSERIALIZER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qstring.h>
#include <QtCore/qbytearray.h>
#include <QtCore/private/qbytearray_p.h>
#include <QtCore/qhash.h>
#include <QtCore/qendian.h>
#include <QtCore/qvariant.h>

#include <QtProtobuf/qprotobufselfcheckiterator.h>
#include <QtProtobuf/qprotobufserializer.h>
#include <QtProtobuf/qtprotobuftypes.h>
#include <QtProtobuf/private/qtprotobuflogging_p.h>
#include <QtProtobuf/qabstractprotobufserializer.h>

#include <optional>
#include <type_traits>

QT_BEGIN_NAMESPACE

using QtProtobufPrivate::QProtobufSelfcheckIterator;

class QProtobufSerializerPrivate
{
    Q_DISABLE_COPY_MOVE(QProtobufSerializerPrivate)
public:

    // Serializer is interface function for serialize method
    using Serializer = QByteArray(*)(const QVariant &, int &);
    // Deserializer is interface function for deserialize method
    using Deserializer = bool(*)(QProtobufSelfcheckIterator &, QVariant &);

    // SerializationHandlers contains set of objects that required for class serializaion/deserialization
    struct ProtobufSerializationHandler {
        Serializer serializer; // serializer assigned to class
        Deserializer deserializer; // deserializer assigned to class
        QtProtobuf::WireTypes type; // Serialization WireType
    };

    using ProtobufSerializerRegistry = QHash<QMetaType, ProtobufSerializationHandler>;

    explicit QProtobufSerializerPrivate(QProtobufSerializer *q);
    ~QProtobufSerializerPrivate() = default;
    //###########################################################################
    //                               Serializers
    //###########################################################################

    template <typename V,
              typename std::enable_if_t<std::is_integral<V>::value
                                        && std::is_unsigned<V>::value, int> = 0>
    static QByteArray serializeVarintCommon(const V &value) {
        qProtoDebug() << "value" << value;
        V varint = value;
        QByteArray result;

        while (varint != 0) {
            //Put 7 bits to result buffer and mark as "not last" (0b10000000)
            result.append((varint & 0b01111111) | 0b10000000);
            //Divide values to chunks of 7 bits and move to next chunk
            varint >>= 7;
        }

        if (result.isEmpty())
            result.append('\0');

        result.data()[result.size() - 1] &= ~0b10000000;
        return result;
    }

    //---------------Integral and floating point types serializers---------------
    /*
     Serialization of fixed-length primitive types

     Natural layout of bits is used: value is encoded in a byte array same way as it is located in memory

     value: Value to serialize
     outFieldIndex: Index of the value in parent structure (ignored)
     returns a byte array with 'value' encoded
    */
    template <typename V,
              typename std::enable_if_t<std::is_floating_point<V>::value, int> = 0>
    static QByteArray serializeBasic(const V &value, int &/*outFieldIndex*/) {
        qProtoDebug() << "value" << value;

        //Reserve required number of bytes
        QByteArray result(sizeof(V), Qt::Uninitialized);
        qToUnaligned(qToLittleEndian(value), result.data());
        return result;
    }

    /*
        Serialization of fixed length integral types

        value: Value to serialize
        outFieldIndex: Index of the value in parent structure (ignored)
        Returns a byte array with 'value' encoded
    */
    template <typename V,
              typename std::enable_if_t<std::is_same<V, QtProtobuf::fixed32>::value
                                        || std::is_same<V, QtProtobuf::fixed64>::value
                                        || std::is_same<V, QtProtobuf::sfixed32>::value
                                        || std::is_same<V, QtProtobuf::sfixed64>::value, int> = 0>
    static QByteArray serializeBasic(const V &value, int &/*outFieldIndex*/) {
        qProtoDebug() << "value" << value;

        //Reserve required number of bytes
        QByteArray result(sizeof(V), Qt::Uninitialized);
        qToUnaligned(qToLittleEndian(value), result.data());
        return result;
    }

    /*
        Serialization of signed integral types
        Uses ZigZag encoding[0] first then apply serialization as for unsigned integral types

        [0]: https://developers.google.com/protocol-buffers/docs/encoding

        value: Value to serialize
        outFieldIndex: Index of the value in parent structure
        Returns a byte array with 'value' encoded
    */
    template<typename V,
             typename std::enable_if_t<std::is_integral<V>::value && std::is_signed_v<V>,
                                       int> = 0>
    static QByteArray serializeBasic(const V &value, int &outFieldIndex)
    {
        qProtoDebug() << "value" << value;
        using UV = std::make_unsigned_t<V>;
        UV uValue = 0;

        //Use ZigZag convertion first and apply unsigned variant next
        V zigZagValue = (value << 1) ^ (value >> (sizeof(UV) * 8 - 1));
        uValue = static_cast<UV>(zigZagValue);
        return serializeBasic(uValue, outFieldIndex);
    }

    template <typename V,
              typename std::enable_if_t<std::is_same<V, QtProtobuf::int32>::value
                                        || std::is_same<V, QtProtobuf::int64>::value, int> = 0>
    static QByteArray serializeBasic(const V &value, int &outFieldIndex) {
        qProtoDebug() << "value" << value;
        // Non-ZigZag signed integers should always be (de)serialized as the
        // QtProtobuf::uint64
        return serializeBasic(static_cast<QtProtobuf::uint64>(value), outFieldIndex);
    }

    /*!
        Serialization of unsigned integral types

        Use Varint encoding[0]:
        "Varints are a method of serializing integers using one or more bytes. Smaller numbers
        [regardless its type] take a smaller number of bytes."

        [0]: https://developers.google.com/protocol-buffers/docs/encoding

        value: Value to serialize
        outFieldIndex: Index of the value in parent structure
        Returns a byte array with 'value' encoded
    */
    template <typename V,
              typename std::enable_if_t<std::is_integral<V>::value
                                        && std::is_unsigned<V>::value, int> = 0>
    static QByteArray serializeBasic(const V &value, int &outFieldIndex) {
        qProtoDebug() << "value" << value;
        quint64 varint = quint64(value);
        QByteArray result;

        while (varint != 0) {
            //Put 7 bits to result buffer and mark as "not last" (0b10000000)
            result.append((varint & 0b01111111) | 0b10000000);
            //Divide values to chunks of 7 bits and move to next chunk
            varint >>= 7;
        }

        // Invalidate field index in case if serialized result is empty
        // NOTE: the field will not be sent if its index is equal to NotUsedFieldIndex
        if (result.size() == 0) {
            outFieldIndex = QtProtobufPrivate::NotUsedFieldIndex;
        } else {
            //Mark last chunk as last by clearing last bit
            result.data()[result.size() - 1] &= ~0b10000000;
        }
        return result;
    }

    //------------------QString and QByteArray types serializers-----------------
    template <typename V,
              typename std::enable_if_t<std::is_same<V, QString>::value, int> = 0>
    static QByteArray serializeBasic(const V &value, int &/*outFieldIndex*/) {
        return serializeLengthDelimited(value.toUtf8());
    }

    template <typename V,
              typename std::enable_if_t<std::is_same<V, QByteArray>::value, int> = 0>
    static QByteArray serializeBasic(const V &value, int &/*outFieldIndex*/) {
        return serializeLengthDelimited(value);
    }

    //--------------------------List types serializers---------------------------
    template<typename V,
             typename std::enable_if_t<!(std::is_same<V, QString>::value
                                       || std::is_base_of<QObject, V>::value), int> = 0>
    static QByteArray serializeListType(const QList<V> &listValue, int &outFieldIndex) {
        qProtoDebug("listValue.count %" PRIdQSIZETYPE " outFieldIndex %d", listValue.count(),
                    outFieldIndex);

        if (listValue.count() <= 0) {
            outFieldIndex = QtProtobufPrivate::NotUsedFieldIndex;
            return QByteArray();
        }

        int empty = QtProtobufPrivate::NotUsedFieldIndex;
        QByteArray serializedList;
        for (auto &value : listValue) {
            QByteArray element = serializeBasic<V>(value, empty);
            if (element.isEmpty())
                element.append('\0');
            serializedList.append(element);
        }
        //If internal field type is not LengthDelimited, exact amount of fields to be specified
        serializedList = prependLengthDelimitedSize(serializedList);
        return serializedList;
    }

    template<typename V,
             typename std::enable_if_t<std::is_same<V, QString>::value, int> = 0>
    static QByteArray serializeListType(const QStringList &listValue, int &outFieldIndex) {
        qProtoDebug("listValue.count %" PRIdQSIZETYPE " outFieldIndex %d", listValue.count(),
                    outFieldIndex);

        if (listValue.count() <= 0) {
            outFieldIndex = QtProtobufPrivate::NotUsedFieldIndex;
            return QByteArray();
        }

        QByteArray serializedList;
        for (const QString &value : listValue) {
            serializedList.append(QProtobufSerializerPrivate::encodeHeader(
                    outFieldIndex, QtProtobuf::WireTypes::LengthDelimited));
            serializedList.append(serializeLengthDelimited(value.toUtf8(), false));
        }

        outFieldIndex = QtProtobufPrivate::NotUsedFieldIndex;
        return serializedList;
    }

    //###########################################################################
    //                               Deserializers
    //###########################################################################
    template <typename V,
              typename std::enable_if_t<std::is_integral<V>::value
                                        && std::is_unsigned<V>::value, int> = 0>
    Q_REQUIRED_RESULT
    static std::optional<V> deserializeVarintCommon(QProtobufSelfcheckIterator &it) {
        qProtoDebug("currentByte: 0x%x", *it);

        quint64 value = 0;
        int k = 0;
        while (true) {
            if (it.bytesLeft() == 0)
                return std::nullopt;
            quint64 byte = quint64(static_cast<unsigned char>(*it));
            value += (byte & 0b01111111) << k;
            k += 7;
            if (((*it) & 0b10000000) == 0)
                break;
            ++it;
        }
        ++it;
        return { V(value) };
    }

    //-------------Integral and floating point types deserializers---------------
    template <typename V,
              typename std::enable_if_t<std::is_floating_point<V>::value
                                        || std::is_same<V, QtProtobuf::fixed32>::value
                                        || std::is_same<V, QtProtobuf::fixed64>::value
                                        || std::is_same<V, QtProtobuf::sfixed32>::value
                                        || std::is_same<V, QtProtobuf::sfixed64>::value, int> = 0>
    Q_REQUIRED_RESULT
    static bool deserializeBasic(QProtobufSelfcheckIterator &it, QVariant &variantValue) {
        qsizetype size = sizeof(V);
        if (it.bytesLeft() < size)
            return false;
        qProtoDebug("currentByte: 0x%x", *it);
        variantValue = QVariant::fromValue(qFromLittleEndian(qFromUnaligned<V>(it.data())));
        it += size;
        return true;
    }

    template <typename V,
              typename std::enable_if_t<std::is_integral<V>::value
                                        && std::is_unsigned<V>::value, int> = 0>
    Q_REQUIRED_RESULT
    static bool deserializeBasic(QProtobufSelfcheckIterator &it, QVariant &variantValue) {
        qProtoDebug("currentByte: 0x%x", *it);

        auto opt = deserializeVarintCommon<V>(it);
        if (!opt)
            return false;
        variantValue = QVariant::fromValue(opt.value());
        return true;
    }

    template<typename V,
             typename std::enable_if_t<std::is_integral<V>::value && std::is_signed_v<V>, int> = 0>
    Q_REQUIRED_RESULT static bool deserializeBasic(QProtobufSelfcheckIterator &it,
                                                   QVariant &variantValue)
    {
        qProtoDebug("currentByte: 0x%x", *it);
        using UV = std::make_unsigned_t<V>;
        auto opt = deserializeVarintCommon<UV>(it);
        if (!opt)
            return false;
        UV unsignedValue = opt.value();
        V value = (unsignedValue >> 1) ^ (-1 * (unsignedValue & 1));
        variantValue = QVariant::fromValue<V>(value);
        return true;
    }

    template<typename V,
             typename std::enable_if_t<std::is_same<QtProtobuf::int32, V>::value
                                               || std::is_same<QtProtobuf::int64, V>::value,
                                       int> = 0>
    Q_REQUIRED_RESULT static bool deserializeBasic(QProtobufSelfcheckIterator &it,
                                                   QVariant &variantValue)
    {
        qProtoDebug("currentByte: 0x%x", *it);
        // Non-ZigZag signed integers should always be (de)serialized as the
        // QtProtobuf::uint64
        auto opt = deserializeVarintCommon<QtProtobuf::uint64>(it);
        if (!opt)
            return false;
        QtProtobuf::uint64 unsignedValue = opt.value();
        V value = static_cast<V>(unsignedValue);
        variantValue = QVariant::fromValue(value);
        return true;
    }

    //-----------------QString and QByteArray types deserializers----------------
    template<typename V, typename std::enable_if_t<std::is_same<QByteArray, V>::value, int> = 0>
    Q_REQUIRED_RESULT
    static bool deserializeBasic(QProtobufSelfcheckIterator &it, QVariant &variantValue)
    {
        std::optional<QByteArray> result = deserializeLengthDelimited(it);
        if (!result) {
            variantValue = QVariant();
            return false;
        }
        variantValue = QVariant::fromValue(*result);
        return true;
    }

    template<typename V, typename std::enable_if_t<std::is_same<QString, V>::value, int> = 0>
    Q_REQUIRED_RESULT
    static bool deserializeBasic(QProtobufSelfcheckIterator &it, QVariant &variantValue)
    {
        std::optional<QByteArray> result = deserializeLengthDelimited(it);
        if (!result) {
            variantValue = QVariant();
            return false;
        }
        variantValue = QVariant::fromValue(QString::fromUtf8(*result));
        return true;
    }

    //-------------------------List types deserializers--------------------------
    template <typename V,
              typename std::enable_if_t<!std::is_base_of<QObject, V>::value, int> = 0>
    Q_REQUIRED_RESULT
    static bool deserializeList(QProtobufSelfcheckIterator &it, QVariant &previousValue) {
        qProtoDebug("currentByte: 0x%x", *it);

        QList<V> out;
        auto opt = deserializeVarintCommon<QtProtobuf::uint64>(it);
        if (!opt)
            return false;
        quint64 count = opt.value();
        if (count > quint64(std::numeric_limits<qsizetype>::max()))
            return false;
        QProtobufSelfcheckIterator lastVarint = it + count;
        if (!lastVarint.isValid())
            return false;
        while (it != lastVarint) {
            QVariant variantValue;
            if (!deserializeBasic<V>(it, variantValue))
                return false;
            out.append(variantValue.value<V>());
        }
        previousValue.setValue(out);
        return true;
    }

    //###########################################################################
    //                             Common functions
    //###########################################################################
    static std::optional<QByteArray> deserializeLengthDelimited(QProtobufSelfcheckIterator &it) {
        if (it.bytesLeft() == 0)
            return std::nullopt;
        qProtoDebug("currentByte: 0x%x", *it);

        auto opt = deserializeVarintCommon<QtProtobuf::uint64>(it);
        if (!opt)
            return std::nullopt;
        quint64 length = opt.value();
        if (!it.isValid() || quint64(it.bytesLeft()) < length || length > quint64(MaxByteArraySize))
            return std::nullopt;
        QByteArray result(it.data(), qsizetype(length));
        it += length;
        return {result};
    }

    static QByteArray serializeLengthDelimited(const QByteArray &data, bool optional = true) {
        qProtoDebug("data.size: %" PRIdQSIZETYPE " data: %s", data.size(), data.toHex().data());
        if (optional && data.isEmpty())
            return {};

        QByteArray result(data);
        //Varint serialize field size and apply result as starting point
        result = prependLengthDelimitedSize(result);
        return result;
    }

    Q_REQUIRED_RESULT
    static bool decodeHeader(QProtobufSelfcheckIterator &it, int &fieldIndex,
                             QtProtobuf::WireTypes &wireType);
    static QByteArray encodeHeader(int fieldIndex, QtProtobuf::WireTypes wireType);

    /*!
        Gets length of a byte-array and prepends to it its serialized length value
        using the appropriate serialization algorithm

        Returns 'data' with its length prepended
    */
    static QByteArray prependLengthDelimitedSize(const QByteArray &data)
    {
        return serializeVarintCommon<uint32_t>(data.size()) + data;
    }

    template <typename T,
               QByteArray(*s)(const T &, int &)>
    static QByteArray serializeWrapper(const QVariant &variantValue, int &fieldIndex) {
        if (variantValue.isNull()) {
            fieldIndex = QtProtobufPrivate::NotUsedFieldIndex;
            return QByteArray();
        }
        return s(variantValue.value<T>(), fieldIndex);
    }

    template <typename T, QByteArray (*s)(const T &, int &), Deserializer d,
              QtProtobuf::WireTypes type,
              typename std::enable_if_t<!std::is_base_of<QObject, T>::value, int> = 0>
    static void wrapSerializer()
    {
        handlers[QMetaType::fromType<T>()] = {
                serializeWrapper<T, s>,
                d,
                type
        };
    }

    template <typename T, typename S, QByteArray (*s)(const S &, int &), Deserializer d,
              QtProtobuf::WireTypes type,
              typename std::enable_if_t<!std::is_base_of<QObject, T>::value, int> = 0>
    static void wrapSerializer()
    {
        handlers[QMetaType::fromType<T>()] = {
                serializeWrapper<S, s>,
                d,
                type
        };
    }

    // this set of 3 methods is used to skip bytes corresponding to an unexpected property
    // in a serialized message met while the message being deserialized
    static qsizetype skipSerializedFieldBytes(QProtobufSelfcheckIterator &it,
                                              QtProtobuf::WireTypes type);
    static void skipVarint(QProtobufSelfcheckIterator &it);
    static void skipLengthDelimited(QProtobufSelfcheckIterator &it);

    QByteArray
    serializeProperty(const QVariant &propertyValue,
                      const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo);
    Q_REQUIRED_RESULT
    bool deserializeProperty(QObject *object, const QtProtobufPrivate::QProtobufPropertyOrdering &ordering, QProtobufSelfcheckIterator &it);

    void setDeserializationError(QAbstractProtobufSerializer::DeserializationError error, const QString &errorString);
    void clearError();
    void setUnexpectedEndOfStreamError();

    Q_REQUIRED_RESULT
    bool deserializeMapPair(QVariant &key, QVariant &value, QProtobufSelfcheckIterator &it);

    QAbstractProtobufSerializer::DeserializationError deserializationError
        = QAbstractProtobufSerializer::NoDeserializerError;
    QString deserializationErrorString;

private:
    static ProtobufSerializerRegistry handlers;
    QProtobufSerializer *q_ptr;
};

//###########################################################################
//                             Common functions
//###########################################################################

/*!
    Encode a property field index and its type into output bytes.

    Header byte
    Meaning    |  Field index  |  Type
    ---------- | ------------- | --------
    bit number | 7  6  5  4  3 | 2  1  0

    fieldIndex: The index of a property in parent object
    wireType: Serialization type used for the property with index @p fieldIndex

    Returns a varint-encoded fieldIndex and wireType
 */
inline QByteArray QProtobufSerializerPrivate::encodeHeader(int fieldIndex,
                                                           QtProtobuf::WireTypes wireType)
{
    uint32_t header = (fieldIndex << 3) | int(wireType);
    return serializeVarintCommon<uint32_t>(header);
}

/*!
    Decode a property field index and its serialization type from input bytes

    Iterator: that points to header with encoded field index and serialization type
    fieldIndex: Decoded index of a property in parent object
    wireType: Decoded serialization type used for the property with index
 *
 \return true if both decoded wireType and fieldIndex have "allowed" values and false, otherwise
 */
inline bool QProtobufSerializerPrivate::decodeHeader(QProtobufSelfcheckIterator &it,
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

QT_END_NAMESPACE

#endif // QPROTOBUFSERIALIZER_P_H
