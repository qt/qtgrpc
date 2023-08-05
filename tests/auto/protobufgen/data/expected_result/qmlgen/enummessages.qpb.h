/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_ENUMMESSAGES_H
#define QPROTOBUF_ENUMMESSAGES_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>

#include <QtQml/qqmlregistration.h>
#include <QtQml/qqmllist.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>

#include <QtCore/qmetatype.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>

#include <memory>

#if defined(QT_SHARED) || !defined(QT_STATIC)
#  if defined(QT_BUILD_TST_QTPROTOBUFGEN_QML_GEN_LIB)
#    define QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT Q_DECL_EXPORT
#  else
#    define QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT
#endif

namespace qtprotobufnamespace::tests {

namespace TestEnumGadget {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(TestEnum)

enum TestEnum : int32_t {
    TEST_ENUM_VALUE0 = 0,
    TEST_ENUM_VALUE1 = 1,
    TEST_ENUM_VALUE2 = 2,
    TEST_ENUM_VALUE3 = 4,
    TEST_ENUM_VALUE4 = 3,
};
Q_ENUM_NS(TestEnum)

using TestEnumRepeated = QList<TestEnum>;
QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT void registerTypes();
} // namespace TestEnumGadget

namespace TestEnumSecondInFileGadget {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(TestEnumSecondInFile)

enum TestEnumSecondInFile : int32_t {
    TEST_ENUM_SIF_VALUE0 = 0,
    TEST_ENUM_SIF_VALUE1 = 1,
    TEST_ENUM_SIF_VALUE2 = 2,
};
Q_ENUM_NS(TestEnumSecondInFile)

using TestEnumSecondInFileRepeated = QList<TestEnumSecondInFile>;
QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT void registerTypes();
} // namespace TestEnumSecondInFileGadget
class SimpleEnumMessage;
using SimpleEnumMessageRepeated = QList<SimpleEnumMessage>;
namespace SimpleEnumMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
enum LocalEnum : int32_t;
using LocalEnumRepeated = QList<LocalEnum>;
} // namespace SimpleEnumMessage_QtProtobufNested

class RepeatedEnumMessage;
using RepeatedEnumMessageRepeated = QList<RepeatedEnumMessage>;
namespace RepeatedEnumMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
enum LocalEnum : int32_t;
using LocalEnumRepeated = QList<LocalEnum>;
} // namespace RepeatedEnumMessage_QtProtobufNested

class MixedEnumUsageMessage;
using MixedEnumUsageMessageRepeated = QList<MixedEnumUsageMessage>;
namespace MixedEnumUsageMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
enum LocalEnum : int32_t;
using LocalEnumRepeated = QList<LocalEnum>;
class NestedEnumHolder;
using NestedEnumHolderRepeated = QList<NestedEnumHolder>;
namespace NestedEnumHolder_QtProtobufNested {
enum class QtProtobufFieldEnum;
enum LocalEnum : int32_t;
using LocalEnumRepeated = QList<LocalEnum>;
} // namespace NestedEnumHolder_QtProtobufNested

class NestedEnumHolderLevel1;
using NestedEnumHolderLevel1Repeated = QList<NestedEnumHolderLevel1>;
namespace NestedEnumHolderLevel1_QtProtobufNested {
enum class QtProtobufFieldEnum;
enum LocalEnum : int32_t;
using LocalEnumRepeated = QList<LocalEnum>;
class NestedEnumHolderLevel2;
using NestedEnumHolderLevel2Repeated = QList<NestedEnumHolderLevel2>;
namespace NestedEnumHolderLevel2_QtProtobufNested {
enum class QtProtobufFieldEnum;
enum LocalEnum : int32_t;
using LocalEnumRepeated = QList<LocalEnum>;
} // namespace NestedEnumHolderLevel2_QtProtobufNested

} // namespace NestedEnumHolderLevel1_QtProtobufNested

class NestedEnumMessage;
using NestedEnumMessageRepeated = QList<NestedEnumMessage>;
namespace NestedEnumMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace NestedEnumMessage_QtProtobufNested

} // namespace MixedEnumUsageMessage_QtProtobufNested

class SimpleFileEnumMessage;
using SimpleFileEnumMessageRepeated = QList<SimpleFileEnumMessage>;
namespace SimpleFileEnumMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleFileEnumMessage_QtProtobufNested

class StepChildEnumMessage;
using StepChildEnumMessageRepeated = QList<StepChildEnumMessage>;
namespace StepChildEnumMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace StepChildEnumMessage_QtProtobufNested

class A;
using ARepeated = QList<A>;
namespace A_QtProtobufNested {
enum class QtProtobufFieldEnum;
enum AEnum : int32_t;
using AEnumRepeated = QList<AEnum>;
} // namespace A_QtProtobufNested

class B;
using BRepeated = QList<B>;
namespace B_QtProtobufNested {
enum class QtProtobufFieldEnum;
enum BEnum : int32_t;
using BEnumRepeated = QList<BEnum>;
} // namespace B_QtProtobufNested


class SimpleEnumMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleEnumMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleEnumMessage)
    QML_VALUE_TYPE(simpleEnumMessage)
    Q_PROPERTY(LocalEnum localEnum READ localEnum WRITE setLocalEnum SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleEnumMessage_QtProtobufNested::QtProtobufFieldEnum;
    using LocalEnum = SimpleEnumMessage_QtProtobufNested::LocalEnum;
    using LocalEnumRepeated = QList<LocalEnum>;
    SimpleEnumMessage();
    ~SimpleEnumMessage();
    SimpleEnumMessage(const SimpleEnumMessage &other);
    SimpleEnumMessage &operator =(const SimpleEnumMessage &other);
    SimpleEnumMessage(SimpleEnumMessage &&other) noexcept;
    SimpleEnumMessage &operator =(SimpleEnumMessage &&other) noexcept;
    bool operator ==(const SimpleEnumMessage &other) const;
    bool operator !=(const SimpleEnumMessage &other) const;

    SimpleEnumMessage_QtProtobufNested::LocalEnum localEnum() const;
    void setLocalEnum(const SimpleEnumMessage_QtProtobufNested::LocalEnum &localEnum);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleEnumMessage_QtProtobufData> dptr;
};
namespace SimpleEnumMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleEnumMessage)

enum class QtProtobufFieldEnum {
    LocalEnumProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

enum LocalEnum : int32_t {
    LOCAL_ENUM_VALUE0 = 0,
    LOCAL_ENUM_VALUE1 = 1,
    LOCAL_ENUM_VALUE2 = 2,
    LOCAL_ENUM_VALUE3 = 3,
};
Q_ENUM_NS(LocalEnum)

using LocalEnumRepeated = QList<LocalEnum>;
} // namespace SimpleEnumMessage_QtProtobufNested

class RepeatedEnumMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT RepeatedEnumMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedEnumMessage)
    QML_VALUE_TYPE(repeatedEnumMessage)
    Q_PROPERTY(qtprotobufnamespace::tests::RepeatedEnumMessage::LocalEnumRepeated localEnumList READ localEnumList WRITE setLocalEnumList SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedEnumMessage_QtProtobufNested::QtProtobufFieldEnum;
    using LocalEnum = RepeatedEnumMessage_QtProtobufNested::LocalEnum;
    using LocalEnumRepeated = QList<LocalEnum>;
    RepeatedEnumMessage();
    ~RepeatedEnumMessage();
    RepeatedEnumMessage(const RepeatedEnumMessage &other);
    RepeatedEnumMessage &operator =(const RepeatedEnumMessage &other);
    RepeatedEnumMessage(RepeatedEnumMessage &&other) noexcept;
    RepeatedEnumMessage &operator =(RepeatedEnumMessage &&other) noexcept;
    bool operator ==(const RepeatedEnumMessage &other) const;
    bool operator !=(const RepeatedEnumMessage &other) const;

    RepeatedEnumMessage_QtProtobufNested::LocalEnumRepeated localEnumList() const;
    RepeatedEnumMessage_QtProtobufNested::LocalEnumRepeated &localEnumList();
    void setLocalEnumList(const RepeatedEnumMessage_QtProtobufNested::LocalEnumRepeated &localEnumList);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedEnumMessage_QtProtobufData> dptr;
};
namespace RepeatedEnumMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(RepeatedEnumMessage)

enum class QtProtobufFieldEnum {
    LocalEnumListProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

enum LocalEnum : int32_t {
    LOCAL_ENUM_VALUE0 = 0,
    LOCAL_ENUM_VALUE1 = 1,
    LOCAL_ENUM_VALUE2 = 2,
    LOCAL_ENUM_VALUE3 = 3,
};
Q_ENUM_NS(LocalEnum)

using LocalEnumRepeated = QList<LocalEnum>;
} // namespace RepeatedEnumMessage_QtProtobufNested

class MixedEnumUsageMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT MixedEnumUsageMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(MixedEnumUsageMessage)
    QML_VALUE_TYPE(mixedEnumUsageMessage)
    Q_PROPERTY(LocalEnum localEnum READ localEnum WRITE setLocalEnum SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::MixedEnumUsageMessage::LocalEnumRepeated localEnumList READ localEnumList WRITE setLocalEnumList SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::MixedEnumUsageMessage::LocalEnumMapEntry localEnumMap READ localEnumMap WRITE setLocalEnumMap SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::MixedEnumUsageMessage::NestedEnumMessageRepeated msgListData READ msgList WRITE setMsgList SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = MixedEnumUsageMessage_QtProtobufNested::QtProtobufFieldEnum;
    using LocalEnum = MixedEnumUsageMessage_QtProtobufNested::LocalEnum;
    using LocalEnumRepeated = QList<LocalEnum>;
    using NestedEnumHolder = MixedEnumUsageMessage_QtProtobufNested::NestedEnumHolder;
    using NestedEnumHolderRepeated = MixedEnumUsageMessage_QtProtobufNested::NestedEnumHolderRepeated;
    using NestedEnumHolderLevel1 = MixedEnumUsageMessage_QtProtobufNested::NestedEnumHolderLevel1;
    using NestedEnumHolderLevel1Repeated = MixedEnumUsageMessage_QtProtobufNested::NestedEnumHolderLevel1Repeated;
    using NestedEnumMessage = MixedEnumUsageMessage_QtProtobufNested::NestedEnumMessage;
    using NestedEnumMessageRepeated = MixedEnumUsageMessage_QtProtobufNested::NestedEnumMessageRepeated;
    using LocalEnumMapEntry = QHash<QString, MixedEnumUsageMessage_QtProtobufNested::LocalEnum>;
    MixedEnumUsageMessage();
    ~MixedEnumUsageMessage();
    MixedEnumUsageMessage(const MixedEnumUsageMessage &other);
    MixedEnumUsageMessage &operator =(const MixedEnumUsageMessage &other);
    MixedEnumUsageMessage(MixedEnumUsageMessage &&other) noexcept;
    MixedEnumUsageMessage &operator =(MixedEnumUsageMessage &&other) noexcept;
    bool operator ==(const MixedEnumUsageMessage &other) const;
    bool operator !=(const MixedEnumUsageMessage &other) const;

    MixedEnumUsageMessage_QtProtobufNested::LocalEnum localEnum() const;

    MixedEnumUsageMessage_QtProtobufNested::LocalEnumRepeated localEnumList() const;
    MixedEnumUsageMessage_QtProtobufNested::LocalEnumRepeated &localEnumList();

    MixedEnumUsageMessage::LocalEnumMapEntry localEnumMap() const;
    MixedEnumUsageMessage::LocalEnumMapEntry &localEnumMap();

    MixedEnumUsageMessage_QtProtobufNested::NestedEnumMessageRepeated msgList() const;
    MixedEnumUsageMessage_QtProtobufNested::NestedEnumMessageRepeated &msgList();
    void setLocalEnum(const MixedEnumUsageMessage_QtProtobufNested::LocalEnum &localEnum);
    void setLocalEnumList(const MixedEnumUsageMessage_QtProtobufNested::LocalEnumRepeated &localEnumList);
    void setLocalEnumMap(const MixedEnumUsageMessage::LocalEnumMapEntry &localEnumMap);
    void setMsgList(const MixedEnumUsageMessage_QtProtobufNested::NestedEnumMessageRepeated &msgList);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<MixedEnumUsageMessage_QtProtobufData> dptr;
};
namespace MixedEnumUsageMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(MixedEnumUsageMessage)

enum class QtProtobufFieldEnum {
    LocalEnumProtoFieldNumber = 1,
    LocalEnumListProtoFieldNumber = 2,
    LocalEnumMapProtoFieldNumber = 3,
    MsgListProtoFieldNumber = 4,
};
Q_ENUM_NS(QtProtobufFieldEnum)

enum LocalEnum : int32_t {
    LOCAL_ENUM_VALUE0 = 0,
    LOCAL_ENUM_VALUE1 = 1,
    LOCAL_ENUM_VALUE2 = 2,
    LOCAL_ENUM_VALUE3 = 3,
};
Q_ENUM_NS(LocalEnum)

using LocalEnumRepeated = QList<LocalEnum>;

class NestedEnumHolder_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT NestedEnumHolder : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(NestedEnumHolder)
    QML_VALUE_TYPE(nestedEnumHolder)

public:
    using QtProtobufFieldEnum = NestedEnumHolder_QtProtobufNested::QtProtobufFieldEnum;
    using LocalEnum = NestedEnumHolder_QtProtobufNested::LocalEnum;
    using LocalEnumRepeated = QList<LocalEnum>;
    NestedEnumHolder();
    ~NestedEnumHolder();
    NestedEnumHolder(const NestedEnumHolder &other);
    NestedEnumHolder &operator =(const NestedEnumHolder &other);
    NestedEnumHolder(NestedEnumHolder &&other) noexcept;
    NestedEnumHolder &operator =(NestedEnumHolder &&other) noexcept;
    bool operator ==(const NestedEnumHolder &other) const;
    bool operator !=(const NestedEnumHolder &other) const;
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<NestedEnumHolder_QtProtobufData> dptr;
};
namespace NestedEnumHolder_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(NestedEnumHolder)

enum LocalEnum : int32_t {
    NESTED_LOCAL_ENUM_VALUE0 = 0,
    NESTED_LOCAL_ENUM_VALUE1 = 1,
    NESTED_LOCAL_ENUM_VALUE2 = 2,
    NESTED_LOCAL_ENUM_VALUE3 = 3,
};
Q_ENUM_NS(LocalEnum)

using LocalEnumRepeated = QList<LocalEnum>;
} // namespace NestedEnumHolder_QtProtobufNested

class NestedEnumHolderLevel1_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT NestedEnumHolderLevel1 : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(NestedEnumHolderLevel1)
    QML_VALUE_TYPE(nestedEnumHolderLevel1)

public:
    using QtProtobufFieldEnum = NestedEnumHolderLevel1_QtProtobufNested::QtProtobufFieldEnum;
    using LocalEnum = NestedEnumHolderLevel1_QtProtobufNested::LocalEnum;
    using LocalEnumRepeated = QList<LocalEnum>;
    using NestedEnumHolderLevel2 = NestedEnumHolderLevel1_QtProtobufNested::NestedEnumHolderLevel2;
    using NestedEnumHolderLevel2Repeated = NestedEnumHolderLevel1_QtProtobufNested::NestedEnumHolderLevel2Repeated;
    NestedEnumHolderLevel1();
    ~NestedEnumHolderLevel1();
    NestedEnumHolderLevel1(const NestedEnumHolderLevel1 &other);
    NestedEnumHolderLevel1 &operator =(const NestedEnumHolderLevel1 &other);
    NestedEnumHolderLevel1(NestedEnumHolderLevel1 &&other) noexcept;
    NestedEnumHolderLevel1 &operator =(NestedEnumHolderLevel1 &&other) noexcept;
    bool operator ==(const NestedEnumHolderLevel1 &other) const;
    bool operator !=(const NestedEnumHolderLevel1 &other) const;
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<NestedEnumHolderLevel1_QtProtobufData> dptr;
};
namespace NestedEnumHolderLevel1_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(NestedEnumHolderLevel1)

enum LocalEnum : int32_t {
    NESTED_LEVEL1_LOCAL_ENUM_VALUE0 = 0,
    NESTED_LEVEL1_LOCAL_ENUM_VALUE1 = 1,
};
Q_ENUM_NS(LocalEnum)

using LocalEnumRepeated = QList<LocalEnum>;

class NestedEnumHolderLevel2_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT NestedEnumHolderLevel2 : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(NestedEnumHolderLevel2)
    QML_VALUE_TYPE(nestedEnumHolderLevel2)

public:
    using QtProtobufFieldEnum = NestedEnumHolderLevel2_QtProtobufNested::QtProtobufFieldEnum;
    using LocalEnum = NestedEnumHolderLevel2_QtProtobufNested::LocalEnum;
    using LocalEnumRepeated = QList<LocalEnum>;
    NestedEnumHolderLevel2();
    ~NestedEnumHolderLevel2();
    NestedEnumHolderLevel2(const NestedEnumHolderLevel2 &other);
    NestedEnumHolderLevel2 &operator =(const NestedEnumHolderLevel2 &other);
    NestedEnumHolderLevel2(NestedEnumHolderLevel2 &&other) noexcept;
    NestedEnumHolderLevel2 &operator =(NestedEnumHolderLevel2 &&other) noexcept;
    bool operator ==(const NestedEnumHolderLevel2 &other) const;
    bool operator !=(const NestedEnumHolderLevel2 &other) const;
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<NestedEnumHolderLevel2_QtProtobufData> dptr;
};
namespace NestedEnumHolderLevel2_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(NestedEnumHolderLevel2)

enum LocalEnum : int32_t {
    NESTED_LEVEL2_LOCAL_ENUM_VALUE0 = 0,
    NESTED_LEVEL2_LOCAL_ENUM_VALUE1 = 1,
};
Q_ENUM_NS(LocalEnum)

using LocalEnumRepeated = QList<LocalEnum>;
} // namespace NestedEnumHolderLevel2_QtProtobufNested
} // namespace NestedEnumHolderLevel1_QtProtobufNested

class NestedEnumMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT NestedEnumMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(NestedEnumMessage)
    QML_VALUE_TYPE(nestedEnumMessage)
    Q_PROPERTY(qtprotobufnamespace::tests::MixedEnumUsageMessage::LocalEnum localEnumField READ localEnumField WRITE setLocalEnumField SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::RepeatedEnumMessage::LocalEnum localEnumField2 READ localEnumField2 WRITE setLocalEnumField2 SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::MixedEnumUsageMessage::NestedEnumHolder::LocalEnum localEnumField3 READ localEnumField3 WRITE setLocalEnumField3 SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::MixedEnumUsageMessage::NestedEnumHolderLevel1::LocalEnum localEnumField4 READ localEnumField4 WRITE setLocalEnumField4 SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::MixedEnumUsageMessage::NestedEnumHolderLevel1::NestedEnumHolderLevel2::LocalEnum localEnumField5 READ localEnumField5 WRITE setLocalEnumField5 SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = NestedEnumMessage_QtProtobufNested::QtProtobufFieldEnum;
    NestedEnumMessage();
    ~NestedEnumMessage();
    NestedEnumMessage(const NestedEnumMessage &other);
    NestedEnumMessage &operator =(const NestedEnumMessage &other);
    NestedEnumMessage(NestedEnumMessage &&other) noexcept;
    NestedEnumMessage &operator =(NestedEnumMessage &&other) noexcept;
    bool operator ==(const NestedEnumMessage &other) const;
    bool operator !=(const NestedEnumMessage &other) const;

    MixedEnumUsageMessage_QtProtobufNested::LocalEnum localEnumField() const;

    RepeatedEnumMessage_QtProtobufNested::LocalEnum localEnumField2() const;

    MixedEnumUsageMessage_QtProtobufNested::NestedEnumHolder_QtProtobufNested::LocalEnum localEnumField3() const;

    MixedEnumUsageMessage_QtProtobufNested::NestedEnumHolderLevel1_QtProtobufNested::LocalEnum localEnumField4() const;

    MixedEnumUsageMessage_QtProtobufNested::NestedEnumHolderLevel1_QtProtobufNested::NestedEnumHolderLevel2_QtProtobufNested::LocalEnum localEnumField5() const;
    void setLocalEnumField(const MixedEnumUsageMessage_QtProtobufNested::LocalEnum &localEnumField);
    void setLocalEnumField2(const RepeatedEnumMessage_QtProtobufNested::LocalEnum &localEnumField2);
    void setLocalEnumField3(const MixedEnumUsageMessage_QtProtobufNested::NestedEnumHolder_QtProtobufNested::LocalEnum &localEnumField3);
    void setLocalEnumField4(const MixedEnumUsageMessage_QtProtobufNested::NestedEnumHolderLevel1_QtProtobufNested::LocalEnum &localEnumField4);
    void setLocalEnumField5(const MixedEnumUsageMessage_QtProtobufNested::NestedEnumHolderLevel1_QtProtobufNested::NestedEnumHolderLevel2_QtProtobufNested::LocalEnum &localEnumField5);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<NestedEnumMessage_QtProtobufData> dptr;
};
namespace NestedEnumMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(NestedEnumMessage)

enum class QtProtobufFieldEnum {
    LocalEnumFieldProtoFieldNumber = 1,
    LocalEnumField2ProtoFieldNumber = 2,
    LocalEnumField3ProtoFieldNumber = 3,
    LocalEnumField4ProtoFieldNumber = 4,
    LocalEnumField5ProtoFieldNumber = 5,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace NestedEnumMessage_QtProtobufNested
} // namespace MixedEnumUsageMessage_QtProtobufNested

class SimpleFileEnumMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleFileEnumMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleFileEnumMessage)
    QML_VALUE_TYPE(simpleFileEnumMessage)
    Q_PROPERTY(qtprotobufnamespace::tests::TestEnumGadget::TestEnum globalEnum READ globalEnum WRITE setGlobalEnum SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::TestEnumGadget::TestEnumRepeated globalEnumList READ globalEnumList WRITE setGlobalEnumList SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleFileEnumMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleFileEnumMessage();
    ~SimpleFileEnumMessage();
    SimpleFileEnumMessage(const SimpleFileEnumMessage &other);
    SimpleFileEnumMessage &operator =(const SimpleFileEnumMessage &other);
    SimpleFileEnumMessage(SimpleFileEnumMessage &&other) noexcept;
    SimpleFileEnumMessage &operator =(SimpleFileEnumMessage &&other) noexcept;
    bool operator ==(const SimpleFileEnumMessage &other) const;
    bool operator !=(const SimpleFileEnumMessage &other) const;

    TestEnumGadget::TestEnum globalEnum() const;

    TestEnumGadget::TestEnumRepeated globalEnumList() const;
    TestEnumGadget::TestEnumRepeated &globalEnumList();
    void setGlobalEnum(const TestEnumGadget::TestEnum &globalEnum);
    void setGlobalEnumList(const TestEnumGadget::TestEnumRepeated &globalEnumList);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleFileEnumMessage_QtProtobufData> dptr;
};
namespace SimpleFileEnumMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleFileEnumMessage)

enum class QtProtobufFieldEnum {
    GlobalEnumProtoFieldNumber = 1,
    GlobalEnumListProtoFieldNumber = 2,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleFileEnumMessage_QtProtobufNested

class StepChildEnumMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT StepChildEnumMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(StepChildEnumMessage)
    QML_VALUE_TYPE(stepChildEnumMessage)
    Q_PROPERTY(qtprotobufnamespace::tests::SimpleEnumMessage::LocalEnum localStepChildEnum READ localStepChildEnum WRITE setLocalStepChildEnum SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::SimpleEnumMessage::LocalEnumRepeated localStepChildList READ localStepChildList WRITE setLocalStepChildList SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = StepChildEnumMessage_QtProtobufNested::QtProtobufFieldEnum;
    StepChildEnumMessage();
    ~StepChildEnumMessage();
    StepChildEnumMessage(const StepChildEnumMessage &other);
    StepChildEnumMessage &operator =(const StepChildEnumMessage &other);
    StepChildEnumMessage(StepChildEnumMessage &&other) noexcept;
    StepChildEnumMessage &operator =(StepChildEnumMessage &&other) noexcept;
    bool operator ==(const StepChildEnumMessage &other) const;
    bool operator !=(const StepChildEnumMessage &other) const;

    SimpleEnumMessage_QtProtobufNested::LocalEnum localStepChildEnum() const;

    SimpleEnumMessage_QtProtobufNested::LocalEnumRepeated localStepChildList() const;
    SimpleEnumMessage_QtProtobufNested::LocalEnumRepeated &localStepChildList();
    void setLocalStepChildEnum(const SimpleEnumMessage_QtProtobufNested::LocalEnum &localStepChildEnum);
    void setLocalStepChildList(const SimpleEnumMessage_QtProtobufNested::LocalEnumRepeated &localStepChildList);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<StepChildEnumMessage_QtProtobufData> dptr;
};
namespace StepChildEnumMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(StepChildEnumMessage)

enum class QtProtobufFieldEnum {
    LocalStepChildEnumProtoFieldNumber = 1,
    LocalStepChildListProtoFieldNumber = 2,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace StepChildEnumMessage_QtProtobufNested

class A_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT A : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(A)
    QML_VALUE_TYPE(a)
    Q_PROPERTY(qtprotobufnamespace::tests::B::BEnum val READ val WRITE setVal SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = A_QtProtobufNested::QtProtobufFieldEnum;
    using AEnum = A_QtProtobufNested::AEnum;
    using AEnumRepeated = QList<AEnum>;
    A();
    ~A();
    A(const A &other);
    A &operator =(const A &other);
    A(A &&other) noexcept;
    A &operator =(A &&other) noexcept;
    bool operator ==(const A &other) const;
    bool operator !=(const A &other) const;

    B_QtProtobufNested::BEnum val() const;
    void setVal(const B_QtProtobufNested::BEnum &val);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<A_QtProtobufData> dptr;
};
namespace A_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(A)

enum class QtProtobufFieldEnum {
    ValProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

enum AEnum : int32_t {
    AVal0 = 0,
    AVal1 = 1,
};
Q_ENUM_NS(AEnum)

using AEnumRepeated = QList<AEnum>;
} // namespace A_QtProtobufNested

class B_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT B : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(B)
    QML_VALUE_TYPE(b)
    Q_PROPERTY(qtprotobufnamespace::tests::A::AEnum val READ val WRITE setVal SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = B_QtProtobufNested::QtProtobufFieldEnum;
    using BEnum = B_QtProtobufNested::BEnum;
    using BEnumRepeated = QList<BEnum>;
    B();
    ~B();
    B(const B &other);
    B &operator =(const B &other);
    B(B &&other) noexcept;
    B &operator =(B &&other) noexcept;
    bool operator ==(const B &other) const;
    bool operator !=(const B &other) const;

    A_QtProtobufNested::AEnum val() const;
    void setVal(const A_QtProtobufNested::AEnum &val);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<B_QtProtobufData> dptr;
};
namespace B_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(B)

enum class QtProtobufFieldEnum {
    ValProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

enum BEnum : int32_t {
    BVal0 = 0,
    BVal1 = 1,
};
Q_ENUM_NS(BEnum)

using BEnumRepeated = QList<BEnum>;
} // namespace B_QtProtobufNested
} // namespace qtprotobufnamespace::tests

Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleEnumMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedEnumMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::MixedEnumUsageMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::MixedEnumUsageMessage::NestedEnumHolder)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::MixedEnumUsageMessage::NestedEnumHolderLevel1)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::MixedEnumUsageMessage::NestedEnumHolderLevel1::NestedEnumHolderLevel2)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::MixedEnumUsageMessage::NestedEnumMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleFileEnumMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::StepChildEnumMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::A)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::B)
#endif // QPROTOBUF_ENUMMESSAGES_H
