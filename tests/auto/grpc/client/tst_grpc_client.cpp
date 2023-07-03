// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QGrpcCallReply>
#if QT_CONFIG(native_grpc)
#  include <QGrpcChannel>
#endif
#include <QGrpcHttp2Channel>
#include <QtGrpc/QGrpcCallOptions>
#include <QtGrpc/QGrpcMetadata>

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QProcess>
#include <QSignalSpy>
#include <QTest>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/qsysinfo.h>

#include <memory>

#if QT_CONFIG(native_grpc)
#  include <grpcpp/security/credentials.h>
#endif
#include "testservice_client.grpc.qpb.h"
#include <message_latency_defs.h>

using namespace Qt::Literals::StringLiterals;

namespace {
constexpr int MessageLatency = QT_GRPC_TEST_MESSAGE_LATENCY;
constexpr int MessageLatencyThreshold = MessageLatency / 5;
constexpr int MessageLatencyWithThreshold = MessageLatency + MessageLatencyThreshold;
constexpr int FailTimeout = QT_GRPC_TEST_MESSAGE_LATENCY * 5;

using namespace qtgrpc::tests;

static std::shared_ptr<TestService::Client> createHttp2Client()
{
    auto client = std::make_shared<TestService::Client>();
    QGrpcChannelOptions channelOptions(QUrl("http://localhost:50051", QUrl::StrictMode));
    auto channel = std::make_shared<QGrpcHttp2Channel>(channelOptions);
    client->attachChannel(std::move(channel));
    return client;
}

#if QT_CONFIG(native_grpc)
#  ifndef Q_OS_WINDOWS
static std::shared_ptr<TestService::Client> createGrpcSocketClient()
{
    auto client = std::make_shared<TestService::Client>();
    QGrpcChannelOptions channelOptions(QUrl("unix:///tmp/test.sock"));
    auto channel = std::make_shared<QGrpcChannel>(channelOptions,
                                                  QGrpcChannel::InsecureChannelCredentials);
    client->attachChannel(std::move(channel));
    return client;
}
#  endif
static std::shared_ptr<TestService::Client> createGrpcHttpClient()
{
    auto client = std::make_shared<TestService::Client>();
    QGrpcChannelOptions channelOptions(QUrl("localhost:50051"));
    auto channel = std::make_shared<QGrpcChannel>(channelOptions,
                                                  QGrpcChannel::InsecureChannelCredentials);
    client->attachChannel(std::move(channel));
    return client;
}
#endif

} // namespace

class QtGrpcClientTest : public QObject
{
    Q_OBJECT
public:
private slots:
    void initTestCase_data()
    {
        QTest::addColumn<QString>("type");
        QTest::addColumn<std::shared_ptr<TestService::Client>>("client");

        QTest::newRow("Http2Client") << "qt" << createHttp2Client();
#if QT_CONFIG(native_grpc)
#  ifndef Q_OS_WINDOWS
        QTest::newRow("GrpcSocket") << "native" << createGrpcSocketClient();
#  endif
        QTest::newRow("GrpcHttp") << "native" << createGrpcHttpClient();
#endif
    }

    void initTestCase()
    {
        startServer();
        qRegisterProtobufTypes();
    }

    void cleanupTestCase()
    {
        if (serverProc && serverProc->state() == QProcess::ProcessState::Running) {
            serverProc->kill();
            serverProc->waitForFinished();
        }
    }

    void init()
    {
        if (QSysInfo::productVersion() == "8.6" && QSysInfo::productType().contains("rhel")) {
            QSKIP("Test case disabled on RHEL due to QTBUG-111098");
        }

        QFETCH_GLOBAL(QString, type);
        channelType = type;

        QFETCH_GLOBAL(std::shared_ptr<TestService::Client>, client);
        _client = std::move(client);

        if (serverProc->state() != QProcess::ProcessState::Running) {
            qInfo() << "Restarting server";
            startServer();
            QVERIFY2(serverProc->state() == QProcess::ProcessState::Running,
                     "Precondition failed - Server cannot be started.");
        }
    }

    void CallAndRecieveStringTest();
    void CallAndAsyncRecieveWithSubscribeStringTest();
    void CallAndAsyncRecieveWithLambdaStringTest();
    void CallAndAsyncImmediateAbortStringTest();
    void CallAndAsyncDeferredAbortStringTest();
    void StreamStringTest();
    void StreamStringAndAbortTest();
    void StreamStringAndDeferredAbortTest();
    // void StreamStringAndGetChangedSignalsTests();
    void StreamBlobTest();
    void CallMessageAsyncTest();
    void CallMessageClientAsyncTest();
    void CallMessageClientTest();
    void StatusMessageClientTest();
    void StreamAndGetAsyncReplyTest();
    void MultipleStreamsTest();
    void MultipleStreamsCancelTest();
    void CallNonCompatibleArgRetTest();
    void CallStringThreadTest();
    void StringAsyncThreadTest();
    void StreamStringThreadTest();
    void StreamCancelWhileErrorTimeoutTest();
    void MetadataTest();
    void CallDeadlineTest_data();
    void CallDeadlineTest();
    void StreamDeadlineTest_data();
    void StreamDeadlineTest();

private:
    void startServer()
    {
        serverProc = std::make_unique<QProcess>();
        QString serverPath = QFINDTESTDATA(TEST_GRPC_SERVER_PATH);
        QVERIFY2(!serverPath.isEmpty(), "testserver binary is missing");
        QObject::connect(serverProc.get(), &QProcess::readyReadStandardOutput, this,
                         [this] { qInfo() << serverProc->readAllStandardOutput(); });
        QObject::connect(serverProc.get(), &QProcess::readyReadStandardError, this,
                         [this] { qInfo() << serverProc->readAllStandardError(); });
        serverProc->start(serverPath);
        serverProc->waitForStarted();
        // Extra time for the server to setup
        QTest::qSleep(1000);
    }

    std::unique_ptr<QProcess> serverProc;
    QString channelType;
    std::shared_ptr<TestService::Client> _client;
};

void QtGrpcClientTest::CallAndRecieveStringTest()
{
    SimpleStringMessage request;
    auto result = std::make_shared<SimpleStringMessage>();
    request.setTestFieldString("Hello Qt!");
    QCOMPARE_EQ(_client->testMethod(request, result.get()), QGrpcStatus::Ok);
    QCOMPARE_EQ(result->testFieldString(), "Hello Qt!");
}

void QtGrpcClientTest::CallAndAsyncRecieveWithSubscribeStringTest()
{
    SimpleStringMessage request;
    SimpleStringMessage result;
    request.setTestFieldString("Hello Qt!");
    QEventLoop waiter;

    std::shared_ptr<QGrpcCallReply> reply = _client->testMethod(request);
    reply->subscribe(this, [reply, &result, &waiter] {
        result = reply->read<SimpleStringMessage>();
        waiter.quit();
    });

    waiter.exec();
    QCOMPARE_EQ(result.testFieldString(), "Hello Qt!");
}

void QtGrpcClientTest::CallAndAsyncRecieveWithLambdaStringTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Hello Qt!");
    QEventLoop waiter;
    _client->testMethod(request, this, [&result, &waiter](std::shared_ptr<QGrpcCallReply> reply) {
        result = reply->read<SimpleStringMessage>();
        waiter.quit();
    });

    waiter.exec();
    QCOMPARE_EQ(result.testFieldString(), "Hello Qt!");
}

void QtGrpcClientTest::CallAndAsyncImmediateAbortStringTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("sleep");

    std::shared_ptr<QGrpcCallReply> reply = _client->testMethod(request);

    result.setTestFieldString("Result not changed by echo");
    QObject::connect(reply.get(), &QGrpcCallReply::finished, this,
                     [&result, reply] { result = reply->read<SimpleStringMessage>(); });

    QSignalSpy replyErrorSpy(reply.get(), &QGrpcCallReply::errorOccurred);
    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);

    QVERIFY(replyErrorSpy.isValid());
    QVERIFY(clientErrorSpy.isValid());

    reply->abort();

    QTRY_COMPARE_EQ_WITH_TIMEOUT(replyErrorSpy.count(), 1, FailTimeout);
    QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, FailTimeout);

    QCOMPARE(qvariant_cast<QGrpcStatus>(replyErrorSpy.at(0).first()).code(),
             QGrpcStatus::StatusCode::Aborted);
    QCOMPARE(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).code(),
             QGrpcStatus::StatusCode::Aborted);

    QCOMPARE_EQ(result.testFieldString(), "Result not changed by echo");
}

void QtGrpcClientTest::CallAndAsyncDeferredAbortStringTest()
{
    SimpleStringMessage request;
    request.setTestFieldString("sleep");

    SimpleStringMessage result;
    result.setTestFieldString("Result not changed by echo");
    std::shared_ptr<QGrpcCallReply> reply = _client->testMethod(request);

    QObject::connect(reply.get(), &QGrpcCallReply::finished, this,
                     [reply, &result] { result = reply->read<SimpleStringMessage>(); });

    QSignalSpy replyErrorSpy(reply.get(), &QGrpcCallReply::errorOccurred);
    QVERIFY(replyErrorSpy.isValid());

    QTimer::singleShot(MessageLatencyThreshold, reply.get(), &QGrpcCallReply::abort);

    QTRY_COMPARE_EQ_WITH_TIMEOUT(replyErrorSpy.count(), 1, FailTimeout);
    QCOMPARE_EQ(result.testFieldString(), "Result not changed by echo");
}

void QtGrpcClientTest::StreamStringTest()
{
    const int ExpectedMessageCount = 4;

    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);

    QSignalSpy messageReceivedSpy(stream.get(), &QGrpcStream::messageReceived);
    QVERIFY(messageReceivedSpy.isValid());
    QSignalSpy streamErrorSpy(stream.get(), &QGrpcStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, stream] {
        SimpleStringMessage ret = stream->read<SimpleStringMessage>();
        result.setTestFieldString(result.testFieldString() + ret.testFieldString());
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    QCOMPARE(streamErrorSpy.count(), 0);

    QCOMPARE(messageReceivedSpy.count(), ExpectedMessageCount);
    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3Stream4");
}

void QtGrpcClientTest::StreamStringAndAbortTest()
{
    const int ExpectedMessageCount = 2;

    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());
    QSignalSpy streamErrorSpy(stream.get(), &QGrpcStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    int i = 0;
    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, &i, stream] {
        SimpleStringMessage ret = stream->read<SimpleStringMessage>();
        result.setTestFieldString(result.testFieldString() + ret.testFieldString());
        if (++i == ExpectedMessageCount)
            stream->abort();
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    QCOMPARE(streamErrorSpy.count(), 0);
    QCOMPARE(i, 2);
    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2");
}

void QtGrpcClientTest::StreamStringAndDeferredAbortTest()
{
    const int ExpectedMessageCount = 3;

    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    QSignalSpy streamErrorSpy(stream.get(), &QGrpcStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    QSignalSpy messageReceivedSpy(stream.get(), &QGrpcStream::messageReceived);
    QVERIFY(messageReceivedSpy.isValid());

    int i = 0;
    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, stream, &i] {
        SimpleStringMessage ret = stream->read<SimpleStringMessage>();
        result.setTestFieldString(result.testFieldString() + ret.testFieldString());
        if (++i == ExpectedMessageCount)
            QTimer::singleShot(MessageLatencyThreshold, stream.get(), &QGrpcStream::abort);
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    QCOMPARE(streamErrorSpy.count(), 0);
    QCOMPARE(messageReceivedSpy.count(), ExpectedMessageCount);

    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3");
}

// TODO: Protobuf property doesn't have changed signal
/*
void QtGrpcClientTest::StreamStringAndGetChangedSignalsTests
{
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto result = std::make_shared<SimpleStringMessage>();

    QSignalSpy testFieldChangedSpy(result, &SimpleStringMessage::testFieldStringChanged);
    QVERIFY(testFieldChangedSpy.isValid());

    _client->streamTestMethodServerStream(request, result);

    QTRY_COMPARE(testFieldChangedSpy.count(), 4);
    QCOMPARE_EQ(result->testFieldString(), "Stream4");
}
*/

void QtGrpcClientTest::StreamBlobTest()
{
    BlobMessage result;
    BlobMessage request;
    QFile testFile(":/assets/testfile");
    QVERIFY(testFile.open(QFile::ReadOnly));

    request.setTestBytes(testFile.readAll());
    QByteArray dataHash = QCryptographicHash::hash(request.testBytes(), QCryptographicHash::Sha256);

    auto stream = _client->streamTestMethodBlobServerStream(request);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());
    QSignalSpy streamErrorSpy(stream.get(), &QGrpcStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, stream] {
        BlobMessage ret = stream->read<BlobMessage>();
        result.setTestBytes(ret.testBytes());
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1, MessageLatencyWithThreshold);
    QCOMPARE_EQ(streamErrorSpy.count(), 0);

    QVERIFY(!result.testBytes().isEmpty());
    QByteArray returnDataHash = QCryptographicHash::hash(result.testBytes(),
                                                         QCryptographicHash::Sha256);
    QCOMPARE_EQ(returnDataHash, dataHash);
}

void QtGrpcClientTest::CallMessageAsyncTest()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");

    std::shared_ptr<QGrpcCallReply> reply = _client->testMethodStatusMessage(request);

    QSignalSpy replyErrorSpy(reply.get(), &QGrpcCallReply::errorOccurred);
    QVERIFY(replyErrorSpy.isValid());

    QTRY_COMPARE_EQ_WITH_TIMEOUT(replyErrorSpy.count(), 1, FailTimeout);
    QCOMPARE(qvariant_cast<QGrpcStatus>(replyErrorSpy.at(0).first()).message(),
             request.testFieldString());
}

void QtGrpcClientTest::CallMessageClientAsyncTest()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");

    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    _client->testMethodStatusMessage(request);

    QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, FailTimeout);

    QCOMPARE(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).message(),
             request.testFieldString());
}

void QtGrpcClientTest::CallMessageClientTest()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");
    auto result = std::make_shared<SimpleStringMessage>();

    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    _client->testMethodStatusMessage(request, result.get());

    QTRY_COMPARE_GE_WITH_TIMEOUT(clientErrorSpy.count(), 1, FailTimeout);

    QCOMPARE(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).message(),
             request.testFieldString());
}

void QtGrpcClientTest::StatusMessageClientTest()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");
    auto result = std::make_shared<SimpleStringMessage>();

    QGrpcStatus status = _client->testMethodStatusMessage(request, result.get());

    QCOMPARE_EQ(status.message(), request.testFieldString());
}

void QtGrpcClientTest::StreamAndGetAsyncReplyTest()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");
    QGrpcStatus::StatusCode asyncStatus = QGrpcStatus::StatusCode::Ok;
    QString statusMessage;

    auto reply = _client->testMethodStatusMessage(request);

    reply->subscribe(
            this, [] { QVERIFY(false); },
            [&asyncStatus, &statusMessage](const QGrpcStatus &status) {
                asyncStatus = status.code();
                statusMessage = status.message();
            });

    QTRY_COMPARE_WITH_TIMEOUT(statusMessage, request.testFieldString(),
                              MessageLatencyWithThreshold);

    SimpleStringMessage result;
    request.setTestFieldString("Hello Qt!");

    reply = _client->testMethod(request);
    reply->subscribe(this, [reply, &result] { result = reply->read<SimpleStringMessage>(); });

    QTRY_COMPARE_WITH_TIMEOUT(result.testFieldString(), request.testFieldString(),
                              MessageLatencyWithThreshold);

    result.setTestFieldString("");
    request.setTestFieldString("Hello Qt1!");

    reply = _client->testMethod(request);
    reply->subscribe(
            this, [reply, &result] { result = reply->read<SimpleStringMessage>(); },
            [] { QVERIFY(false); });

    QTRY_COMPARE_WITH_TIMEOUT(result.testFieldString(), request.testFieldString(),
                              MessageLatencyWithThreshold);
}

void QtGrpcClientTest::MultipleStreamsTest()
{
    const int ExpectedMessageCount = 4;
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);
    // Ensure we're not reusing streams
    QCOMPARE_NE(stream, _client->streamTestMethodServerStream(request));

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    QSignalSpy streamErrorSpy(stream.get(), &QGrpcStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    QSignalSpy steamMessageRecievedSpy(stream.get(), &QGrpcStream::messageReceived);
    QVERIFY(steamMessageRecievedSpy.isValid());

    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, stream] {
        SimpleStringMessage ret = stream->read<SimpleStringMessage>();
        result.setTestFieldString(result.testFieldString() + ret.testFieldString());
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    QCOMPARE_EQ(streamErrorSpy.count(), 0);

    QCOMPARE_EQ(steamMessageRecievedSpy.count(), ExpectedMessageCount);
    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3Stream4");
}

void QtGrpcClientTest::MultipleStreamsCancelTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);
    auto streamNext = _client->streamTestMethodServerStream(request);

    QCOMPARE_NE(stream, streamNext);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());
    QSignalSpy streamErrorSpy(stream.get(), &QGrpcStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    QSignalSpy streamNextFinishedSpy(streamNext.get(), &QGrpcStream::finished);
    QVERIFY(streamNextFinishedSpy.isValid());
    QSignalSpy streamNextErrorSpy(streamNext.get(), &QGrpcStream::errorOccurred);
    QVERIFY(streamNextErrorSpy.isValid());

    streamNext->abort();

    QCOMPARE(streamFinishedSpy.count(), 0);
    QCOMPARE(streamErrorSpy.count(), 0);

    QCOMPARE(streamNextFinishedSpy.count(), 1);
    QCOMPARE(streamNextErrorSpy.count(), 0);

    stream = _client->streamTestMethodServerStream(request);
    QCOMPARE_NE(stream, streamNext);

    streamNext = _client->streamTestMethodServerStream(request);

    QCOMPARE_NE(stream, streamNext);

    QSignalSpy otherStreamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());
    QSignalSpy otherStreamErrorSpy(stream.get(), &QGrpcStream::errorOccurred);
    QVERIFY(otherStreamErrorSpy.isValid());

    QSignalSpy otherStreamNextFinishedSpy(streamNext.get(), &QGrpcStream::finished);
    QVERIFY(streamNextFinishedSpy.isValid());
    QSignalSpy otherStreamNextErrorSpy(streamNext.get(), &QGrpcStream::errorOccurred);
    QVERIFY(otherStreamNextErrorSpy.isValid());

    stream->abort();

    QCOMPARE(otherStreamFinishedSpy.count(), 1);
    QCOMPARE_EQ(otherStreamErrorSpy.count(), 0);

    QCOMPARE(otherStreamNextFinishedSpy.count(), 0);
    QCOMPARE_EQ(otherStreamNextErrorSpy.count(), 0);
}

void QtGrpcClientTest::CallNonCompatibleArgRetTest()
{
    const QtProtobuf::sint32 TestValue = 2048;
    const QString TestValueString = QString::number(TestValue);

    SimpleIntMessage request;
    request.setTestField(TestValue);
    auto result = std::make_shared<SimpleStringMessage>();
    QCOMPARE_EQ(_client->testMethodNonCompatibleArgRet(request, result.get()), QGrpcStatus::Ok);
    QCOMPARE_EQ(result->testFieldString(), TestValueString);
}

void QtGrpcClientTest::CallStringThreadTest()
{
    SimpleStringMessage request;
    auto result = std::make_shared<SimpleStringMessage>();

    request.setTestFieldString("Hello Qt from thread!");

    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    bool ok = false;
    const std::unique_ptr<QThread> thread(QThread::create([&] {
        ok = _client->testMethod(request, result.get()) == QGrpcStatus::Ok;
    }));

    thread->start();

    QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, MessageLatencyWithThreshold);
    QVERIFY(!ok);
    QVERIFY(result->testFieldString().isEmpty());
    QVERIFY(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first())
                    .message()
                    .startsWith("QAbstractGrpcClient::call is called from a different thread."));
}

void QtGrpcClientTest::StringAsyncThreadTest()
{
    SimpleStringMessage request;
    SimpleStringMessage result;
    request.setTestFieldString("Hello Qt from thread!");

    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    const std::unique_ptr<QThread> thread(QThread::create([&] {
        QEventLoop waiter;
        std::shared_ptr<QGrpcCallReply> reply = _client->testMethod(request);
        QObject::connect(reply.get(), &QGrpcCallReply::finished, &waiter,
                         [reply, &result, &waiter] {
                             result = reply->read<SimpleStringMessage>();
                             waiter.quit();
                         });
        waiter.exec();
    }));

    thread->start();
    QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, FailTimeout);
    QTRY_VERIFY(result.testFieldString().isEmpty());
    QTRY_VERIFY(
            qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first())
                    .message()
                    .startsWith("QAbstractGrpcClient::call is called from a different thread."));
    ;
}

void QtGrpcClientTest::StreamStringThreadTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    int i = 0;
    const std::unique_ptr<QThread> thread(QThread::create([&] {
        QEventLoop waiter;
        auto stream = _client->streamTestMethodServerStream(request);
        QObject::connect(stream.get(), &QGrpcStream::messageReceived, &waiter,
                         [&result, &i, &waiter, stream] {
                             SimpleStringMessage ret = stream->read<SimpleStringMessage>();
                             result.setTestFieldString(result.testFieldString()
                                                       + ret.testFieldString());
                             if (++i == 4)
                                 waiter.quit();
                         });

        waiter.exec();
    }));

    thread->start();

    QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, FailTimeout);
    QTRY_VERIFY(result.testFieldString().isEmpty());
    QTRY_VERIFY(
            qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first())
                    .message()
                    .startsWith("QAbstractGrpcClient::stream is called from a different thread."));
}

void QtGrpcClientTest::StreamCancelWhileErrorTimeoutTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());
    QSignalSpy streamErrorSpy(stream.get(), &QGrpcStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    stream->abort();
    stream.reset();

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1, MessageLatencyWithThreshold);
    QCOMPARE_EQ(streamErrorSpy.count(), 0);
}

void QtGrpcClientTest::MetadataTest()
{
    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    QGrpcCallOptions opt;
    opt.withMetadata({ { "client_header", "1" }, { "client_return_header", "valid_value" } });
    auto reply = _client->testMetadata({}, opt);

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
    if (channelType == "native")
        QEXPECT_FAIL("", "Unimplemented in the reference gRPC channel", Abort);
    QCOMPARE_EQ(serverHeaderCount, 1);
    QCOMPARE_EQ(clientReturnHeader, "valid_value"_ba);
}

void QtGrpcClientTest::CallDeadlineTest_data()
{
    QTest::addColumn<std::chrono::milliseconds>("timeout");
    constexpr std::array<qreal, 4> messageLatencyFractions{ 0.7, 0.9, 1.0, 1.3 };
    for (const auto &fraction : messageLatencyFractions)
        QTest::newRow(QString("MessageLatency * %1").arg(fraction).toStdString().c_str())
                << std::chrono::milliseconds(static_cast<int64_t>(MessageLatency * fraction));
}

void QtGrpcClientTest::CallDeadlineTest()
{
    QFETCH(const std::chrono::milliseconds, timeout);

    QGrpcCallOptions opt;
    opt.withDeadline(timeout);

    SimpleStringMessage request;
    request.setTestFieldString("sleep");

    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    auto reply = _client->testMethod(request, opt);
    QSignalSpy callFinishedSpy(reply.get(), &QGrpcCallReply::finished);
    QVERIFY(callFinishedSpy.isValid());

    if (timeout.count() < MessageLatency) {
        QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, FailTimeout);
        const auto code = qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).code();
        // Really low timeout can trigger before service becomes available
        QVERIFY(code == QGrpcStatus::StatusCode::Cancelled
                || code == QGrpcStatus::StatusCode::Unavailable);
    } else if (timeout.count() >= MessageLatencyWithThreshold) {
        QTRY_COMPARE_EQ_WITH_TIMEOUT(callFinishedSpy.count(), 1, MessageLatencyWithThreshold);
        QCOMPARE(reply->read<SimpleStringMessage>().testFieldString(), request.testFieldString());
    } else {
        // Because we're can't be sure about the result,
        // abort the call, that might affect other tests.
        reply->abort();
    }
}

void QtGrpcClientTest::StreamDeadlineTest_data()
{
    const int ExpectedMessageCount = 4;
    QTest::addColumn<std::chrono::milliseconds>("timeout");
    QTest::addColumn<int>("ExpectedMessageCount");
    constexpr std::array<qreal, 4> messageLatencyFractions{ 0.7, 0.9, 1.0, 1.3 };
    for (const auto &fraction : messageLatencyFractions)
        QTest::newRow(QString("MessageLatency * ExpectedMessageCount * %1")
                              .arg(fraction)
                              .toStdString()
                              .c_str())
                << std::chrono::milliseconds(static_cast<int64_t>((MessageLatency * fraction
                                                                   * ExpectedMessageCount)))
                << ExpectedMessageCount;
}

void QtGrpcClientTest::StreamDeadlineTest()
{
    QFETCH(const std::chrono::milliseconds, timeout);
    QFETCH(const int, ExpectedMessageCount);

    QGrpcCallOptions opt;
    opt.withDeadline(timeout);

    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request, opt);

    QSignalSpy streamErrorSpy(stream.get(), &QGrpcStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());
    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    SimpleStringMessage result;
    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, stream] {
        SimpleStringMessage ret = stream->read<SimpleStringMessage>();
        result.setTestFieldString(result.testFieldString() + ret.testFieldString());
    });

    if (timeout.count() < MessageLatency * ExpectedMessageCount) {
        QTRY_COMPARE_EQ_WITH_TIMEOUT(streamErrorSpy.count(), 1, FailTimeout);
        const auto code = qvariant_cast<QGrpcStatus>(streamErrorSpy.at(0).first()).code();
        // Really low timeout can trigger before service becomes available
        QVERIFY(code == QGrpcStatus::StatusCode::Cancelled
                || code == QGrpcStatus::StatusCode::Unavailable);
    } else if (timeout.count() >= MessageLatencyWithThreshold * ExpectedMessageCount) {
        QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                     MessageLatencyWithThreshold * ExpectedMessageCount);
        QCOMPARE(streamErrorSpy.count(), 0);
        QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3Stream4");
    } else {
        // Because we're can't be sure about the result,
        // abort the stream, that might affect other tests.
        stream->abort();
    }
}

QTEST_MAIN(QtGrpcClientTest)

#include "tst_grpc_client.moc"
