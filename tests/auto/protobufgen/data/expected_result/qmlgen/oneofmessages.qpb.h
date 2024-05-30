/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_ONEOFMESSAGES_H
#define QPROTOBUF_ONEOFMESSAGES_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtQml/qqmlregistration.h>
#include <QtQml/qqmllist.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>
#include <QtProtobuf/qprotobufoneof.h>
#include "basicmessages.qpb.h"
#include "tst_qtprotobufgen_qml_gen_exports.qpb.h"

#include <QtCore/qmetatype.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>

#include <memory>


namespace qtprotobufnamespace::tests {
class OneofSimpleMessage;
using OneofSimpleMessageRepeated = QList<OneofSimpleMessage>;
namespace OneofSimpleMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
enum class TestOneofFields;
} // namespace OneofSimpleMessage_QtProtobufNested

class OneofComplexMessage;
using OneofComplexMessageRepeated = QList<OneofComplexMessage>;
namespace OneofComplexMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
enum class TestOneofFields;
enum class SecondOneofFields;
} // namespace OneofComplexMessage_QtProtobufNested


class OneofSimpleMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT OneofSimpleMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    QML_VALUE_TYPE(oneofSimpleMessage)
    Q_PROPERTY(QtProtobuf::int32 testOneofFieldInt READ testOneofFieldInt_p WRITE setTestOneofFieldInt_p)
    Q_PROPERTY(bool hasTestOneofFieldInt READ hasTestOneofFieldInt)
    Q_PROPERTY(QtProtobuf::int32 testOneofFieldSecondInt READ testOneofFieldSecondInt_p WRITE setTestOneofFieldSecondInt_p)
    Q_PROPERTY(bool hasTestOneofFieldSecondInt READ hasTestOneofFieldSecondInt)

public:
    using QtProtobufFieldEnum = OneofSimpleMessage_QtProtobufNested::QtProtobufFieldEnum;
    using TestOneofFields = OneofSimpleMessage_QtProtobufNested::TestOneofFields;
    OneofSimpleMessage();
    ~OneofSimpleMessage();
    OneofSimpleMessage(const OneofSimpleMessage &other);
    OneofSimpleMessage &operator =(const OneofSimpleMessage &other);
    OneofSimpleMessage(OneofSimpleMessage &&other) noexcept;
    OneofSimpleMessage &operator =(OneofSimpleMessage &&other) noexcept;

    bool hasTestOneofFieldInt() const;
    QtProtobuf::int32 testOneofFieldInt() const;

    bool hasTestOneofFieldSecondInt() const;
    QtProtobuf::int32 testOneofFieldSecondInt() const;
    TestOneofFields testOneofField() const;
    void setTestOneofFieldInt(const QtProtobuf::int32 &testOneofFieldInt);
    void setTestOneofFieldSecondInt(const QtProtobuf::int32 &testOneofFieldSecondInt);
    Q_INVOKABLE void clearTestOneof();
    static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT bool comparesEqual(const OneofSimpleMessage &lhs, const OneofSimpleMessage &rhs) noexcept;
    friend bool operator==(const OneofSimpleMessage &lhs, const OneofSimpleMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const OneofSimpleMessage &lhs, const OneofSimpleMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QtProtobuf::int32 testOneofFieldInt_p() const;
    QtProtobuf::int32 testOneofFieldSecondInt_p() const;
    void setTestOneofFieldInt_p(QtProtobuf::int32 testOneofFieldInt);
    void setTestOneofFieldSecondInt_p(QtProtobuf::int32 testOneofFieldSecondInt);
    QExplicitlySharedDataPointer<OneofSimpleMessage_QtProtobufData> dptr;
};
namespace OneofSimpleMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(OneofSimpleMessage)

enum class QtProtobufFieldEnum {
    TestOneofFieldIntProtoFieldNumber = 1,
    TestOneofFieldSecondIntProtoFieldNumber = 2,
};
Q_ENUM_NS(QtProtobufFieldEnum)

enum class TestOneofFields {
    UninitializedField = QtProtobuf::InvalidFieldNumber,
    TestOneofFieldInt = 1,
    TestOneofFieldSecondInt = 2,
};
Q_ENUM_NS(TestOneofFields)

} // namespace OneofSimpleMessage_QtProtobufNested

class OneofComplexMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT OneofComplexMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    QML_VALUE_TYPE(oneofComplexMessage)
    Q_PROPERTY(QtProtobuf::int32 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE true)
    Q_PROPERTY(QtProtobuf::int32 testOneofFieldInt READ testOneofFieldInt_p WRITE setTestOneofFieldInt_p)
    Q_PROPERTY(bool hasTestOneofFieldInt READ hasTestOneofFieldInt)
    Q_PROPERTY(qtprotobufnamespace::tests::ComplexMessage *testOneofComplexField_p READ testOneofComplexField_p WRITE setTestOneofComplexField_p SCRIPTABLE false)
    Q_PROPERTY(bool hasTestOneofComplexField READ hasTestOneofComplexField)
    Q_PROPERTY(qtprotobufnamespace::tests::ComplexMessage *testOneofSecondComplexField_p READ testOneofSecondComplexField_p WRITE setTestOneofSecondComplexField_p SCRIPTABLE false)
    Q_PROPERTY(bool hasTestOneofSecondComplexField READ hasTestOneofSecondComplexField)
    Q_PROPERTY(QtProtobuf::int32 secondFieldInt READ secondFieldInt_p WRITE setSecondFieldInt_p)
    Q_PROPERTY(bool hasSecondFieldInt READ hasSecondFieldInt)
    Q_PROPERTY(qtprotobufnamespace::tests::ComplexMessage *secondComplexField_p READ secondComplexField_p WRITE setSecondComplexField_p SCRIPTABLE false)
    Q_PROPERTY(bool hasSecondComplexField READ hasSecondComplexField)
    Q_PROPERTY(qtprotobufnamespace::tests::ComplexMessage *secondSecondComplexField_p READ secondSecondComplexField_p WRITE setSecondSecondComplexField_p SCRIPTABLE false)
    Q_PROPERTY(bool hasSecondSecondComplexField READ hasSecondSecondComplexField)
    Q_PROPERTY(QtProtobuf::int32 testSnakeCaseField READ testSnakeCaseField_p WRITE setTestSnakeCaseField_p)
    Q_PROPERTY(bool hasTestSnakeCaseField READ hasTestSnakeCaseField)
    Q_PROPERTY(qtprotobufnamespace::tests::ComplexMessage testOneofComplexField READ testOneofComplexField WRITE setTestOneofComplexField)
    Q_PROPERTY(qtprotobufnamespace::tests::ComplexMessage testOneofSecondComplexField READ testOneofSecondComplexField WRITE setTestOneofSecondComplexField)
    Q_PROPERTY(qtprotobufnamespace::tests::ComplexMessage secondComplexField READ secondComplexField WRITE setSecondComplexField)
    Q_PROPERTY(qtprotobufnamespace::tests::ComplexMessage secondSecondComplexField READ secondSecondComplexField WRITE setSecondSecondComplexField)

public:
    using QtProtobufFieldEnum = OneofComplexMessage_QtProtobufNested::QtProtobufFieldEnum;
    using TestOneofFields = OneofComplexMessage_QtProtobufNested::TestOneofFields;
    using SecondOneofFields = OneofComplexMessage_QtProtobufNested::SecondOneofFields;
    OneofComplexMessage();
    ~OneofComplexMessage();
    OneofComplexMessage(const OneofComplexMessage &other);
    OneofComplexMessage &operator =(const OneofComplexMessage &other);
    OneofComplexMessage(OneofComplexMessage &&other) noexcept;
    OneofComplexMessage &operator =(OneofComplexMessage &&other) noexcept;

    QtProtobuf::int32 testFieldInt() const;

    bool hasTestOneofFieldInt() const;
    QtProtobuf::int32 testOneofFieldInt() const;

    bool hasTestOneofComplexField() const;
    ComplexMessage &testOneofComplexField() const;

    bool hasTestOneofSecondComplexField() const;
    ComplexMessage &testOneofSecondComplexField() const;

    bool hasSecondFieldInt() const;
    QtProtobuf::int32 secondFieldInt() const;

    bool hasSecondComplexField() const;
    ComplexMessage &secondComplexField() const;

    bool hasSecondSecondComplexField() const;
    ComplexMessage &secondSecondComplexField() const;

    bool hasTestSnakeCaseField() const;
    QtProtobuf::int32 testSnakeCaseField() const;
    TestOneofFields testOneofField() const;
    SecondOneofFields secondOneofField() const;
    void setTestFieldInt(const QtProtobuf::int32 &testFieldInt);
    void setTestOneofFieldInt(const QtProtobuf::int32 &testOneofFieldInt);
    void setTestOneofComplexField(const ComplexMessage &testOneofComplexField);
    void setTestOneofSecondComplexField(const ComplexMessage &testOneofSecondComplexField);
    void setSecondFieldInt(const QtProtobuf::int32 &secondFieldInt);
    void setSecondComplexField(const ComplexMessage &secondComplexField);
    void setSecondSecondComplexField(const ComplexMessage &secondSecondComplexField);
    void setTestSnakeCaseField(const QtProtobuf::int32 &testSnakeCaseField);
    Q_INVOKABLE void clearTestOneof();
    Q_INVOKABLE void clearSecondOneof();
    static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT bool comparesEqual(const OneofComplexMessage &lhs, const OneofComplexMessage &rhs) noexcept;
    friend bool operator==(const OneofComplexMessage &lhs, const OneofComplexMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const OneofComplexMessage &lhs, const OneofComplexMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QtProtobuf::int32 testOneofFieldInt_p() const;
    ComplexMessage *testOneofComplexField_p();
    ComplexMessage *testOneofSecondComplexField_p();
    QtProtobuf::int32 secondFieldInt_p() const;
    ComplexMessage *secondComplexField_p();
    ComplexMessage *secondSecondComplexField_p();
    QtProtobuf::int32 testSnakeCaseField_p() const;
    void setTestOneofFieldInt_p(QtProtobuf::int32 testOneofFieldInt);
    void setTestOneofComplexField_p(ComplexMessage *testOneofComplexField);
    void setTestOneofSecondComplexField_p(ComplexMessage *testOneofSecondComplexField);
    void setSecondFieldInt_p(QtProtobuf::int32 secondFieldInt);
    void setSecondComplexField_p(ComplexMessage *secondComplexField);
    void setSecondSecondComplexField_p(ComplexMessage *secondSecondComplexField);
    void setTestSnakeCaseField_p(QtProtobuf::int32 testSnakeCaseField);
    QExplicitlySharedDataPointer<OneofComplexMessage_QtProtobufData> dptr;
};
namespace OneofComplexMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(OneofComplexMessage)

enum class QtProtobufFieldEnum {
    TestFieldIntProtoFieldNumber = 1,
    TestOneofFieldIntProtoFieldNumber = 42,
    TestOneofComplexFieldProtoFieldNumber = 3,
    TestOneofSecondComplexFieldProtoFieldNumber = 4,
    SecondFieldIntProtoFieldNumber = 43,
    SecondComplexFieldProtoFieldNumber = 5,
    SecondSecondComplexFieldProtoFieldNumber = 6,
    TestSnakeCaseFieldProtoFieldNumber = 7,
};
Q_ENUM_NS(QtProtobufFieldEnum)

enum class TestOneofFields {
    UninitializedField = QtProtobuf::InvalidFieldNumber,
    TestOneofFieldInt = 42,
    TestOneofComplexField = 3,
    TestOneofSecondComplexField = 4,
};
Q_ENUM_NS(TestOneofFields)

enum class SecondOneofFields {
    UninitializedField = QtProtobuf::InvalidFieldNumber,
    SecondFieldInt = 43,
    SecondComplexField = 5,
    SecondSecondComplexField = 6,
    TestSnakeCaseField = 7,
};
Q_ENUM_NS(SecondOneofFields)

} // namespace OneofComplexMessage_QtProtobufNested
} // namespace qtprotobufnamespace::tests

Q_DECLARE_METATYPE(qtprotobufnamespace::tests::OneofSimpleMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::OneofComplexMessage)
#endif // QPROTOBUF_ONEOFMESSAGES_H
