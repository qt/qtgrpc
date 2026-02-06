// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <mockserver.h>

#include <proto/server/interceptor1.grpc.pb.h>
#include <proto/server/interceptor1.pb.h>
#include <proto/server/interceptor2.grpc.pb.h>
#include <proto/server/interceptor2.pb.h>

#include <grpcpp/completion_queue.h>
#include <grpcpp/server_context.h>

#include <proto/client/interceptor1_client.grpc.qpb.h>
#include <proto/client/interceptor2_client.grpc.qpb.h>

#include <QtGrpc/qgrpchttp2channel.h>
#include <QtGrpc/qgrpcinterceptor.h>

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <QtTest/qsignalspy.h>
#include <QtTest/qtest.h>

#include <QtCore/qtimer.h>

#undef QTEST_FAIL_ACTION
#define QTEST_FAIL_ACTION                         \
    do {                                          \
        std::cerr << "Test failed!" << std::endl; \
        std::abort();                             \
    } while (0)

using namespace Qt::Literals::StringLiterals;
using Capability = QtGrpcPrivate::InterceptorCapability;

namespace {

struct InterceptorCall
{
    QByteArray name;
    Capability capability;

    bool operator==(const InterceptorCall &other) const
    {
        return other.name == name && other.capability == capability;
    }

    friend QDebug operator<<(QDebug debug, const InterceptorCall &call)
    {
        const QDebugStateSaver save(debug);
        debug.nospace() << "{ name: " << call.name
                        << ", capability: " << qToUnderlying(call.capability) << " }";
        return debug;
    }
};

inline QList<InterceptorCall> CallLog = {};

class LoggingInterceptor : public QGrpcStartInterceptor,
                           public QGrpcInitialMetadataInterceptor,
                           public QGrpcMessageReceivedInterceptor,
                           public QGrpcWriteMessageInterceptor,
                           public QGrpcWritesDoneInterceptor,
                           public QGrpcTrailingMetadataInterceptor,
                           public QGrpcFinishedInterceptor,
                           public QGrpcCancelInterceptor
{
public:
    explicit LoggingInterceptor(QByteArray name) : m_name(std::move(name)) { }

    Continuation onStart(QGrpcInterceptionContext &, QProtobufMessage &,
                         QGrpcCallOptions &) override
    {
        CallLog.push_back({ m_name, Capability::Start });
        return Continuation::Proceed;
    }

    void onInitialMetadata(QGrpcInterceptionContext &,
                           QMultiHash<QByteArray, QByteArray> &) override
    {
        CallLog.push_back({ m_name, Capability::InitialMetadata });
    }

    void onMessageReceived(QGrpcInterceptionContext &, QByteArray &) override
    {
        CallLog.push_back({ m_name, Capability::MessageReceived });
    }

    void onWriteMessage(QGrpcInterceptionContext &, QProtobufMessage &) override
    {
        CallLog.push_back({ m_name, Capability::WriteMessage });
    }

    void onWritesDone(QGrpcInterceptionContext &) override
    {
        CallLog.push_back({ m_name, Capability::WritesDone });
    }

    void onTrailingMetadata(QGrpcInterceptionContext &,
                            QMultiHash<QByteArray, QByteArray> &) override
    {
        CallLog.push_back({ m_name, Capability::TrailingMetadata });
    }

    void onFinished(QGrpcInterceptionContext &, QGrpcStatus &) override
    {
        CallLog.push_back({ m_name, Capability::Finished });
    }

    void onCancel(QGrpcInterceptionContext &) override
    {
        CallLog.push_back({ m_name, Capability::Cancel });
    }

protected:
    QByteArray m_name;
};

class PartialInterceptor : public QGrpcStartInterceptor, public QGrpcFinishedInterceptor
{
public:
    explicit PartialInterceptor(QByteArray name) : m_name(std::move(name)) { }

    Continuation onStart(QGrpcInterceptionContext &, QProtobufMessage &,
                         QGrpcCallOptions &) override
    {
        CallLog.push_back({ m_name, Capability::Start });
        return Continuation::Proceed;
    }

    void onFinished(QGrpcInterceptionContext &, QGrpcStatus &) override
    {
        CallLog.push_back({ m_name, Capability::Finished });
    }

private:
    QByteArray m_name;
};

class DroppingInterceptor : public QGrpcStartInterceptor
{
public:
    using Predicate = std::function<Continuation(QtGrpc::RpcDescriptor)>;

    explicit DroppingInterceptor(QByteArray name, Predicate shouldDrop)
        : m_name(std::move(name)), m_shouldDrop(std::move(shouldDrop))
    {
    }

    Continuation onStart(QGrpcInterceptionContext &ctx,
                         QProtobufMessage &, QGrpcCallOptions &) override
    {
        CallLog.push_back({ m_name, Capability::Start });
        return m_shouldDrop(ctx.descriptor());
    }

private:
    QByteArray m_name;
    Predicate m_shouldDrop;
};

class ContextVerifyingInterceptor : public QGrpcStartInterceptor,
                                    public QGrpcInitialMetadataInterceptor,
                                    public QGrpcFinishedInterceptor
{
public:
    explicit ContextVerifyingInterceptor(QByteArray name) : m_name(std::move(name)) { }

    Continuation onStart(QGrpcInterceptionContext &context,
                         QProtobufMessage &, QGrpcCallOptions &) override
    {
        CallLog.push_back({ m_name, Capability::Start });
        capturedDescriptor1 = std::make_unique<QtGrpc::RpcDescriptor>(context.descriptor());
        return Continuation::Proceed;
    }

    void onInitialMetadata(QGrpcInterceptionContext &context,
                           QMultiHash<QByteArray, QByteArray> &) override
    {
        CallLog.push_back({ m_name, Capability::InitialMetadata });
        capturedDescriptor2 = std::make_unique<QtGrpc::RpcDescriptor>(context.descriptor());
    }

    void onFinished(QGrpcInterceptionContext &context, QGrpcStatus &) override
    {
        capturedCallOptions = context.callOptions();
        capturedDescriptor3 = std::make_unique<QtGrpc::RpcDescriptor>(context.descriptor());
        CallLog.push_back({ m_name, Capability::Finished });
    }

    std::unique_ptr<QtGrpc::RpcDescriptor> capturedDescriptor1;
    std::unique_ptr<QtGrpc::RpcDescriptor> capturedDescriptor2;
    std::unique_ptr<QtGrpc::RpcDescriptor> capturedDescriptor3;
    QGrpcCallOptions capturedCallOptions;

private:
    QByteArray m_name;
};

} // namespace

class QtGrpcClientInterceptorsTest : public QObject
{
    Q_OBJECT

public:
    static std::string serverAddress() { return "localhost:50051"; }

    static std::vector<ListeningPort> serverPorts()
    {
        return {
            { serverAddress(), grpc::InsecureServerCredentials() }
        };
    }

    static std::shared_ptr<QGrpcHttp2Channel> createChannel()
    {
        return std::make_shared<QGrpcHttp2Channel>(QUrl("http://"
                                                        + QString::fromStdString(serverAddress())));
    }

private Q_SLOTS:
    void initTestCase()
    {
        QTest::failOnWarning();
        m_service1 = std::make_unique<tst::i1::Interceptor::AsyncService>();
        m_service2 = std::make_unique<tst::i2::Interceptor::AsyncService>();
        m_server = std::make_unique<MockServer>();
        QVERIFY(m_server->start(serverPorts(), { m_service1.get(), m_service2.get() }));
    }

    void cleanupTestCase()
    {
        QVERIFY(m_server->stop());
        m_service1.reset();
        m_service2.reset();
    }

    void init() { QVERIFY(m_service1 && m_service2); }
    void cleanup() { CallLog.clear(); }

    void unaryCallOrder();
    void bidiStreamCallOrder();
    void clientStreamCallOrder();
    void failedCallOrder();
    void cancelledCallOrder();

    void partialCapabilities();
    void onStartDrop();
    void onStartDropFromSecond();
    void modifyArguments();

    void interceptionContextAccessors();

    void sharedBetweenChannels();
    void addInterceptorVariations();
    void removeInterceptor_data();
    void removeInterceptor();
    void removeAllInterceptorsMultiple();

private:
    struct UnaryHandler
    {
        grpc::ServerContext ctx;
        grpc::ServerAsyncResponseWriter<tst::i1::CallMessage> op{ &ctx };
        tst::i1::CallMessage request;
        tst::i1::CallMessage response;
    };

    struct BidiStreamHandler
    {
        grpc::ServerContext ctx;
        grpc::ServerAsyncReaderWriter<tst::i2::StreamMessage, tst::i2::StreamMessage> op{ &ctx };
        tst::i2::StreamMessage request;
    };

    struct ClientStreamHandler
    {
        grpc::ServerContext ctx;
        grpc::ServerAsyncReader<tst::i2::StreamMessage, tst::i2::StreamMessage> op{ &ctx };
        tst::i2::StreamMessage request;
    };

    void setupUnaryEcho(std::unique_ptr<TagProcessor> &processor);
    void setupUnaryError(std::unique_ptr<TagProcessor> &processor, grpc::StatusCode code);
    void setupBidiStreamEcho(std::unique_ptr<TagProcessor> &processor);
    void setupClientStreamSink(std::unique_ptr<TagProcessor> &processor,
                               std::atomic<bool> &cancelled);

    std::unique_ptr<MockServer> m_server;
    std::unique_ptr<tst::i1::Interceptor::AsyncService> m_service1;
    std::unique_ptr<tst::i2::Interceptor::AsyncService> m_service2;
    qt::tst::i1::Interceptor::Client m_client1;
    qt::tst::i2::Interceptor::Client m_client2;
};

void QtGrpcClientInterceptorsTest::setupUnaryEcho(std::unique_ptr<TagProcessor> &processor)
{
    auto *data = new UnaryHandler;

    auto *handler = new CallbackTag(
        [data, &processor](bool ok) {
            QVERIFY(ok);
            data->response = data->request;
            data->op.Finish(data->response, grpc::Status::OK,
                            new DeleteTag<UnaryHandler>(data, processor.get()));
            return CallbackTag::Delete;
        },
        processor.get());

    m_service1->RequestUnary(&data->ctx, &data->request, &data->op, m_server->cq(), m_server->cq(),
                             handler);
}

void QtGrpcClientInterceptorsTest::setupUnaryError(std::unique_ptr<TagProcessor> &processor,
                                                   grpc::StatusCode code)
{
    auto *data = new UnaryHandler;

    auto *handler = new CallbackTag(
        [data, code, &processor](bool ok) {
            QVERIFY(ok);
            data->op.FinishWithError(grpc::Status(code, "error"),
                                     new DeleteTag<UnaryHandler>(data, processor.get()));
            return CallbackTag::Delete;
        },
        processor.get());

    m_service1->RequestUnary(&data->ctx, &data->request, &data->op, m_server->cq(), m_server->cq(),
                             handler);
}

void QtGrpcClientInterceptorsTest::setupBidiStreamEcho(std::unique_ptr<TagProcessor> &processor)
{
    auto *data = new BidiStreamHandler;

    auto reader = std::make_shared<CallbackTag *>(nullptr);
    *reader = new CallbackTag(
        [data, &processor, reader](bool ok) {
            if (!ok) {
                data->op.Finish(grpc::Status::OK,
                                new DeleteTag<BidiStreamHandler>(data, processor.get()));
                return CallbackTag::Delete;
            }
            data->op.Write(data->request,
                           new CallbackTag(
                               [](bool ok) {
                                   QVERIFY(ok);
                                   return CallbackTag::Delete;
                               },
                               processor.get()));
            data->op.Read(&data->request, *reader);
            return CallbackTag::Proceed;
        },
        processor.get());

    auto *handler = new CallbackTag(
        [data, reader](bool ok) {
            QVERIFY(ok);
            data->op.Read(&data->request, *reader);
            return CallbackTag::Delete;
        },
        processor.get());

    m_service2->RequestBidiStream(&data->ctx, &data->op, m_server->cq(), m_server->cq(), handler);
}

void QtGrpcClientInterceptorsTest::setupClientStreamSink(std::unique_ptr<TagProcessor> &processor,
                                                         std::atomic<bool> &cancelled)
{
    auto *data = new ClientStreamHandler;

    auto reader = std::make_shared<CallbackTag *>(nullptr);
    *reader = new CallbackTag(
        [data, &processor, reader](bool ok) {
            if (!ok) {
                data->op.Finish(data->request, grpc::Status::OK,
                                new DeleteTag<ClientStreamHandler>(data, processor.get()));
                return CallbackTag::Delete;
            }
            data->op.Read(&data->request, *reader);
            return CallbackTag::Proceed;
        },
        processor.get());

    data->ctx.AsyncNotifyWhenDone(new CallbackTag(
        [&cancelled, data](bool ok) {
            QVERIFY(ok);
            cancelled = data->ctx.IsCancelled();
            return CallbackTag::Delete;
        },
        processor.get()));

    auto *handler = new CallbackTag(
        [data, reader](bool ok) {
            QVERIFY(ok);
            data->op.Read(&data->request, *reader);
            return CallbackTag::Delete;
        },
        processor.get());

    m_service2->RequestClientStream(&data->ctx, &data->op, m_server->cq(), m_server->cq(), handler);
}

void QtGrpcClientInterceptorsTest::unaryCallOrder()
{
    auto processor = m_server->createProcessor();
    setupUnaryEcho(processor);

    auto channel = createChannel();
    auto interceptorA = std::make_unique<LoggingInterceptor>("A");
    auto interceptorB = std::make_unique<LoggingInterceptor>("B");
    QVERIFY(channel->addInterceptor(interceptorA.get()));
    QVERIFY(channel->addInterceptor(interceptorB.get()));
    m_client1.attachChannel(channel);

    auto reply = m_client1.Unary(qt::tst::i1::CallMessage{});
    QVERIFY(reply);

    QSignalSpy finishedSpy(reply.get(), &QGrpcCallReply::finished);
    QVERIFY(finishedSpy.wait());
    QVERIFY(reply->read<qt::tst::i1::CallMessage>().has_value());

    const QList<InterceptorCall> expected = {
        { "A", Capability::Start            },
        { "B", Capability::Start            },
        { "B", Capability::InitialMetadata  },
        { "A", Capability::InitialMetadata  },
        { "B", Capability::MessageReceived  },
        { "A", Capability::MessageReceived  },
        { "B", Capability::TrailingMetadata },
        { "A", Capability::TrailingMetadata },
        { "B", Capability::Finished         },
        { "A", Capability::Finished         },
    };
    QCOMPARE(CallLog, expected);
}

void QtGrpcClientInterceptorsTest::bidiStreamCallOrder()
{
    auto processor = m_server->createProcessor();
    setupBidiStreamEcho(processor);

    auto channel = createChannel();
    auto interceptorA = std::make_unique<LoggingInterceptor>("A");
    auto interceptorB = std::make_unique<LoggingInterceptor>("B");
    QVERIFY(channel->addInterceptor(interceptorA.get()));
    QVERIFY(channel->addInterceptor(interceptorB.get()));
    m_client2.attachChannel(channel);

    qt::tst::i2::StreamMessage msg;
    msg.setTag(1);

    auto stream = m_client2.BidiStream(msg);
    QVERIFY(stream);

    connect(stream.get(), &QGrpcBidiStream::messageReceived, stream.get(), [&]() {
        auto received = stream->read<qt::tst::i2::StreamMessage>();
        QVERIFY(received.has_value());
        if (received->tag() > 0) {
            received->setTag(received->tag() - 1);
            stream->writeMessage(*received);
        } else {
            stream->writesDone();
        }
    });

    QSignalSpy finishedSpy(stream.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.wait());

    const QList<InterceptorCall> expected = {
        { "A", Capability::Start            },
        { "B", Capability::Start            },
        { "B", Capability::InitialMetadata  },
        { "A", Capability::InitialMetadata  },
        { "B", Capability::MessageReceived  }, // receive tag=1
        { "A", Capability::MessageReceived  },
        { "A", Capability::WriteMessage     }, // send tag=0
        { "B", Capability::WriteMessage     },
        { "B", Capability::MessageReceived  }, // receive tag=0
        { "A", Capability::MessageReceived  },
        { "A", Capability::WritesDone       },
        { "B", Capability::WritesDone       },
        { "B", Capability::TrailingMetadata },
        { "A", Capability::TrailingMetadata },
        { "B", Capability::Finished         },
        { "A", Capability::Finished         },
    };
    QCOMPARE(CallLog, expected);
}

void QtGrpcClientInterceptorsTest::clientStreamCallOrder()
{
    std::atomic<bool> cancelled{ false };
    auto processor = m_server->createProcessor();
    setupClientStreamSink(processor, cancelled);

    auto channel = createChannel();
    auto interceptorA = std::make_unique<LoggingInterceptor>("A");
    auto interceptorB = std::make_unique<LoggingInterceptor>("B");
    QVERIFY(channel->addInterceptor(interceptorA.get()));
    QVERIFY(channel->addInterceptor(interceptorB.get()));
    m_client2.attachChannel(channel);

    auto stream = m_client2.ClientStream(qt::tst::i2::StreamMessage{});
    QVERIFY(stream);

    qt::tst::i2::StreamMessage msg;
    msg.setTag(1);
    stream->writeMessage(msg);
    msg.setTag(2);
    stream->writeMessage(msg);
    stream->writesDone();

    QSignalSpy finishedSpy(stream.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.wait());

    const QList<InterceptorCall> expected = {
        { "A", Capability::Start            },
        { "B", Capability::Start            },
        { "A", Capability::WriteMessage     },
        { "B", Capability::WriteMessage     },
        { "A", Capability::WriteMessage     },
        { "B", Capability::WriteMessage     },
        { "A", Capability::WritesDone       },
        { "B", Capability::WritesDone       },
        { "B", Capability::InitialMetadata  },
        { "A", Capability::InitialMetadata  },
        { "B", Capability::MessageReceived  },
        { "A", Capability::MessageReceived  },
        { "B", Capability::TrailingMetadata },
        { "A", Capability::TrailingMetadata },
        { "B", Capability::Finished         },
        { "A", Capability::Finished         },
    };
    QCOMPARE(CallLog, expected);
}

void QtGrpcClientInterceptorsTest::failedCallOrder()
{
    auto processor = m_server->createProcessor();
    setupUnaryError(processor, grpc::CANCELLED);

    auto channel = createChannel();
    auto interceptorA = std::make_unique<LoggingInterceptor>("A");
    auto interceptorB = std::make_unique<LoggingInterceptor>("B");
    QVERIFY(channel->addInterceptor(interceptorA.get()));
    QVERIFY(channel->addInterceptor(interceptorB.get()));
    m_client1.attachChannel(channel);

    auto reply = m_client1.Unary(qt::tst::i1::CallMessage{});
    QVERIFY(reply);

    QSignalSpy finishedSpy(reply.get(), &QGrpcCallReply::finished);
    QVERIFY(finishedSpy.wait());

    auto args = finishedSpy.takeFirst();
    auto status = args.at(0).value<QGrpcStatus>();
    QCOMPARE(status.code(), QtGrpc::StatusCode::Cancelled);

    // Server error = trailers-only response
    const QList<InterceptorCall> expected = {
        { "A", Capability::Start            },
        { "B", Capability::Start            },
        { "B", Capability::TrailingMetadata },
        { "A", Capability::TrailingMetadata },
        { "B", Capability::Finished         },
        { "A", Capability::Finished         },
    };
    QCOMPARE(CallLog, expected);
}

void QtGrpcClientInterceptorsTest::cancelledCallOrder()
{
    std::atomic<bool> serverCancelled{ false };
    auto processor = m_server->createProcessor();
    setupClientStreamSink(processor, serverCancelled);

    auto channel = createChannel();
    auto interceptorA = std::make_unique<LoggingInterceptor>("A");
    auto interceptorB = std::make_unique<LoggingInterceptor>("B");
    QVERIFY(channel->addInterceptor(interceptorA.get()));
    QVERIFY(channel->addInterceptor(interceptorB.get()));
    m_client2.attachChannel(channel);

    auto stream = m_client2.ClientStream(qt::tst::i2::StreamMessage{});
    QVERIFY(stream);

    // Wait a bit to ensure headers are received before cancelling
    QTimer::singleShot(50, [&]() {
        qt::tst::i2::StreamMessage msg;
        msg.setTag(1);
        stream->writeMessage(msg);
        stream->cancel();
    });

    QSignalSpy finishedSpy(stream.get(), &QGrpcOperation::finished);
    QTRY_VERIFY_WITH_TIMEOUT(serverCancelled, 5000);
    QCOMPARE(finishedSpy.count(), 1);

    auto args = finishedSpy.takeFirst();
    auto status = args.at(0).value<QGrpcStatus>();
    QCOMPARE(status.code(), QtGrpc::StatusCode::Cancelled);

    const QList<InterceptorCall> expected = {
        { "A", Capability::Start        },
        { "B", Capability::Start        },
        { "A", Capability::WriteMessage },
        { "B", Capability::WriteMessage },
        { "A", Capability::Cancel       },
        { "B", Capability::Cancel       },
        { "B", Capability::Finished     },
        { "A", Capability::Finished     },
    };
    QCOMPARE(CallLog, expected);
}

void QtGrpcClientInterceptorsTest::partialCapabilities()
{
    auto processor = m_server->createProcessor();
    setupUnaryEcho(processor);

    auto channel = createChannel();
    auto partialInterceptor = std::make_unique<PartialInterceptor>("Partial");
    auto fullInterceptor = std::make_unique<LoggingInterceptor>("Full");
    QVERIFY(channel->addInterceptor(partialInterceptor.get()));
    QVERIFY(channel->addInterceptor(fullInterceptor.get()));
    m_client1.attachChannel(channel);

    auto reply = m_client1.Unary(qt::tst::i1::CallMessage{});
    QVERIFY(reply);

    QSignalSpy finishedSpy(reply.get(), &QGrpcCallReply::finished);
    QVERIFY(finishedSpy.wait());

    const QList<InterceptorCall> expected = {
        { "Partial", Capability::Start            },
        { "Full",    Capability::Start            },
        { "Full",    Capability::InitialMetadata  },
        { "Full",    Capability::MessageReceived  },
        { "Full",    Capability::TrailingMetadata },
        { "Full",    Capability::Finished         },
        { "Partial", Capability::Finished         },
    };
    QCOMPARE(CallLog, expected);
}

void QtGrpcClientInterceptorsTest::onStartDrop()
{
    auto processor = m_server->createProcessor();
    setupUnaryEcho(processor);

    auto channel = createChannel();
    auto dropper = std::make_unique<DroppingInterceptor>("Drop", [](QtGrpc::RpcDescriptor desc) {
        if (desc.service == "tst.i1.Interceptor"_L1 && desc.method == "Unary"_L1
            && desc.type == QtGrpc::RpcType::UnaryCall) {
            return QGrpcStartInterceptor::Continuation::Proceed;
        }
        return QGrpcStartInterceptor::Continuation::Drop;
    });
    auto logger = std::make_unique<LoggingInterceptor>("After");
    QVERIFY(channel->addInterceptor(dropper.get()));
    QVERIFY(channel->addInterceptor(logger.get()));
    m_client1.attachChannel(channel);
    m_client2.attachChannel(channel);

    {
        auto stream = m_client2.ClientStream(qt::tst::i2::StreamMessage{});
        QSignalSpy streamSpy(stream.get(), &QGrpcCallReply::finished);
        QVERIFY(streamSpy.isValid());
        QTRY_COMPARE(streamSpy.count(), 1);
        auto status = streamSpy.takeFirst().at(0).value<QGrpcStatus>();
        QCOMPARE_EQ(status.code(), QtGrpc::StatusCode::Aborted);
    }

    {
        auto stream = m_client2.BidiStream(qt::tst::i2::StreamMessage{});
        QSignalSpy streamSpy(stream.get(), &QGrpcCallReply::finished);
        QVERIFY(streamSpy.isValid());
        QTRY_COMPARE(streamSpy.count(), 1);
        auto status = streamSpy.takeFirst().at(0).value<QGrpcStatus>();
        QCOMPARE_EQ(status.code(), QtGrpc::StatusCode::Aborted);
    }

    {
        auto reply = m_client1.Unary(qt::tst::i1::CallMessage{});
        QSignalSpy replySpy(reply.get(), &QGrpcCallReply::finished);
        QVERIFY(replySpy.isValid());
        QVERIFY(replySpy.wait());
        auto status = replySpy.takeFirst().at(0).value<QGrpcStatus>();
        QCOMPARE_EQ(status.code(), QtGrpc::StatusCode::Ok);
    }

    const QList<InterceptorCall> expected = {
        { "Drop",  Capability::Start            }, // stream1 - fail
        { "Drop",  Capability::Start            }, // stream2 - fail

        { "Drop",  Capability::Start            }, // unary - success
        { "After", Capability::Start            },
        { "After", Capability::InitialMetadata  },
        { "After", Capability::MessageReceived  },
        { "After", Capability::TrailingMetadata },
        { "After", Capability::Finished         },
    };
    QCOMPARE(CallLog, expected);
}

void QtGrpcClientInterceptorsTest::onStartDropFromSecond()
{
    auto processor = m_server->createProcessor();
    setupUnaryEcho(processor);

    auto channel = createChannel();
    auto first = std::make_unique<LoggingInterceptor>("First");
    QGrpcStartInterceptor::Continuation continuation = QGrpcStartInterceptor::Continuation::Drop;
    auto dropper = std::make_unique<DroppingInterceptor>("Drop", [&continuation](QtGrpc::RpcDescriptor) {
                                                             return continuation;
                                                         });
    auto third = std::make_unique<LoggingInterceptor>("Proceed");

    QVERIFY(channel->addInterceptor(first.get()));
    QVERIFY(channel->addInterceptor(dropper.get()));
    QVERIFY(channel->addInterceptor(third.get()));
    m_client1.attachChannel(channel);

    const auto makeCall = [this](QtGrpc::StatusCode expectedCode) {
        auto reply = m_client1.Unary(qt::tst::i1::CallMessage{});
        QVERIFY(reply);
        QSignalSpy finishedSpy(reply.get(), &QGrpcCallReply::finished);
        QTRY_COMPARE(finishedSpy.count(), 1);
        auto status = finishedSpy.takeFirst().at(0).value<QGrpcStatus>();
        QCOMPARE(status.code(), expectedCode);
    };
    makeCall(QtGrpc::StatusCode::Aborted);
    continuation = QGrpcStartInterceptor::Continuation::Proceed;
    makeCall(QtGrpc::StatusCode::Ok);

    QByteArrayList onStartNames;
    for (const auto &call : std::as_const(CallLog)) {
        if (call.capability == Capability::Start)
            onStartNames.append(call.name);
    }
    QByteArrayList onStartExpected = {
        "First", "Drop",
        "First", "Drop", "Proceed"
    };
    QCOMPARE(onStartNames, onStartExpected);
}

void QtGrpcClientInterceptorsTest::modifyArguments()
{
    const QByteArray clientMdKey = "client-key";
    const QByteArray serverInitialMdKey = "server-initial-key";
    const QByteArray serverTrailingMdKey = "server-trailing-key";

    auto processor = m_server->createProcessor();

    struct ModifyingBidiHandler
    {
        grpc::ServerContext ctx;
        grpc::ServerAsyncReaderWriter<tst::i2::StreamMessage, tst::i2::StreamMessage> op{ &ctx };
        tst::i2::StreamMessage request;
    };

    auto *data = new ModifyingBidiHandler;
    data->ctx.AddInitialMetadata(serverInitialMdKey.toStdString(), "Base");
    data->ctx.AddTrailingMetadata(serverTrailingMdKey.toStdString(), "Base");

    auto reader = std::make_shared<CallbackTag *>(nullptr);
    *reader = new CallbackTag(
        [data, &processor, reader](bool ok) {
            if (!ok) {
                data->op.Finish(grpc::Status::OK,
                                new DeleteTag<ModifyingBidiHandler>(data, processor.get()));
                return CallbackTag::Delete;
            }
            // Echo back
            data->op.Write(data->request,
                           new CallbackTag([](bool) { return CallbackTag::Delete; },
                                           processor.get()));
            data->op.Read(&data->request, *reader);
            return CallbackTag::Proceed;
        },
        processor.get());

    auto *handler = new CallbackTag(
        [data, reader](bool ok) {
            QVERIFY(ok);
            data->op.Read(&data->request, *reader);
            return CallbackTag::Delete;
        },
        processor.get());

    m_service2->RequestBidiStream(&data->ctx, &data->op, m_server->cq(), m_server->cq(), handler);

    class ModifyingInterceptor : public QGrpcStartInterceptor,
                                 public QGrpcInitialMetadataInterceptor,
                                 public QGrpcMessageReceivedInterceptor,
                                 public QGrpcWriteMessageInterceptor,
                                 public QGrpcTrailingMetadataInterceptor,
                                 public QGrpcFinishedInterceptor
    {
    public:
        explicit ModifyingInterceptor(QByteArray name) : m_name(std::move(name)) { }

        Continuation onStart(QGrpcInterceptionContext &, QProtobufMessage &message,
                             QGrpcCallOptions &opts) override
        {
            CallLog.push_back({ m_name, Capability::Start });
            if (auto *msg = qprotobufmessage_cast<qt::tst::i2::StreamMessage *>(&message))
                msg->setData(msg->data() + ';' + m_name);
            // Modify metadata
            auto md = opts.metadata(QtGrpc::MultiValue);
            for (auto it = md.begin(); it != md.end(); ++it)
                it.value() += ';' + m_name;
            opts.setMetadata(md);
            return Continuation::Proceed;
        }

        void onInitialMetadata(QGrpcInterceptionContext &,
                               QMultiHash<QByteArray, QByteArray> &metadata) override
        {
            CallLog.push_back({ m_name, Capability::InitialMetadata });
            for (auto it = metadata.begin(); it != metadata.end(); ++it)
                it.value() += ';' + m_name;
        }

        void onMessageReceived(QGrpcInterceptionContext &ctx, QByteArray &bytes) override
        {
            CallLog.push_back({ m_name, Capability::MessageReceived });
            qt::tst::i2::StreamMessage msg;
            ctx.channel().serializer()->deserialize(&msg, bytes);
            msg.setData(msg.data() + ';' + m_name);
            bytes = ctx.channel().serializer()->serialize(&msg);
        }

        void onWriteMessage(QGrpcInterceptionContext &, QProtobufMessage &message) override
        {
            CallLog.push_back({ m_name, Capability::WriteMessage });
            if (auto *msg = qprotobufmessage_cast<qt::tst::i2::StreamMessage *>(&message))
                msg->setData(msg->data() + ';' + m_name);
        }

        void onTrailingMetadata(QGrpcInterceptionContext &,
                                QMultiHash<QByteArray, QByteArray> &metadata) override
        {
            CallLog.push_back({ m_name, Capability::TrailingMetadata });
            for (auto it = metadata.begin(); it != metadata.end(); ++it)
                it.value() += ';' + m_name;
        }

        void onFinished(QGrpcInterceptionContext &, QGrpcStatus &status) override
        {
            CallLog.push_back({ m_name, Capability::Finished });
            status = QGrpcStatus(status.code(), status.message() + ';' + m_name);
        }

    private:
        QByteArray m_name;
    };

    auto channel = createChannel();
    auto modifyingInterceptor1 = std::make_unique<ModifyingInterceptor>("i1");
    auto modifyingInterceptor2 = std::make_unique<ModifyingInterceptor>("i2");
    QVERIFY(channel->addInterceptor(modifyingInterceptor1.get()));
    QVERIFY(channel->addInterceptor(modifyingInterceptor2.get()));
    m_client2.attachChannel(channel);

    QGrpcCallOptions opts;
    opts.addMetadata(clientMdKey, "Base");

    qt::tst::i2::StreamMessage request;
    request.setData("Base");

    auto stream = m_client2.BidiStream(request, opts);
    QVERIFY(stream);

    bool receivedMessage = false;
    connect(stream.get(), &QGrpcBidiStream::messageReceived, stream.get(), [&]() {
        auto msg = stream->read<qt::tst::i2::StreamMessage>();
        QVERIFY(msg.has_value());
        QCOMPARE(msg->data(), "Base;i1;i2;i2;i1");
        receivedMessage = true;
        stream->writesDone();
    });

    QSignalSpy finishedSpy(stream.get(), &QGrpcOperation::finished);
    QVERIFY(finishedSpy.wait());
    QVERIFY(receivedMessage);

    auto initialMd = stream->serverInitialMetadata();
    QCOMPARE(initialMd.value(serverInitialMdKey), "Base;i2;i1");

    auto trailingMd = stream->serverTrailingMetadata();
    QCOMPARE(trailingMd.value(serverTrailingMdKey), "Base;i2;i1");
}

void QtGrpcClientInterceptorsTest::interceptionContextAccessors()
{
    auto processor = m_server->createProcessor();
    setupUnaryEcho(processor);

    auto interceptor = std::make_unique<ContextVerifyingInterceptor>("Ctx");
    auto channel = createChannel();
    QVERIFY(channel->addInterceptor(interceptor.get()));

    QGrpcCallOptions opts;
    opts.addMetadata("test-key", "test-value");
    opts.setDeadlineTimeout(std::chrono::seconds(30));

    m_client1.attachChannel(channel);
    auto reply = m_client1.Unary(qt::tst::i1::CallMessage{}, opts);
    QVERIFY(reply);

    QSignalSpy finishedSpy(reply.get(), &QGrpcCallReply::finished);
    QVERIFY(finishedSpy.wait());

    QCOMPARE(interceptor->capturedDescriptor1->service, "tst.i1.Interceptor"_L1);
    QCOMPARE(interceptor->capturedDescriptor1->method, "Unary"_L1);
    QCOMPARE(interceptor->capturedDescriptor1->type, QtGrpc::RpcType::UnaryCall);

    QCOMPARE_EQ(*interceptor->capturedDescriptor1, *interceptor->capturedDescriptor2);
    QCOMPARE_EQ(*interceptor->capturedDescriptor1, *interceptor->capturedDescriptor3);

    QCOMPARE_EQ(interceptor->capturedCallOptions.deadlineTimeout(), opts.deadlineTimeout());
    QCOMPARE_EQ(interceptor->capturedCallOptions.metadata(QtGrpc::MultiValue),
                opts.metadata(QtGrpc::MultiValue));
}

void QtGrpcClientInterceptorsTest::sharedBetweenChannels()
{
    auto processor = m_server->createProcessor();

    // Setup two unary handlers
    auto *data1 = new UnaryHandler;
    auto *data2 = new UnaryHandler;

    auto setupHandler = [&processor, this](UnaryHandler *d) {
        auto *h = new CallbackTag(
            [d, &processor](bool ok) {
                QVERIFY(ok);
                d->op.Finish(d->response, grpc::Status::OK,
                             new DeleteTag<UnaryHandler>(d, processor.get()));
                return CallbackTag::Delete;
            },
            processor.get());
        m_service1->RequestUnary(&d->ctx, &d->request, &d->op, m_server->cq(), m_server->cq(), h);
    };
    setupHandler(data1);
    setupHandler(data2);

    // Same interceptors shared between two channels
    auto shared = std::make_unique<LoggingInterceptor>("Shared");

    auto channel1 = createChannel();
    QVERIFY(channel1->addInterceptor(shared.get()));
    qt::tst::i1::Interceptor::Client client1;
    client1.attachChannel(channel1);

    auto channel2 = createChannel();
    QVERIFY(channel2->addInterceptor(shared.get()));
    qt::tst::i1::Interceptor::Client client2;
    client2.attachChannel(channel2);

    auto reply1 = client1.Unary(qt::tst::i1::CallMessage{});
    auto reply2 = client2.Unary(qt::tst::i1::CallMessage{});
    QVERIFY(reply1);
    QVERIFY(reply2);

    QSignalSpy spy1(reply1.get(), &QGrpcCallReply::finished);
    QSignalSpy spy2(reply2.get(), &QGrpcCallReply::finished);

    QVERIFY(spy1.wait());
    QTRY_COMPARE(spy2.count(), 1);

    // 5 stages per call * 2 calls = 10 entries
    QCOMPARE(CallLog.size(), 10);
}

void QtGrpcClientInterceptorsTest::addInterceptorVariations()
{
    auto processor = m_server->createProcessor();
    setupUnaryEcho(processor);

    auto channel = createChannel();

    auto singleInterceptor = std::make_unique<LoggingInterceptor>("Single");
    auto loggingInterceptor1 = std::make_unique<LoggingInterceptor>("V1");
    auto loggingInterceptor2 = std::make_unique<LoggingInterceptor>("V2");
    auto loggingInterceptor3 = std::make_unique<LoggingInterceptor>("V3");

    QVERIFY(channel->addInterceptor(singleInterceptor.get()));
    QVERIFY(channel->addInterceptor(loggingInterceptor1.get()));
    QVERIFY(channel->addInterceptor(loggingInterceptor2.get()));
    QVERIFY(channel->addInterceptor(loggingInterceptor3.get()));

    m_client1.attachChannel(channel);

    auto reply = m_client1.Unary(qt::tst::i1::CallMessage{});
    QVERIFY(reply);

    QSignalSpy finishedSpy(reply.get(), &QGrpcCallReply::finished);
    QVERIFY(finishedSpy.wait());

    QByteArrayList startOrder;
    for (const auto &call : std::as_const(CallLog)) {
        if (call.capability == Capability::Start)
            startOrder.append(call.name);
    }
    QCOMPARE(startOrder, QByteArrayList({ "Single", "V1", "V2", "V3" }));
}

void QtGrpcClientInterceptorsTest::removeInterceptor_data()
{
    QTest::addColumn<QByteArrayList>("initial");
    QTest::addColumn<QByteArrayList>("toRemove");
    QTest::addColumn<int>("expectedSuccessCount");
    QTest::addColumn<QByteArrayList>("expectedRemaining");

    QTest::addRow("removeFirst")
        << QByteArrayList{ "A", "B", "C" }
        << QByteArrayList{ "A" }
        << 1
        << QByteArrayList{ "B", "C" };

    QTest::addRow("removeMiddle")
        << QByteArrayList{ "A", "B", "C" }
        << QByteArrayList{ "B" }
        << 1
        << QByteArrayList{ "A", "C" };

    QTest::addRow("removeLast")
        << QByteArrayList{ "A", "B", "C" }
        << QByteArrayList{ "C" }
        << 1
        << QByteArrayList{ "A", "B" };

    QTest::addRow("removeFirstAndLast")
        << QByteArrayList{ "A", "B", "C", "D", "E" }
        << QByteArrayList{ "A", "E" }
        << 2
        << QByteArrayList{ "B", "C", "D" };

    QTest::addRow("removeNonExistent")
        << QByteArrayList{ "A", "B" }
        << QByteArrayList{ "X" }
        << 0
        << QByteArrayList{ "A", "B" };

    QTest::addRow("removeSameTwice")
        << QByteArrayList{ "A", "B", "C" }
        << QByteArrayList{ "B", "B" }
        << 1
        << QByteArrayList{ "A", "C" };

    QTest::addRow("removeFromEmpty")
        << QByteArrayList{}
        << QByteArrayList{ "A" }
        << 0
        << QByteArrayList{};
}

void QtGrpcClientInterceptorsTest::removeInterceptor()
{
    QFETCH(const QByteArrayList, initial);
    QFETCH(const QByteArrayList, toRemove);
    QFETCH(const int, expectedSuccessCount);
    QFETCH(const QByteArrayList, expectedRemaining);

    auto processor = m_server->createProcessor();
    setupUnaryEcho(processor);

    QHash<QByteArray, std::shared_ptr<LoggingInterceptor>> interceptors;
    for (const auto &name : initial + toRemove) {
        if (!interceptors.contains(name))
            interceptors[name] = std::make_shared<LoggingInterceptor>(name);
    }

    auto channel = createChannel();

    for (const auto &name : initial)
        QVERIFY(channel->addInterceptor(interceptors[name].get()));

    int successCount = 0;
    for (const auto &name : toRemove) {
        if (channel->removeInterceptor(interceptors[name].get()))
            ++successCount;
    }

    QCOMPARE(successCount, expectedSuccessCount);

    LoggingInterceptor *nullInterceptor = nullptr;
    QVERIFY(!channel->removeInterceptor(nullInterceptor));

    m_client1.attachChannel(channel);

    auto reply = m_client1.Unary(qt::tst::i1::CallMessage{});
    QVERIFY(reply);

    QSignalSpy finishedSpy(reply.get(), &QGrpcCallReply::finished);
    QVERIFY(finishedSpy.wait());

    QByteArrayList actualRemaining;
    for (const auto &call : std::as_const(CallLog)) {
        if (call.capability == Capability::Start)
            actualRemaining.append(call.name);
    }
    QCOMPARE(actualRemaining, expectedRemaining);
}

void QtGrpcClientInterceptorsTest::removeAllInterceptorsMultiple()
{
    auto processor = m_server->createProcessor();
    setupUnaryEcho(processor);

    auto channel = createChannel();
    auto interceptorA = std::make_unique<LoggingInterceptor>("A");
    auto interceptorB = std::make_unique<LoggingInterceptor>("B");
    QVERIFY(channel->addInterceptor(interceptorA.get()));
    QVERIFY(channel->addInterceptor(interceptorB.get()));

    channel->removeAllInterceptors();
    channel->removeAllInterceptors();

    auto interceptorNew = std::make_unique<LoggingInterceptor>("New");
    QVERIFY(channel->addInterceptor(interceptorNew.get()));

    m_client1.attachChannel(channel);

    auto reply = m_client1.Unary(qt::tst::i1::CallMessage{});
    QVERIFY(reply);

    QSignalSpy finishedSpy(reply.get(), &QGrpcCallReply::finished);
    QVERIFY(finishedSpy.wait());

    QByteArrayList names;
    for (const auto &call : std::as_const(CallLog)) {
        if (call.capability == Capability::Start)
            names.append(call.name);
    }
    QCOMPARE(names, QByteArrayList{ "New" });
}

QTEST_MAIN(QtGrpcClientInterceptorsTest)

#include "tst_grpc_client_interceptors.moc"
