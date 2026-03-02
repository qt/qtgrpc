
#include "non-final-properties.qpb.h"

#include <QtProtobuf/qprotobufregistration.h>

static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarTestMessages(qRegisterProtobufType<TestMessages>);
static bool RegisterNon_final_propertiesProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();

