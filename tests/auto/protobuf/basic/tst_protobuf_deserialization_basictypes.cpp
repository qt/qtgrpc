// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2022 Alexey Edelev <semlanik@gmail.com>, Viktor Kopp <vifactor@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "basicmessages.qpb.h"
#include "fieldindexrange.qpb.h"

#include <QObject>

#include <QTest>
#include <QProtobufSerializer>

#include <limits>

class QtProtobufTypesDeserializationTest : public QObject
{
    Q_OBJECT
private slots:
    void init()
    {
        serializer.reset(new QProtobufSerializer);
    }

    void FixedInt32MessageDeserializeTest();
    void FixedInt64MessageDeserializeTest();
    void SFixedInt32MessageDeserializeTest();
    void SFixedInt64MessageDeserializeTest();
    void FloatMessageDeserializeTest();
    void DoubleMessageDeserializeTest();
    void IntMessageDeserializeTest();
    void StringMessageDeserializeTest();
    void ComplexTypeDeserializeTest();
    void SIntMessageDeserializeTest();
    void UIntMessageDeserializeTest();
    void BoolDeserializeTest();
    void RedundantFieldIsIgnoredAtDeserializationTest();
    void FieldIndexRangeTest();
private:
    std::unique_ptr<QProtobufSerializer> serializer;
};

using namespace qtprotobufnamespace::tests;

void QtProtobufTypesDeserializationTest::FixedInt32MessageDeserializeTest()
{
    SimpleFixedInt32Message test;
    test.deserialize(serializer.get(), QByteArray::fromHex("0d0f000000"));
    QCOMPARE(test.testFieldFixedInt32(), 15u);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d2c010000"));
    QCOMPARE(test.testFieldFixedInt32(), 300u);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d09000100"));
    QCOMPARE(test.testFieldFixedInt32(), 65545u);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d00000000"));
    QCOMPARE(test.testFieldFixedInt32(), 0u);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d2c010000"));
    QCOMPARE(test.testFieldFixedInt32(), 300u);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d00010000"));
    QCOMPARE(test.testFieldFixedInt32(), (uint32_t)UINT8_MAX + 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d00000100"));
    QCOMPARE(test.testFieldFixedInt32(), (uint32_t)UINT16_MAX + 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("0dff000000"));
    QCOMPARE(test.testFieldFixedInt32(), (uint32_t)UINT8_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("0dffff0000"));
    QCOMPARE(test.testFieldFixedInt32(), (uint32_t)UINT16_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("0dffffffff"));
    QCOMPARE(test.testFieldFixedInt32(), (uint32_t)UINT32_MAX);

    test.setTestFieldFixedInt32(11);
    test.deserialize(serializer.get(), QByteArray::fromHex(""));
    QCOMPARE(test.testFieldFixedInt32(), 0u);
}

void QtProtobufTypesDeserializationTest::FixedInt64MessageDeserializeTest()
{
    SimpleFixedInt64Message test;
    test.deserialize(serializer.get(), QByteArray::fromHex("090f00000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), 15u);


    test.deserialize(serializer.get(), QByteArray::fromHex("092c01000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), 300u);

    test.deserialize(serializer.get(), QByteArray::fromHex("090900010000000000"));
    QCOMPARE(test.testFieldFixedInt64(), 65545u);

    test.deserialize(serializer.get(), QByteArray::fromHex("090000000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), 0u);

    test.deserialize(serializer.get(), QByteArray::fromHex("090001000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), (uint64_t)UINT8_MAX + 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("090000010000000000"));
    QCOMPARE(test.testFieldFixedInt64(), (uint64_t)UINT16_MAX + 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("090000000001000000"));
    QCOMPARE(test.testFieldFixedInt64(), (uint64_t)UINT32_MAX + 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("09ff00000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), (uint64_t)UINT8_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("09ffff000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), (uint64_t)UINT16_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("09ffffffff00000000"));
    QCOMPARE(test.testFieldFixedInt64(), (uint64_t)UINT32_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("09ffffffffffffffff"));
    QCOMPARE(test.testFieldFixedInt64(), (uint64_t)UINT64_MAX);

    test.setTestFieldFixedInt64(11);
    test.deserialize(serializer.get(), QByteArray::fromHex(""));
    QCOMPARE(test.testFieldFixedInt64(), 0u);
}

void QtProtobufTypesDeserializationTest::SFixedInt32MessageDeserializeTest()
{
    SimpleSFixedInt32Message test;
    test.deserialize(serializer.get(), QByteArray::fromHex("0d0f000000"));
    QCOMPARE(test.testFieldFixedInt32(), 15);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d2c010000"));
    QCOMPARE(test.testFieldFixedInt32(), 300);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d09000100"));
    QCOMPARE(test.testFieldFixedInt32(), 65545);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d00000000"));
    QCOMPARE(test.testFieldFixedInt32(), 0);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d2c010000"));
    QCOMPARE(test.testFieldFixedInt32(), 300);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d80000000"));
    QCOMPARE(test.testFieldFixedInt32(), INT8_MAX + 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d00800000"));
    QCOMPARE(test.testFieldFixedInt32(), INT16_MAX + 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d7f000000"));
    QCOMPARE(test.testFieldFixedInt32(), INT8_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("0dff7f0000"));
    QCOMPARE(test.testFieldFixedInt32(), INT16_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("0dffffff7f"));
    QCOMPARE(test.testFieldFixedInt32(), INT32_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d7fffffff"));
    QCOMPARE(test.testFieldFixedInt32(), INT8_MIN - 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("0dff7fffff"));
    QCOMPARE(test.testFieldFixedInt32(), INT16_MIN - 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d80ffffff"));
    QCOMPARE(test.testFieldFixedInt32(), INT8_MIN);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d0080ffff"));
    QCOMPARE(test.testFieldFixedInt32(), INT16_MIN);

    test.deserialize(serializer.get(), QByteArray::fromHex("0d00000080"));
    QCOMPARE(test.testFieldFixedInt32(), INT32_MIN);

    test.setTestFieldFixedInt32(11);
    test.deserialize(serializer.get(), QByteArray::fromHex(""));
    QCOMPARE(test.testFieldFixedInt32(), 0);
}

void QtProtobufTypesDeserializationTest::SFixedInt64MessageDeserializeTest()
{
    SimpleSFixedInt64Message test;
    test.deserialize(serializer.get(), QByteArray::fromHex("090f00000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), 15);

    test.deserialize(serializer.get(), QByteArray::fromHex("092c01000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), 300);

    test.deserialize(serializer.get(), QByteArray::fromHex("090900010000000000"));
    QCOMPARE(test.testFieldFixedInt64(), 65545);

    test.deserialize(serializer.get(), QByteArray::fromHex("090000000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), 0);

    test.deserialize(serializer.get(), QByteArray::fromHex("098000000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), INT8_MAX + 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("090080000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), INT16_MAX + 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("090000008000000000"));
    QCOMPARE(test.testFieldFixedInt64(), (int64_t)INT32_MAX + 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("097f00000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), INT8_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("09ff7f000000000000"));
    QCOMPARE(test.testFieldFixedInt64(), INT16_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("09ffffff7f00000000"));
    QCOMPARE(test.testFieldFixedInt64(), INT32_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("09ffffffffffffff7f"));
    QCOMPARE(test.testFieldFixedInt64(), INT64_MAX);

    test.deserialize(serializer.get(), QByteArray::fromHex("097fffffffffffffff"));
    QCOMPARE(test.testFieldFixedInt64(), INT8_MIN - 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("09ff7fffffffffffff"));
    QCOMPARE(test.testFieldFixedInt64(), INT16_MIN - 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("09ffffff7fffffffff"));
    QCOMPARE(test.testFieldFixedInt64(), (qlonglong)INT32_MIN - 1);

    test.deserialize(serializer.get(), QByteArray::fromHex("0980ffffffffffffff"));
    QCOMPARE(test.testFieldFixedInt64(), INT8_MIN);

    test.deserialize(serializer.get(), QByteArray::fromHex("090080ffffffffffff"));
    QCOMPARE(test.testFieldFixedInt64(), INT16_MIN);

    test.deserialize(serializer.get(), QByteArray::fromHex("0900000080ffffffff"));
    QCOMPARE(test.testFieldFixedInt64(), INT32_MIN);

    test.deserialize(serializer.get(), QByteArray::fromHex("090000000000000080"));
    QCOMPARE(test.testFieldFixedInt64(), INT64_MIN);

    test.setTestFieldFixedInt64(11);
    test.deserialize(serializer.get(), QByteArray::fromHex(""));
    QCOMPARE(test.testFieldFixedInt64(), 0);
}

void QtProtobufTypesDeserializationTest::FloatMessageDeserializeTest()
{
    SimpleFloatMessage test;
    test.deserialize(serializer.get(), QByteArray::fromHex("3dcdcccc3d"));
    QCOMPARE(test.testFieldFloat(), 0.1f);

    test.deserialize(serializer.get(), QByteArray::fromHex("3d00008000"));
    QCOMPARE(test.testFieldFloat(), std::numeric_limits<float>::min());

    test.deserialize(serializer.get(), QByteArray::fromHex("3dffff7f7f"));
    QCOMPARE(test.testFieldFloat(), std::numeric_limits<float>::max());

    test.deserialize(serializer.get(), QByteArray::fromHex("3d666686c0"));
    QCOMPARE(test.testFieldFloat(), -4.2f);

    test.deserialize(serializer.get(), QByteArray::fromHex("3d00000000"));
    QCOMPARE(test.testFieldFloat(), -0.0f);

    test.setTestFieldFloat(11.0f);
    test.deserialize(serializer.get(), QByteArray::fromHex(""));
    QCOMPARE(test.testFieldFloat(), 0.0f);
}

void QtProtobufTypesDeserializationTest::DoubleMessageDeserializeTest()
{
    SimpleDoubleMessage test;
    test.deserialize(serializer.get(), QByteArray::fromHex("419a9999999999b93f"));
    QCOMPARE(0.1, test.testFieldDouble());

    test.deserialize(serializer.get(), QByteArray::fromHex("410000000000001000"));
    QCOMPARE(std::numeric_limits<double>::min(), test.testFieldDouble());

    test.deserialize(serializer.get(), QByteArray::fromHex("41ffffffffffffef7f"));
    QCOMPARE(std::numeric_limits<double>::max(), test.testFieldDouble());

    test.deserialize(serializer.get(), QByteArray::fromHex("41cdcccccccccc10c0"));
    QCOMPARE(-4.2, test.testFieldDouble());

    test.deserialize(serializer.get(), QByteArray::fromHex("410000000000000000"));
    QCOMPARE(0.0, test.testFieldDouble());

    test.setTestFieldDouble(11.0);
    test.deserialize(serializer.get(), QByteArray::fromHex(""));
    QCOMPARE(test.testFieldDouble(), 0.0f);
}

void QtProtobufTypesDeserializationTest::IntMessageDeserializeTest()
{
    qtprotobufnamespace::tests::SimpleIntMessage test;
    test.deserialize(serializer.get(), QByteArray::fromHex("080f"));
    QCOMPARE(test.testFieldInt(), 15);

    test.deserialize(serializer.get(), QByteArray::fromHex("08ac02"));
    QCOMPARE(test.testFieldInt(), 300);

    test.deserialize(serializer.get(), QByteArray::fromHex("08898004"));
    QCOMPARE(test.testFieldInt(), 65545);

    test.deserialize(serializer.get(), QByteArray::fromHex("088001"));
    QCOMPARE(INT8_MAX + 1, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08808002"));
    QCOMPARE(INT16_MAX + 1, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("087f"));
    QCOMPARE(INT8_MAX, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08ffff01"));
    QCOMPARE(INT16_MAX, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08ffffffff07"));
    QCOMPARE(INT32_MAX, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08ffffffffffffffffff01"));
    QCOMPARE(-1, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08b2fcffffffffffffff01"));
    QCOMPARE(-462, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("089f8ffcffffffffffff01"));
    QCOMPARE(-63585, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("0880ffffffffffffffff01"));
    QCOMPARE(INT8_MIN, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("088080feffffffffffff01"));
    QCOMPARE(INT16_MIN, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("0880808080f8ffffffff01"));
    QCOMPARE(INT32_MIN, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08fffeffffffffffffff01"));
    QCOMPARE(INT8_MIN - 1, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08fffffdffffffffffff01"));
    QCOMPARE(INT16_MIN - 1, test.testFieldInt());

    test.setTestFieldInt(11);
    test.deserialize(serializer.get(), QByteArray::fromHex(""));
    QCOMPARE(test.testFieldInt(), 0);
}

void QtProtobufTypesDeserializationTest::StringMessageDeserializeTest()
{
    SimpleStringMessage test;
    test.deserialize(serializer.get(), QByteArray::fromHex("3206717765727479"));
    QCOMPARE(test.testFieldString().toStdString().c_str(), "qwerty");

    test.deserialize(serializer.get(), QByteArray::fromHex("3280046f6570534e4c4956473038554a706b3257374a74546b6b4278794b303658306c51364d4c37494d6435354b3858433154707363316b4457796d3576387a3638623446517570394f393551536741766a48494131354f583642753638657362514654394c507a5341444a367153474254594248583551535a67333274724364484d6a383058754448717942674d34756636524b71326d675762384f76787872304e774c786a484f66684a384d726664325237686255676a65737062596f5168626748456a32674b45563351766e756d596d7256586531426b437a5a684b56586f6444686a304f6641453637766941793469334f6167316872317a34417a6f384f3558713638504f455a3143735a506f3244584e4e52386562564364594f7a3051364a4c50536c356a61734c434672514e374569564e6a516d437253735a4852674c4e796c76676f454678475978584a39676d4b346d72304f47645a63474a4f5252475a4f514370514d68586d68657a46616c4e494a584d50505861525658695268524150434e55456965384474614357414d717a346e4e5578524d5a355563584258735850736879677a6b7979586e4e575449446f6a466c7263736e4b71536b5131473645383567535a6274495942683773714f36474458486a4f72585661564356435575626a634a4b54686c79736c7432397a48754973354a47707058785831"));
    QCOMPARE(test.testFieldString().toStdString().c_str(), "oepSNLIVG08UJpk2W7JtTkkBxyK06X0lQ6ML7IMd55K8XC1Tpsc1kDWym5v8z68b4FQup9O95QSgAvjHIA15OX6Bu68esbQFT9LPzSADJ6qSGBTYBHX5QSZg32trCdHMj80XuDHqyBgM4uf6RKq2mgWb8Ovxxr0NwLxjHOfhJ8Mrfd2R7hbUgjespbYoQhbgHEj2gKEV3QvnumYmrVXe1BkCzZhKVXodDhj0OfAE67viAy4i3Oag1hr1z4Azo8O5Xq68POEZ1CsZPo2DXNNR8ebVCdYOz0Q6JLPSl5jasLCFrQN7EiVNjQmCrSsZHRgLNylvgoEFxGYxXJ9gmK4mr0OGdZcGJORRGZOQCpQMhXmhezFalNIJXMPPXaRVXiRhRAPCNUEie8DtaCWAMqz4nNUxRMZ5UcXBXsXPshygzkyyXnNWTIDojFlrcsnKqSkQ1G6E85gSZbtIYBh7sqO6GDXHjOrXVaVCVCUubjcJKThlyslt29zHuIs5JGppXxX1");

    test.setTestFieldString("qwerty");
    test.deserialize(serializer.get(), QByteArray());
    QCOMPARE(test.testFieldString().toStdString().c_str(), "");
}

void QtProtobufTypesDeserializationTest::ComplexTypeDeserializeTest()
{
    ComplexMessage test;
    SimpleStringMessage stringMsg;
    stringMsg.setTestFieldString("qwerty");

    test.deserialize(serializer.get(), QByteArray::fromHex("1208320671776572747908d3ffffffffffffffff01"));
    QCOMPARE(test.testFieldInt(), -45);
    QCOMPARE(test.testComplexField().testFieldString(), QString::fromUtf8("qwerty"));

    test.deserialize(serializer.get(), QByteArray::fromHex("08d3ffffffffffffffff0112083206717765727479"));
    QCOMPARE(test.testFieldInt(), -45);
    QCOMPARE(test.testComplexField().testFieldString(), QString::fromUtf8("qwerty"));

    test.deserialize(serializer.get(), QByteArray::fromHex("128404328104595652664a766a78716267764677533159764f5a5867746a356666474c533741694e487a396f5a496f4b626d377a3848373978427579506b70515876476f4f30394f5939785261777833654f417339786a6f544131784a68727732385441637131436562596c43395755665143366849616e74614e647948694b546f666669305a74376c613432535278585a53503447757862635a49703533704a6e79437766437931716446637a5430646d6e3768386670794164656d456176774665646134643050417047665355326a4c74333958386b595542784e4d325767414c524267486456646538377136506935553639546a684d6432385731534644314478796f67434372714f6374325a5049436f4c6e72716446334f644e7a6a52564c6665797651384c674c76524e4652395766574179417a37396e4b6742616d64384e746c7674344d6733354535675653326737415137726b6d37326342646e5739734345794761626558417548356a34475262754c543771425a574463464c463453734364533357664647644e48667761696a7a796b42796f3731507646566c54584832574a576f4676523546414c6a42546e37624364503070416953624c435938587a324d73633364426235466639474953506255704e6d557642644d5a4d485176714f6d544e584550704e306237344d444f4d5166574a53684f6f334e6b41764d6a73082a"));
    QCOMPARE(test.testFieldInt(), 42);
    QCOMPARE(test.testComplexField().testFieldString(),
             QString::fromUtf8(
                     "YVRfJvjxqbgvFwS1YvOZXgtj5ffGLS7AiNHz9oZIoKbm7z8H79xBuyPkpQXvGoO09OY9xRawx3eOA"
                     "s9xjoTA1xJhrw28TAcq1CebYlC9WUfQC6hIantaNdyHiKToffi0Zt7la42SRxXZSP4GuxbcZIp53p"
                     "JnyCwfCy1qdFczT0dmn7h8fpyAdemEavwFeda4d0PApGfSU2jLt39X8kYUBxNM2WgALRBgHdVde87"
                     "q6Pi5U69TjhMd28W1SFD1DxyogCCrqOct2ZPICoLnrqdF3OdNzjRVLfeyvQ8LgLvRNFR9WfWAyAz7"
                     "9nKgBamd8Ntlvt4Mg35E5gVS2g7AQ7rkm72cBdnW9sCEyGabeXAuH5j4GRbuLT7qBZWDcFLF4SsCd"
                     "S3WfFGdNHfwaijzykByo71PvFVlTXH2WJWoFvR5FALjBTn7bCdP0pAiSbLCY8Xz2Msc3dBb5Ff9GI"
                     "SPbUpNmUvBdMZMHQvqOmTNXEPpN0b74MDOMQfWJShOo3NkAvMjs"));

    test.deserialize(serializer.get(), QByteArray::fromHex("082a128404328104595652664a766a78716267764677533159764f5a5867746a356666474c533741694e487a396f5a496f4b626d377a3848373978427579506b70515876476f4f30394f5939785261777833654f417339786a6f544131784a68727732385441637131436562596c43395755665143366849616e74614e647948694b546f666669305a74376c613432535278585a53503447757862635a49703533704a6e79437766437931716446637a5430646d6e3768386670794164656d456176774665646134643050417047665355326a4c74333958386b595542784e4d325767414c524267486456646538377136506935553639546a684d6432385731534644314478796f67434372714f6374325a5049436f4c6e72716446334f644e7a6a52564c6665797651384c674c76524e4652395766574179417a37396e4b6742616d64384e746c7674344d6733354535675653326737415137726b6d37326342646e5739734345794761626558417548356a34475262754c543771425a574463464c463453734364533357664647644e48667761696a7a796b42796f3731507646566c54584832574a576f4676523546414c6a42546e37624364503070416953624c435938587a324d73633364426235466639474953506255704e6d557642644d5a4d485176714f6d544e584550704e306237344d444f4d5166574a53684f6f334e6b41764d6a73"));
    QCOMPARE(test.testFieldInt(), 42);
    QCOMPARE(test.testComplexField().testFieldString(),
             QString::fromUtf8(
                     "YVRfJvjxqbgvFwS1YvOZXgtj5ffGLS7AiNHz9oZIoKbm7z8H79xBuyPkpQXvGoO09OY9xRawx3eOA"
                     "s9xjoTA1xJhrw28TAcq1CebYlC9WUfQC6hIantaNdyHiKToffi0Zt7la42SRxXZSP4GuxbcZIp53p"
                     "JnyCwfCy1qdFczT0dmn7h8fpyAdemEavwFeda4d0PApGfSU2jLt39X8kYUBxNM2WgALRBgHdVde87"
                     "q6Pi5U69TjhMd28W1SFD1DxyogCCrqOct2ZPICoLnrqdF3OdNzjRVLfeyvQ8LgLvRNFR9WfWAyAz7"
                     "9nKgBamd8Ntlvt4Mg35E5gVS2g7AQ7rkm72cBdnW9sCEyGabeXAuH5j4GRbuLT7qBZWDcFLF4SsCd"
                     "S3WfFGdNHfwaijzykByo71PvFVlTXH2WJWoFvR5FALjBTn7bCdP0pAiSbLCY8Xz2Msc3dBb5Ff9GI"
                     "SPbUpNmUvBdMZMHQvqOmTNXEPpN0b74MDOMQfWJShOo3NkAvMjs"));

    test.setTestFieldInt(-45);
    test.setTestComplexField(stringMsg);
    test.deserialize(serializer.get(), QByteArray::fromHex("08d3ffffffffffffffff01"));
    QCOMPARE(test.testFieldInt(), -45);
    QCOMPARE(test.testComplexField().testFieldString(), QLatin1String(""));

    test.setTestFieldInt(-45);
    test.setTestComplexField(stringMsg);
    test.deserialize(serializer.get(), QByteArray::fromHex("12083206717765727479"));
    QCOMPARE(test.testFieldInt(), 0);
    QCOMPARE(test.testComplexField().testFieldString(), QLatin1String("qwerty"));
}

void QtProtobufTypesDeserializationTest::SIntMessageDeserializeTest()
{
    SimpleSIntMessage test;
    test.deserialize(serializer.get(), QByteArray::fromHex("081e"));
    QCOMPARE(test.testFieldInt(), 15);

    test.deserialize(serializer.get(), QByteArray::fromHex("08d804"));
    QCOMPARE(test.testFieldInt(), 300);

    test.deserialize(serializer.get(), QByteArray::fromHex("08928008"));
    QCOMPARE(test.testFieldInt(), 65545);

    test.deserialize(serializer.get(), QByteArray::fromHex(""));
    QCOMPARE(test.testFieldInt(), 0);

    test.deserialize(serializer.get(), QByteArray::fromHex("088002"));
    QCOMPARE(INT8_MAX + 1, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08808004"));
    QCOMPARE(INT16_MAX + 1, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08fe01"));
    QCOMPARE(INT8_MAX, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08feff03"));
    QCOMPARE(INT16_MAX, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08feffffff0f"));
    QCOMPARE(INT32_MAX, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("0801"));
    QCOMPARE(-1, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("089b07"));
    QCOMPARE(-462, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08c1e107"));
    QCOMPARE(-63585, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08ff01"));
    QCOMPARE(INT8_MIN, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08ffff03"));
    QCOMPARE(INT16_MIN, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08ffffffff0f"));
    QCOMPARE(INT32_MIN, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("088102"));
    QCOMPARE(INT8_MIN - 1, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08818004"));
    QCOMPARE(INT16_MIN - 1, test.testFieldInt());
}

void QtProtobufTypesDeserializationTest::UIntMessageDeserializeTest()
{
    SimpleUIntMessage test;
    test.deserialize(serializer.get(), QByteArray::fromHex("080f"));
    QCOMPARE(test.testFieldInt(), 15u);

    test.deserialize(serializer.get(), QByteArray::fromHex("08ac02"));
    QCOMPARE(test.testFieldInt(), 300u);

    test.deserialize(serializer.get(), QByteArray::fromHex("08898004"));
    QCOMPARE(test.testFieldInt(), 65545u);

    test.deserialize(serializer.get(), QByteArray::fromHex(""));
    QCOMPARE(test.testFieldInt(), 0u);

    test.deserialize(serializer.get(), QByteArray::fromHex("088002"));
    QCOMPARE((uint32_t)UINT8_MAX + 1, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08808004"));
    QCOMPARE((uint32_t)UINT16_MAX + 1, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08ff01"));
    QCOMPARE((uint32_t)UINT8_MAX, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08ffff03"));
    QCOMPARE((uint32_t)UINT16_MAX, test.testFieldInt());

    test.deserialize(serializer.get(), QByteArray::fromHex("08ffffffff0f"));
    QCOMPARE((uint32_t)UINT32_MAX, test.testFieldInt());
}

void QtProtobufTypesDeserializationTest::BoolDeserializeTest()
{
    SimpleBoolMessage test;
    test.deserialize(serializer.get(), QByteArray::fromHex("0801"));
    QCOMPARE(test.testFieldBool(), true);

    test.deserialize(serializer.get(), QByteArray::fromHex(""));
    test.deserialize(serializer.get(), QByteArray::fromHex("0800"));
    QCOMPARE(test.testFieldBool(), false);
}

void QtProtobufTypesDeserializationTest::RedundantFieldIsIgnoredAtDeserializationTest()
{
    ComplexMessage test;
    //3206717765727479 length delimited field number 6
    test.deserialize(serializer.get(), QByteArray::fromHex("120832067177657274793206717765727479"));
    QCOMPARE(test.testFieldInt(), 0);
    QCOMPARE(test.testComplexField().testFieldString().toStdString().c_str(), "qwerty");

    //3dcdcccc3d fixed32 field number 7
    test.deserialize(serializer.get(), QByteArray::fromHex("120832067177657274793dcdcccc3d"));
    QCOMPARE(test.testFieldInt(), 0);
    QCOMPARE(test.testComplexField().testFieldString().toStdString().c_str(), "qwerty");

    //419a9999999999b93f fixed64 field number 8
    test.deserialize(serializer.get(), QByteArray::fromHex("12083206717765727479419a9999999999b93f"));
    QCOMPARE(test.testFieldInt(), 0);
    QCOMPARE(test.testComplexField().testFieldString().toStdString().c_str(), "qwerty");

    //60d3ffffffffffffffff01 varint field number 12
    test.deserialize(serializer.get(), QByteArray::fromHex("60d3ffffffffffffffff0112083206717765727479"));
    QCOMPARE(test.testFieldInt(), 0);
    QCOMPARE(test.testComplexField().testFieldString().toStdString().c_str(), "qwerty");
}

void QtProtobufTypesDeserializationTest::FieldIndexRangeTest()
{
    FieldIndexTest1Message msg1;
    msg1.deserialize(serializer.get(), QByteArray::fromHex("f80102"));
    QCOMPARE(msg1.testField(), 1);

    FieldIndexTest2Message msg2;
    msg2.deserialize(serializer.get(), QByteArray::fromHex("f8ff0302"));
    QCOMPARE(msg2.testField(), 1);

    FieldIndexTest3Message msg3;
    msg3.deserialize(serializer.get(), QByteArray::fromHex("f8ffff0702"));
    QCOMPARE(msg3.testField(), 1);

    FieldIndexTest4Message msg4;
    msg4.deserialize(serializer.get(), QByteArray::fromHex("f8ffffff0f02"));
    QCOMPARE(msg4.testField(), 1);
}

QTEST_MAIN(QtProtobufTypesDeserializationTest)
#include "tst_protobuf_deserialization_basictypes.moc"
