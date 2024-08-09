
#include "extranamespace.qpb.h"

#include <QtProtobuf/qprotobufregistration.h>

namespace qtprotobufnamespace::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarEmptyMessage(qRegisterProtobufType<EmptyMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleStringMessage(qRegisterProtobufType<SimpleStringMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarComplexMessage(qRegisterProtobufType<ComplexMessage>);
static bool RegisterExtranamespaceProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();
} // namespace qtprotobufnamespace::tests

