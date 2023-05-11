#include <QtProtobuf/qprotobufserializer.h>
#include "nopackage.qpb.h"

static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarEmptyMessage(qRegisterProtobufType<EmptyMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleIntMessage(qRegisterProtobufType<SimpleIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarNoPackageExternalMessage(qRegisterProtobufType<NoPackageExternalMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarNoPackageMessage(qRegisterProtobufType<NoPackageMessage>);

