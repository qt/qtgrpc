#include <QtProtobuf/qprotobufserializer.h>
#include "extranamespace.qpb.h"

namespace MyTopLevelNamespace::qtprotobufnamespace::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarEmptyMessage(qRegisterProtobufType<EmptyMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleStringMessage(qRegisterProtobufType<SimpleStringMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarComplexMessage(qRegisterProtobufType<ComplexMessage>);
} // namespace MyTopLevelNamespace::qtprotobufnamespace::tests

