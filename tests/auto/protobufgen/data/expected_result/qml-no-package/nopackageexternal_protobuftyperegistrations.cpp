
#include "nopackageexternal.qpb.h"

#include <QtProtobuf/qprotobufregistration.h>

static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarSimpleIntMessageExt(qRegisterProtobufType<SimpleIntMessageExt>);
static bool RegisterNopackageexternalProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();

