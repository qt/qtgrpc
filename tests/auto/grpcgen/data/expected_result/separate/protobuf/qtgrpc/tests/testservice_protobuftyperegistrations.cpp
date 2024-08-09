
#include "qtgrpc/tests/testservice.qpb.h"

#include <QtProtobuf/qprotobufregistration.h>

namespace qtgrpc::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleStringMessage(qRegisterProtobufType<SimpleStringMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleIntMessage(qRegisterProtobufType<SimpleIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarBlobMessage(qRegisterProtobufType<BlobMessage>);
static bool RegisterTestserviceProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();
} // namespace qtgrpc::tests

