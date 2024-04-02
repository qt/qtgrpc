/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_TESTSERIVCENOMESSAGES_CLIENT_H
#define QPROTOBUF_TESTSERIVCENOMESSAGES_CLIENT_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtGrpc/qabstractgrpcclient.h>
#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcstream.h>

#include <memory>

#include "basicmessages.qpb.h"

namespace qtgrpc::tests::nomessages {
namespace TestService {

class  Client : public QAbstractGrpcClient
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    std::shared_ptr<QGrpcCallReply> testMethod(const qtprotobufnamespace::tests::SimpleStringMessage &arg, const QGrpcCallOptions &options = {});
    Q_INVOKABLE void testMethod(const qtprotobufnamespace::tests::SimpleStringMessage &arg, const QObject *context, const std::function<void(std::shared_ptr<QGrpcCallReply>)> &callback, const QGrpcCallOptions &options = {});


};
} // namespace TestService
} // namespace qtgrpc::tests::nomessages

#endif // QPROTOBUF_TESTSERIVCENOMESSAGES_CLIENT_H
