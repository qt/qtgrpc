// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/QGrpcHttp2Channel>
#include <QtGrpc/QGrpcSerializationFormat>

#include <QCoreApplication>
#include <QSignalSpy>
#include <QTest>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtProtobuf/QProtobufJsonSerializer>
#include <QtProtobuf/QProtobufSerializer>

#include "testservice_client.grpc.qpb.h"

namespace {
using namespace qtgrpc::tests;
using namespace Qt::StringLiterals;

class DummySerializer : public QAbstractProtobufSerializer
{
public:
    virtual QAbstractProtobufSerializer::DeserializationError deserializationError() const override
    {
        return QAbstractProtobufSerializer::NoDeserializerError;
    }

    virtual QString deserializationErrorString() const override { return {}; }

protected:
    virtual QByteArray
    serializeMessage(const QProtobufMessage *) const override
    {
        return {};
    }

    virtual bool deserializeMessage(QProtobufMessage *,
                                    QByteArrayView) const override
    {
        return true;
    }

    void serializeObject(const QProtobufMessage *,
                         const QtProtobufPrivate::QProtobufPropertyOrderingInfo &) const override
    {
    }
    bool deserializeObject(QProtobufMessage *) const override
    {
        return true;
    }

    void
    serializeListObject(const QProtobufMessage *,
                        const QtProtobufPrivate::QProtobufPropertyOrderingInfo &) const override
    {
    }
    bool deserializeListObject(QProtobufMessage *) const override
    {
        return true;
    }

    void serializeMapPair(const QVariant &, const QVariant &,
                          const QtProtobufPrivate::QProtobufPropertyOrderingInfo &) const override
    {
    }
    bool deserializeMapPair(QVariant &, QVariant &) const override { return true; }

    void serializeEnum(QtProtobuf::int64, const QMetaEnum &,
                       const QtProtobufPrivate::QProtobufPropertyOrderingInfo &) const override
    {
    }
    void serializeEnumList(const QList<QtProtobuf::int64> &, const QMetaEnum &,
                           const QtProtobufPrivate::QProtobufPropertyOrderingInfo &) const override
    {
    }

    bool deserializeEnum(QtProtobuf::int64 &, const QMetaEnum &) const override { return true; }

    bool deserializeEnumList(QList<QtProtobuf::int64> &, const QMetaEnum &) const override
    {
        return true;
    }
};
}

class QGrpcHttp2ChannelTest : public QObject
{
    Q_OBJECT
private slots:
    void CheckMethodsGeneration();
    void AttachChannelThreadTest();
    void SerializationFormat();
    void SerializationFormatWithHeaders();
};

void QGrpcHttp2ChannelTest::CheckMethodsGeneration()
{
    // Dummy compile time check of functions generation and interface compatibility
    TestService::Client client;
    QGrpcChannelOptions channelOptions{ QUrl() };
    client.attachChannel(std::make_shared<QGrpcHttp2Channel>(channelOptions));
    SimpleStringMessage request;
    client.testMethod(request);
    client.testMethod(request, &client, [](std::shared_ptr<QGrpcCallReply>) {});
}


void QGrpcHttp2ChannelTest::AttachChannelThreadTest()
{
    std::shared_ptr<QGrpcHttp2Channel> channel;
    QGrpcChannelOptions channelOptions(QUrl("http://localhost:50051", QUrl::StrictMode));

    std::shared_ptr<QThread> thread(QThread::create([&] {
        channel = std::make_shared<QGrpcHttp2Channel>(channelOptions);
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
                    .startsWith("QAbstractGrpcClient::attachChannel is called from a different "
                                "thread."));
}

void QGrpcHttp2ChannelTest::SerializationFormat()
{
    std::shared_ptr<QAbstractGrpcChannel>
        channel = std::make_shared<QGrpcHttp2Channel>(QGrpcChannelOptions{
            QUrl("http://localhost:50051", QUrl::StrictMode) });
    QVERIFY(dynamic_cast<QProtobufSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withSerializationFormat({ QGrpcSerializationFormat::Format::
                                                              Json }));
    QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withSerializationFormat({ QGrpcSerializationFormat::Format::
                                                              Protobuf }));
    QVERIFY(dynamic_cast<QProtobufSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withSerializationFormat({ "dummy",
                                                          std::make_shared<DummySerializer>() }));
    QVERIFY(dynamic_cast<DummySerializer *>(channel->serializer().get()) != nullptr);
}

void QGrpcHttp2ChannelTest::SerializationFormatWithHeaders()
{
    std::shared_ptr<QAbstractGrpcChannel> channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba, "application/grpc"_ba } }));
    QVERIFY(dynamic_cast<QProtobufSerializer *>(channel->serializer().get()) != nullptr);

    // Initialize with various content-type headers
    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba,
                                                 "application/grpc+json"_ba } }));
    QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba,
                                                 "application/grpc+proto"_ba } }));
    QVERIFY(dynamic_cast<QProtobufSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba,
                                                 "application/grpc+unknown"_ba } }));
    QVERIFY(dynamic_cast<QProtobufSerializer *>(channel->serializer().get()) != nullptr);

    // Initialize with the default content-type header and various serialization formats
    channel = std::make_shared<QGrpcHttp2Channel>(QGrpcChannelOptions{
        QUrl("http://localhost:50051", QUrl::StrictMode) }
                                                      .withMetadata({ { "content-type"_ba,
                                                                        "application/grpc"_ba } })
                                                      .withSerializationFormat({}));
    QVERIFY(dynamic_cast<QProtobufSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba, "application/grpc"_ba } })
                               .withSerializationFormat({ QGrpcSerializationFormat::Format::
                                                              Json }));
    QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba, "application/grpc"_ba } })
                               .withSerializationFormat({ QGrpcSerializationFormat::Format::
                                                              Protobuf }));
    QVERIFY(dynamic_cast<QProtobufSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba, "application/grpc"_ba } })
                               .withSerializationFormat({ "dummy",
                                                          std::make_shared<DummySerializer>() }));
    QVERIFY(dynamic_cast<DummySerializer *>(channel->serializer().get()) != nullptr);

    // Initialize with the content-type header incompatible with serialization format
    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba, "application/grpc+json"_ba } })
                               .withSerializationFormat({ QGrpcSerializationFormat::Format::Protobuf }));
    QVERIFY(dynamic_cast<QProtobufSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba, "application/grpc+json"_ba } })
                               .withSerializationFormat({}));
    QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba, "application/grpc+json"_ba } })
                               .withSerializationFormat({ "dummy",
                                                          std::make_shared<DummySerializer>() }));
    QVERIFY(dynamic_cast<DummySerializer *>(channel->serializer().get()) != nullptr);

    // Initialize with the content-type header matching the serialization format
    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba, "application/grpc+proto"_ba } })
                               .withSerializationFormat({ QGrpcSerializationFormat::Format::
                                                              Protobuf }));
    QVERIFY(dynamic_cast<QProtobufSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba, "application/grpc+json"_ba } })
                               .withSerializationFormat({ QGrpcSerializationFormat::Format::
                                                              Json }));
    QVERIFY(dynamic_cast<QProtobufJsonSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<QGrpcHttp2Channel>(QGrpcChannelOptions{
        QUrl("http://localhost:50051", QUrl::StrictMode) }
                                                      .withMetadata({ { "content-type"_ba,
                                                                        "application/grpc"_ba } })
                                                      .withSerializationFormat({}));
    QVERIFY(dynamic_cast<QProtobufSerializer *>(channel->serializer().get()) != nullptr);

    channel = std::make_shared<
        QGrpcHttp2Channel>(QGrpcChannelOptions{ QUrl("http://localhost:50051", QUrl::StrictMode) }
                               .withMetadata({ { "content-type"_ba, "application/grpc+dummy"_ba } })
                               .withSerializationFormat({ "dummy",
                                                          std::make_shared<DummySerializer>() }));
    QVERIFY(dynamic_cast<DummySerializer *>(channel->serializer().get()) != nullptr);
}

QTEST_MAIN(QGrpcHttp2ChannelTest)

#include "tst_qgrpchttp2channel.moc"
