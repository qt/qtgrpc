// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/private/qcomparisontesthelper_p.h>
#include <QtTest/qtest.h>

#include <QtGrpc/qtgrpcnamespace.h>

#include <QtCore/qdebug.h>
#include <QtCore/qbuffer.h>
#include <QtCore/qbytearray.h>

using namespace Qt::Literals::StringLiterals;
using namespace QtGrpc;

class QtGrpcNamespaceTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void rpcDescriptorEquality() const;
    void rpcDescriptorHashing() const;
    void rpcDescriptorStreamsToDebug() const;
    void rpcDescriptorHasMetatype() const;
};

void QtGrpcNamespaceTest::rpcDescriptorEquality() const
{
    QTestPrivate::testEqualityOperatorsCompile<RpcDescriptor>();

    constexpr auto serviceA = "MyService"_L1;
    constexpr auto serviceB = "OtherService"_L1;
    constexpr auto methodA = "MyMethod"_L1;
    constexpr auto methodB = "OtherMethod"_L1;
    constexpr RpcType typeA = RpcType::UnaryCall;
    constexpr RpcType typeB = RpcType::ServerStreaming;

    constexpr RpcDescriptor base{ serviceA, methodA, typeA };
    constexpr auto same = base;

    QT_TEST_EQUALITY_OPS(base, same, true);

    constexpr RpcDescriptor diffService{ serviceB, methodA, typeA };
    QT_TEST_EQUALITY_OPS(base, diffService, false);

    constexpr RpcDescriptor diffMethod{ serviceA, methodB, typeA };
    QT_TEST_EQUALITY_OPS(base, diffMethod, false);

    constexpr RpcDescriptor diffType{ serviceA, methodA, typeB };
    QT_TEST_EQUALITY_OPS(base, diffType, false);

    constexpr RpcDescriptor diffAll{ serviceB, methodB, typeB };
    QT_TEST_EQUALITY_OPS(base, diffAll, false);
}

void QtGrpcNamespaceTest::rpcDescriptorHashing() const
{
    RpcDescriptor d1{ "service1"_L1, "method1"_L1, RpcType::UnaryCall };
    auto d2 = d1;
    QCOMPARE_EQ(qHash(d1), qHash(d2));
}

void QtGrpcNamespaceTest::rpcDescriptorStreamsToDebug() const
{
    constexpr QLatin1StringView expected(
        R"(QtGrpc::RpcDescriptor( service: "s1", method: "m1", type: QtGrpc::RpcType::UnaryCall ))");
    RpcDescriptor d1{ "s1"_L1, "m1"_L1, RpcType::UnaryCall };
    QCOMPARE_EQ(QDebug::toBytes(d1), expected);
}

void QtGrpcNamespaceTest::rpcDescriptorHasMetatype() const
{
    const QMetaType metaType = QMetaType::fromType<QtGrpc::RpcDescriptor>();
    QVERIFY(metaType.isValid());
    QCOMPARE(metaType.id(), qMetaTypeId<QtGrpc::RpcDescriptor>());
}

QTEST_MAIN(QtGrpcNamespaceTest)

#include "tst_qtgrpcnamespace.moc"
