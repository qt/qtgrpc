// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <proto/bench_client.grpc.qpb.h>
#include <qrpcbench_common.h>

#include <QtGrpc/qgrpchttp2channel.h>

#include <QtCore/qcoreapplication.h>
#include <QtCore/qelapsedtimer.h>
#include <QtCore/qeventloop.h>
#include <QtCore/qobject.h>
#include <QtCore/qtimer.h>

using namespace std::chrono_literals;

class QtGrpcClientBenchmark : public QObject
{
    Q_OBJECT
public:
    explicit QtGrpcClientBenchmark(quint64 calls) : mCalls(calls)
    {
        QUrl uri(QString("http://") + HostUri.data());
        mClient.attachChannel(std::make_shared<QGrpcHttp2Channel>(std::move(uri)));
    }
    ~QtGrpcClientBenchmark() override = default;

    QtGrpcClientBenchmark(const QtGrpcClientBenchmark &) = delete;
    QtGrpcClientBenchmark &operator=(const QtGrpcClientBenchmark &) = delete;

    QtGrpcClientBenchmark(QtGrpcClientBenchmark &&) = delete;
    QtGrpcClientBenchmark &operator=(QtGrpcClientBenchmark &&) = delete;

    void unaryCall();
    void serverStreaming();
    void clientStreaming();
    void bidirStreaming();

private:
    void unaryCallHelper(qt::bench::UnaryCallRequest &request, quint64 &writes);

private:
    qt::bench::BenchmarkService::Client mClient;
    QEventLoop mLoop;
    QElapsedTimer mTimer;
    uint64_t mCalls;
};

void QtGrpcClientBenchmark::unaryCall()
{
    quint64 writes = 0;
    qt::bench::UnaryCallRequest request;
    unaryCallHelper(request, writes);
    mLoop.exec();
}

void QtGrpcClientBenchmark::unaryCallHelper(qt::bench::UnaryCallRequest &request, quint64 &writes)
{
    // recursively enqueue a message after the previous message finished
    request.setPing(writes);
    const auto reply = mClient.UnaryCall(request);

    auto connection = std::make_shared<QMetaObject::Connection>();
    *connection = QObject::connect(reply.get(), &QGrpcCallReply::finished, &mClient,
                     [connection, reply, this, &request, &writes](const QGrpcStatus &status) {
                         if (writes == 0)
                             mTimer.start();
                         if (status.isOk()) {
                             if (++writes < mCalls) {
                                 unaryCallHelper(request, writes);
                             } else {
                                 Client::printRpcResult("UnaryCall", mTimer.nsecsElapsed(), writes);
                                 mLoop.quit();
                             }
                         } else {
                             qDebug() << "FAILED: " << status;
                             mLoop.quit();
                         }
                         QObject::disconnect(*connection);
                     });
}

void QtGrpcClientBenchmark::serverStreaming()
{
    quint64 counter = 0;
    qt::bench::ServerStreamingRequest request;
    request.setPing(mCalls);
    const auto stream = mClient.ServerStreaming(request);

    QObject::connect(stream.get(), &QGrpcServerStream::messageReceived, this,
                     [this, stream, &counter]() {
                         if (counter == 0)
                             mTimer.start();
                         const auto response = stream->read<qt::bench::ServerStreamingResponse>();
                         Q_ASSERT(response->pong() == counter);
                         ++counter;
                     });

    QObject::connect(stream.get(), &QGrpcServerStream::finished, this,
                     [this, &counter](const QGrpcStatus &status) {
                         if (status.isOk()) {
                             Client::printRpcResult("ServerStreaming", mTimer.nsecsElapsed(),
                                                    counter);
                         } else {
                             qDebug() << "FAILED: " << status;
                         }
                         mLoop.quit();
                     });
    mLoop.exec();
}

void QtGrpcClientBenchmark::clientStreaming()
{
    quint64 counter = 0;
    qt::bench::ClientStreamingRequest request;
    const auto stream = mClient.ClientStreaming(request);

    QTimer::singleShot(0, [this, &stream, &counter, &request]() {
        // Run on event loop
        mTimer.start();
        for (; counter < mCalls; ++counter) {
            request.setPing(counter);
            stream->writeMessage(request);
        }
        stream->writesDone();
    });

    QObject::connect(stream.get(), &QGrpcServerStream::finished, this,
                     [this, &stream, &counter](const QGrpcStatus &status) {
                         if (status.isOk()) {
                             Client::printRpcResult("ClientStreaming", mTimer.nsecsElapsed(),
                                                    counter);
                             const auto resp = stream->read<qt::bench::ClientStreamingResponse>();
                         } else {
                             qDebug() << "FAILED: " << status;
                         }
                         mLoop.quit();
                     });
    mLoop.exec();
}

void QtGrpcClientBenchmark::bidirStreaming()
{
    quint64 counter = 0;
    qt::bench::BiDirStreamingRequest request;
    qt::bench::BiDirStreamingResponse response;
    const auto stream = mClient.BiDirStreaming(request);

    QObject::connect(stream.get(), &QGrpcBidirStream::messageReceived, this,
                     [this, stream, &counter, &response, &request]() {
                         if (counter == 0)
                             mTimer.start();
                         if (stream->read(&response)) {
                             if (counter < mCalls) {
                                 request.setPing(counter);
                                 stream->writeMessage(&request);
                                 ++counter;
                             } else {
                                 stream->writesDone();
                             }
                         } else {
                             qDebug() << "FAILED: read()";
                             mLoop.quit();
                         }
                     });

    QObject::connect(stream.get(), &QGrpcServerStream::finished, this,
                     [this, &counter](const QGrpcStatus &status) {
                         if (status.isOk()) {
                             Client::printRpcResult("BidirStreaming", mTimer.nsecsElapsed(),
                                                    counter);
                         } else {
                             qDebug() << "FAILED: " << status;
                         }
                         mLoop.quit();
                     });
    mLoop.exec();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Client::benchmarkMain<QtGrpcClientBenchmark>("QtGrpcClient", argc, argv);
}

#include "bench_qtgrpcclient.moc"
