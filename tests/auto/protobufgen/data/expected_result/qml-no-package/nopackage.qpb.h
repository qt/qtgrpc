/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_NOPACKAGE_H
#define QPROTOBUF_NOPACKAGE_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtQml/qqmlregistration.h>
#include <QtQml/qqmllist.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>
#include "nopackageexternal.qpb.h"
#include "tst_qtprotobufgen_nopackage_qml_gen_exports.qpb.h"

#include <QtCore/qmetatype.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>

#include <memory>



namespace TestEnumGadget {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(TestEnum)

enum TestEnum : int32_t {
    LOCAL_ENUM_VALUE0 = 0,
    LOCAL_ENUM_VALUE1 = 1,
    LOCAL_ENUM_VALUE2 = 2,
    LOCAL_ENUM_VALUE3 = 5,
};
Q_ENUM_NS(TestEnum)

using TestEnumRepeated = QList<TestEnum>;
QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT void registerTypes();
} // namespace TestEnumGadget
class EmptyMessage;
using EmptyMessageRepeated = QList<EmptyMessage>;
namespace EmptyMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace EmptyMessage_QtProtobufNested

class SimpleIntMessage;
using SimpleIntMessageRepeated = QList<SimpleIntMessage>;
namespace SimpleIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleIntMessage_QtProtobufNested

class NoPackageExternalMessage;
using NoPackageExternalMessageRepeated = QList<NoPackageExternalMessage>;
namespace NoPackageExternalMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace NoPackageExternalMessage_QtProtobufNested

class NoPackageMessage;
using NoPackageMessageRepeated = QList<NoPackageMessage>;
namespace NoPackageMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace NoPackageMessage_QtProtobufNested


class EmptyMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT EmptyMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    QML_VALUE_TYPE(emptyMessage)

public:
    using QtProtobufFieldEnum = EmptyMessage_QtProtobufNested::QtProtobufFieldEnum;
    EmptyMessage();
    ~EmptyMessage();
    EmptyMessage(const EmptyMessage &other);
    EmptyMessage &operator =(const EmptyMessage &other);
    EmptyMessage(EmptyMessage &&other) noexcept;
    EmptyMessage &operator =(EmptyMessage &&other) noexcept;
    static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT bool comparesEqual(const EmptyMessage &lhs, const EmptyMessage &rhs) noexcept;
    friend bool operator==(const EmptyMessage &lhs, const EmptyMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const EmptyMessage &lhs, const EmptyMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<EmptyMessage_QtProtobufData> dptr;
};
namespace EmptyMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(EmptyMessage)

} // namespace EmptyMessage_QtProtobufNested

class SimpleIntMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT SimpleIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    QML_VALUE_TYPE(simpleIntMessage)
    Q_PROPERTY(QtProtobuf::int32 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleIntMessage();
    ~SimpleIntMessage();
    SimpleIntMessage(const SimpleIntMessage &other);
    SimpleIntMessage &operator =(const SimpleIntMessage &other);
    SimpleIntMessage(SimpleIntMessage &&other) noexcept;
    SimpleIntMessage &operator =(SimpleIntMessage &&other) noexcept;

    QtProtobuf::int32 testFieldInt() const;
    void setTestFieldInt(const QtProtobuf::int32 &testFieldInt);
    static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT bool comparesEqual(const SimpleIntMessage &lhs, const SimpleIntMessage &rhs) noexcept;
    friend bool operator==(const SimpleIntMessage &lhs, const SimpleIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const SimpleIntMessage &lhs, const SimpleIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<SimpleIntMessage_QtProtobufData> dptr;
};
namespace SimpleIntMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleIntMessage)

enum class QtProtobufFieldEnum {
    TestFieldIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleIntMessage_QtProtobufNested

class NoPackageExternalMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT NoPackageExternalMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    QML_VALUE_TYPE(noPackageExternalMessage)
    Q_PROPERTY(SimpleIntMessageExt *testField_p READ testField_p WRITE setTestField_p SCRIPTABLE false)
    Q_PROPERTY(bool hasTestField READ hasTestField)
    Q_PROPERTY(SimpleIntMessageExt testField READ testField WRITE setTestField)

public:
    using QtProtobufFieldEnum = NoPackageExternalMessage_QtProtobufNested::QtProtobufFieldEnum;
    NoPackageExternalMessage();
    ~NoPackageExternalMessage();
    NoPackageExternalMessage(const NoPackageExternalMessage &other);
    NoPackageExternalMessage &operator =(const NoPackageExternalMessage &other);
    NoPackageExternalMessage(NoPackageExternalMessage &&other) noexcept;
    NoPackageExternalMessage &operator =(NoPackageExternalMessage &&other) noexcept;

    bool hasTestField() const;
    SimpleIntMessageExt &testField() const;
    Q_INVOKABLE void clearTestField();
    void setTestField(const SimpleIntMessageExt &testField);
    static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT bool comparesEqual(const NoPackageExternalMessage &lhs, const NoPackageExternalMessage &rhs) noexcept;
    friend bool operator==(const NoPackageExternalMessage &lhs, const NoPackageExternalMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const NoPackageExternalMessage &lhs, const NoPackageExternalMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    SimpleIntMessageExt *testField_p();
    void setTestField_p(SimpleIntMessageExt *testField);
    QExplicitlySharedDataPointer<NoPackageExternalMessage_QtProtobufData> dptr;
};
namespace NoPackageExternalMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(NoPackageExternalMessage)

enum class QtProtobufFieldEnum {
    TestFieldProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace NoPackageExternalMessage_QtProtobufNested

class NoPackageMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT NoPackageMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    QML_VALUE_TYPE(noPackageMessage)
    Q_PROPERTY(SimpleIntMessage *testField_p READ testField_p WRITE setTestField_p SCRIPTABLE false)
    Q_PROPERTY(bool hasTestField READ hasTestField)
    Q_PROPERTY(SimpleIntMessage testField READ testField WRITE setTestField)

public:
    using QtProtobufFieldEnum = NoPackageMessage_QtProtobufNested::QtProtobufFieldEnum;
    NoPackageMessage();
    ~NoPackageMessage();
    NoPackageMessage(const NoPackageMessage &other);
    NoPackageMessage &operator =(const NoPackageMessage &other);
    NoPackageMessage(NoPackageMessage &&other) noexcept;
    NoPackageMessage &operator =(NoPackageMessage &&other) noexcept;

    bool hasTestField() const;
    SimpleIntMessage &testField() const;
    Q_INVOKABLE void clearTestField();
    void setTestField(const SimpleIntMessage &testField);
    static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT bool comparesEqual(const NoPackageMessage &lhs, const NoPackageMessage &rhs) noexcept;
    friend bool operator==(const NoPackageMessage &lhs, const NoPackageMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const NoPackageMessage &lhs, const NoPackageMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    SimpleIntMessage *testField_p();
    void setTestField_p(SimpleIntMessage *testField);
    QExplicitlySharedDataPointer<NoPackageMessage_QtProtobufData> dptr;
};
namespace NoPackageMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_NOPACKAGE_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(NoPackageMessage)

enum class QtProtobufFieldEnum {
    TestFieldProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace NoPackageMessage_QtProtobufNested

Q_DECLARE_METATYPE(EmptyMessage)
Q_DECLARE_METATYPE(SimpleIntMessage)
Q_DECLARE_METATYPE(NoPackageExternalMessage)
Q_DECLARE_METATYPE(NoPackageMessage)
#endif // QPROTOBUF_NOPACKAGE_H
