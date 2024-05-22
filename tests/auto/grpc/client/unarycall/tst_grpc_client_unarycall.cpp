// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <grpcclienttestbase.h>

#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <QtGrpc/QGrpcCallOptions>
#include <QtGrpc/QGrpcMetadata>

#include <QtTest/QSignalSpy>
#include <QtTest/QTest>

#include <memory>

#include <message_latency_defs.h>
#include <server_proc_runner.h>
#include <testservice_client.grpc.qpb.h>

using namespace Qt::Literals::StringLiterals;
using namespace qtgrpc::tests;

class QtGrpcClientUnaryCallTest : public GrpcClientTestBase
{
    Q_OBJECT
public:
    QtGrpcClientUnaryCallTest()
        : GrpcClientTestBase(GrpcClientTestBase::Channels{
            GrpcClientTestBase::Channel::Qt,
            GrpcClientTestBase::Channel::Json,
            #if !defined(Q_OS_DARWIN) && !defined(Q_OS_WIN32)
                            GrpcClientTestBase::Channel::Ssl,
            #endif
        })
    {
    }

private Q_SLOTS:
    void asyncWithSubscribe();
    void asyncWithLambda();
    void immediateCancel();
    void deferredCancel();
    void asyncClientStatusMessage();
    void asyncStatusMessage();
    void inThread();
    void asyncInThread();
    void metadata();
};

void QtGrpcClientUnaryCallTest::asyncWithSubscribe()
{
    SimpleStringMessage request;
    request.setTestFieldString("Hello Qt!");
    std::optional<SimpleStringMessage> result;

    bool waitForReply = false;
    std::shared_ptr<QGrpcCallReply> reply = client()->testMethod(request);
    reply->subscribe(this, [reply, &result, &waitForReply](const auto &status) {
        QCOMPARE_EQ(status.code(), QGrpcStatus::StatusCode::Ok);
        result = reply->read<SimpleStringMessage>();
        waitForReply = true;
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(waitForReply, true, MessageLatency);
    QVERIFY(result.has_value());
    QCOMPARE_EQ(result->testFieldString(), "Hello Qt!");
}

void QtGrpcClientUnaryCallTest::asyncWithLambda()
{
    std::optional<SimpleStringMessage> result = SimpleStringMessage();
    SimpleStringMessage request;
    request.setTestFieldString("Hello Qt!");
    bool waitForReply = false;
    client()->testMethod(request, this,
                         [&result, &waitForReply](std::shared_ptr<QGrpcCallReply> reply) {
                             result = reply->read<SimpleStringMessage>();
                             waitForReply = true;
                         });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(waitForReply, true, MessageLatency);
    QVERIFY(result.has_value());
    QCOMPARE_EQ(result->testFieldString(), "Hello Qt!");
}

void QtGrpcClientUnaryCallTest::immediateCancel()
{
    SimpleStringMessage request;
    request.setTestFieldString("sleep");

    std::shared_ptr<QGrpcCallReply> reply = client()->testMethod(request);

    QSignalSpy replyFinishedSpy(reply.get(), &QGrpcCallReply::finished);
    QSignalSpy clientErrorSpy(client().get(), &TestService::Client::errorOccurred);

    QVERIFY(replyFinishedSpy.isValid());
    QVERIFY(clientErrorSpy.isValid());

    reply->cancel();

    QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, FailTimeout);
    QTRY_COMPARE_EQ_WITH_TIMEOUT(replyFinishedSpy.count(), 1, FailTimeout);

    QCOMPARE_EQ(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).code(),
                QGrpcStatus::Cancelled);

    auto args = replyFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QVERIFY(args.first().value<QGrpcStatus>() == QGrpcStatus::StatusCode::Cancelled);
}

void QtGrpcClientUnaryCallTest::deferredCancel()
{
    SimpleStringMessage request;
    request.setTestFieldString("sleep");

    std::shared_ptr<QGrpcCallReply> reply = client()->testMethod(request);

    QSignalSpy replyFinishedSpy(reply.get(), &QGrpcCallReply::finished);
    QVERIFY(replyFinishedSpy.isValid());

    QTimer::singleShot(MessageLatencyThreshold, reply.get(), &QGrpcCallReply::cancel);

    QTRY_COMPARE_EQ_WITH_TIMEOUT(replyFinishedSpy.count(), 1, FailTimeout);
    auto args = replyFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QCOMPARE_EQ(args.first().value<QGrpcStatus>().code(), QGrpcStatus::StatusCode::Cancelled);
}

void QtGrpcClientUnaryCallTest::asyncClientStatusMessage()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");

    QSignalSpy clientErrorSpy(client().get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    client()->testMethodStatusMessage(request);

    QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, FailTimeout);

    QCOMPARE(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).message(),
             request.testFieldString());
}

void QtGrpcClientUnaryCallTest::asyncStatusMessage()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");

    std::shared_ptr<QGrpcCallReply> reply = client()->testMethodStatusMessage(request);

    QSignalSpy replyFinishedSpy(reply.get(), &QGrpcCallReply::finished);
    QVERIFY(replyFinishedSpy.isValid());

    QTRY_COMPARE_EQ_WITH_TIMEOUT(replyFinishedSpy.count(), 1, FailTimeout);
    auto args = replyFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QCOMPARE(args.first().value<QGrpcStatus>().message(), request.testFieldString());
}

void QtGrpcClientUnaryCallTest::inThread()
{
    SimpleStringMessage request;

    request.setTestFieldString("Hello Qt from thread!");

    QSignalSpy clientErrorSpy(client().get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    std::shared_ptr<QGrpcCallReply> reply;
    const std::unique_ptr<QThread> thread(QThread::create(
        [&] { reply = client()->testMethod(request); }));

    thread->start();

    QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, MessageLatencyWithThreshold);
    QVERIFY(reply == nullptr);
    QVERIFY(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first())
                    .message()
                    .startsWith("QGrpcClientBase::call is called from a different thread."));
}

void QtGrpcClientUnaryCallTest::asyncInThread()
{
    SimpleStringMessage request;
    request.setTestFieldString("Hello Qt from thread!");

    QSignalSpy clientErrorSpy(client().get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    std::optional<SimpleStringMessage> result = SimpleStringMessage();
    const std::unique_ptr<QThread> thread(QThread::create([&] {
        QEventLoop waiter;
        std::shared_ptr<QGrpcCallReply> reply = client()->testMethod(request);
        QObject::connect(reply.get(), &QGrpcCallReply::finished, &waiter,
                         [reply, &result, &waiter] {
                             result = reply->read<SimpleStringMessage>();
                             waiter.quit();
                         });
        waiter.exec();
    }));

    thread->start();
    QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, FailTimeout);
    QTRY_VERIFY(result.has_value());
    QTRY_VERIFY(
            qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first())
                    .message()
                    .startsWith("QGrpcClientBase::call is called from a different thread."));
}

void QtGrpcClientUnaryCallTest::metadata()
{
    QSignalSpy clientErrorSpy(client().get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    QGrpcCallOptions opt;
    opt.setMetadata({
        { "client_header",        "1"           },
        { "client_return_header", "valid_value" }
    });
    auto reply = client()->testMetadata({}, opt);

    QGrpcMetadata metadata;
    QEventLoop waiter;
    reply->subscribe(this, [reply, &metadata, &waiter] {
        metadata = reply->metadata();
        waiter.quit();
    });

    waiter.exec();

    int serverHeaderCount = 0;
    QByteArray clientReturnHeader;
    for (const auto &header : reply->metadata()) {
        if (header.first == "server_header") {
            QCOMPARE_EQ(QString::fromLatin1(header.second), QString::number(++serverHeaderCount));
        } else if (header.first == "client_return_header") {
            clientReturnHeader = header.second;
        }
    }

    QCOMPARE_EQ(clientErrorSpy.count(), 0);
    QCOMPARE_EQ(serverHeaderCount, 1);
    QCOMPARE_EQ(clientReturnHeader, "valid_value"_ba);
}

QTEST_MAIN(QtGrpcClientUnaryCallTest)

#include "tst_grpc_client_unarycall.moc"
