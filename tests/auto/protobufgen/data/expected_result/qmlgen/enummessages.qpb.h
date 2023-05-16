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
#  if defined(QT_BUILD_TST_QTPROTOBUFGEN_QML_PLUGIN_LIB)
#    define QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT Q_DECL_EXPORT
#  else
#    define QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT
#endif

namespace qtprotobufnamespace::tests {

namespace TestEnumGadget {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT)
QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT void registerTypes();
enum TestEnum {
    TEST_ENUM_VALUE0 = 0,
    TEST_ENUM_VALUE1 = 1,
    TEST_ENUM_VALUE2 = 2,
    TEST_ENUM_VALUE3 = 4,
    TEST_ENUM_VALUE4 = 3,
};
Q_ENUM_NS(TestEnum)
using TestEnumRepeated = QList<TestEnum>;
};

namespace TestEnumSecondInFileGadget {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT)
QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT void registerTypes();
enum TestEnumSecondInFile {
    TEST_ENUM_SIF_VALUE0 = 0,
    TEST_ENUM_SIF_VALUE1 = 1,
    TEST_ENUM_SIF_VALUE2 = 2,
};
Q_ENUM_NS(TestEnumSecondInFile)
using TestEnumSecondInFileRepeated = QList<TestEnumSecondInFile>;
};
class SimpleEnumMessage;
using SimpleEnumMessageRepeated = QList<SimpleEnumMessage>;
class RepeatedEnumMessage;
using RepeatedEnumMessageRepeated = QList<RepeatedEnumMessage>;
class MixedEnumUsageMessage;
using MixedEnumUsageMessageRepeated = QList<MixedEnumUsageMessage>;
class SimpleFileEnumMessage;
using SimpleFileEnumMessageRepeated = QList<SimpleFileEnumMessage>;
class StepChildEnumMessage;
using StepChildEnumMessageRepeated = QList<StepChildEnumMessage>;

class SimpleEnumMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT SimpleEnumMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleEnumMessage)
    QML_VALUE_TYPE(simpleEnumMessage)
    Q_PROPERTY(LocalEnum localEnum READ localEnum WRITE setLocalEnum SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        LocalEnumProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    enum LocalEnum {
        LOCAL_ENUM_VALUE0 = 0,
        LOCAL_ENUM_VALUE1 = 1,
        LOCAL_ENUM_VALUE2 = 2,
        LOCAL_ENUM_VALUE3 = 3,
    };
    Q_ENUM(LocalEnum)
    using LocalEnumRepeated = QList<LocalEnum>;
    SimpleEnumMessage();
    ~SimpleEnumMessage();
    SimpleEnumMessage(const SimpleEnumMessage &other);
    SimpleEnumMessage &operator =(const SimpleEnumMessage &other);
    SimpleEnumMessage(SimpleEnumMessage &&other) noexcept;
    SimpleEnumMessage &operator =(SimpleEnumMessage &&other) noexcept;
    bool operator ==(const SimpleEnumMessage &other) const;
    bool operator !=(const SimpleEnumMessage &other) const;

    SimpleEnumMessage::LocalEnum localEnum() const;
    void setLocalEnum(const SimpleEnumMessage::LocalEnum &localEnum);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleEnumMessage_QtProtobufData> dptr;
};

class RepeatedEnumMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT RepeatedEnumMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedEnumMessage)
    QML_VALUE_TYPE(repeatedEnumMessage)
    Q_PROPERTY(qtprotobufnamespace::tests::RepeatedEnumMessage::LocalEnumRepeated localEnumList READ localEnumList WRITE setLocalEnumList SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        LocalEnumListProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    enum LocalEnum {
        LOCAL_ENUM_VALUE0 = 0,
        LOCAL_ENUM_VALUE1 = 1,
        LOCAL_ENUM_VALUE2 = 2,
        LOCAL_ENUM_VALUE3 = 3,
    };
    Q_ENUM(LocalEnum)
    using LocalEnumRepeated = QList<LocalEnum>;
    RepeatedEnumMessage();
    ~RepeatedEnumMessage();
    RepeatedEnumMessage(const RepeatedEnumMessage &other);
    RepeatedEnumMessage &operator =(const RepeatedEnumMessage &other);
    RepeatedEnumMessage(RepeatedEnumMessage &&other) noexcept;
    RepeatedEnumMessage &operator =(RepeatedEnumMessage &&other) noexcept;
    bool operator ==(const RepeatedEnumMessage &other) const;
    bool operator !=(const RepeatedEnumMessage &other) const;

    RepeatedEnumMessage::LocalEnumRepeated localEnumList() const;
    RepeatedEnumMessage::LocalEnumRepeated &localEnumList();
    void setLocalEnumList(const RepeatedEnumMessage::LocalEnumRepeated &localEnumList);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedEnumMessage_QtProtobufData> dptr;
};

class MixedEnumUsageMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT MixedEnumUsageMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(MixedEnumUsageMessage)
    QML_VALUE_TYPE(mixedEnumUsageMessage)
    Q_PROPERTY(LocalEnum localEnum READ localEnum WRITE setLocalEnum SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::MixedEnumUsageMessage::LocalEnumRepeated localEnumList READ localEnumList WRITE setLocalEnumList SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::MixedEnumUsageMessage::LocalEnumMapEntry localEnumMap READ localEnumMap WRITE setLocalEnumMap SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        LocalEnumProtoFieldNumber = 1,
        LocalEnumListProtoFieldNumber = 2,
        LocalEnumMapProtoFieldNumber = 3,
    };
    Q_ENUM(QtProtobufFieldEnum)

    enum LocalEnum {
        LOCAL_ENUM_VALUE0 = 0,
        LOCAL_ENUM_VALUE1 = 1,
        LOCAL_ENUM_VALUE2 = 2,
        LOCAL_ENUM_VALUE3 = 3,
    };
    Q_ENUM(LocalEnum)
    using LocalEnumRepeated = QList<LocalEnum>;
    using LocalEnumMapEntry = QHash<QString, MixedEnumUsageMessage::LocalEnum>;
    MixedEnumUsageMessage();
    ~MixedEnumUsageMessage();
    MixedEnumUsageMessage(const MixedEnumUsageMessage &other);
    MixedEnumUsageMessage &operator =(const MixedEnumUsageMessage &other);
    MixedEnumUsageMessage(MixedEnumUsageMessage &&other) noexcept;
    MixedEnumUsageMessage &operator =(MixedEnumUsageMessage &&other) noexcept;
    bool operator ==(const MixedEnumUsageMessage &other) const;
    bool operator !=(const MixedEnumUsageMessage &other) const;

    MixedEnumUsageMessage::LocalEnum localEnum() const;

    MixedEnumUsageMessage::LocalEnumRepeated localEnumList() const;
    MixedEnumUsageMessage::LocalEnumRepeated &localEnumList();

    MixedEnumUsageMessage::LocalEnumMapEntry localEnumMap() const;
    MixedEnumUsageMessage::LocalEnumMapEntry &localEnumMap();
    void setLocalEnum(const MixedEnumUsageMessage::LocalEnum &localEnum);
    void setLocalEnumList(const MixedEnumUsageMessage::LocalEnumRepeated &localEnumList);
    void setLocalEnumMap(const MixedEnumUsageMessage::LocalEnumMapEntry &localEnumMap);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<MixedEnumUsageMessage_QtProtobufData> dptr;
};

class SimpleFileEnumMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT SimpleFileEnumMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleFileEnumMessage)
    QML_VALUE_TYPE(simpleFileEnumMessage)
    Q_PROPERTY(qtprotobufnamespace::tests::TestEnumGadget::TestEnum globalEnum READ globalEnum WRITE setGlobalEnum SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::TestEnumGadget::TestEnumRepeated globalEnumList READ globalEnumList WRITE setGlobalEnumList SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        GlobalEnumProtoFieldNumber = 1,
        GlobalEnumListProtoFieldNumber = 2,
    };
    Q_ENUM(QtProtobufFieldEnum)

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

class StepChildEnumMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_PLUGIN_EXPORT StepChildEnumMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(StepChildEnumMessage)
    QML_VALUE_TYPE(stepChildEnumMessage)
    Q_PROPERTY(qtprotobufnamespace::tests::SimpleEnumMessage::LocalEnum localStepChildEnum READ localStepChildEnum WRITE setLocalStepChildEnum SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::SimpleEnumMessage::LocalEnumRepeated localStepChildList READ localStepChildList WRITE setLocalStepChildList SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        LocalStepChildEnumProtoFieldNumber = 1,
        LocalStepChildListProtoFieldNumber = 2,
    };
    Q_ENUM(QtProtobufFieldEnum)

    StepChildEnumMessage();
    ~StepChildEnumMessage();
    StepChildEnumMessage(const StepChildEnumMessage &other);
    StepChildEnumMessage &operator =(const StepChildEnumMessage &other);
    StepChildEnumMessage(StepChildEnumMessage &&other) noexcept;
    StepChildEnumMessage &operator =(StepChildEnumMessage &&other) noexcept;
    bool operator ==(const StepChildEnumMessage &other) const;
    bool operator !=(const StepChildEnumMessage &other) const;

    SimpleEnumMessage::LocalEnum localStepChildEnum() const;

    SimpleEnumMessage::LocalEnumRepeated localStepChildList() const;
    SimpleEnumMessage::LocalEnumRepeated &localStepChildList();
    void setLocalStepChildEnum(const SimpleEnumMessage::LocalEnum &localStepChildEnum);
    void setLocalStepChildList(const SimpleEnumMessage::LocalEnumRepeated &localStepChildList);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<StepChildEnumMessage_QtProtobufData> dptr;
};
} // namespace qtprotobufnamespace::tests

Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleEnumMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedEnumMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::MixedEnumUsageMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleFileEnumMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::StepChildEnumMessage)
#endif // QPROTOBUF_ENUMMESSAGES_H
