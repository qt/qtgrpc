
#include "string-escaping.qpb.h"

#include <QtProtobuf/qprotobufregistration.h>

namespace qtprotobufnamespace::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarEscapedStrings(qRegisterProtobufType<EscapedStrings>);
static bool RegisterString_escapingProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();
} // namespace qtprotobufnamespace::tests

