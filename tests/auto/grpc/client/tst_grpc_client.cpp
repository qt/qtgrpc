// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QGrpcCallReply>
#if QT_CONFIG(native_grpc)
#  include <QGrpcChannel>
#endif
#include <QGrpcCredentials>
#include <QGrpcHttp2Channel>
#include <QGrpcInsecureCallCredentials>
#include <QGrpcInsecureChannelCredentials>

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QProcess>
#include <QSignalSpy>
#include <QTest>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#if QT_CONFIG(native_grpc)
#  include <grpcpp/security/credentials.h>
#endif
#include "testservice_client.grpc.qpb.h"

namespace {
using namespace qtgrpc::tests;

static QSharedPointer<TestService::Client> createHttp2Client()
{
    auto client = QSharedPointer<TestService::Client>::create();
    client->attachChannel(QSharedPointer<QGrpcHttp2Channel>::create(
            QUrl("http://localhost:50051", QUrl::StrictMode),
            QGrpcInsecureChannelCredentials() | QGrpcInsecureCallCredentials()));
    return client;
}

#if QT_CONFIG(native_grpc)
#  ifndef Q_OS_WINDOWS
static QSharedPointer<TestService::Client> createGrpcSocketClient()
{
    auto client = QSharedPointer<TestService::Client>::create();
    client->attachChannel(QSharedPointer<QGrpcChannel>::create(QString("unix:///tmp/test.sock"),
                                                               QGrpcChannel::InsecureChannelCredentials));
    return client;
}
#  endif
static QSharedPointer<TestService::Client> createGrpcHttpClient()
{
    auto client = QSharedPointer<TestService::Client>::create();
    client->attachChannel(QSharedPointer<QGrpcChannel>::create(QString("localhost:50051"),
                                                               QGrpcChannel::InsecureChannelCredentials));
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
        QTest::addColumn<QSharedPointer<TestService::Client>>("client");

        QTest::newRow("Http2Client") << createHttp2Client();
#if QT_CONFIG(native_grpc)
#  ifndef Q_OS_WINDOWS
        QTest::newRow("GrpcSocket") << createGrpcSocketClient();
#  endif
        QTest::newRow("GrpcHttp") << createGrpcHttpClient();
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
        QFETCH_GLOBAL(QSharedPointer<TestService::Client>, client);
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

private:
    void startServer()
    {
        serverProc = std::make_unique<QProcess>();
        QString serverPath = QFINDTESTDATA("../shared/test_server/testserver");
        QVERIFY2(!serverPath.isEmpty(), "testserver binary is missing");
        serverProc->start(serverPath);
        serverProc->waitForStarted();
        // Extra time for the server to setup
        QTest::qSleep(1000);
    }

    std::unique_ptr<QProcess> serverProc;
    QSharedPointer<TestService::Client> _client;
};

void QtGrpcClientTest::CallAndRecieveStringTest()
{
    SimpleStringMessage request;
    auto result = QSharedPointer<SimpleStringMessage>::create();
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

    QSharedPointer<QGrpcCallReply> reply = _client->testMethod(request);
    reply->subscribe(this, [reply, &result, &waiter]() {
        result = reply->read<SimpleStringMessage>();
        reply->deleteLater();
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
    _client->testMethod(request, this, [&result, &waiter](QSharedPointer<QGrpcCallReply> reply) {
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

    QSharedPointer<QGrpcCallReply> reply = _client->testMethod(request);

    result.setTestFieldString("Result not changed by echo");
    QObject::connect(reply.get(), &QGrpcCallReply::finished, this, [&result, reply]() {
        result = reply->read<SimpleStringMessage>();
        reply->deleteLater();
    });

    QSignalSpy replyErrorSpy(reply.get(), &QGrpcCallReply::errorOccurred);
    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);

    QVERIFY(replyErrorSpy.isValid());
    QVERIFY(clientErrorSpy.isValid());

    reply->abort();

    QTRY_COMPARE(replyErrorSpy.count(), 1);
    QTRY_COMPARE(clientErrorSpy.count(), 1);

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
    QSharedPointer<QGrpcCallReply> reply = _client->testMethod(request);

    QObject::connect(reply.get(), &QGrpcCallReply::finished, this,
                     [reply, &result]() { result = reply->read<SimpleStringMessage>(); });

    QSignalSpy replyErrorSpy(reply.get(), &QGrpcCallReply::errorOccurred);
    QVERIFY(replyErrorSpy.isValid());

    QTimer::singleShot(500, reply.get(), &QGrpcCallReply::abort);

    QTRY_COMPARE(replyErrorSpy.count(), 1);
    QCOMPARE_EQ(result.testFieldString(), "Result not changed by echo");
}

void QtGrpcClientTest::StreamStringTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);

    QSignalSpy messageReceivedSpy(stream.get(), &QGrpcStream::messageReceived);
    QVERIFY(messageReceivedSpy.isValid());

    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, stream]() {
        SimpleStringMessage ret = stream->read<SimpleStringMessage>();
        result.setTestFieldString(result.testFieldString() + ret.testFieldString());
    });

    QTRY_COMPARE(messageReceivedSpy.count(), 4);
    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3Stream4");
}

void QtGrpcClientTest::StreamStringAndAbortTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    int i = 0;
    auto stream = _client->streamTestMethodServerStream(request);
    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, &i, stream]() {
        SimpleStringMessage ret = stream->read<SimpleStringMessage>();
        ++i;
        result.setTestFieldString(result.testFieldString() + ret.testFieldString());
        if (i == 2) {
            stream->abort();
        }
    });

    QTRY_COMPARE(i, 2);
    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2");
}

void QtGrpcClientTest::StreamStringAndDeferredAbortTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);
    QTimer::singleShot(3400, stream.get(), [stream]() { stream->abort(); });

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    QSignalSpy streamErrorSpy(stream.get(), &QGrpcStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    QSignalSpy messageReceivedSpy(stream.get(), &QGrpcStream::messageReceived);
    QVERIFY(messageReceivedSpy.isValid());

    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, stream]() {
        SimpleStringMessage ret = stream->read<SimpleStringMessage>();
        result.setTestFieldString(result.testFieldString() + ret.testFieldString());
    });

    QTRY_COMPARE(streamFinishedSpy.count(), 1);
    QTRY_COMPARE(streamErrorSpy.count(), 0);
    QTRY_COMPARE(messageReceivedSpy.count(), 3);

    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3");
}

// TODO: Protobuf property doesn't have changed signal
/*
void QtGrpcClientTest::StreamStringAndGetChangedSignalsTests
{
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto result = QSharedPointer<SimpleStringMessage>(new SimpleStringMessage);

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

    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, stream]() {
        BlobMessage ret = stream->read<BlobMessage>();
        result.setTestBytes(ret.testBytes());
    });

    QTRY_VERIFY(!result.testBytes().isEmpty());
    QByteArray returnDataHash = QCryptographicHash::hash(result.testBytes(),
                                                         QCryptographicHash::Sha256);
    QCOMPARE_EQ(returnDataHash, dataHash);
}

void QtGrpcClientTest::CallMessageAsyncTest()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");

    QSharedPointer<QGrpcCallReply> reply = _client->testMethodStatusMessage(request);

    QSignalSpy replyErrorSpy(reply.get(), &QGrpcCallReply::errorOccurred);
    QVERIFY(replyErrorSpy.isValid());

    QTRY_COMPARE(replyErrorSpy.count(), 1);
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

    QTRY_COMPARE(clientErrorSpy.count(), 1);

    QCOMPARE(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).message(),
             request.testFieldString());
}

void QtGrpcClientTest::CallMessageClientTest()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");
    auto result = QSharedPointer<SimpleStringMessage>::create();

    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    _client->testMethodStatusMessage(request, result.get());

    QTRY_COMPARE_GE(clientErrorSpy.count(), 1);

    QCOMPARE(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).message(),
             request.testFieldString());
}

void QtGrpcClientTest::StatusMessageClientTest()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");
    auto result = QSharedPointer<SimpleStringMessage>::create();
    QString statusMessage;

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
            this, []() { QVERIFY(false); },
            [&asyncStatus, &statusMessage](const QGrpcStatus &status) {
                asyncStatus = status.code();
                statusMessage = status.message();
            });

    QTRY_COMPARE_EQ(statusMessage, request.testFieldString());

    SimpleStringMessage result;
    request.setTestFieldString("Hello Qt!");

    reply = _client->testMethod(request);
    reply->subscribe(this, [reply, &result]() { result = reply->read<SimpleStringMessage>(); });

    QTRY_COMPARE_EQ(result.testFieldString(), request.testFieldString());

    result.setTestFieldString("");
    request.setTestFieldString("Hello Qt1!");

    reply = _client->testMethod(request);
    reply->subscribe(
            this, [reply, &result]() { result = reply->read<SimpleStringMessage>(); },
            []() { QVERIFY(false); });

    QTRY_COMPARE_EQ(result.testFieldString(), request.testFieldString());
}

void QtGrpcClientTest::MultipleStreamsTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);
    QCOMPARE_EQ(stream, _client->streamTestMethodServerStream(request));

    QSignalSpy steamMessageRecievedSpy(stream.get(), &QGrpcStream::messageReceived);
    QVERIFY(steamMessageRecievedSpy.isValid());

    QObject::connect(stream.get(), &QGrpcStream::messageReceived, this, [&result, stream]() {
        SimpleStringMessage ret = stream->read<SimpleStringMessage>();
        result.setTestFieldString(result.testFieldString() + ret.testFieldString());
    });

    QTRY_COMPARE(steamMessageRecievedSpy.count(), 4);
    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3Stream4");
}

void QtGrpcClientTest::MultipleStreamsCancelTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);
    auto streamNext = _client->streamTestMethodServerStream(request);

    QCOMPARE_EQ(stream, streamNext);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    QSignalSpy streamNextFinishedSpy(streamNext.get(), &QGrpcStream::finished);
    QVERIFY(streamNextFinishedSpy.isValid());

    streamNext->abort();

    QCOMPARE(streamFinishedSpy.count(), 1);
    QCOMPARE(streamNextFinishedSpy.count(), 1);

    stream = _client->streamTestMethodServerStream(request);
    QCOMPARE_NE(stream, streamNext);

    streamNext = _client->streamTestMethodServerStream(request);

    QCOMPARE_EQ(stream, streamNext);

    QSignalSpy otherStreamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    QSignalSpy otherStreamNextFinishedSpy(streamNext.get(), &QGrpcStream::finished);
    QVERIFY(streamNextFinishedSpy.isValid());

    stream->abort();

    QCOMPARE(otherStreamFinishedSpy.count(), 1);
    QCOMPARE(otherStreamNextFinishedSpy.count(), 1);
}

void QtGrpcClientTest::CallNonCompatibleArgRetTest()
{
    SimpleIntMessage request;
    request.setTestField(2048);
    auto result = QSharedPointer<SimpleStringMessage>::create();
    QCOMPARE_EQ(_client->testMethodNonCompatibleArgRet(request, result.get()), QGrpcStatus::Ok);
    QCOMPARE_EQ(result->testFieldString(), "2048");
}

void QtGrpcClientTest::CallStringThreadTest()
{
    SimpleStringMessage request;
    auto result = QSharedPointer<SimpleStringMessage>::create();

    request.setTestFieldString("Hello Qt from thread!");

    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    bool ok = false;
    QSharedPointer<QThread> thread(QThread::create([&]() {
        ok = _client->testMethod(request, result.get()) == QGrpcStatus::Ok;
    }));

    thread->start();
    QEventLoop wait;
    QTimer::singleShot(1000, &wait, &QEventLoop::quit);
    wait.exec();

    QCOMPARE_EQ(clientErrorSpy.count(), 1);
    QVERIFY(!ok);
    QVERIFY(result->testFieldString().isEmpty());
    QVERIFY(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first())
                    .message()
                    .startsWith("QAbstractGrpcClient::call is called from different "
                                "thread."));
}

void QtGrpcClientTest::StringAsyncThreadTest()
{
    SimpleStringMessage request;
    SimpleStringMessage result;
    request.setTestFieldString("Hello Qt from thread!");

    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    QSharedPointer<QThread> thread(QThread::create([&]() {
        QEventLoop waiter;
        QSharedPointer<QGrpcCallReply> reply = _client->testMethod(request);
        QObject::connect(reply.get(), &QGrpcCallReply::finished, &waiter,
                         [reply, &result, &waiter]() {
                             result = reply->read<SimpleStringMessage>();
                             waiter.quit();
                         });
        waiter.exec();
    }));

    thread->start();
    QTRY_COMPARE_EQ(clientErrorSpy.count(), 1);
    QTRY_VERIFY(result.testFieldString().isEmpty());
    QTRY_VERIFY(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first())
                        .message()
                        .startsWith("QAbstractGrpcClient::call is called from different "
                                    "thread."));
}

void QtGrpcClientTest::StreamStringThreadTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    QSignalSpy clientErrorSpy(_client.get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    int i = 0;
    QSharedPointer<QThread> thread(QThread::create([&]() {
        QEventLoop waiter;
        auto stream = _client->streamTestMethodServerStream(request);
        QObject::connect(stream.get(), &QGrpcStream::messageReceived, &waiter,
                         [&result, &i, &waiter, stream]() {
                             SimpleStringMessage ret = stream->read<SimpleStringMessage>();
                             result.setTestFieldString(result.testFieldString()
                                                       + ret.testFieldString());
                             ++i;
                             if (i == 4) {
                                 waiter.quit();
                             }
                         });

        waiter.exec();
    }));

    thread->start();

    QTRY_COMPARE_EQ(clientErrorSpy.count(), 1);
    QTRY_VERIFY(result.testFieldString().isEmpty());
    QTRY_VERIFY(qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first())
                        .message()
                        .startsWith("QAbstractGrpcClient::stream is called from different "
                                    "thread."));
}

void QtGrpcClientTest::StreamCancelWhileErrorTimeoutTest()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = _client->streamTestMethodServerStream(request);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    stream->abort();
    stream.reset();

    QTRY_COMPARE(streamFinishedSpy.count(), 1);
}

QTEST_MAIN(QtGrpcClientTest)

#include "tst_grpc_client.moc"
