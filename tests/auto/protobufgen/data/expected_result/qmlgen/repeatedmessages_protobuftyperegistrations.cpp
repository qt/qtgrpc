#include <QtProtobuf/qprotobufserializer.h>
#include "repeatedmessages.qpb.h"

namespace qtprotobufnamespace::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedStringMessage(qRegisterProtobufType<RepeatedStringMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedDoubleMessage(qRegisterProtobufType<RepeatedDoubleMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedBytesMessage(qRegisterProtobufType<RepeatedBytesMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedFloatMessage(qRegisterProtobufType<RepeatedFloatMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedComplexMessage(qRegisterProtobufType<RepeatedComplexMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedSIntMessage(qRegisterProtobufType<RepeatedSIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedIntMessage(qRegisterProtobufType<RepeatedIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedUIntMessage(qRegisterProtobufType<RepeatedUIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedSInt64Message(qRegisterProtobufType<RepeatedSInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedInt64Message(qRegisterProtobufType<RepeatedInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedUInt64Message(qRegisterProtobufType<RepeatedUInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedFixedIntMessage(qRegisterProtobufType<RepeatedFixedIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedSFixedIntMessage(qRegisterProtobufType<RepeatedSFixedIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedFixedInt64Message(qRegisterProtobufType<RepeatedFixedInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedSFixedInt64Message(qRegisterProtobufType<RepeatedSFixedInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedBoolMessage(qRegisterProtobufType<RepeatedBoolMessage>);
static bool RegisterRepeatedmessagesProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();
} // namespace qtprotobufnamespace::tests

