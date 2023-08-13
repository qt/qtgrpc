/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#include "testservice_client.grpc.qpb.h"

namespace qtgrpc::tests {
namespace TestService {
using namespace Qt::StringLiterals;

Client::Client(QObject *parent)
    : QAbstractGrpcClient("qtgrpc.tests.TestService"_L1, parent)
{
}

QGrpcStatus Client::testMethod(const qtgrpc::tests::SimpleStringMessage &arg, qtgrpc::tests::SimpleStringMessage *ret, const QGrpcCallOptions &options)
{
    return call<qtgrpc::tests::SimpleStringMessage>("testMethod"_L1, arg, ret, options);
}

std::shared_ptr<QGrpcCallReply> Client::testMethod(const qtgrpc::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options)
{
    return call<qtgrpc::tests::SimpleStringMessage>("testMethod"_L1, arg, options);
}

void Client::testMethod(const qtgrpc::tests::SimpleStringMessage &arg, const QObject *context, const std::function<void(std::shared_ptr<QGrpcCallReply>)> &callback, const QGrpcCallOptions &options)
{
    std::shared_ptr<QGrpcCallReply> reply = call<qtgrpc::tests::SimpleStringMessage>("testMethod"_L1, arg, options);
    QObject::connect(reply.get(), &QGrpcCallReply::finished, context, [reply, callback]() {
        callback(reply);
    }, Qt::SingleShotConnection);
}

std::shared_ptr<QGrpcStream> Client::streamTestMethodServerStream(const qtgrpc::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options)
{
    return startStream<qtgrpc::tests::SimpleStringMessage>("testMethodServerStream"_L1, arg, options);
}

} // namespace TestService
} // namespace qtgrpc::tests

