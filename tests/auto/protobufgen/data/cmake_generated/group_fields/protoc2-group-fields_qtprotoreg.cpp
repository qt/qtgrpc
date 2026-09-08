
#include "protoc2-group-fields.qpb.h"

#include <QtProtobuf/qprotobufregistration.h>

namespace GroupFieldMessage_QtProtobufNested {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSkippedGroup(qRegisterProtobufType<SkippedGroup>);
} // namespace GroupFieldMessage_QtProtobufNested
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarGroupFieldMessage(qRegisterProtobufType<GroupFieldMessage>);
static bool RegisterProtoc2_group_fieldsProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();

