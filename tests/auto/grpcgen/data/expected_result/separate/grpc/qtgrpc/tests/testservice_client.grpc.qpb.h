/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_TESTSERVICE_CLIENT_H
#define QPROTOBUF_TESTSERVICE_CLIENT_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>

#include <QtGrpc/qabstractgrpcclient.h>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstream.h>

#include <memory>

#include "qtgrpc/tests/testservice.qpb.h"
#include "tst_qtgrpcgen_client_grpc_only_exports.qpb.h"

namespace qtgrpc::tests {
namespace TestService {

class QPB_TST_QTGRPCGEN_CLIENT_GRPC_ONLY_EXPORT Client : public QAbstractGrpcClient
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    std::shared_ptr<QGrpcCallReply> testMethod(const qtgrpc::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options = {});
    Q_INVOKABLE void testMethod(const qtgrpc::tests::SimpleStringMessage &arg, const QObject *context, const std::function<void(std::shared_ptr<QGrpcCallReply>)> &callback, const QGrpcCallOptions &options = {});

    std::shared_ptr<QGrpcServerStream> streamTestMethodServerStream(const qtgrpc::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options = {});

    std::shared_ptr<QGrpcClientStream> streamTestMethodClientStream(const qtgrpc::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options = {});

    std::shared_ptr<QGrpcBidirStream> streamTestMethodBiStream(const qtgrpc::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options = {});


};
} // namespace TestService
} // namespace qtgrpc::tests

#endif // QPROTOBUF_TESTSERVICE_CLIENT_H
