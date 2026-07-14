// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <grpccommonoptions.h>

#include <QtGrpc/qgrpcchanneloptions.h>
#include <QtGrpc/qgrpcserializationformat.h>

#include <QtTest/qtest.h>

#include <cstring>

class QGrpcChannelOptionsTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void hasSpecialMemberFunctions() const { common.hasSpecialMemberFunctions(); }
    void hasImplicitQVariant() const { common.hasImplicitQVariant(); }
    void hasMemberSwap() const { common.hasMemberSwap(); }
#if QT_DEPRECATED_SINCE(6, 13)
    void deprecatedPropertyMetadata() const { common.deprecatedPropertyMetadata(); }
    void propertyMetadataCompat() const { common.propertyMetadataCompat(); }
#endif
    void propertyMetadata() const { common.propertyMetadata(); }
    void propertyDeadline() const { common.propertyDeadline(); }
    void propertyFilterServerMetadata() const { common.propertyFilterServerMetadata(); }
    void propertyRequestCompression() const { common.propertyRequestCompression(); }
    void propertyAcceptedCompressionAlgorithms() const;
    void streamsToDebug() const { common.streamsToDebug(); }
    void comparesEqual() const { common.comparesEqual(); }

    void propertySerializationFormat() const;
    void propertyBackoff() const;
#if QT_CONFIG(ssl)
    void propertySslConfiguration() const;
#endif

private:
    GrpcCommonOptionsTest<QGrpcChannelOptions> common;
};

void QGrpcChannelOptionsTest::propertyAcceptedCompressionAlgorithms() const
{
    using namespace QtGrpc;

    const auto supported = QGrpcChannelOptions::supportedCompressionAlgorithms();

    QGrpcChannelOptions o1;
    QCOMPARE_EQ(o1.acceptedCompressionAlgorithms(), supported);
    QVERIFY(o1.acceptedCompressionAlgorithms().testFlag(CompressionAlgorithm::Identity));

    // Refuse all compressed encodings: pass {Identity} explicitly.
    auto o1Detach = o1;
    o1.setAcceptedCompressionAlgorithms(CompressionAlgorithm::Identity);
    QCOMPARE_NE(o1.acceptedCompressionAlgorithms(), o1Detach.acceptedCompressionAlgorithms());
    QCOMPARE_EQ(o1.acceptedCompressionAlgorithms(),
                CompressionAlgorithms{ CompressionAlgorithm::Identity });
    QVERIFY(o1.acceptedCompressionAlgorithms().testFlag(CompressionAlgorithm::Identity));
    QVERIFY(!o1.acceptedCompressionAlgorithms().testFlag(CompressionAlgorithm::Deflate));
    QVERIFY(!o1.acceptedCompressionAlgorithms().testFlag(CompressionAlgorithm::Gzip));

    // Single algorithm: Identity is added back by the setter (spec-mandated).
    o1.setAcceptedCompressionAlgorithms(CompressionAlgorithm::Gzip);
    QCOMPARE_EQ(o1.acceptedCompressionAlgorithms(),
                CompressionAlgorithm::Identity | CompressionAlgorithm::Gzip);

    // Explicit Identity OR'd with another algorithm: stored as-is.
    o1.setAcceptedCompressionAlgorithms(CompressionAlgorithm::Identity
                                        | CompressionAlgorithm::Deflate);
    QCOMPARE_EQ(o1.acceptedCompressionAlgorithms(),
                CompressionAlgorithm::Identity | CompressionAlgorithm::Deflate);

    // All compressed algorithms: Identity is added back by the setter.
    o1.setAcceptedCompressionAlgorithms(CompressionAlgorithm::Deflate | CompressionAlgorithm::Gzip);
    QCOMPARE_EQ(o1.acceptedCompressionAlgorithms(),
                CompressionAlgorithm::Identity | CompressionAlgorithm::Deflate
                    | CompressionAlgorithm::Gzip);
}

void QGrpcChannelOptionsTest::propertySerializationFormat() const
{
    QGrpcSerializationFormat fmt(QtGrpc::SerializationFormat::Json);

    QGrpcChannelOptions o1;
    auto o1Detach = o1;
    QVERIFY(o1.serializationFormat().suffix().isEmpty());
    o1.setSerializationFormat(fmt);
    QVERIFY(!o1.serializationFormat().suffix().isEmpty());
    QCOMPARE_EQ(o1.serializationFormat().suffix(), fmt.suffix());
    QCOMPARE_NE(o1.serializationFormat().suffix(), o1Detach.serializationFormat().suffix());
}

#if QT_CONFIG(ssl)
void QGrpcChannelOptionsTest::propertySslConfiguration() const
{
    QSslConfiguration sslConfig;
    sslConfig.setSessionTicket("test");

    QGrpcChannelOptions o1;
    auto o1Detach = o1;
    QVERIFY(!o1.sslConfiguration());
    o1.setSslConfiguration(sslConfig);
    QVERIFY(o1.sslConfiguration());
    QCOMPARE_EQ(o1.sslConfiguration()->sessionTicket(), sslConfig.sessionTicket());
    QCOMPARE_NE(o1.sslConfiguration(), o1Detach.sslConfiguration());
}
#endif

void QGrpcChannelOptionsTest::propertyBackoff() const
{
    using namespace std::chrono_literals;

    QGrpcChannelOptions o1;
    QCOMPARE_EQ(o1.initialReconnectBackoff(), 1000ms);
    QCOMPARE_EQ(o1.maximumReconnectBackoff(), 120000ms);
    QCOMPARE_EQ(o1.connectTimeout(), 20000ms);

    auto o1Detach = o1;

    o1.setInitialReconnectBackoff(200ms);
    QCOMPARE_EQ(o1.initialReconnectBackoff(), 200ms);
    QCOMPARE_NE(o1.initialReconnectBackoff(), o1Detach.initialReconnectBackoff());

    o1 = o1Detach;
    o1.setMaximumReconnectBackoff(2000ms);
    QCOMPARE_EQ(o1.maximumReconnectBackoff(), 2000ms);
    QCOMPARE_NE(o1.maximumReconnectBackoff(), o1Detach.maximumReconnectBackoff());

    o1 = o1Detach;
    o1.setConnectTimeout(500ms);
    QCOMPARE_EQ(o1.connectTimeout(), 500ms);
    QCOMPARE_NE(o1.connectTimeout(), o1Detach.connectTimeout());

    o1 = o1Detach;
    o1.setInitialReconnectBackoff(200ms)
        .setMaximumReconnectBackoff(2000ms)
        .setConnectTimeout(500ms);
    QCOMPARE_EQ(o1.initialReconnectBackoff(), 200ms);
    QCOMPARE_EQ(o1.maximumReconnectBackoff(), 2000ms);
    QCOMPARE_EQ(o1.connectTimeout(), 500ms);

    QTest::ignoreMessage(QtWarningMsg,
                         "QGrpcChannelOptions::setInitialReconnectBackoff: negative intervals "
                         "aren't allowed (-1ms); clamping to 0.");
    o1.setInitialReconnectBackoff(-1ms);
    QCOMPARE_EQ(o1.initialReconnectBackoff(), 0ms);

    QTest::ignoreMessage(QtWarningMsg,
                         "QGrpcChannelOptions::setMaximumReconnectBackoff: negative intervals "
                         "aren't allowed (-1ms); clamping to 0.");
    o1.setMaximumReconnectBackoff(-1ms);
    QCOMPARE_EQ(o1.maximumReconnectBackoff(), 0ms);

    QTest::ignoreMessage(QtWarningMsg,
                         "QGrpcChannelOptions::setConnectTimeout: negative intervals "
                         "aren't allowed (-1ms); clamping to 0.");
    o1.setConnectTimeout(-1ms);
    QCOMPARE_EQ(o1.connectTimeout(), 0ms);
}

QTEST_MAIN(QGrpcChannelOptionsTest)

#include "tst_qgrpcchanneloptions.moc"
