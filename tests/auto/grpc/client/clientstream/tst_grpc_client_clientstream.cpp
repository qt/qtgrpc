// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include <grpcclienttestbase.h>

#include <QtCore/QTimer>
#include <QtTest/QTest>
#include <QtTest/QSignalSpy>

#include <testservice_client.grpc.qpb.h>
#include <message_latency_defs.h>

using namespace Qt::Literals::StringLiterals;
using namespace qtgrpc::tests;

class QtGrpcClientClientStreamTest : public GrpcClientTestBase
{
    Q_OBJECT
public:
    QtGrpcClientClientStreamTest()
        : GrpcClientTestBase(
                Channels{ GrpcClientTestBase::Channel::Qt })
    {
    }

private slots:
    void valid();
    void sequentialSend();
    void sequentialSendWithDone();
};

void QtGrpcClientClientStreamTest::valid()
{
    const int ExpectedMessageCount = 4;

    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = client()->streamTestMethodClientStream(request);

    int i = 0;
    QTimer sendTimer;
    QObject::connect(&sendTimer, &QTimer::timeout, this, [&]() {
        stream->sendMessage(request);
        if (++i == ExpectedMessageCount)
            sendTimer.stop();
    });

    sendTimer.start(MessageLatency);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());
    QSignalSpy streamErrorSpy(stream.get(), &QGrpcServerStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    QCOMPARE(streamErrorSpy.count(), 0);

    const auto result = stream->read<SimpleStringMessage>();
    QCOMPARE_EQ(result->testFieldString(), "Stream1Stream2Stream3Stream4");
}

void QtGrpcClientClientStreamTest::sequentialSend()
{
    const int ExpectedMessageCount = 4;

    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = client()->streamTestMethodClientStream(request);

    // Ensure that messages are not lost during the sequential sending right after the stream is
    // instanciated.
    for (int i = 0; i < ExpectedMessageCount; ++i) {
        stream->sendMessage(request);
    }

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());
    QSignalSpy streamErrorSpy(stream.get(), &QGrpcServerStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    QCOMPARE(streamErrorSpy.count(), 0);

    const auto result = stream->read<SimpleStringMessage>();
    QVERIFY(result.has_value());
    QCOMPARE_EQ(result->testFieldString(), "Stream1Stream2Stream3Stream4");
}

void QtGrpcClientClientStreamTest::sequentialSendWithDone()
{
    const int ExpectedMessageCount = 4;

    SimpleStringMessage request;
    request.setTestFieldString("Stream");

    auto stream = client()->streamTestMethodClientStreamWithDone(request);

    // Ensure that messages are not lost during the sequential sending right after the stream is
    // instanciated.
    for (int i = 1; i < (ExpectedMessageCount - 1); ++i) {
        stream->sendMessage(request);
    }
    stream->writesDone();
    request.setTestFieldString("StreamWrong");
    stream->sendMessage(request);

    QSignalSpy streamFinishedSpy(stream.get(), &QGrpcServerStream::finished);
    QVERIFY(streamFinishedSpy.isValid());
    QSignalSpy streamErrorSpy(stream.get(), &QGrpcServerStream::errorOccurred);
    QVERIFY(streamErrorSpy.isValid());

    QTRY_COMPARE_EQ_WITH_TIMEOUT(streamFinishedSpy.count(), 1,
                                 MessageLatencyWithThreshold * ExpectedMessageCount);
    QCOMPARE(streamErrorSpy.count(), 0);

    std::optional<SimpleStringMessage> result = stream->read<SimpleStringMessage>();
    QCOMPARE_EQ(result->testFieldString(), "Stream1Stream2Stream3");
}

QTEST_MAIN(QtGrpcClientClientStreamTest)

#include "tst_grpc_client_clientstream.moc"
