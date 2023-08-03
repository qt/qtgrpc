// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include <QGrpcCallReply>
#if QT_CONFIG(native_grpc)
#  include <QGrpcChannel>
#endif
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QGrpcHttp2Channel>
#include <QSignalSpy>
#include <QTest>
#include <QtGrpc/QGrpcCallOptions>
#include <QtGrpc/QGrpcChannelOptions>
#include <memory>
#if QT_CONFIG(native_grpc)
#  include <grpcpp/security/credentials.h>
#endif
#include "testservice_client.grpc.qpb.h"
#include <message_latency_defs.h>
#include <server_proc_runner.h>

using namespace Qt::Literals::StringLiterals;
using namespace qtgrpc::tests;

namespace {

class ChannelFactory
{
public:
    enum ChannelType {
        HTTP2,
        GRPC_SOCKET,
        GRPC_HTTP,
    };

    ChannelFactory(ChannelType type) : type(type) { }

    std::shared_ptr<QAbstractGrpcChannel> create(const QGrpcChannelOptions &options) const
    {
        switch (type) {
        case HTTP2:
            return std::make_shared<QGrpcHttp2Channel>(options);
#if QT_CONFIG(native_grpc)
        case GRPC_SOCKET:
        case GRPC_HTTP:
            return std::make_shared<QGrpcChannel>(options,
                                                  QGrpcChannel::InsecureChannelCredentials);
#endif
        }
        return nullptr;
    }

    QUrl getUrl() const
    {
        switch (type) {
        case HTTP2:
            return QUrl("http://localhost:50051", QUrl::StrictMode);
#if QT_CONFIG(native_grpc)
        case GRPC_SOCKET:
            return QUrl("unix:///tmp/test.sock");
        case GRPC_HTTP:
            return QUrl("localhost:50051");
#endif
        }
        return QUrl();
    }

private:
    ChannelType type;
};

} // namespace

class QtGrpcClientNoChannelTest : public QObject
{
    Q_OBJECT
private slots:
    void createClient(const QGrpcChannelOptions &channelOptions)
    {
        client = std::make_shared<TestService::Client>();
        auto channel = channelFactory->create(channelOptions);
        QVERIFY2(channel, "Channel could not been created.");
        client->attachChannel(std::move(channel));
    }

    void initTestCase_data()
    {
        QTest::addColumn<std::shared_ptr<ChannelFactory>>("factory");
        QTest::newRow("Http2Client") << std::make_shared<ChannelFactory>(ChannelFactory::HTTP2);
#if QT_CONFIG(native_grpc)
#  ifndef Q_OS_WINDOWS
        QTest::newRow("GrpcSocket")
                << std::make_shared<ChannelFactory>(ChannelFactory::GRPC_SOCKET);

#  endif
        QTest::newRow("GrpcHttp") << std::make_shared<ChannelFactory>(ChannelFactory::GRPC_HTTP);

#endif
    }

    void initTestCase() { qRegisterProtobufTypes(); }

    void init()
    {
        if (serverProc.state() != QProcess::ProcessState::Running) {
            qInfo() << "Restarting server";
            serverProc.restart();
            QVERIFY2(serverProc.state() == QProcess::ProcessState::Running,
                     "Precondition failed - Server cannot be started.");
        }

        QFETCH_GLOBAL(std::shared_ptr<ChannelFactory>, factory);
        channelFactory = std::move(factory);
    }

    void ChannelAndCallDeadlineTest();

private:
    ServerProcRunner serverProc{ QFINDTESTDATA(TEST_GRPC_SERVER_PATH) };
    std::shared_ptr<ChannelFactory> channelFactory;
    std::shared_ptr<TestService::Client> client;
};

void QtGrpcClientNoChannelTest::ChannelAndCallDeadlineTest()
{
    constexpr auto channelTimeout = std::chrono::milliseconds(static_cast<int64_t>(MessageLatency
                                                                                   * 0.25));
    constexpr auto callTimeout = std::chrono::milliseconds(static_cast<int64_t>(MessageLatency
                                                                                * 0.6));
    QGrpcChannelOptions channelOpts(channelFactory->getUrl());
    channelOpts.withDeadline(channelTimeout);
    createClient(channelOpts);
    QGrpcCallOptions callOpts;
    callOpts.withDeadline(callTimeout);
    auto executeTest = [this](uint64_t minTimeout, uint64_t maxTimeout,
                              std::optional<QGrpcCallOptions> callOptions = std::nullopt) {
        SimpleStringMessage request;
        request.setTestFieldString("sleep");
        QSignalSpy clientErrorSpy(client.get(), &TestService::Client::errorOccurred);
        QVERIFY(clientErrorSpy.isValid());
        std::shared_ptr<QGrpcCallReply> reply;
        if (callOptions)
            reply = client->testMethod(request, *callOptions);
        else
            reply = client->testMethod(request);
        QSignalSpy callFinishedSpy(reply.get(), &QGrpcCallReply::finished);
        QVERIFY(callFinishedSpy.isValid());
        // Still waiting for a timeout
        QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 0, minTimeout);
        // Time window to receive the timout
        QTRY_COMPARE_EQ_WITH_TIMEOUT(clientErrorSpy.count(), 1, maxTimeout);
        const auto code = qvariant_cast<QGrpcStatus>(clientErrorSpy.at(0).first()).code();
        // Really low timeout can trigger before service becomes available
        QVERIFY(code == QGrpcStatus::StatusCode::Cancelled
                || code == QGrpcStatus::StatusCode::Unavailable);
    };
    executeTest(0, channelTimeout.count() + MessageLatencyThreshold);
    executeTest(channelTimeout.count(), callTimeout.count() + MessageLatencyThreshold, callOpts);
}

QTEST_MAIN(QtGrpcClientNoChannelTest)
#include "tst_grpc_client_no_channel.moc"
