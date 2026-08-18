// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <mockserver.h>

#include <proto/server/event.pb.h>
#include <proto/server/eventhub.grpc.pb.h>

#include <proto/client/event.qpb.h>
#include <proto/client/eventhub_client.grpc.qpb.h>

#include <QtGrpc/qgrpccalloptions.h>
#include <QtGrpc/qgrpcchanneloptions.h>
#include <QtGrpc/qgrpchttp2channel.h>

#include <QtTest/qsignalspy.h>
#include <QtTest/qtest.h>

#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qelapsedtimer.h>
#include <QtCore/qhash.h>
#include <QtCore/qregularexpression.h>
#include <QtCore/qscopeguard.h>
#include <QtCore/qtenvironmentvariables.h>
#include <QtCore/qtimer.h>

#include <atomic>
#include <string>
#include <vector>

using namespace Qt::Literals::StringLiterals;
using namespace QtGrpc;
using MultiHash = QMultiHash<QByteArray, QByteArray>;

class QtGrpcClientEnd2EndTest : public QObject
{
    Q_OBJECT

public:
    static std::string serverHttpAddress() { return "localhost:50061"; }
    static std::string serverHttpsAddress() { return "localhost:50062"; }
    static std::string serverUnixAddress() { return "unix:///tmp/qtgrpc_test_end2end.sock"; }
    static std::string serverUnixAbstractAddress() { return "unix-abstract:qtgrpc_test_end2end"; }
    static std::vector<ListeningPort> serverListeningPorts()
    {
        return {
            { serverHttpAddress(),         grpc::InsecureServerCredentials() },
#if QT_CONFIG(ssl)
            { serverHttpsAddress(),        serverSslCredentials()            },
#endif
#ifdef Q_OS_UNIX
            { serverUnixAddress(),         grpc::InsecureServerCredentials() },
#endif

#ifdef Q_OS_LINUX
            { serverUnixAbstractAddress(), grpc::InsecureServerCredentials() },
#endif
        };
    }

private Q_SLOTS:
    void initTestCase_data() const;
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    // Testcases:
    void clientMetadataReceived_data() const;
    void clientMetadataReceived();
    void serverMetadataReceived_data() const;
    void serverMetadataReceived();
    void serverInitialMetadataEmitted();

    void bidiStreamsInOrder();
    void hostUriRoundTrip();

    void clientHandlesCompression_data() const;
    void clientHandlesCompression();

    void channelChangeCancelsInFlightRPCs_data() const;
    void channelChangeCancelsInFlightRPCs();
    void channelTeardownCancelsActiveStream();

    void maximumReceiveMessageSize_data() const;
    void maximumReceiveMessageSize();

    void maximumMetadataSize_data() const;
    void maximumMetadataSize();

    void burstWritesDoNotOverflowStack();

    void receiveWindowConfig_data() const;
    void receiveWindowConfig();

    void reconnectBackoff_data() const;
    void reconnectBackoff();
    void reconnectRecoversAfterServerRestart();
    void connectTimeout_data() const;
    void connectTimeout();
    void channelTeardownAfterConnectTimeoutAbort();

private:
    static std::shared_ptr<grpc::ServerCredentials> serverSslCredentials()
    {
        grpc::SslServerCredentialsOptions opts(GRPC_SSL_DONT_REQUEST_CLIENT_CERTIFICATE);
        opts.pem_key_cert_pairs.push_back({ SslKey, SslCert });
        return grpc::SslServerCredentials(opts);
    }

    void restartServer(const grpc::ChannelArguments &channelArgs = {})
    {
        if (m_server)
            QVERIFY(m_server->stop());
        m_service = std::make_unique<EventHub::AsyncService>();
        m_server = std::make_unique<MockServer>();
        QVERIFY(m_server->start(serverListeningPorts(), { m_service.get() }, channelArgs));
    }

private:
    std::unique_ptr<MockServer> m_server;
    std::unique_ptr<EventHub::AsyncService> m_service;
    std::unique_ptr<qt::EventHub::Client> m_client;
};

void QtGrpcClientEnd2EndTest::initTestCase_data() const
{
    QTest::addColumn<QUrl>("hostUri");
    QTest::addColumn<QGrpcChannelOptions>("channelOptions");

    QTest::newRow("http") << QUrl("http://"_ba + QByteArrayView(serverHttpAddress()))
                          << QGrpcChannelOptions{};

#if QT_CONFIG(ssl)
    QSslConfiguration tlsConfig;
    tlsConfig.setProtocol(QSsl::TlsV1_2);
    tlsConfig.setCaCertificates({ QSslCertificate{ QByteArray(SslCert) } });
    tlsConfig.setAllowedNextProtocols({ "h2"_ba });
    QGrpcChannelOptions tlsOpts;
    tlsOpts.setSslConfiguration(tlsConfig);
    QTest::newRow("https") << QUrl("https://"_ba + QByteArrayView(serverHttpsAddress())) << tlsOpts;
#endif

#ifdef Q_OS_UNIX
    QTest::newRow("unix") << QUrl(serverUnixAddress().data()) << QGrpcChannelOptions{};
#endif

#ifdef Q_OS_LINUX
    QTest::newRow("unix-abstract")
        << QUrl(serverUnixAbstractAddress().data()) << QGrpcChannelOptions{};
#endif
}

void QtGrpcClientEnd2EndTest::initTestCase()
{
    QTest::failOnWarning();
    restartServer();
}

void QtGrpcClientEnd2EndTest::cleanupTestCase()
{
    m_client.reset();
    QVERIFY(m_server->stop());
    m_service.reset();
}

void QtGrpcClientEnd2EndTest::init()
{
    QVERIFY(m_service && m_server);
    QFETCH_GLOBAL(const QUrl, hostUri);
    QFETCH_GLOBAL(const QGrpcChannelOptions, channelOptions);
    m_client = std::make_unique<qt::EventHub::Client>();
    QVERIFY(m_client->attachChannel(std::make_shared<QGrpcHttp2Channel>(hostUri, channelOptions)));
}

void QtGrpcClientEnd2EndTest::cleanup()
{
    m_client.reset();
}

void QtGrpcClientEnd2EndTest::clientMetadataReceived_data() const
{
    QTest::addColumn<MultiHash>("callMetadata");
    QTest::addColumn<MultiHash>("channelMetadata");
    MultiHash callMd{
        { "client-call-single", "call-value-1" },
        { "client-call-multi",  "call-a"       },
        { "client-call-multi",  "call-b"       }
    };
    MultiHash channelMd{
        { "client-channel-single", "channel-value-1" },
        { "client-channel-multi",  "channel-a"       },
        { "client-channel-multi",  "channel-b"       }
    };
    QTest::addRow("call") << callMd << MultiHash{};
    QTest::addRow("channel") << MultiHash{} << channelMd;
    QTest::addRow("call+channel") << callMd << channelMd;
}

void QtGrpcClientEnd2EndTest::clientMetadataReceived()
{
    QFETCH(const MultiHash, callMetadata);
    QFETCH(const MultiHash, channelMetadata);

    // Setup Server-side handling
    auto processor = m_server->createProcessor();
    struct ServerData
    {
        grpc::ServerAsyncResponseWriter<None> op{ &ctx };
        grpc::ServerContext ctx;

        Event request;
        None response;
    };
    ServerData *data = new ServerData;

    CallbackTag *callHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);

            const std::multimap<grpc::string_ref, grpc::string_ref>
                &receivedMd = data->ctx.client_metadata();
            auto mergedMd = channelMetadata;
            mergedMd.unite(callMetadata);

            for (auto it = mergedMd.cbegin(); it != mergedMd.cend(); ++it) {
                // Check that each key-value pair sent by the client exists on the server
                auto serverRange = receivedMd.equal_range(it.key().toStdString());
                auto clientRange = mergedMd.equal_range(it.key());

                QCOMPARE_EQ(std::distance(serverRange.first, serverRange.second),
                            std::distance(clientRange.first, clientRange.second));
                while (clientRange.first != clientRange.second) {
                    // Look for the exact entry in the server range. The order may
                    // be changed but it must be present.
                    const auto it = std::find_if(serverRange.first, serverRange.second,
                                                 [&](auto it) {
                                                     return it.first
                                                         == clientRange.first.key().toStdString()
                                                         && it.second
                                                         == clientRange.first.value().toStdString();
                                                 });
                    QVERIFY(it != serverRange.second);
                    std::advance(clientRange.first, 1);
                }
            }
            data->op.Finish(data->response, grpc::Status::OK,
                            new DeleteTag<ServerData>(data, processor.get()));
            return CallbackTag::Delete;
        },
        processor.get());
    m_service->RequestPush(&data->ctx, &data->request, &data->op, m_server->cq(), m_server->cq(),
                           callHandler);

    // Setup Client-side call
    m_client->channel()->setChannelOptions(QGrpcChannelOptions().setMetadata(channelMetadata));
    auto call = m_client->Push(qt::Event{}, QGrpcCallOptions().setMetadata(callMetadata));
    QVERIFY(call);

    connect(call.get(), &QGrpcOperation::finished, this, [&](const QGrpcStatus &status) {
        QVERIFY(status.isOk());
        auto response = call->read<qt::None>();
        QVERIFY(response.has_value());
    });

    QSignalSpy finishedSpy(call.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(finishedSpy.wait());
}

void QtGrpcClientEnd2EndTest::serverMetadataReceived_data() const
{
    QTest::addColumn<bool>("filterServerMetadata");
    QTest::addColumn<MultiHash>("expectedInitialMd");
    QTest::addColumn<MultiHash>("expectedTrailingMd");

    MultiHash initialMd{
        { "initial-1", "ivalue-1" },
        { "initial-2", "ivalue-2" }
    };
    MultiHash trailingMd{
        { "trailing-1",     "tvalue-1" },
        { "trailing-multi", "tvalue-x" },
        { "trailing-multi", "tvalue-y" }
    };

    QTest::addRow("filter(true)") << true << initialMd << trailingMd;
    QTest::addRow("filter(false)") << false << initialMd << trailingMd;
}

void QtGrpcClientEnd2EndTest::serverMetadataReceived()
{
    using MultiHash = QMultiHash<QByteArray, QByteArray>;
    QFETCH(const bool, filterServerMetadata);
    QFETCH(const MultiHash, expectedInitialMd);
    QFETCH(const MultiHash, expectedTrailingMd);

    // Setup Server-side handling
    auto processor = m_server->createProcessor();
    struct ServerData
    {
        grpc::ServerAsyncResponseWriter<None> op{ &ctx };
        grpc::ServerContext ctx;

        Event request;
        None response;
    };
    ServerData *data = new ServerData;

    for (auto it = expectedInitialMd.cbegin(); it != expectedInitialMd.cend(); ++it)
        data->ctx.AddInitialMetadata(it.key().toStdString(), it.value().toStdString());
    for (auto it = expectedTrailingMd.cbegin(); it != expectedTrailingMd.cend(); ++it)
        data->ctx.AddTrailingMetadata(it.key().toStdString(), it.value().toStdString());

    CallbackTag *callHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            data->op.Finish(data->response, grpc::Status::OK,
                            new DeleteTag<ServerData>(data, processor.get()));
            return CallbackTag::Delete;
        },
        processor.get());
    m_service->RequestPush(&data->ctx, &data->request, &data->op, m_server->cq(), m_server->cq(),
                           callHandler);

    // Setup Client-side call
    auto chOpts = QGrpcChannelOptions().setFilterServerMetadata(filterServerMetadata);
    m_client->channel()->setChannelOptions(chOpts);

    auto call = m_client->Push(qt::Event{});
    QVERIFY(call);

    connect(call.get(), &QGrpcOperation::finished, this, [&](const QGrpcStatus &status) {
        QVERIFY(status.isOk());
        auto response = call->read<qt::None>();
        QVERIFY(response.has_value());

        const auto &initialMd = call->serverInitialMetadata();
        const auto &trailingMd = call->serverTrailingMetadata();

        if (filterServerMetadata) {
            QCOMPARE(initialMd, expectedInitialMd);
            QCOMPARE(trailingMd, expectedTrailingMd);
        } else {
            QCOMPARE_GE(initialMd.size(), expectedInitialMd.size());
            QCOMPARE_GE(trailingMd.size(), expectedTrailingMd.size());
            for (auto it = expectedInitialMd.cbegin(); it != expectedInitialMd.cend(); ++it)
                QVERIFY(initialMd.contains(it.key(), it.value()));
            for (auto it = expectedTrailingMd.cbegin(); it != expectedTrailingMd.cend(); ++it)
                QVERIFY(trailingMd.contains(it.key(), it.value()));
        }
    });

    QSignalSpy finishedSpy(call.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(finishedSpy.wait());
}

void QtGrpcClientEnd2EndTest::serverInitialMetadataEmitted()
{
    // Setup Server-side handling
    auto processor = m_server->createProcessor();
    struct ServerData
    {
        grpc::ServerAsyncResponseWriter<None> op{ &ctx };
        grpc::ServerContext ctx;

        Event request;
        None response;
    };
    ServerData *data = new ServerData;
    data->ctx.AddInitialMetadata("initial", "value");
    data->ctx.AddTrailingMetadata("trailing", "value");

    CallbackTag *callHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            data->op.SendInitialMetadata(new CallbackTag(
                [&](bool ok) {
                    QVERIFY(ok);
                    // Wait one second before emitting finished.
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    data->op.Finish(data->response, grpc::Status::OK,
                                    new DeleteTag<ServerData>(data, processor.get()));
                    return CallbackTag::Delete;
                },
                processor.get()));
            return CallbackTag::Delete;
        },
        processor.get());
    m_service->RequestPush(&data->ctx, &data->request, &data->op, m_server->cq(), m_server->cq(),
                           callHandler);

    // Setup Client-side call
    QDateTime initialMetadataTime;
    QDateTime finishedTime;
    auto call = m_client->Push(qt::Event{}, QGrpcCallOptions{}.setFilterServerMetadata(true));
    QVERIFY(call);

    connect(call.get(), &QGrpcOperation::finished, this, [&](const QGrpcStatus &status) {
        finishedTime = QDateTime::currentDateTime();
        QVERIFY(status.isOk());
        QCOMPARE_EQ(call->serverTrailingMetadata().size(), 1);
    });
    connect(call.get(), &QGrpcOperation::serverInitialMetadataReceived, this,
            [&](const QMultiHash<QByteArray, QByteArray> &metadata) {
                initialMetadataTime = QDateTime::currentDateTime();
                QCOMPARE_EQ(call->serverInitialMetadata().size(), 1);
                QCOMPARE_EQ(call->serverInitialMetadata(), metadata);
            });

    QSignalSpy finishedSpy(call.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());
    QSignalSpy initialMetadataSpy(call.get(), &QGrpcOperation::serverInitialMetadataReceived);
    QVERIFY(initialMetadataSpy.isValid());

    finishedSpy.wait();

    QCOMPARE_EQ(initialMetadataSpy.count(), 1);
    QCOMPARE_LT(initialMetadataTime, finishedTime);
}

void QtGrpcClientEnd2EndTest::bidiStreamsInOrder()
{
    constexpr auto SleepTime = std::chrono::milliseconds(5);

    // Setup Server-side handling
    auto processor = m_server->createProcessor();
    struct ServerData
    {
        grpc::ServerAsyncReaderWriter<Event, Event> op{ &ctx };
        grpc::ServerContext ctx;

        Event request;
        Event response;
        unsigned long count = 0;
        std::atomic<bool> readerDone = false;
        std::atomic<bool> writerDone = false;

        void updateResponse()
        {
            response.set_type(Event::SERVER);
            response.set_number(response.number() + 1);
            response.set_name("server-" + std::to_string(response.number()));
        }
    };
    ServerData *data = new ServerData;

    CallbackTag *reader = new CallbackTag(
        [&, current = 1u](bool ok) mutable {
            if (!ok) {
                data->readerDone = true;
                if (data->writerDone)
                    data->op.Finish(grpc::Status::OK,
                                    new DeleteTag<ServerData>(data, processor.get()));
                return CallbackTag::Delete;
            }
            QCOMPARE_EQ(data->request.type(), Event::CLIENT);
            QCOMPARE_EQ(data->request.number(), current);
            std::string name = "client-" + std::to_string(current);
            QCOMPARE_EQ(data->request.name(), name);
            ++current;

            data->op.Read(&data->request, reader);
            return CallbackTag::Proceed;
        },
        processor.get());
    CallbackTag *writer = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            if (data->response.number() >= data->count) {
                data->writerDone = true;
                if (data->readerDone)
                    data->op.Finish(grpc::Status::OK,
                                    new DeleteTag<ServerData>(data, processor.get()));
                return CallbackTag::Delete;
            }
            std::this_thread::sleep_for(SleepTime);
            data->updateResponse();
            data->op.Write(data->response, writer);
            return CallbackTag::Proceed;
        },
        processor.get());
    CallbackTag *callHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            const auto &md = data->ctx.client_metadata();
            const auto countIt = md.find("call-count");
            QVERIFY(countIt != md.cend());
            data->count = std::stoul(std::string(countIt->second.data(), countIt->second.length()));
            QCOMPARE_GT(data->count, 0u);

            data->op.Read(&data->request, reader);
            data->updateResponse();
            data->op.Write(data->response, writer);

            return CallbackTag::Delete;
        },
        processor.get());
    m_service->RequestExchange(&data->ctx, &data->op, m_server->cq(), m_server->cq(), callHandler);

    // Client bidi stream
    uint callCount = 25;
    qt::Event request;

    auto updateRequest = [&] {
        request.setType(qt::Event::Type::CLIENT);
        request.setNumber(request.number() + 1);
        request.setName("client-"_L1 + QString::number(request.number()));
    };

    updateRequest();
    auto copts = QGrpcCallOptions().addMetadata("call-count", QByteArray::number(callCount));
    auto stream = m_client->Exchange(request, copts);
    QVERIFY(stream);

    connect(stream.get(), &QGrpcOperation::finished, this,
            [](const QGrpcStatus &status) { QVERIFY(status.isOk()); });
    connect(stream.get(), &QGrpcBidiStream::messageReceived, this, [&, current = 1u]() mutable {
        const auto response = stream->read<qt::Event>();
        QVERIFY(response.has_value());
        QCOMPARE_EQ(response->type(), qt::Event::Type::SERVER);
        QCOMPARE_EQ(response->number(), current);
        QString name = "server-"_L1 + QString::number(current);
        QCOMPARE_EQ(response->name(), name);
        ++current;
    });

    QTimer delayedWriter;
    connect(&delayedWriter, &QTimer::timeout, this, [&, current = 1u]() mutable {
        if (current >= callCount) {
            stream->writesDone();
            delayedWriter.stop();
        }
        updateRequest();
        stream->writeMessage(request);
        ++current;
    });
    delayedWriter.start(SleepTime);

    QSignalSpy finishedSpy(stream.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(finishedSpy.wait());
}

void QtGrpcClientEnd2EndTest::hostUriRoundTrip()
{
    auto *initialChannel = static_cast<QGrpcHttp2Channel *>(m_client->channel().get());
    QVERIFY(initialChannel);

    const QUrl sanitizedUri = initialChannel->hostUri();
    auto rebuiltChannel = std::make_shared<QGrpcHttp2Channel>(sanitizedUri,
                                                              initialChannel->channelOptions());
    // sanitizeHostUri must be idempotent: feeding hostUri() back in yields the
    // same URI, which is what keeps unix/unix-abstract channels reconstructible.
    QCOMPARE(rebuiltChannel->hostUri(), sanitizedUri);
    QVERIFY(m_client->attachChannel(rebuiltChannel));

    auto processor = m_server->createProcessor();
    struct ServerData
    {
        grpc::ServerAsyncResponseWriter<None> op{ &ctx };
        grpc::ServerContext ctx;
        Event request;
        None response;
    };
    ServerData *data = new ServerData;
    CallbackTag *callHandler = new CallbackTag(
        [&, data](bool ok) {
            QVERIFY(ok);
            data->op.Finish(data->response, grpc::Status::OK,
                            new DeleteTag<ServerData>(data, processor.get()));
            return CallbackTag::Delete;
        },
        processor.get());
    m_service->RequestPush(&data->ctx, &data->request, &data->op, m_server->cq(), m_server->cq(),
                           callHandler);

    auto call = m_client->Push(qt::Event{});
    QVERIFY(call);
    QSignalSpy finishedSpy(call.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(finishedSpy.wait());
    QCOMPARE_EQ(finishedSpy.count(), 1);
    QVERIFY(finishedSpy.takeFirst().at(0).value<QGrpcStatus>().isOk());
}

void QtGrpcClientEnd2EndTest::clientHandlesCompression_data() const
{
    QTest::addColumn<grpc_compression_algorithm>("compressionAlgo");
    QTest::addRow("compress(None)") << GRPC_COMPRESS_NONE;
    QTest::addRow("compress(Deflate)") << GRPC_COMPRESS_DEFLATE;
    QTest::addRow("compress(Gzip)") << GRPC_COMPRESS_GZIP;
}

void QtGrpcClientEnd2EndTest::clientHandlesCompression()
{
    QFETCH(const grpc_compression_algorithm, compressionAlgo);
    EventList serverResponses;

    class SubscribeListHandler : public AbstractRpcTag
    {
    public:
        SubscribeListHandler(EventList &responses_, EventHub::AsyncService &service_,
                             const grpc_compression_algorithm compressionAlgo_,
                             TagProcessor *processor)
            : AbstractRpcTag(processor), op(&context()), service(service_), responses(responses_),
              compressionAlgo(compressionAlgo_)
        {
            context().set_compression_algorithm(compressionAlgo);
            context().set_compression_level(GRPC_COMPRESS_LEVEL_HIGH);
            // create some 'compressable' data. Try to make it more complex
            // as compression is not guaranteed to actually be applied.
            for (size_t i = 0; i < 100; ++i) {
                const auto v = i % 10;
                Event ev;
                ev.set_name("server;server;" + std::to_string(v));
                ev.set_number(v);
                responses.mutable_events()->Add(std::move(ev));
            }
        }
        void start(grpc::ServerCompletionQueue *cq) override
        {
            service.RequestSubscribeList(&context(), &request, &op, cq, cq, this);
        }
        void process(bool ok) override
        {
            QVERIFY(ok);
            if (index >= responseCount) {
                op.Finish(grpc::Status::OK, new DeleteTag<SubscribeListHandler>(this, processor));
                return;
            }

            grpc::WriteOptions wopts;
            // Enable and disable the compression per-message
            if (index % 2 == 0)
                wopts.set_no_compression();
            op.Write(responses, wopts, this);
            ++index;
        }

        grpc::ServerAsyncWriter<EventList> op;
        EventHub::AsyncService &service;

        None request;
        EventList &responses;

        size_t index = 0;
        const grpc_compression_algorithm compressionAlgo;
        const size_t responseCount = 20;
    };

    auto processor = m_server->createProcessor();
    SubscribeListHandler *handler = new SubscribeListHandler(serverResponses, *m_service,
                                                             compressionAlgo, processor.get());
    m_server->startRpcTag(handler);

    auto call = m_client->SubscribeList(qt::None{});
    QVERIFY(call);

    connect(call.get(), &QGrpcOperation::finished, this,
            [&](const QGrpcStatus &status) { QCOMPARE(status.code(), QtGrpc::StatusCode::Ok); });
    connect(call.get(), &QGrpcServerStream::messageReceived, this, [&] {
        auto response = call->read<qt::EventList>();
        QVERIFY(response);
        QCOMPARE_EQ(response->events().size(), serverResponses.events().size());
        for (int i = 0; i < response->events().size(); ++i) {
            const auto &next = response->events().at(i);
            const auto &baseline = serverResponses.events().at(i);
            QCOMPARE_EQ(next.name(), QString::fromStdString(baseline.name()));
            QCOMPARE_EQ(next.number(), baseline.number());
        }
    });

    QSignalSpy finishedSpy(call.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(finishedSpy.wait());
}

void QtGrpcClientEnd2EndTest::channelChangeCancelsInFlightRPCs_data() const
{
    QTest::addColumn<bool>("waitForServer");
    // Active: cancel hits an RPC the server has already accepted.
    QTest::addRow("Active") << true;
    // Idle: cancel races the first write; client still observes Cancelled.
    QTest::addRow("Idle") << false;
}

void QtGrpcClientEnd2EndTest::channelChangeCancelsInFlightRPCs()
{
    QFETCH(const bool, waitForServer);

    // Cancelled RPC leaves a half-drained server; rebuild for the next row.
    const auto restoreServer = qScopeGuard([this] { restartServer(); });

    auto processor = m_server->createProcessor();
    struct ServerData
    {
        grpc::ServerAsyncResponseWriter<None> op{ &ctx };
        grpc::ServerContext ctx;

        Event request;
        None response;
        std::atomic<bool> requestReceived = false;
        std::atomic<bool> notifyWhenDone = false;
    };
    auto data = std::make_unique<ServerData>();

    if (waitForServer) {
        data->ctx.AsyncNotifyWhenDone(new CallbackTag(
            [&](bool ok) {
                QVERIFY(ok);
                data->notifyWhenDone = true;
                return CallbackTag::Delete;
            },
            processor.get()));
        CallbackTag *callHandler = new CallbackTag(
            [&](bool ok) {
                QVERIFY(ok);
                data->requestReceived = true;
                return CallbackTag::Delete;
            },
            processor.get());
        m_service->RequestPush(&data->ctx, &data->request, &data->op, m_server->cq(),
                               m_server->cq(), callHandler);
    }

    auto call = m_client->Push(qt::Event{});
    QVERIFY(call);

    connect(call.get(), &QGrpcOperation::finished, this, [&](const QGrpcStatus &status) {
        QCOMPARE_EQ(status.code(), QtGrpc::StatusCode::Cancelled);
    });

    QSignalSpy finishedSpy(call.get(), &QGrpcOperation::finished);
    QSignalSpy channelChangedSpy(m_client.get(), &QGrpcClientBase::channelChanged);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(channelChangedSpy.isValid());

    if (waitForServer) {
        // Ensure the server has committed to the RPC before we cancel.
        QTRY_VERIFY(data->requestReceived.load());
    }

    auto *channelPtr = static_cast<QGrpcHttp2Channel *>(m_client->channel().get());
    QVERIFY(channelPtr);
    const auto uri = channelPtr->hostUri();
    const auto opts = channelPtr->channelOptions();
    QVERIFY(m_client->attachChannel(std::make_shared<QGrpcHttp2Channel>(uri, opts)));

    // attachChannel() cancellation might emit finished() before we reach this line.
    QTRY_COMPARE_EQ(finishedSpy.count(), 1);
    QCOMPARE_EQ(channelChangedSpy.count(), 1);
    if (waitForServer)
        QTRY_VERIFY(data->notifyWhenDone.load());
}

// Walks the HTTP/2 frames a client sent after its connection preface and
// reports whether one of them has the given frame type.
static bool containsHttp2Frame(QByteArrayView data, quint8 frameType)
{
    constexpr QByteArrayView Preface("PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n");
    if (!data.startsWith(Preface))
        return false;
    qsizetype i = Preface.size();
    while (i + 9 <= data.size()) {
        if (quint8(data[i + 3]) == frameType)
            return true;
        const auto payloadLength = (quint32(quint8(data[i])) << 16)
            | (quint32(quint8(data[i + 1])) << 8) | quint8(data[i + 2]);
        i += 9 + qsizetype(payloadLength);
    }
    return false;
}

void QtGrpcClientEnd2EndTest::channelTeardownCancelsActiveStream()
{
    constexpr quint8 HeadersFrame = 0x01;
    constexpr quint8 RstStreamFrame = 0x03;

    QFETCH_GLOBAL(const QUrl, hostUri);
    if (hostUri.scheme() != "http"_L1)
        QSKIP("Transport independent; runs on the plain HTTP row only.");

    // A minimal HTTP/2 server: acknowledges the client preface with an empty
    // SETTINGS frame so the client opens its stream, then stays silent.
    QTcpServer fakeServer;
    QVERIFY(fakeServer.listen(QHostAddress::LocalHost));
    QByteArray received;
    QObject::connect(&fakeServer, &QTcpServer::newConnection, &fakeServer, [&] {
        QTcpSocket *serverSide = fakeServer.nextPendingConnection();
        serverSide->write(QByteArray::fromHex("000000040000000000"    // SETTINGS
                                              "000000040100000000")); // SETTINGS ACK
        QObject::connect(serverSide, &QTcpSocket::readyRead, serverSide,
                         [&received, serverSide] { received.append(serverSide->readAll()); });
        received.append(serverSide->readAll());
    });
    const QUrl url("http://127.0.0.1:" + QByteArray::number(fakeServer.serverPort()));

    auto client = std::make_unique<qt::EventHub::Client>();
    QVERIFY(client->attachChannel(std::make_shared<QGrpcHttp2Channel>(url)));

    auto call = client->Push(qt::Event{});
    QVERIFY(call);
    QSignalSpy finishedSpy(call.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());
    // Once HEADERS arrive the stream is open on the wire; the server never
    // answers it, so it stays open until the client tears down.
    QTRY_VERIFY(containsHttp2Frame(received, HeadersFrame));
    QVERIFY(!containsHttp2Frame(received, RstStreamFrame));

    client.reset();
    QTRY_VERIFY2(containsHttp2Frame(received, RstStreamFrame),
                 "the channel was destroyed without cancelling its active stream");
    // The wire is cancelled; the local call must also terminate, not hang.
    QCOMPARE(finishedSpy.count(), 1);
    QCOMPARE_EQ(qvariant_cast<QGrpcStatus>(finishedSpy.at(0).first()).code(),
                StatusCode::Unavailable);
}

void QtGrpcClientEnd2EndTest::maximumReceiveMessageSize_data() const
{
    constexpr std::optional<quint64> None = std::nullopt;
    constexpr std::optional<quint64> Small = 512;
    // No "unlimited" setting exists: a value at/above the transport cap is
    // clamped down to it, so a huge value is effectively unlimited. The env var
    // is parsed as a signed 64-bit integer, so stay inside that range.
    constexpr std::optional<quint64> Unlimited = quint64((std::numeric_limits<qint64>::max)());

    constexpr std::optional<quint64> Zero = 0;
    constexpr int Payload = 2048;
    constexpr int Empty = 0;

    QTest::addColumn<std::optional<quint64>>("envLimit");
    QTest::addColumn<int>("serverPayloadBytes");
    QTest::addColumn<QRegularExpression>("expectedWarning");
    QTest::addColumn<StatusCode>("expectedCode");

    const auto noWarning = QRegularExpression();
    const auto aboveCap = QRegularExpression("exceeds the HTTP/2 transport cap of \\d+ bytes");

    QTest::newRow("env-default-accepts") << None << Payload << noWarning << StatusCode::Ok;
    QTest::newRow("env-var-rejects")
        << Small << Payload << noWarning << StatusCode::ResourceExhausted;
    QTest::newRow("env-unlimited-accepts") << Unlimited << Payload << aboveCap << StatusCode::Ok;
    // Literal-0 semantics: a payload-containing message is rejected, but an empty
    // message (e.g. google.protobuf.Empty) still passes.
    QTest::newRow("env-zero-rejects-payload")
        << Zero << Payload << noWarning << StatusCode::ResourceExhausted;
    QTest::newRow("env-zero-accepts-empty") << Zero << Empty << noWarning << StatusCode::Ok;
}

void QtGrpcClientEnd2EndTest::maximumReceiveMessageSize()
{
    QFETCH(const std::optional<quint64>, envLimit);
    QFETCH(const int, serverPayloadBytes);
    QFETCH(const QRegularExpression, expectedWarning);
    QFETCH(const QtGrpc::StatusCode, expectedCode);

    if (envLimit)
        qputenv("QT_GRPC_MAXIMUM_RECEIVE_MESSAGE_SIZE", QByteArray::number(*envLimit));
    const auto cleanup = qScopeGuard([] { qunsetenv("QT_GRPC_MAXIMUM_RECEIVE_MESSAGE_SIZE"); });

    // Re-attach a fresh channel so the env var (set above) is sampled at
    // channel construction time, not the one created in init().
    QFETCH_GLOBAL(const QUrl, hostUri);
    QFETCH_GLOBAL(const QGrpcChannelOptions, channelOptions);
    QVERIFY(m_client->attachChannel(std::make_shared<QGrpcHttp2Channel>(hostUri, channelOptions)));

    // Server sends an Event message of the requested payload size, then finishes OK.
    auto processor = m_server->createProcessor();
    struct ServerData
    {
        grpc::ServerAsyncWriter<Event> op{ &ctx };
        grpc::ServerContext ctx;

        None request;
        Event response;
    };
    ServerData *data = new ServerData;
    if (serverPayloadBytes > 0)
        data->response.set_name(std::string(serverPayloadBytes, 'x'));

    CallbackTag *writeHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            data->op.Finish(grpc::Status::OK, new DeleteTag<ServerData>(data, processor.get()));
            return CallbackTag::Delete;
        },
        processor.get());
    CallbackTag *callHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            data->op.Write(data->response, writeHandler);
            return CallbackTag::Delete;
        },
        processor.get());
    m_service->RequestSubscribe(&data->ctx, &data->request, &data->op, m_server->cq(),
                                m_server->cq(), callHandler);

    if (!expectedWarning.pattern().isEmpty())
        QTest::ignoreMessage(QtWarningMsg, expectedWarning);

    auto call = m_client->Subscribe(qt::None{});
    QVERIFY(call);

    QSignalSpy finishedSpy(call.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(finishedSpy.wait());

    const QVariant &finishedArg = finishedSpy.at(0).first();
    const auto *status = get_if<QGrpcStatus>(&finishedArg);
    QVERIFY(status);
    QCOMPARE_EQ(status->code(), expectedCode);
    if (status->code() == QtGrpc::StatusCode::ResourceExhausted)
        QVERIFY(status->message().contains("exceeds"));
}

void QtGrpcClientEnd2EndTest::maximumMetadataSize_data() const
{
    constexpr int SmallMetadata = 2048;
    constexpr int LargeMetadata = 8192;
    constexpr int OversizedMetadata = 20480;

    QTest::addColumn<QByteArray>("envLimit");
    QTest::addColumn<int>("serverPaddingBytes");
    QTest::addColumn<QRegularExpression>("expectedWarning");
    QTest::addColumn<StatusCode>("expectedCode");

    const auto noEnv = QByteArray();
    const auto noWarning = QRegularExpression();
    const auto belowMin = QRegularExpression("below the minimum of 4096 bytes");

    // No limit configured falls back to the 16 KiB default, which accepts a
    // normal header block and rejects one beyond it.
    QTest::newRow("default-accepts") << noEnv << SmallMetadata << noWarning << StatusCode::Ok;
    QTest::newRow("default-rejects-oversized")
        << noEnv << OversizedMetadata << noWarning << StatusCode::ResourceExhausted;
    // Zero and any sub-floor value are raised to the 4 KiB minimum.
    QTest::newRow("zero-floored-to-min") << "0"_ba << SmallMetadata << belowMin << StatusCode::Ok;
    QTest::newRow("subfloor-accepts-normal")
        << "512"_ba << SmallMetadata << belowMin << StatusCode::Ok;
    // The 4 KiB floor still rejects a larger header block.
    QTest::newRow("floored-rejects-large")
        << "512"_ba << LargeMetadata << belowMin << StatusCode::ResourceExhausted;
    // Raising the limit above the default accepts what the default rejects.
    QTest::newRow("raised-accepts-oversized")
        << "32768"_ba << OversizedMetadata << noWarning << StatusCode::Ok;
}

void QtGrpcClientEnd2EndTest::maximumMetadataSize()
{
    QFETCH_GLOBAL(const QUrl, hostUri);
    QFETCH_GLOBAL(const QGrpcChannelOptions, channelOptions);

    QFETCH(const QByteArray, envLimit);
    QFETCH(const int, serverPaddingBytes);
    QFETCH(const QRegularExpression, expectedWarning);
    QFETCH(const QtGrpc::StatusCode, expectedCode);

    const auto restoreEnv = qScopeGuard([] { qunsetenv("QT_GRPC_MAXIMUM_METADATA_SIZE"); });
    if (!envLimit.isNull())
        qputenv("QT_GRPC_MAXIMUM_METADATA_SIZE", envLimit);

    // Server attaches an initial-metadata entry of the requested size, then finishes OK.
    TagProcessor processor(m_server.get());
    struct ServerData
    {
        grpc::ServerAsyncWriter<Event> op{ &ctx };
        grpc::ServerContext ctx;

        None request;
        Event response;
    };
    ServerData *data = new ServerData;
    data->ctx.AddInitialMetadata("x-padding", std::string(serverPaddingBytes, 'a'));

    CallbackTag *writeHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            data->op.Finish(grpc::Status::OK, new DeleteTag<ServerData>(data, &processor));
            return CallbackTag::Delete;
        },
        &processor);
    CallbackTag *callHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            data->op.Write(data->response, writeHandler);
            return CallbackTag::Delete;
        },
        &processor);
    m_service->RequestSubscribe(&data->ctx, &data->request, &data->op, m_server->cq(),
                                m_server->cq(), callHandler);

    if (!expectedWarning.pattern().isEmpty())
        QTest::ignoreMessage(QtWarningMsg, expectedWarning);

    // Re-attach a fresh channel so the env var (set above) is sampled at
    // channel construction time, not the one created in init().
    qt::EventHub::Client client;
    QVERIFY(client.attachChannel(std::make_shared<QGrpcHttp2Channel>(hostUri, channelOptions)));

    // Rejecting the oversized header block is a connection error, which
    // QHttp2Connection reports with a qCCritical of its own.
    if (expectedCode != QtGrpc::StatusCode::Ok) {
        QTest::ignoreMessage(QtCriticalMsg,
                             QRegularExpression("Connection error: Header list size "
                                                "limit exceeded"));
    }

    auto call = client.Subscribe(qt::None{});
    QVERIFY(call);

    QSignalSpy finishedSpy(call.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(finishedSpy.wait());

    const QVariant &finishedArg = finishedSpy.at(0).first();
    const auto *status = get_if<QGrpcStatus>(&finishedArg);
    QVERIFY(status);
    QCOMPARE_EQ(status->code(), expectedCode);
    // An oversized header block is rejected by the advertised SETTINGS_MAX_HEADER_LIST_SIZE;
    // QHttp2Connection reports it as ENHANCE_YOUR_CALM with this error string.
    if (expectedCode != QtGrpc::StatusCode::Ok)
        QVERIFY(status->message().contains("Header list size limit exceeded"));
}

void QtGrpcClientEnd2EndTest::burstWritesDoNotOverflowStack()
{
    // Regression test: Http2Handler::processQueue() used to drain the outgoing
    // queue recursively (sendDATA -> uploadFinished -> processQueue), so a large
    // burst-written backlog overflowed the stack when it drained. Queue a big
    // backlog before the event loop runs, then require the RPC to finish.
    constexpr int TotalMessages = 20000;

    TagProcessor processor(m_server.get());
    struct ServerData
    {
        grpc::ServerAsyncReader<None, Event> op{ &ctx };
        grpc::ServerContext ctx;
        Event request;
        None response;
    };
    ServerData *data = new ServerData;

    CallbackTag *reader = new CallbackTag(
        [&](bool ok) {
            if (!ok) {
                data->op.Finish(data->response, grpc::Status::OK,
                                new DeleteTag<ServerData>(data, &processor));
                return CallbackTag::Delete;
            }
            data->op.Read(&data->request, reader);
            return CallbackTag::Proceed;
        },
        &processor);
    CallbackTag *callHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            data->op.Read(&data->request, reader);
            return CallbackTag::Delete;
        },
        &processor);
    m_service->RequestNotify(&data->ctx, &data->op, m_server->cq(), m_server->cq(), callHandler);

    qt::Event event;
    event.setNumber(0);
    auto stream = m_client->Notify(event);
    QVERIFY(stream);
    QSignalSpy finishedSpy(stream.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());

    for (int i = 1; i < TotalMessages; ++i) {
        qt::Event next;
        next.setNumber(i);
        stream->writeMessage(next);
    }
    stream->writesDone();

    QVERIFY(finishedSpy.wait(std::chrono::seconds(60)));
    QVERIFY(qvariant_cast<QGrpcStatus>(finishedSpy.at(0).first()).isOk());
}

void QtGrpcClientEnd2EndTest::receiveWindowConfig_data() const
{
    QTest::addColumn<QByteArray>("streamWindowSize");
    QTest::addColumn<QByteArray>("connectionWindowSize");
    QTest::addColumn<QRegularExpression>("expectedWarning");

    const auto emptyEnv = QByteArray();

    // The following rows exercise valid configurations. None of them should warn.
    QTest::newRow("stream-only-valid") << "8388608"_ba << emptyEnv << QRegularExpression();
    QTest::newRow("connection-only-valid") << emptyEnv << "33554432"_ba << QRegularExpression();
    QTest::newRow("stream-and-connection-valid")
        << "8388608"_ba << "67108864"_ba << QRegularExpression();

    // The connection window is floored at the protocol's initial of 65535.
    // The per-stream window allows a smaller 1024 floor.
    QTest::newRow("stream-below-minimum-floored")
        << "0"_ba << emptyEnv
        << QRegularExpression("STREAM_RECEIVE_WINDOW_SIZE.*below the minimum of 1024 bytes");
    QTest::newRow("connection-below-minimum-floored")
        << "1024"_ba << "10000"_ba
        << QRegularExpression("CONNECTION_RECEIVE_WINDOW_SIZE.*below the minimum of 65535 bytes");

    // A sub-default per-stream window (below the 64 KiB initial, above the 1 KiB
    // floor) is accepted verbatim, with no warning.
    QTest::newRow("stream-below-rfc-default-valid")
        << "4096"_ba << emptyEnv << QRegularExpression();

    // A stream window above the cap is clamped
    QTest::newRow("stream-above-cap-clamped")
        << "2147483648"_ba << emptyEnv
        << QRegularExpression("STREAM_RECEIVE_WINDOW_SIZE.*"
                              "exceeds the HTTP/2 transport cap of 2147483647 bytes");

    // A connection window above the cap is clamped the same way.
    QTest::newRow("connection-above-cap-clamped")
        << emptyEnv << "2147483648"_ba
        << QRegularExpression("CONNECTION_RECEIVE_WINDOW_SIZE.*"
                              "exceeds the HTTP/2 transport cap of 2147483647 bytes");

    // A connection window below the stream window clamps the stream window,
    // whether the stream window was set explicitly or is the 4 MiB default.
    QTest::newRow("stream-above-connection-clamped")
        << "8388608"_ba << "1048576"_ba
        << QRegularExpression("CONNECTION_RECEIVE_WINDOW_SIZE.*"
                              "smaller than the stream receive-window");
    QTest::newRow("connection-only-below-default-stream-clamped")
        << emptyEnv << "1048576"_ba
        << QRegularExpression("CONNECTION_RECEIVE_WINDOW_SIZE.*"
                              "smaller than the stream receive-window");
}

void QtGrpcClientEnd2EndTest::receiveWindowConfig()
{
    QFETCH(const QByteArray, streamWindowSize);
    QFETCH(const QByteArray, connectionWindowSize);
    QFETCH(const QRegularExpression, expectedWarning);

    if (!streamWindowSize.isEmpty())
        qputenv("QT_GRPC_HTTP2_STREAM_RECEIVE_WINDOW_SIZE", streamWindowSize);
    if (!connectionWindowSize.isEmpty())
        qputenv("QT_GRPC_HTTP2_CONNECTION_RECEIVE_WINDOW_SIZE", connectionWindowSize);
    const auto cleanup = qScopeGuard([] {
        qunsetenv("QT_GRPC_HTTP2_STREAM_RECEIVE_WINDOW_SIZE");
        qunsetenv("QT_GRPC_HTTP2_CONNECTION_RECEIVE_WINDOW_SIZE");
    });

    if (!expectedWarning.pattern().isEmpty())
        QTest::ignoreMessage(QtWarningMsg, expectedWarning);

    QFETCH_GLOBAL(const QUrl, hostUri);
    QFETCH_GLOBAL(const QGrpcChannelOptions, channelOptions);
    QVERIFY(m_client->attachChannel(std::make_shared<QGrpcHttp2Channel>(hostUri, channelOptions)));

    TagProcessor processor(m_server.get());
    struct ServerData
    {
        grpc::ServerAsyncWriter<Event> op{ &ctx };
        grpc::ServerContext ctx;

        None request;
        Event response;
    };
    ServerData *data = new ServerData;

    CallbackTag *writeHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            data->op.Finish(grpc::Status::OK, new DeleteTag<ServerData>(data, &processor));
            return CallbackTag::Delete;
        },
        &processor);
    CallbackTag *callHandler = new CallbackTag(
        [&](bool ok) {
            QVERIFY(ok);
            data->op.Write(data->response, writeHandler);
            return CallbackTag::Delete;
        },
        &processor);
    m_service->RequestSubscribe(&data->ctx, &data->request, &data->op, m_server->cq(),
                                m_server->cq(), callHandler);

    auto call = m_client->Subscribe(qt::None{});
    QVERIFY(call);

    QSignalSpy finishedSpy(call.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(finishedSpy.wait());

    const QVariant &finishedArg = finishedSpy.at(0).first();
    const auto *status = get_if<QGrpcStatus>(&finishedArg);
    QVERIFY(status);
    QCOMPARE_EQ(status->code(), QtGrpc::StatusCode::Ok);
}

void QtGrpcClientEnd2EndTest::reconnectBackoff_data() const
{
    QTest::addColumn<const QByteArray>("envInitial");
    QTest::addColumn<const QByteArray>("envMaximum");
    QTest::addColumn<const int>("failingCalls");
    QTest::addColumn<const int>("minLastCallMs");
    QTest::addColumn<const int>("maxLastCallMs"); // -1: unbounded
    QTest::addColumn<const QRegularExpression>("constructionWarning");

    const QByteArray noEnv;
    const QRegularExpression noWarning;

    // By the fourth call the delay must exceed the initial backoff's range.
    QTest::newRow("delays-grow") << "100"_ba << "2000"_ba << 4 << 150 << -1 << noWarning;
    // Uncapped growth would put the sixth call beyond 500ms; the maximum
    // keeps it near 200ms.
    QTest::newRow("delays-capped-at-maximum")
        << "100"_ba << "200"_ba << 6 << 100 << 500 << noWarning;
    // Zero disables the backoff; retries stay immediate, far below the 1s
    // default that would apply if the zero were not honored.
    QTest::newRow("zero-disables-backoff") << "0"_ba << noEnv << 3 << 0 << 800 << noWarning;
    // Even a 1ms initial backoff must keep growing into a measurable delay.
    QTest::newRow("one-ms-grows") << "1"_ba << "2000"_ba << 12 << 100 << -1 << noWarning;
    // An initial backoff above the maximum warns and is clamped down to it.
    QTest::newRow("initial-clamped-to-maximum")
        << "500"_ba << "100"_ba << 2 << 60 << 400
        << QRegularExpression("Initial reconnect backoff of 500ms exceeds the maximum of 100ms");
    // The maximum alone applies: the default 1s initial clamps down to it.
    QTest::newRow("maximum-only-applies")
        << noEnv << "0"_ba << 2 << 0 << 800
        << QRegularExpression("Initial reconnect backoff of 1000ms exceeds the maximum of 0ms");
    // Invalid values warn and are ignored.
    QTest::newRow("invalid-ignored")
        << "-5"_ba << noEnv << 1 << 0 << -1
        << QRegularExpression("QT_GRPC_INITIAL_RECONNECT_BACKOFF_MS has an invalid value "
                              "\\(-5\\); ignoring");
}

void QtGrpcClientEnd2EndTest::reconnectBackoff()
{
    using namespace std::chrono_literals;

    QFETCH_GLOBAL(const QUrl, hostUri);
    if (hostUri.scheme() != "http"_L1)
        QSKIP("Transport independent; runs on the plain HTTP row only.");

    QFETCH(const QByteArray, envInitial);
    QFETCH(const QByteArray, envMaximum);
    QFETCH(const int, failingCalls);
    QFETCH(const int, minLastCallMs);
    QFETCH(const int, maxLastCallMs);
    QFETCH(const QRegularExpression, constructionWarning);

    const auto restoreEnv = qScopeGuard([] {
        qunsetenv("QT_GRPC_INITIAL_RECONNECT_BACKOFF_MS");
        qunsetenv("QT_GRPC_MAXIMUM_RECONNECT_BACKOFF_MS");
        qunsetenv("QT_GRPC_CONNECT_TIMEOUT_MS");
    });
    if (!envInitial.isNull())
        qputenv("QT_GRPC_INITIAL_RECONNECT_BACKOFF_MS", envInitial);
    if (!envMaximum.isNull())
        qputenv("QT_GRPC_MAXIMUM_RECONNECT_BACKOFF_MS", envMaximum);
    // A pathological environment must fail the attempt, not stall the test.
    qputenv("QT_GRPC_CONNECT_TIMEOUT_MS", "5000");

    if (!constructionWarning.pattern().isEmpty())
        QTest::ignoreMessage(QtWarningMsg, constructionWarning);

    // Accepts TCP and aborts right away, so every call fails quickly after
    // its reconnect attempt regardless of how the OS reports refused ports.
    QTcpServer refusingServer;
    QObject::connect(&refusingServer, &QTcpServer::newConnection, &refusingServer, [&] {
        while (QTcpSocket *socket = refusingServer.nextPendingConnection()) {
            socket->abort();
            socket->deleteLater();
        }
    });
    QVERIFY(refusingServer.listen(QHostAddress::LocalHost));
    const QUrl url("http://127.0.0.1:" + QByteArray::number(refusingServer.serverPort()));

    qt::EventHub::Client client;
    QVERIFY(client.attachChannel(std::make_shared<QGrpcHttp2Channel>(url)));

    qint64 lastCallMs = 0;
    QElapsedTimer callTimer;
    for (int i = 0; i < failingCalls; ++i) {
        callTimer.start();
        auto call = client.Push(qt::Event{});
        QVERIFY(call);
        QSignalSpy spy(call.get(), &QGrpcOperation::finished);
        QVERIFY(spy.isValid());
        QVERIFY(spy.wait(15s));
        lastCallMs = callTimer.elapsed();
        QCOMPARE_EQ(qvariant_cast<QGrpcStatus>(spy.at(0).first()).code(), StatusCode::Unavailable);
    }

    QVERIFY2(lastCallMs >= minLastCallMs,
             qPrintable(u"last call took %1ms, expected at least %2ms"_s.arg(lastCallMs)
                            .arg(minLastCallMs)));
    if (maxLastCallMs >= 0) {
        QVERIFY2(lastCallMs <= maxLastCallMs,
                 qPrintable(u"last call took %1ms, expected at most %2ms"_s.arg(lastCallMs)
                                .arg(maxLastCallMs)));
    }
}

void QtGrpcClientEnd2EndTest::reconnectRecoversAfterServerRestart()
{
    using namespace std::chrono_literals;

    QFETCH_GLOBAL(const QUrl, hostUri);
    QFETCH_GLOBAL(const QGrpcChannelOptions, channelOptions);

    const auto restoreEnv = qScopeGuard([] {
        qunsetenv("QT_GRPC_INITIAL_RECONNECT_BACKOFF_MS");
        qunsetenv("QT_GRPC_MAXIMUM_RECONNECT_BACKOFF_MS");
        qunsetenv("QT_GRPC_CONNECT_TIMEOUT_MS");
    });
    qputenv("QT_GRPC_INITIAL_RECONNECT_BACKOFF_MS", "200");
    qputenv("QT_GRPC_MAXIMUM_RECONNECT_BACKOFF_MS", "2000");
#if defined Q_OS_WINDOWS
    // Windows CI does not report refused loopback connects; without this the
    // first attempt only fails at the 20s default connect timeout.
    qputenv("QT_GRPC_CONNECT_TIMEOUT_MS", "1000");
    QTest::ignoreMessage(QtWarningMsg,
                         QRegularExpression("Connection attempt timed out after 1000ms"));
#endif

    const auto restoreServer = qScopeGuard([this] { restartServer(); });
    QVERIFY(m_server->stop());

    // The server is down when the channel is created; after the first failure
    // the server is brought back up so the next reconnect attempt succeeds.
    qt::EventHub::Client client;
    QVERIFY(client.attachChannel(std::make_shared<QGrpcHttp2Channel>(hostUri, channelOptions)));

    auto call1 = client.Push(qt::Event{});
    QVERIFY(call1);
    QSignalSpy spy1(call1.get(), &QGrpcOperation::finished);
    QVERIFY(spy1.isValid());
    QVERIFY(spy1.wait());
    QCOMPARE_EQ(qvariant_cast<QGrpcStatus>(spy1.at(0).first()).code(), StatusCode::Unavailable);

    // Restart the server and register a handler so the channel's next reconnect succeeds.
    restartServer();
    TagProcessor processor(m_server.get());
    struct ServerData
    {
        grpc::ServerAsyncResponseWriter<None> op{ &ctx };
        grpc::ServerContext ctx;
        Event request;
        None response;
    };
    ServerData *data = new ServerData;
    CallbackTag *callHandler = new CallbackTag(
        [data, &processor](bool ok) {
            QVERIFY(ok);
            data->op.Finish(data->response, grpc::Status::OK,
                            new DeleteTag<ServerData>(data, &processor));
            return CallbackTag::Delete;
        },
        &processor);
    m_service->RequestPush(&data->ctx, &data->request, &data->op, m_server->cq(), m_server->cq(),
                           callHandler);

    auto call2 = client.Push(qt::Event{});
    QVERIFY(call2);
    QSignalSpy spy2(call2.get(), &QGrpcOperation::finished);
    QVERIFY(spy2.isValid());
    QVERIFY(!spy2.wait(50ms)); // backoff timer still active; must not complete immediately
    QVERIFY(spy2.wait());
    QCOMPARE_EQ(qvariant_cast<QGrpcStatus>(spy2.at(0).first()).code(), StatusCode::Ok);
}

void QtGrpcClientEnd2EndTest::connectTimeout_data() const
{
    QTest::addColumn<const QByteArray>("envTimeout");
    QTest::addColumn<const bool>("expectTimeout");

    // The timing-out row uses an effective timeout of 300ms; the warning
    // asserted below pins that value.
    QTest::newRow("env-applies") << "300"_ba << true;
    QTest::newRow("zero-disables-timeout") << "0"_ba << false;
}

void QtGrpcClientEnd2EndTest::connectTimeout()
{
    using namespace std::chrono_literals;

    QFETCH_GLOBAL(const QUrl, hostUri);
    if (hostUri.scheme() != "http"_L1)
        QSKIP("Transport independent; runs on the plain HTTP row only.");

    QFETCH(const QByteArray, envTimeout);
    QFETCH(const bool, expectTimeout);

    const auto restoreEnv = qScopeGuard([] { qunsetenv("QT_GRPC_CONNECT_TIMEOUT_MS"); });
    qputenv("QT_GRPC_CONNECT_TIMEOUT_MS", envTimeout);

    // Accepts TCP but never sends HTTP/2 data, keeping the connect attempt pending.
    QTcpServer silentServer;
    QVERIFY(silentServer.listen(QHostAddress::LocalHost));
    const QUrl url("http://127.0.0.1:" + QByteArray::number(silentServer.serverPort()));

    if (expectTimeout) {
        QTest::ignoreMessage(QtWarningMsg,
                             QRegularExpression("Connection attempt timed out after 300ms"));
    } else {
        QTest::failOnWarning(QRegularExpression("Connection attempt timed out"));
    }

    qt::EventHub::Client client;
    QVERIFY(client.attachChannel(std::make_shared<QGrpcHttp2Channel>(url)));

    QElapsedTimer callTimer;
    callTimer.start();
    auto call = client.Push(qt::Event{});
    QVERIFY(call);
    QSignalSpy spy(call.get(), &QGrpcOperation::finished);
    QVERIFY(spy.isValid());

    if (!expectTimeout) {
        QVERIFY(!spy.wait(700ms));
        return;
    }

    QVERIFY(spy.wait(15s));
    QCOMPARE_GE(callTimer.elapsed(), 250);
    const auto status = qvariant_cast<QGrpcStatus>(spy.at(0).first());
    QCOMPARE_EQ(status.code(), StatusCode::Unavailable);
    QVERIFY2(status.message().contains("ConnectTimeout"), qPrintable(status.message()));
}

void QtGrpcClientEnd2EndTest::channelTeardownAfterConnectTimeoutAbort()
{
    QFETCH_GLOBAL(const QUrl, hostUri);
    if (hostUri.scheme() != "http"_L1)
        QSKIP("Transport independent; runs on the plain HTTP row only.");

    // Accepts TCP but never sends HTTP/2 data; the connect attempt only ends
    // when the channel's own connect timeout aborts it.
    QTcpServer silentServer;
    QVERIFY(silentServer.listen(QHostAddress::LocalHost));
    const QUrl url("http://127.0.0.1:" + QByteArray::number(silentServer.serverPort()));

    const auto restoreEnv = qScopeGuard([] { qunsetenv("QT_GRPC_CONNECT_TIMEOUT_MS"); });
    qputenv("QT_GRPC_CONNECT_TIMEOUT_MS", "200");

    QTest::ignoreMessage(QtWarningMsg,
                         QRegularExpression("Connection attempt timed out after 200ms"));

    auto client = std::make_unique<qt::EventHub::Client>();
    QVERIFY(client->attachChannel(std::make_shared<QGrpcHttp2Channel>(url)));

    auto call = client->Push(qt::Event{});
    QVERIFY(call);
    QSignalSpy spy(call.get(), &QGrpcOperation::finished);
    QVERIFY(spy.isValid());
    QVERIFY(spy.wait());
    QCOMPARE_EQ(qvariant_cast<QGrpcStatus>(spy.at(0).first()).code(), StatusCode::Unavailable);

    // Tear down while a second call is mid-reconnect right after the abort,
    // mirroring a test that bails out of a failed check on this path.
    auto call2 = client->Push(qt::Event{});
    QVERIFY(call2);
    QSignalSpy spy2(call2.get(), &QGrpcOperation::finished);
    QVERIFY(spy2.isValid());
    client.reset();
    QCOMPARE_EQ(spy2.count(), 1);
    QCOMPARE_EQ(qvariant_cast<QGrpcStatus>(spy2.at(0).first()).code(), StatusCode::Unavailable);
}

QTEST_MAIN(QtGrpcClientEnd2EndTest)

#include "tst_grpc_client_end2end.moc"
