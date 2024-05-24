// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include <grpcclienttestbase.h>

#include <QtGrpc/QGrpcCallOptions>

#include <QtCore/QCryptographicHash>
#include <QtCore/QFile>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <QtTest/QSignalSpy>
#include <QtTest/QTest>

#include <message_latency_defs.h>
#include <testservice_client.grpc.qpb.h>

using namespace Qt::Literals::StringLiterals;
using namespace qtgrpc::tests;

class QtGrpcClientServerStreamTest : public GrpcClientTestBase
{
    Q_OBJECT
public:
    QtGrpcClientServerStreamTest()
        : GrpcClientTestBase(
                Channels{ GrpcClientTestBase::Channel::Qt })
    {
    }

private Q_SLOTS:
    void valid();
    void cancel();
    void deferredCancel();
    void hugeBlob();
    void getAsyncReply();
    void multipleStreams();
    void multipleStreamsCancel();
    void inThread();
    void cancelWhileErrorTimeout();
    void deadline_data();
    void deadline();
};

void QtGrpcClientServerStreamTest::valid()
{
    const int ExpectedMessageCount = 4;

    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = client()->testMethodServerStream(request);

    QSignalSpy messageReceivedSpy(stream.get(), &QGrpcServerStream::messageReceived);
    QVERIFY(messageReceivedSpy.isValid());

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, this, [&result, stream] {
        const auto ret = stream->read<SimpleStringMessage>();
        QVERIFY(ret.has_value());
        result.setTestFieldString(result.testFieldString() + ret->testFieldString());
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    auto args = streamFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QCOMPARE_EQ(args.first().value<QGrpcStatus>().code(), QGrpcStatus::StatusCode::Ok);

    QCOMPARE(messageReceivedSpy.count(), ExpectedMessageCount);
    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3Stream4");
}

void QtGrpcClientServerStreamTest::cancel()
{
    const int ExpectedMessageCount = 2;

    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = client()->testMethodServerStream(request);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    int i = 0;
    QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, this, [&] {
        const auto ret = stream->read<SimpleStringMessage>();
        QVERIFY(ret.has_value());
        result.setTestFieldString(result.testFieldString() + ret->testFieldString());
        if (++i == ExpectedMessageCount)
            stream->cancel();
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);

    auto args = streamFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QVERIFY(args.first().value<QGrpcStatus>() == QGrpcStatus::StatusCode::Cancelled);

    QCOMPARE(i, 2);
    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2");
}

void QtGrpcClientServerStreamTest::deferredCancel()
{
    const int ExpectedMessageCount = 3;

    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = client()->testMethodServerStream(request);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    QSignalSpy messageReceivedSpy(stream.get(), &QGrpcServerStream::messageReceived);
    QVERIFY(messageReceivedSpy.isValid());

    int i = 0;
    QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, this, [&] {
        const auto ret = stream->read<SimpleStringMessage>();
        QVERIFY(ret.has_value());
        result.setTestFieldString(result.testFieldString() + ret->testFieldString());
        if (++i == ExpectedMessageCount)
            QTimer::singleShot(MessageLatencyThreshold, stream.get(), &QGrpcServerStream::cancel);
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    auto args = streamFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QVERIFY(args.first().value<QGrpcStatus>() == QGrpcStatus::StatusCode::Cancelled);
    QCOMPARE(messageReceivedSpy.count(), ExpectedMessageCount);

    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3");
}

void QtGrpcClientServerStreamTest::hugeBlob()
{
    BlobMessage result;
    BlobMessage request;
    QFile testFile(":/assets/testfile");
    QVERIFY(testFile.open(QFile::ReadOnly));

    request.setTestBytes(testFile.readAll());
    QByteArray dataHash = QCryptographicHash::hash(request.testBytes(), QCryptographicHash::Sha256);

    auto stream = client()->testMethodBlobServerStream(request);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, this, [&result, stream] {
        const auto ret = stream->read<BlobMessage>();
        QVERIFY(ret.has_value());
        result.setTestBytes(ret->testBytes());
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1, MessageLatencyWithThreshold);
    auto args = streamFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QCOMPARE_EQ(args.first().value<QGrpcStatus>().code(), QGrpcStatus::StatusCode::Ok);

    QVERIFY(!result.testBytes().isEmpty());
    QByteArray returnDataHash =
            QCryptographicHash::hash(result.testBytes(), QCryptographicHash::Sha256);
    QCOMPARE_EQ(returnDataHash, dataHash);
}

void QtGrpcClientServerStreamTest::getAsyncReply()
{
    SimpleStringMessage request;
    request.setTestFieldString("Some status message");
    QGrpcStatus::StatusCode asyncStatus;
    QString statusMessage;

    auto reply = client()->testMethodStatusMessage(request);

    bool finishedCalled = false;
    reply->subscribe(
            this, [&finishedCalled] { finishedCalled = true; },
            [&asyncStatus, &statusMessage](const QGrpcStatus &status) {
                asyncStatus = status.code();
                statusMessage = status.message();
            });

    QTRY_COMPARE_WITH_TIMEOUT(statusMessage, request.testFieldString(),
                              MessageLatencyWithThreshold);
    QVERIFY(!finishedCalled);

    SimpleStringMessage result;
    request.setTestFieldString("Hello Qt!");

    reply = client()->testMethod(request);
    reply->subscribe(this, [reply, &result] {
        const auto ret = reply->read<SimpleStringMessage>();
        QVERIFY(ret.has_value());
        result = *ret;
    });

    QTRY_COMPARE_WITH_TIMEOUT(result.testFieldString(), request.testFieldString(),
                              MessageLatencyWithThreshold);

    result.setTestFieldString("");
    request.setTestFieldString("Hello Qt1!");

    reply = client()->testMethod(request);
    reply->subscribe(
        this,
        [reply, &result] {
            const auto ret = reply->read<SimpleStringMessage>();
            QVERIFY(ret.has_value());
            result = *ret;
        },
        [](const auto &) { QVERIFY(false); });

    QTRY_COMPARE_WITH_TIMEOUT(result.testFieldString(), request.testFieldString(),
                              MessageLatencyWithThreshold);
}

void QtGrpcClientServerStreamTest::multipleStreams()
{
    const int ExpectedMessageCount = 4;
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = client()->testMethodServerStream(request);
    // Ensure we're not reusing streams
    QCOMPARE_NE(stream, client()->testMethodServerStream(request));

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    QSignalSpy steamMessageRecievedSpy(stream.get(), &QGrpcServerStream::messageReceived);
    QVERIFY(steamMessageRecievedSpy.isValid());

    QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, this, [&result, stream] {
        const auto ret = stream->read<SimpleStringMessage>();
        QVERIFY(ret.has_value());
        result.setTestFieldString(result.testFieldString() + ret->testFieldString());
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    auto args = streamFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QCOMPARE_EQ(args.first().value<QGrpcStatus>().code(), QGrpcStatus::StatusCode::Ok);

    QCOMPARE_EQ(steamMessageRecievedSpy.count(), ExpectedMessageCount);
    QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3Stream4");
}

void QtGrpcClientServerStreamTest::multipleStreamsCancel()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = client()->testMethodServerStream(request);
    auto streamNext = client()->testMethodServerStream(request);

    QCOMPARE_NE(stream, streamNext);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    QSignalSpy streamNextFinishedSpy(streamNext.get(), &QGrpcServerStream::finished);
    QVERIFY(streamNextFinishedSpy.isValid());

    streamNext->cancel();

    QCOMPARE(streamFinishedSpy.count(), 0);

    QCOMPARE(streamNextFinishedSpy.count(), 1);
    auto args = streamNextFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QVERIFY(args.first().value<QGrpcStatus>() == QGrpcStatus::StatusCode::Cancelled);

    stream = client()->testMethodServerStream(request);
    QCOMPARE_NE(stream, streamNext);

    streamNext = client()->testMethodServerStream(request);

    QCOMPARE_NE(stream, streamNext);

    QSignalSpy otherStreamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(otherStreamFinishedSpy.isValid());

    QSignalSpy otherStreamNextFinishedSpy(streamNext.get(), &QGrpcServerStream::finished);
    QVERIFY(otherStreamNextFinishedSpy.isValid());

    stream->cancel();

    QCOMPARE(otherStreamFinishedSpy.count(), 1);
    args = otherStreamFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QVERIFY(args.first().value<QGrpcStatus>() == QGrpcStatus::StatusCode::Cancelled);

    const int ExpectedMessageCount = 4;
    QTRY_COMPARE_EQ_WITH_TIMEOUT(otherStreamNextFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    args = otherStreamNextFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QCOMPARE_EQ(args.first().value<QGrpcStatus>().code(), QGrpcStatus::StatusCode::Ok);
}

void QtGrpcClientServerStreamTest::inThread()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    QSignalSpy clientErrorSpy(client().get(), &TestService::Client::errorOccurred);
    QVERIFY(clientErrorSpy.isValid());

    int i = 0;
    const std::unique_ptr<QThread> thread(QThread::create([&] {
        QEventLoop waiter;
        auto stream = client()->testMethodServerStream(request);
        QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, &waiter,
                         [&result, &i, &waiter, stream] {
                             const auto ret = stream->read<SimpleStringMessage>();
                             QVERIFY(ret.has_value());
                             result.setTestFieldString(result.testFieldString()
                                                       + ret->testFieldString());
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
                    .startsWith(
                            "QGrpcClientBase::startStream<QGrpcServerStream> is called from a "
                            "different thread."));
}

void QtGrpcClientServerStreamTest::cancelWhileErrorTimeout()
{
    SimpleStringMessage result;
    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = client()->testMethodServerStream(request);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());

    stream->cancel();
    stream.reset();

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1, MessageLatencyWithThreshold);
    auto args = streamFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    QVERIFY(args.first().value<QGrpcStatus>() == QGrpcStatus::StatusCode::Cancelled);
}

void QtGrpcClientServerStreamTest::deadline_data()
{
    const int ExpectedMessageCount = 4;
    QTest::addColumn<QGrpcDuration>("timeout");
    QTest::addColumn<int>("ExpectedMessageCount");
    constexpr std::array<qreal, 4> messageLatencyFractions{ 0.7, 0.9, 1.0, 1.3 };
    for (const auto &fraction : messageLatencyFractions)
        QTest::newRow(QString("MessageLatency * ExpectedMessageCount * %1")
                              .arg(fraction)
                              .toStdString()
                              .c_str())
                << QGrpcDuration(
                           static_cast<int64_t>((MessageLatency * fraction * ExpectedMessageCount)))
                << ExpectedMessageCount;
}

void QtGrpcClientServerStreamTest::deadline()
{
    QFETCH(const QGrpcDuration, timeout);
    QFETCH(const int, ExpectedMessageCount);

    QGrpcCallOptions opt;
    opt.setDeadline(timeout);

    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = client()->testMethodServerStream(request, opt);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());
    QObject::connect(stream.get(), &QGrpcServerStream::finished, this,
                     [](const auto &status) { qDebug() << status; });

    SimpleStringMessage result;
    QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, this, [&result, stream] {
        const auto ret = stream->read<SimpleStringMessage>();
        QVERIFY(ret.has_value());
        result.setTestFieldString(result.testFieldString() + ret->testFieldString());
    });

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1, FailTimeout);
    auto args = streamFinishedSpy.first();
    QCOMPARE(args.count(), 1);
    const auto code = args.first().value<QGrpcStatus>().code();
    if (timeout.count() < MessageLatency * ExpectedMessageCount) {
        // Really low timeout can trigger before service becomes available
        QVERIFY(code == QGrpcStatus::StatusCode::Cancelled
                || code == QGrpcStatus::StatusCode::Unavailable);
    } else if (timeout.count() >= MessageLatencyWithThreshold * ExpectedMessageCount) {
        QVERIFY(code == QGrpcStatus::StatusCode::Ok);
        QCOMPARE_EQ(result.testFieldString(), "Stream1Stream2Stream3Stream4");
    } else {
        // Because we're can't be sure about the result,
        // cancel the stream, that might affect other tests.
        stream->cancel();
    }
}

QTEST_MAIN(QtGrpcClientServerStreamTest)

#include "tst_grpc_client_serverstream.moc"
