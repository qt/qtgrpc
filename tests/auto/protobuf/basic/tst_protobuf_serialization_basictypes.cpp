// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2022 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "basicmessages.qpb.h"
#include "fieldindexrange.qpb.h"

#include <QProtobufSerializer>
#include <QTest>

#include <limits>

using namespace Qt::Literals::StringLiterals;

class QtProtobufTypesSerializationTest : public QObject
{
    Q_OBJECT
private slots:
    void init() { m_serializer.reset(new QProtobufSerializer); }

    void BoolMessageSerializeTest_data();
    void BoolMessageSerializeTest();
    void IntMessageSerializeTest_data();
    void IntMessageSerializeTest();
    void UIntMessageSerializeTest_data();
    void UIntMessageSerializeTest();
    void SIntMessageSerializeTest_data();
    void SIntMessageSerializeTest();
    void Int64MessageSerializeTest_data();
    void Int64MessageSerializeTest();
    void UInt64MessageSerializeTest_data();
    void UInt64MessageSerializeTest();
    void SInt64MessageSerializeTest_data();
    void SInt64MessageSerializeTest();
    void FixedInt32MessageSerializeTest_data();
    void FixedInt32MessageSerializeTest();
    void FixedInt64MessageSerializeTest_data();
    void FixedInt64MessageSerializeTest();
    void SFixedInt32MessageSerializeTest_data();
    void SFixedInt32MessageSerializeTest();
    void SFixedInt64MessageSerializeTest_data();
    void SFixedInt64MessageSerializeTest();
    void FloatMessageSerializeTest_data();
    void FloatMessageSerializeTest();
    void DoubleMessageSerializeTest_data();
    void DoubleMessageSerializeTest();
    void StringMessageSerializeTest();
    void ComplexTypeSerializeTest_data();
    void ComplexTypeSerializeTest();
    void EmptyBytesMessageTest();
    void EmptyStringMessageTest();
    void FieldIndexRangeTest();
    void OneofMessageEmptyTest();
    void OneofMessageClearTest();
    void OneofMessageIntTest();
    void OneofMessageComplexTest();

private:
    std::unique_ptr<QProtobufSerializer> m_serializer;
};

using namespace qtprotobufnamespace::tests;

constexpr qsizetype Fixed32IntSize = 5;
constexpr qsizetype Fixed64IntSize = 9;

template <typename T>
struct SerializeData
{
    T value;
    qsizetype expectedSize;
    QByteArray expectedData;
    std::optional<QString> name = std::nullopt;
};

enum class IntTypes : uint8_t {
    Int = 0,
    UInt,
    SInt,
    Fixed,
    SFixed,
};

template <typename T>
QList<SerializeData<T>> getCommonIntValues(IntTypes type)
{
    QList<SerializeData<T>> data{ { (T)0, 0, ""_ba, u"empty_data"_s } };
    switch (type) {
    case IntTypes::Int:
        data.append({ { (T)-1, 11ll, "08ffffffffffffffffff01"_ba },
                      { (T)-462, 11ll, "08b2fcffffffffffffff01"_ba },
                      { (T)-63585, 11ll, "089f8ffcffffffffffff01"_ba } });
        Q_FALLTHROUGH(); // Got negative values, fallthrough to positive
    case IntTypes::UInt:
        data.append({ { (T)15, 2ll, "080f"_ba },
                      { (T)300, 3ll, "08ac02"_ba },
                      { (T)65545, 4ll, "08898004"_ba } });
        break;
    case IntTypes::SInt:
        data.append({ { (T)15, 2ll, "081e"_ba },
                      { (T)300, 3ll, "08d804"_ba },
                      { (T)65545, 4ll, "08928008"_ba },
                      { (T)-1, 2ll, "0801"_ba },
                      { (T)-462, 3ll, "089b07"_ba },
                      { (T)-63585, 4ll, "08c1e107"_ba } });
        break;
    case IntTypes::SFixed:
        data.append({ { (T)-1, Fixed32IntSize, "0dffffffff"_ba },
                      { (T)-462, Fixed32IntSize, "0d32feffff"_ba },
                      { (T)-63585, Fixed32IntSize, "0d9f07ffff"_ba } });
        Q_FALLTHROUGH(); // Got negative values, fallthrough to positive
    case IntTypes::Fixed:
        data.append({ { (T)15, Fixed32IntSize, "0d0f000000"_ba },
                      { (T)300, Fixed32IntSize, "0d2c010000"_ba },
                      { (T)65545, Fixed32IntSize, "0d09000100"_ba } });
        break;
    }
    if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
        if (type == IntTypes::Fixed || type == IntTypes::SFixed) {
            // Do not merge with 32-bit version
            data = { { (T)0, 0, ""_ba, u"empty_data"_s },
                     { (T)15, Fixed64IntSize, "090f00000000000000"_ba },
                     { (T)300, Fixed64IntSize, "092c01000000000000"_ba },
                     { (T)65545, Fixed64IntSize, "090900010000000000"_ba } };
        }
        if (type == IntTypes::SFixed) {
            data.append({ { (T)-1, Fixed64IntSize, "09ffffffffffffffff"_ba },
                          { (T)-462, Fixed64IntSize, "0932feffffffffffff"_ba },
                          { (T)-63585, Fixed64IntSize, "099f07ffffffffffff"_ba } });
        }
    }
    return data;
}

#define INT_MIN_SDATA(type, size, data) \
  std::numeric_limits<type>::min(), size, data, QString("%1_min").arg(#type)
#define INT_MAX_SDATA(type, size, data) \
  std::numeric_limits<type>::max(), size, data, QString("%1_max").arg(#type)
#define BELOW_INT_MAX_SDATA(type, size, data) \
  std::numeric_limits<type>::min() - 1, size, data, QString("below_%1_min").arg(#type)
#define OVER_INT_MAX_SDATA(type, size, data) \
  std::numeric_limits<type>::max() + 1, size, data, QString("over_%1_max").arg(#type)

template <typename T>
constexpr QList<SerializeData<T>> getInt32LimitValues(IntTypes type)
{
    switch (type) {
    case IntTypes::Int:
        return { { (T)INT_MAX_SDATA(int8_t, 2ll, "087f"_ba) },
                 { (T)INT_MAX_SDATA(int16_t, 4ll, "08ffff01"_ba) },
                 { (T)INT_MAX_SDATA(int32_t, 6ll, "08ffffffff07"_ba) },
                 { (T)OVER_INT_MAX_SDATA(int8_t, 3ll, "088001"_ba) },
                 { (T)OVER_INT_MAX_SDATA(int16_t, 4ll, "08808002"_ba) },
                 { (T)INT_MIN_SDATA(int8_t, 11ll, "0880ffffffffffffffff01"_ba) },
                 { (T)INT_MIN_SDATA(int16_t, 11ll, "088080feffffffffffff01"_ba) },
                 { (T)INT_MIN_SDATA(int32_t, 11ll, "0880808080f8ffffffff01"_ba) },
                 { (T)BELOW_INT_MAX_SDATA(int8_t, 11ll, "08fffeffffffffffffff01"_ba) },
                 { (T)BELOW_INT_MAX_SDATA(int16_t, 11ll, "08fffffdffffffffffff01"_ba) } };
    case IntTypes::UInt:
        return { { (T)INT_MAX_SDATA(uint8_t, 3ll, "08ff01"_ba) },
                 { (T)INT_MAX_SDATA(uint16_t, 4ll, "08ffff03"_ba) },
                 { (T)INT_MAX_SDATA(uint32_t, 6ll, "08ffffffff0f"_ba) },
                 { (T)OVER_INT_MAX_SDATA(uint8_t, 3ll, "088002"_ba) },
                 { (T)OVER_INT_MAX_SDATA(uint16_t, 4ll, "08808004"_ba) } };
    case IntTypes::SInt:
        return { { (T)INT_MAX_SDATA(int8_t, 3ll, "08fe01"_ba) },
                 { (T)INT_MAX_SDATA(int16_t, 4ll, "08feff03"_ba) },
                 { (T)INT_MAX_SDATA(int32_t, 6ll, "08feffffff0f"_ba) },
                 { (T)OVER_INT_MAX_SDATA(int8_t, 3ll, "088002"_ba) },
                 { (T)OVER_INT_MAX_SDATA(int16_t, 4ll, "08808004"_ba) },
                 { (T)INT_MIN_SDATA(int8_t, 3ll, "08ff01"_ba) },
                 { (T)INT_MIN_SDATA(int16_t, 4ll, "08ffff03"_ba) },
                 { (T)INT_MIN_SDATA(int32_t, 6ll, "08ffffffff0f"_ba) },
                 { (T)BELOW_INT_MAX_SDATA(int8_t, 3ll, "088102"_ba) },
                 { (T)BELOW_INT_MAX_SDATA(int16_t, 4ll, "08818004"_ba) } };
    case IntTypes::Fixed:
        if constexpr (std::is_same_v<T, uint32_t>) {
            return { { (T)INT_MAX_SDATA(uint8_t, Fixed32IntSize, "0dff000000"_ba) },
                     { (T)INT_MAX_SDATA(uint16_t, Fixed32IntSize, "0dffff0000"_ba) },
                     { (T)INT_MAX_SDATA(uint32_t, Fixed32IntSize, "0dffffffff"_ba) },
                     { (T)OVER_INT_MAX_SDATA(uint8_t, Fixed32IntSize, "0d00010000"_ba) },
                     { (T)OVER_INT_MAX_SDATA(uint16_t, Fixed32IntSize, "0d00000100"_ba) } };
        }
    case IntTypes::SFixed:
        if constexpr (std::is_same_v<T, int32_t>) {
            return { { (T)INT_MAX_SDATA(int8_t, Fixed32IntSize, "0d7f000000"_ba) },
                     { (T)INT_MAX_SDATA(int16_t, Fixed32IntSize, "0dff7f0000"_ba) },
                     { (T)INT_MAX_SDATA(int32_t, Fixed32IntSize, "0dffffff7f"_ba) },
                     { (T)OVER_INT_MAX_SDATA(int8_t, Fixed32IntSize, "0d80000000"_ba) },
                     { (T)OVER_INT_MAX_SDATA(int16_t, Fixed32IntSize, "0d00800000"_ba) },
                     { (T)INT_MIN_SDATA(int8_t, Fixed32IntSize, "0d80ffffff"_ba) },
                     { (T)INT_MIN_SDATA(int16_t, Fixed32IntSize, "0d0080ffff"_ba) },
                     { (T)INT_MIN_SDATA(int32_t, Fixed32IntSize, "0d00000080"_ba) },
                     { (T)BELOW_INT_MAX_SDATA(int8_t, Fixed32IntSize, "0d7fffffff"_ba) },
                     { (T)BELOW_INT_MAX_SDATA(int16_t, Fixed32IntSize, "0dff7fffff"_ba) } };
        }
    }
    return {};
}

template <typename T>
constexpr QList<SerializeData<T>> getInt64LimitValues(IntTypes type)
{
    if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
        switch (type) {
        case IntTypes::Int:
            return { { (T)INT_MAX_SDATA(int64_t, 10ll, "08ffffffffffffffff7f"_ba) },
                     { (T)OVER_INT_MAX_SDATA(int32_t, 6ll, "088080808008"_ba) },
                     { (T)INT_MIN_SDATA(int64_t, 11ll, "0880808080808080808001"_ba) },
                     { (T)BELOW_INT_MAX_SDATA(int32_t, 11ll, "08fffffffff7ffffffff01"_ba) } };
        case IntTypes::UInt:
            return { { (T)INT_MAX_SDATA(int64_t, 10ll, "08ffffffffffffffff7f"_ba) },
                     { (T)OVER_INT_MAX_SDATA(int32_t, 6ll, "088080808008"_ba) } };
        case IntTypes::SInt:
            return { { (T)INT_MAX_SDATA(int64_t, 11ll, "08feffffffffffffffff01"_ba) },
                     { (T)OVER_INT_MAX_SDATA(int32_t, 6ll, "088080808010"_ba) },
                     { (T)INT_MIN_SDATA(int64_t, 11ll, "08ffffffffffffffffff01"_ba) },
                     { (T)BELOW_INT_MAX_SDATA(int32_t, 6ll, "088180808010"_ba) } };
        case IntTypes::Fixed:
            return { { (T)INT_MAX_SDATA(uint8_t, Fixed64IntSize, "09ff00000000000000"_ba) },
                     { (T)INT_MAX_SDATA(uint16_t, Fixed64IntSize, "09ffff000000000000"_ba) },
                     { (T)INT_MAX_SDATA(uint32_t, Fixed64IntSize, "09ffffffff00000000"_ba) },
                     { (T)INT_MAX_SDATA(uint64_t, Fixed64IntSize, "09ffffffffffffffff"_ba) },
                     { (T)OVER_INT_MAX_SDATA(uint8_t, Fixed64IntSize, "090001000000000000"_ba) },
                     { (T)OVER_INT_MAX_SDATA(uint16_t, Fixed64IntSize, "090000010000000000"_ba) },
                     { (T)OVER_INT_MAX_SDATA(uint32_t, Fixed64IntSize, "090000000001000000"_ba) } };
        case IntTypes::SFixed:
            return { { (T)INT_MAX_SDATA(int8_t, Fixed64IntSize, "097f00000000000000"_ba) },
                     { (T)INT_MAX_SDATA(int16_t, Fixed64IntSize, "09ff7f000000000000"_ba) },
                     { (T)INT_MAX_SDATA(int32_t, Fixed64IntSize, "09ffffff7f00000000"_ba) },
                     { (T)INT_MAX_SDATA(int64_t, Fixed64IntSize, "09ffffffffffffff7f"_ba) },
                     { (T)OVER_INT_MAX_SDATA(int8_t, Fixed64IntSize, "098000000000000000"_ba) },
                     { (T)OVER_INT_MAX_SDATA(int16_t, Fixed64IntSize, "090080000000000000"_ba) },
                     { (T)OVER_INT_MAX_SDATA(int32_t, Fixed64IntSize, "090000008000000000"_ba) },
                     { (T)INT_MIN_SDATA(int8_t, Fixed64IntSize, "0980ffffffffffffff"_ba) },
                     { (T)INT_MIN_SDATA(int16_t, Fixed64IntSize, "090080ffffffffffff"_ba) },
                     { (T)INT_MIN_SDATA(int32_t, Fixed64IntSize, "0900000080ffffffff"_ba) },
                     { (T)INT_MIN_SDATA(int64_t, Fixed64IntSize, "090000000000000080"_ba) },
                     { (T)BELOW_INT_MAX_SDATA(int8_t, Fixed64IntSize, "097fffffffffffffff"_ba) },
                     { (T)BELOW_INT_MAX_SDATA(int16_t, Fixed64IntSize, "09ff7fffffffffffff"_ba) },
                     { (T)BELOW_INT_MAX_SDATA(int32_t, Fixed64IntSize, "09ffffff7fffffffff"_ba) } };
        }
    }
    return {};
}

template <typename T>
QList<SerializeData<T>> getSerializeData(IntTypes type)
{
    return getCommonIntValues<T>(type) + getInt32LimitValues<T>(type)
            + getInt64LimitValues<T>(type);
}

template <typename T>
void generateTestData(IntTypes type)
{
    QTest::addColumn<T>("value");
    QTest::addColumn<qsizetype>("expectedSize");
    QTest::addColumn<QByteArray>("expectedData");

    const auto data = getSerializeData<T>(type);
    for (const auto &test : data) {
        auto testName = QString("value_%1")
                                .arg(test.name ? *(test.name) : QString::number(test.value));
        QTest::newRow(testName.toStdString().c_str())
                << test.value << test.expectedSize << test.expectedData;
    }
}

void QtProtobufTypesSerializationTest::BoolMessageSerializeTest_data()
{
    QTest::addColumn<bool>("value");
    QTest::addColumn<qsizetype>("expectedSize");
    QTest::addColumn<QByteArray>("expectedData");

    QTest::newRow("True") << true << qsizetype(2) << "0801"_ba;
    QTest::newRow("False") << false << qsizetype(0) << ""_ba;
}

void QtProtobufTypesSerializationTest::BoolMessageSerializeTest()
{
    QFETCH(const bool, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleBoolMessage test;
    test.setTestFieldBool(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::IntMessageSerializeTest_data()
{
    generateTestData<int32_t>(IntTypes::Int);
}

void QtProtobufTypesSerializationTest::IntMessageSerializeTest()
{
    QFETCH(const int32_t, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleIntMessage test;
    test.setTestFieldInt(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::UIntMessageSerializeTest_data()
{
    generateTestData<uint32_t>(IntTypes::UInt);
}

void QtProtobufTypesSerializationTest::UIntMessageSerializeTest()
{
    QFETCH(const uint32_t, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleUIntMessage test;
    test.setTestFieldInt(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::SIntMessageSerializeTest_data()
{
    generateTestData<int32_t>(IntTypes::SInt);
}

void QtProtobufTypesSerializationTest::SIntMessageSerializeTest()
{
    QFETCH(const int32_t, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleSIntMessage test;
    test.setTestFieldInt(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::Int64MessageSerializeTest_data()
{
    generateTestData<int64_t>(IntTypes::Int);
}
void QtProtobufTypesSerializationTest::Int64MessageSerializeTest()
{
    QFETCH(const int64_t, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleInt64Message test;
    test.setTestFieldInt(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::UInt64MessageSerializeTest_data()
{
    generateTestData<uint64_t>(IntTypes::UInt);
}
void QtProtobufTypesSerializationTest::UInt64MessageSerializeTest()
{
    QFETCH(const QtProtobuf::uint64, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleUInt64Message test;
    test.setTestFieldInt(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::SInt64MessageSerializeTest_data()
{
    generateTestData<int64_t>(IntTypes::SInt);
}
void QtProtobufTypesSerializationTest::SInt64MessageSerializeTest()
{
    QFETCH(const int64_t, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleSInt64Message test;
    test.setTestFieldInt(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::FixedInt32MessageSerializeTest_data()
{
    generateTestData<uint32_t>(IntTypes::Fixed);
}

void QtProtobufTypesSerializationTest::FixedInt32MessageSerializeTest()
{
    QFETCH(const uint32_t, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleFixedInt32Message test;
    test.setTestFieldFixedInt32(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::FixedInt64MessageSerializeTest_data()
{
    generateTestData<uint64_t>(IntTypes::Fixed);
}

void QtProtobufTypesSerializationTest::FixedInt64MessageSerializeTest()
{
    QFETCH(const uint64_t, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleFixedInt64Message test;
    test.setTestFieldFixedInt64(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::SFixedInt32MessageSerializeTest_data()
{
    generateTestData<int32_t>(IntTypes::SFixed);
}
void QtProtobufTypesSerializationTest::SFixedInt32MessageSerializeTest()
{
    QFETCH(const int32_t, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleSFixedInt32Message test;
    test.setTestFieldFixedInt32(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::SFixedInt64MessageSerializeTest_data()
{
    generateTestData<int64_t>(IntTypes::SFixed);
}

void QtProtobufTypesSerializationTest::SFixedInt64MessageSerializeTest()
{
    QFETCH(const int64_t, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleSFixedInt64Message test;
    test.setTestFieldFixedInt64(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::FloatMessageSerializeTest_data()
{
    QTest::addColumn<float>("value");
    QTest::addColumn<qsizetype>("expectedSize");
    QTest::addColumn<QByteArray>("expectedData");
    QTest::addColumn<std::optional<int>>("optionalAt0");

    constexpr qsizetype FloatMessageSize = 5;

    QTest::newRow("float_value_0_1")
            << 0.1f << FloatMessageSize << "3dcdcccc3d"_ba << std::optional<int>();
    QTest::newRow("float_value_min") << std::numeric_limits<float>::min() << FloatMessageSize
                                     << "3d00008000"_ba << std::optional<int>();
    QTest::newRow("float_value_max") << std::numeric_limits<float>::max() << FloatMessageSize
                                     << "3dffff7f7f"_ba << std::make_optional(0x3d);
    QTest::newRow("float_neg_value_4_2")
            << -4.2f << FloatMessageSize << "3d666686c0"_ba << std::make_optional(0x3d);
    QTest::newRow("float_neg_value_0_0") << -0.0f << qsizetype(0) << ""_ba << std::optional<int>();
}

void QtProtobufTypesSerializationTest::FloatMessageSerializeTest()
{
    QFETCH(const float, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);
    QFETCH(const std::optional<int>, optionalAt0);

    SimpleFloatMessage test;
    test.setTestFieldFloat(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    if (optionalAt0)
        QCOMPARE(result.at(0), *optionalAt0);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::DoubleMessageSerializeTest_data()
{
    QTest::addColumn<double>("value");
    QTest::addColumn<qsizetype>("expectedSize");
    QTest::addColumn<QByteArray>("expectedData");

    constexpr qsizetype DoubleMessageSize = 9;

    QTest::newRow("double_value_0_1") << 0.1 << DoubleMessageSize << "419a9999999999b93f"_ba;
    QTest::newRow("double_value_min")
            << std::numeric_limits<double>::min() << DoubleMessageSize << "410000000000001000"_ba;
    QTest::newRow("double_value_max")
            << std::numeric_limits<double>::max() << DoubleMessageSize << "41ffffffffffffef7f"_ba;
    QTest::newRow("double_neg_value_4_2") << -4.2 << DoubleMessageSize << "41cdcccccccccc10c0"_ba;
    QTest::newRow("double_value_0_0") << 0.0 << qsizetype(0) << ""_ba;
}

void QtProtobufTypesSerializationTest::DoubleMessageSerializeTest()
{
    QFETCH(const double, value);
    QFETCH(const qsizetype, expectedSize);
    QFETCH(const QByteArray, expectedData);

    SimpleDoubleMessage test;
    test.setTestFieldDouble(value);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.size(), expectedSize);
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::StringMessageSerializeTest()
{
    SimpleStringMessage test;
    test.setTestFieldString("qwerty");
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "3206717765727479"_ba);

    test.setTestFieldString("oepSNLIVG08UJpk2W7JtTkkBxyK06X0lQ6ML7IMd55K8XC1Tps"
                            "c1kDWym5v8z68b4FQu"
                            "p9O95QSgAvjHIA15OX6Bu68esbQFT9LPzSADJ6qSGBTYBHX5QS"
                            "Zg32trCdHMj80XuDHq"
                            "yBgM4uf6RKq2mgWb8Ovxxr0NwLxjHOfhJ8Mrfd2R7hbUgjespb"
                            "YoQhbgHEj2gKEV3Qvn"
                            "umYmrVXe1BkCzZhKVXodDhj0OfAE67viAy4i3Oag1hr1z4Azo8"
                            "O5Xq68POEZ1CsZPo2D"
                            "XNNR8ebVCdYOz0Q6JLPSl5jasLCFrQN7EiVNjQmCrSsZHRgLNy"
                            "lvgoEFxGYxXJ9gmK4m"
                            "r0OGdZcGJORRGZOQCpQMhXmhezFalNIJXMPPXaRVXiRhRAPCNU"
                            "Eie8DtaCWAMqz4nNUx"
                            "RMZ5UcXBXsXPshygzkyyXnNWTIDojFlrcsnKqSkQ1G6E85gSZb"
                            "tIYBh7sqO6GDXHjOrX"
                            "VaVCVCUubjcJKThlyslt29zHuIs5JGppXxX1");
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(),
             "3280046f6570534e4c4956473038554a706b3257374a74546"
             "b6b4278794b303658306c51364d4c37494d6435354b385843"
             "3154707363316b4457796d3576387a3638623446517570394"
             "f393551536741766a48494131354f58364275363865736251"
             "4654394c507a5341444a367153474254594248583551535a6"
             "7333274724364484d6a383058754448717942674d34756636"
             "524b71326d675762384f76787872304e774c786a484f66684"
             "a384d726664325237686255676a65737062596f5168626748"
             "456a32674b45563351766e756d596d7256586531426b437a5"
             "a684b56586f6444686a304f6641453637766941793469334f"
             "6167316872317a34417a6f384f3558713638504f455a31437"
             "35a506f3244584e4e52386562564364594f7a3051364a4c50"
             "536c356a61734c434672514e374569564e6a516d437253735"
             "a4852674c4e796c76676f454678475978584a39676d4b346d"
             "72304f47645a63474a4f5252475a4f514370514d68586d686"
             "57a46616c4e494a584d50505861525658695268524150434e"
             "55456965384474614357414d717a346e4e5578524d5a35556"
             "3584258735850736879677a6b7979586e4e575449446f6a46"
             "6c7263736e4b71536b5131473645383567535a62744959426"
             "83773714f36474458486a4f72585661564356435575626a63"
             "4a4b54686c79736c7432397a487549"
             "73354a47707058785831"_ba);
}

void QtProtobufTypesSerializationTest::ComplexTypeSerializeTest_data()
{
    QTest::addColumn<int>("intField");
    QTest::addColumn<QString>("stringField");
    QTest::addColumn<QByteArray>("expectedData");

    QTest::newRow("empty_value") << 0 << u""_s << ""_ba;
    QTest::newRow("value_only_int") << 42 << u""_s << "082a"_ba;
    QTest::newRow("value_only_string") << 0 << u"qwerty"_s << "12083206717765727479"_ba;
    QTest::newRow("int_and_string") << 42 << u"qwerty"_s << "082a12083206717765727479"_ba;
    QTest::newRow("int_and_big_string") << 42
                                        << u"YVRfJvjxqbgvFwS1YvOZXgtj5ffGLS7AiNHz9oZIoKbm7z"
                                           "8H79xBuyPkpQXvGoO09OY9xRawx3eOAs9xjoTA1xJhrw28"
                                           "TAcq1CebYlC9WUfQC6hIantaNdyHiKToffi0Zt7la42SRx"
                                           "XZSP4GuxbcZIp53pJnyCwfCy1qdFczT0dmn7h8fpyAdemE"
                                           "avwFeda4d0PApGfSU2jLt39X8kYUBxNM2WgALRBgHdVde8"
                                           "7q6Pi5U69TjhMd28W1SFD1DxyogCCrqOct2ZPICoLnrqdF"
                                           "3OdNzjRVLfeyvQ8LgLvRNFR9WfWAyAz79nKgBamd8Ntlvt"
                                           "4Mg35E5gVS2g7AQ7rkm72cBdnW9sCEyGabeXAuH5j4GRbu"
                                           "LT7qBZWDcFLF4SsCdS3WfFGdNHfwaijzykByo71PvFVlTX"
                                           "H2WJWoFvR5FALjBTn7bCdP0pAiSbLCY8Xz2Msc3dBb5Ff9"
                                           "GISPbUpNmUvBdMZMHQvqOmTNXEPpN0b74MDOMQfWJShOo3NkAvMjs"_s
                                        << "082a128404328104595652664a766a78716267764"
                                           "677533159764f5a5867746a356666474c53374169"
                                           "4e487a396f5a496f4b626d377a384837397842757"
                                           "9506b70515876476f4f30394f5939785261777833"
                                           "654f417339786a6f544131784a687277323854416"
                                           "37131436562596c43395755665143366849616e74"
                                           "614e647948694b546f666669305a74376c6134325"
                                           "35278585a53503447757862635a49703533704a6e"
                                           "79437766437931716446637a5430646d6e3768386"
                                           "670794164656d4561767746656461346430504170"
                                           "47665355326a4c74333958386b595542784e4d325"
                                           "767414c5242674864566465383771365069355536"
                                           "39546a684d6432385731534644314478796f67434"
                                           "372714f6374325a5049436f4c6e72716446334f64"
                                           "4e7a6a52564c6665797651384c674c76524e46523"
                                           "95766574179417a37396e4b6742616d64384e746c"
                                           "7674344d6733354535675653326737415137726b6"
                                           "d37326342646e5739734345794761626558417548"
                                           "356a34475262754c543771425a574463464c46345"
                                           "3734364533357664647644e48667761696a7a796b"
                                           "42796f3731507646566c54584832574a576f46765"
                                           "23546414c6a42546e37624364503070416953624c"
                                           "435938587a324d736333644262354666394749535"
                                           "06255704e6d557642644d5a4d485176714f6d544e"
                                           "584550704e306237344d444f4d5166574a53684f6"
                                           "f334e6b41764d6a73"_ba;
    QTest::newRow("neg_int_and_string")
            << -45 << u"qwerty"_s << "08d3ffffffffffffffff0112083206717765727479"_ba;
}

void QtProtobufTypesSerializationTest::ComplexTypeSerializeTest()
{
    QFETCH(const int, intField);
    QFETCH(const QString, stringField);
    QFETCH(const QByteArray, expectedData);

    SimpleStringMessage stringMessage;
    stringMessage.setTestFieldString(stringField);
    ComplexMessage test;
    test.setTestFieldInt(intField);
    test.setTestComplexField(stringMessage);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), expectedData);
}

void QtProtobufTypesSerializationTest::EmptyBytesMessageTest()
{
    SimpleBytesMessage msg;

    QByteArray result = msg.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufTypesSerializationTest::EmptyStringMessageTest()
{
    SimpleStringMessage msg;

    QByteArray result = msg.serialize(m_serializer.get());
    QVERIFY(result.isEmpty());
}

void QtProtobufTypesSerializationTest::FieldIndexRangeTest()
{
    FieldIndexTest1Message msg1;
    msg1.setTestField(1);
    QByteArray result = msg1.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "f80102"_ba);

    FieldIndexTest2Message msg2;
    msg2.setTestField(1);
    result = msg2.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "f8ff0302"_ba);

    FieldIndexTest3Message msg3;
    msg3.setTestField(1);
    result = msg3.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "f8ffff0702"_ba);

    FieldIndexTest4Message msg4;
    msg4.setTestField(1);
    result = msg4.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "f8ffffff0f02"_ba);
}

void QtProtobufTypesSerializationTest::OneofMessageEmptyTest()
{
    OneofMessage test;
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "");
}

void QtProtobufTypesSerializationTest::OneofMessageIntTest()
{
    OneofMessage test;
    test.setTestFieldInt(-45);
    test.setTestOneofFieldInt(-45);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(),
             "08d3ffffffffffffffff01"
             "d002d3ffffffffffffffff01");

    test.setTestFieldInt(0);
    test.setTestOneofFieldInt(0);
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "d00200");
}

void QtProtobufTypesSerializationTest::OneofMessageClearTest()
{
    OneofMessage test;
    test.setTestFieldInt(-45);
    test.setTestOneofFieldInt(-45);

    test.clearTestOneof();
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "08d3ffffffffffffffff01");
}

void QtProtobufTypesSerializationTest::OneofMessageComplexTest()
{
    ComplexMessage complexField;
    SimpleStringMessage stringField;
    stringField.setTestFieldString("qwerty");
    complexField.setTestFieldInt(42);
    complexField.setTestComplexField(stringField);

    OneofMessage test;
    test.setTestOneofComplexField(complexField);
    QByteArray result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "1a0c082a12083206717765727479");

    test.clearTestOneof();

    test.setTestOneofComplexField({});
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "1a00");

    test.setSecondComplexField({});
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "1a002a00");

    // Check that partially intialized complex field doesn't apply its 'oneof'
    // peculiarity to the child fields.
    complexField.setTestComplexField({});
    complexField.setTestFieldInt(42);
    test.setTestOneofComplexField(complexField);
    test.setSecondComplexField({});
    result = test.serialize(m_serializer.get());
    QCOMPARE(result.toHex(), "1a02082a2a00");
}

QTEST_MAIN(QtProtobufTypesSerializationTest)
#include "tst_protobuf_serialization_basictypes.moc"
