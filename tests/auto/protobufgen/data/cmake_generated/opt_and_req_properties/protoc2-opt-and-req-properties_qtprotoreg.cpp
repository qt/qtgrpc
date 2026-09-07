
#include "protoc2-opt-and-req-properties.qpb.h"

#include <QtProtobuf/qprotobufregistration.h>

static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleBoolMessage(qRegisterProtobufType<SimpleBoolMessage>);
static bool RegisterProtoc2_opt_and_req_propertiesProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();

