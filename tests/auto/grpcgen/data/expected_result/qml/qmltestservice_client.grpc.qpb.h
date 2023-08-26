/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_QMLTESTSERVICE_CLIENT_H
#define QPROTOBUF_QMLTESTSERVICE_CLIENT_H

#include <QtProtobuf/qprotobufmessage.h>
#include "testservice_client.grpc.qpb.h"
#include <QtQml/qjsengine.h>
#include <QtQml/qjsvalue.h>
#include <QtQml/qqmlengine.h>

namespace qtgrpc::tests {
namespace TestService {

class QPB_TST_QTGRPCGEN_PLUGIN_EXPORT QmlClient : public Client
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit QmlClient(QObject *parent = nullptr);
    Q_INVOKABLE void testMethod(const qtgrpc::tests::SimpleStringMessage &arg, const QJSValue &callback, const QJSValue &errorCallback, const QGrpcCallOptions &options = {});
};
} // namespace TestService
} // namespace qtgrpc::tests

#endif // QPROTOBUF_QMLTESTSERVICE_CLIENT_H