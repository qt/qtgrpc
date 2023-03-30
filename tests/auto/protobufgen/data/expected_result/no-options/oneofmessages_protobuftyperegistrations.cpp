#include <QtProtobuf/qprotobufserializer.h>
#include "oneofmessages.qpb.h"

namespace qtprotobufnamespace::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarOneofSimpleMessage(qRegisterProtobufType<OneofSimpleMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarOneofComplexMessage(qRegisterProtobufType<OneofComplexMessage>);
} // namespace qtprotobufnamespace::tests

