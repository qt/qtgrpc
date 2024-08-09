
#include "optional.qpb.h"

#include <QtProtobuf/qprotobufregistration.h>

namespace qtprotobufnamespace::optional::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarTestStringMessage(qRegisterProtobufType<TestStringMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarOptionalMessage(qRegisterProtobufType<OptionalMessage>);
static bool RegisterOptionalProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();
} // namespace qtprotobufnamespace::optional::tests

