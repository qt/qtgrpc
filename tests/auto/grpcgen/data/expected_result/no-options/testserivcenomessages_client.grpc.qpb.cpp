/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#include "testserivcenomessages_client.grpc.qpb.h"

namespace qtgrpc::tests::nomessages {
namespace TestService {
using namespace Qt::StringLiterals;

Client::Client(QObject *parent)
    : QGrpcClientBase("qtgrpc.tests.nomessages.TestService"_L1, parent)
{
}

Client::~Client() = default;

std::unique_ptr<QGrpcCallReply> Client::testMethod(const qtprotobufnamespace::tests::SimpleStringMessage &arg)
{
    return testMethod(arg, {});
}


std::unique_ptr<QGrpcCallReply> Client::testMethod(const qtprotobufnamespace::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options)
{
    auto reply = call("testMethod"_L1, arg, options);
    if (auto *replyPtr = reply.get(); replyPtr != nullptr) {
        setOperationResponseMetaType(replyPtr, QMetaType::fromType<qtprotobufnamespace::tests::SimpleStringMessage>());
    }
    return reply;
}

} // namespace TestService
} // namespace qtgrpc::tests::nomessages

