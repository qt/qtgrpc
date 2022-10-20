// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QGrpcHttp2Channel>
#include <QGrpcInsecureCallCredentials>
#include <QGrpcInsecureChannelCredentials>

#include <QCoreApplication>
#include <QSignalSpy>
#include <QTest>
#include <QtCore/QThread>

#include "testservice_client.grpc.qpb.h"

namespace {
using namespace qtgrpc::tests;
}

class QtGrpcUnattachedChannelClientTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase() { qRegisterProtobufTypes(); }

    void CheckMethodsGeneration();
    void ClientSyncTestUnattachedChannel();
    void ClientSyncTestUnattachedChannelSignal();
    void AttachChannelThreadTest();
};

void QtGrpcUnattachedChannelClientTest::CheckMethodsGeneration()
{
    // Dummy compile time check of functions generation and interface compatibility
    TestService::Client client;
    client.attachChannel(QSharedPointer<QGrpcHttp2Channel>::create(
            QUrl(), QGrpcInsecureChannelCredentials() | QGrpcInsecureCallCredentials()));
    SimpleStringMessage request;
    auto result = QSharedPointer<SimpleStringMessage>::create();
    client.testMethod(request, result.get());
    client.testMethod(request);
    client.testMethod(request, &client, [](QSharedPointer<QGrpcCallReply>) {});
}

void QtGrpcUnattachedChannelClientTest::ClientSyncTestUnattachedChannel()
{
    TestService::Client client;
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");
    auto result = QSharedPointer<SimpleStringMessage>::create();

    QGrpcStatus status = client.testMethodStatusMessage(request, result.get());

    QCOMPARE_EQ(status.code(), QGrpcStatus::Unknown);
    QCOMPARE_EQ("Serializing failed. Serializer is not ready", status.message());
}

void QtGrpcUnattachedChannelClientTest::ClientSyncTestUnattachedChannelSignal()
{
    TestService::Client client;
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");
    auto result = QSharedPointer<SimpleStringMessage>::create();

    QSignalSpy clientErrorSpy(&client, &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    client.testMethodStatusMessage(request, result.get());

    QTRY_COMPARE_EQ(clientErrorSpy.count(), 1);
    QCOMPARE(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).code(), QGrpcStatus::Unknown);
    QCOMPARE(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).message(),
             "Serializing failed. Serializer is not ready");
}

void QtGrpcUnattachedChannelClientTest::AttachChannelThreadTest()
{
    QSharedPointer<QGrpcHttp2Channel> channel;
    QSharedPointer<QThread> thread(QThread::create([&]() {
        channel = QSharedPointer<QGrpcHttp2Channel>::create(
                QUrl("http://localhost:50051", QUrl::StrictMode),
                QGrpcInsecureCallCredentials() | QGrpcInsecureChannelCredentials());
    }));
    thread->start();
    thread->wait();

    TestService::Client client;

    QSignalSpy clientErrorSpy(&client, &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    client.attachChannel(channel);

    QTRY_COMPARE_EQ(clientErrorSpy.count(), 1);
    QCOMPARE(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).code(), QGrpcStatus::Unknown);
    QVERIFY(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first())
                    .message()
                    .startsWith("QAbstractGrpcClient::attachChannel is called from different "
                                "thread."));
}

QTEST_MAIN(QtGrpcUnattachedChannelClientTest)

#include "tst_grpc_unattached_channel.moc"
