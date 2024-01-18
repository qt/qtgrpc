#include <QtProtobuf/qprotobufserializer.h>
#include "optional.qpb.h"

namespace qtprotobufnamespace::optional::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarTestStringMessage(qRegisterProtobufType<TestStringMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarOptionalMessage(qRegisterProtobufType<OptionalMessage>);
} // namespace qtprotobufnamespace::optional::tests

