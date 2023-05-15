#include <QtProtobuf/qprotobufserializer.h>
#include "enummessages.qpb.h"

namespace qtprotobufnamespace::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleEnumMessage(qRegisterProtobufType<SimpleEnumMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedEnumMessage(qRegisterProtobufType<RepeatedEnumMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarMixedEnumUsageMessage(qRegisterProtobufType<MixedEnumUsageMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleFileEnumMessage(qRegisterProtobufType<SimpleFileEnumMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarStepChildEnumMessage(qRegisterProtobufType<StepChildEnumMessage>);
} // namespace qtprotobufnamespace::tests

