
#include "qtprotobufnamespace/tests/oneofmessages.qpb.h"

#include <QtProtobuf/qprotobufregistration.h>

namespace qtprotobufnamespace::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarOneofSimpleMessage(qRegisterProtobufType<OneofSimpleMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarOneofComplexMessage(qRegisterProtobufType<OneofComplexMessage>);
static bool RegisterOneofmessagesProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();
} // namespace qtprotobufnamespace::tests

