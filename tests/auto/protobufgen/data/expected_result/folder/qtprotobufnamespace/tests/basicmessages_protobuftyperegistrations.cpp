#include <QtProtobuf/qprotobufserializer.h>
#include "qtprotobufnamespace/tests/basicmessages.qpb.h"

namespace qtprotobufnamespace::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarEmptyMessage(qRegisterProtobufType<EmptyMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleBoolMessage(qRegisterProtobufType<SimpleBoolMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleIntMessage(qRegisterProtobufType<SimpleIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleSIntMessage(qRegisterProtobufType<SimpleSIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleUIntMessage(qRegisterProtobufType<SimpleUIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleInt64Message(qRegisterProtobufType<SimpleInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleSInt64Message(qRegisterProtobufType<SimpleSInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleUInt64Message(qRegisterProtobufType<SimpleUInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleStringMessage(qRegisterProtobufType<SimpleStringMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleFloatMessage(qRegisterProtobufType<SimpleFloatMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleDoubleMessage(qRegisterProtobufType<SimpleDoubleMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleBytesMessage(qRegisterProtobufType<SimpleBytesMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleFixedInt32Message(qRegisterProtobufType<SimpleFixedInt32Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleFixedInt64Message(qRegisterProtobufType<SimpleFixedInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleSFixedInt32Message(qRegisterProtobufType<SimpleSFixedInt32Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleSFixedInt64Message(qRegisterProtobufType<SimpleSFixedInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarComplexMessage(qRegisterProtobufType<ComplexMessage>);
static bool RegisterBasicmessagesProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();
} // namespace qtprotobufnamespace::tests

