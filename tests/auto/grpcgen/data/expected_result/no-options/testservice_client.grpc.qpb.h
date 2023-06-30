/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef Q_PROTOBUF_testservice_client_H
#define Q_PROTOBUF_testservice_client_H

#include <QtProtobuf/QProtobufMessage>
#include <QtCore/QMetaType>
#include <QtCore/QList>

#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>

#include <memory>

#include <QAbstractGrpcClient>
#include <QGrpcCallReply>
#include <QGrpcStream>
#include "testservice.qpb.h"


namespace qtgrpc::tests {
namespace TestService {

class  Client : public QAbstractGrpcClient
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    QGrpcStatus testMethod(const qtgrpc::tests::SimpleStringMessage &arg, qtgrpc::tests::SimpleStringMessage *ret);
    std::shared_ptr<QGrpcCallReply> testMethod(const qtgrpc::tests::SimpleStringMessage &arg);
    Q_INVOKABLE void testMethod(const qtgrpc::tests::SimpleStringMessage &arg, const QObject *context, const std::function<void(std::shared_ptr<QGrpcCallReply>)> &callback);

    std::shared_ptr<QGrpcStream> streamTestMethodServerStream(const qtgrpc::tests::SimpleStringMessage &arg);
    std::shared_ptr<QGrpcStream> streamTestMethodServerStream(const qtgrpc::tests::SimpleStringMessage &arg, const QWeakPointer<qtgrpc::tests::SimpleStringMessage> &ret);

    QGrpcStatus testMethodClientStream(const qtgrpc::tests::SimpleStringMessage &arg, qtgrpc::tests::SimpleStringMessage *ret);
    std::shared_ptr<QGrpcCallReply> testMethodClientStream(const qtgrpc::tests::SimpleStringMessage &arg);
    Q_INVOKABLE void testMethodClientStream(const qtgrpc::tests::SimpleStringMessage &arg, const QObject *context, const std::function<void(std::shared_ptr<QGrpcCallReply>)> &callback);

    std::shared_ptr<QGrpcStream> streamTestMethodBiStream(const qtgrpc::tests::SimpleStringMessage &arg);
    std::shared_ptr<QGrpcStream> streamTestMethodBiStream(const qtgrpc::tests::SimpleStringMessage &arg, const QWeakPointer<qtgrpc::tests::SimpleStringMessage> &ret);


};

} // namespace TestService
} // namespace qtgrpc::tests

#endif // Q_PROTOBUF_testservice_client_H
