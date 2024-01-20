/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#include "testservice_client.grpc.qpb.h"

namespace qtgrpc::tests {
namespace TestService {
using namespace Qt::StringLiterals;

Client::Client(QObject *parent)
    : QAbstractGrpcClient("qtgrpc.tests.TestService"_L1, parent)
{
}


std::shared_ptr<QGrpcCallReply> Client::testMethod(const qtgrpc::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options)
{
    return call("testMethod"_L1, arg, options);
}

void Client::testMethod(const qtgrpc::tests::SimpleStringMessage &arg, const QObject *context, const std::function<void(std::shared_ptr<QGrpcCallReply>)> &callback, const QGrpcCallOptions &options)
{
    std::shared_ptr<QGrpcCallReply> reply = call("testMethod"_L1, arg, options);
    QObject::connect(reply.get(), &QGrpcCallReply::finished, context, [reply, callback]() {
        callback(reply);
    }, Qt::SingleShotConnection);
}

std::shared_ptr<QGrpcServerStream> Client::streamTestMethodServerStream(const qtgrpc::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options)
{
    return startStream<QGrpcServerStream>("testMethodServerStream"_L1, arg, options);
}

std::shared_ptr<QGrpcClientStream> Client::streamTestMethodClientStream(const qtgrpc::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options)
{
    return startStream<QGrpcClientStream>("testMethodClientStream"_L1, arg, options);
}

std::shared_ptr<QGrpcBidirStream> Client::streamTestMethodBiStream(const qtgrpc::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options)
{
    return startStream<QGrpcBidirStream>("testMethodBiStream"_L1, arg, options);
}

} // namespace TestService
} // namespace qtgrpc::tests

