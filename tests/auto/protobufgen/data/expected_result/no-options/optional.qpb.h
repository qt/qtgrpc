/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_OPTIONAL_H
#define QPROTOBUF_OPTIONAL_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>
#include <QtProtobuf/qprotobufoneof.h>
#include <optional>

#include <QtCore/qmetatype.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>


namespace qtprotobufnamespace::optional::tests {
class TestStringMessage;
using TestStringMessageRepeated = QList<TestStringMessage>;
namespace TestStringMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace TestStringMessage_QtProtobufNested

class OptionalMessage;
using OptionalMessageRepeated = QList<OptionalMessage>;
namespace OptionalMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace OptionalMessage_QtProtobufNested


class TestStringMessage_QtProtobufData;
class TestStringMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QString stringField READ stringField WRITE setStringField SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = TestStringMessage_QtProtobufNested::QtProtobufFieldEnum;
    TestStringMessage();
    ~TestStringMessage();
    TestStringMessage(const TestStringMessage &other);
    TestStringMessage &operator =(const TestStringMessage &other);
    TestStringMessage(TestStringMessage &&other) noexcept;
    TestStringMessage &operator =(TestStringMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(TestStringMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QString stringField() const;
    void setStringField(const QString &stringField);
    static void registerTypes();

private:
    friend  bool comparesEqual(const TestStringMessage &lhs, const TestStringMessage &rhs) noexcept;
    friend bool operator==(const TestStringMessage &lhs, const TestStringMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const TestStringMessage &lhs, const TestStringMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<TestStringMessage_QtProtobufData> dptr;
};
namespace TestStringMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    StringFieldProtoFieldNumber = 2,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace TestStringMessage_QtProtobufNested

class OptionalMessage_QtProtobufData;
class OptionalMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::sint32 testField READ testField WRITE setTestField SCRIPTABLE true)
    Q_PROPERTY(QtProtobuf::sint32 testFieldOpt READ testFieldOpt_p WRITE setTestFieldOpt_p)
    Q_PROPERTY(bool hasTestFieldOpt READ hasTestFieldOpt)
    Q_PROPERTY(bool testFieldBool READ testFieldBool WRITE setTestFieldBool SCRIPTABLE true)
    Q_PROPERTY(bool testFieldBoolOpt READ testFieldBoolOpt_p WRITE setTestFieldBoolOpt_p)
    Q_PROPERTY(bool hasTestFieldBoolOpt READ hasTestFieldBoolOpt)
    Q_PROPERTY(QByteArray testFieldBytes READ testFieldBytes WRITE setTestFieldBytes SCRIPTABLE true)
    Q_PROPERTY(QByteArray testFieldBytesOpt READ testFieldBytesOpt_p WRITE setTestFieldBytesOpt_p)
    Q_PROPERTY(bool hasTestFieldBytesOpt READ hasTestFieldBytesOpt)
    Q_PROPERTY(QString testFieldString READ testFieldString WRITE setTestFieldString SCRIPTABLE true)
    Q_PROPERTY(QString testFieldStringOpt READ testFieldStringOpt_p WRITE setTestFieldStringOpt_p)
    Q_PROPERTY(bool hasTestFieldStringOpt READ hasTestFieldStringOpt)
    Q_PROPERTY(qtprotobufnamespace::optional::tests::TestStringMessage *testFieldMessage_p READ testFieldMessage_p WRITE setTestFieldMessage_p SCRIPTABLE false)
    Q_PROPERTY(bool hasTestFieldMessage READ hasTestFieldMessage)
    Q_PROPERTY(qtprotobufnamespace::optional::tests::TestStringMessage *testFieldMessageOpt_p READ testFieldMessageOpt_p WRITE setTestFieldMessageOpt_p SCRIPTABLE false)
    Q_PROPERTY(bool hasTestFieldMessageOpt READ hasTestFieldMessageOpt)

public:
    using QtProtobufFieldEnum = OptionalMessage_QtProtobufNested::QtProtobufFieldEnum;
    OptionalMessage();
    ~OptionalMessage();
    OptionalMessage(const OptionalMessage &other);
    OptionalMessage &operator =(const OptionalMessage &other);
    OptionalMessage(OptionalMessage &&other) noexcept;
    OptionalMessage &operator =(OptionalMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(OptionalMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::sint32 testField() const;

    bool hasTestFieldOpt() const;
    QtProtobuf::sint32 testFieldOpt() const;

    bool testFieldBool() const;

    bool hasTestFieldBoolOpt() const;
    bool testFieldBoolOpt() const;

    QByteArray testFieldBytes() const;

    bool hasTestFieldBytesOpt() const;
    QByteArray testFieldBytesOpt() const;

    QString testFieldString() const;

    bool hasTestFieldStringOpt() const;
    QString testFieldStringOpt() const;

    bool hasTestFieldMessage() const;
    TestStringMessage &testFieldMessage();
    const TestStringMessage &testFieldMessage() const;
    void clearTestFieldMessage();

    bool hasTestFieldMessageOpt() const;
    TestStringMessage &testFieldMessageOpt();
    const TestStringMessage &testFieldMessageOpt() const;
    void clearTestFieldMessageOpt();
    void setTestField(const QtProtobuf::sint32 &testField);
    void setTestFieldOpt(const QtProtobuf::sint32 &testFieldOpt);
    void clearTestFieldOpt();
    void setTestFieldBool(const bool &testFieldBool);
    void setTestFieldBoolOpt(const bool &testFieldBoolOpt);
    void clearTestFieldBoolOpt();
    void setTestFieldBytes(const QByteArray &testFieldBytes);
    void setTestFieldBytesOpt(const QByteArray &testFieldBytesOpt);
    void clearTestFieldBytesOpt();
    void setTestFieldString(const QString &testFieldString);
    void setTestFieldStringOpt(const QString &testFieldStringOpt);
    void clearTestFieldStringOpt();
    void setTestFieldMessage(const TestStringMessage &testFieldMessage);
    void setTestFieldMessageOpt(const TestStringMessage &testFieldMessageOpt);
    static void registerTypes();

private:
    friend  bool comparesEqual(const OptionalMessage &lhs, const OptionalMessage &rhs) noexcept;
    friend bool operator==(const OptionalMessage &lhs, const OptionalMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const OptionalMessage &lhs, const OptionalMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QtProtobuf::sint32 testFieldOpt_p() const;
    bool testFieldBoolOpt_p() const;
    QByteArray testFieldBytesOpt_p() const;
    QString testFieldStringOpt_p() const;
    TestStringMessage *testFieldMessage_p();
    TestStringMessage *testFieldMessageOpt_p();
    void setTestFieldOpt_p(QtProtobuf::sint32 testFieldOpt);
    void setTestFieldBoolOpt_p(bool testFieldBoolOpt);
    void setTestFieldBytesOpt_p(QByteArray testFieldBytesOpt);
    void setTestFieldStringOpt_p(QString testFieldStringOpt);
    void setTestFieldMessage_p(TestStringMessage *testFieldMessage);
    void setTestFieldMessageOpt_p(TestStringMessage *testFieldMessageOpt);
    QExplicitlySharedDataPointer<OptionalMessage_QtProtobufData> dptr;
};
namespace OptionalMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestFieldProtoFieldNumber = 1,
    TestFieldOptProtoFieldNumber = 2,
    TestFieldBoolProtoFieldNumber = 3,
    TestFieldBoolOptProtoFieldNumber = 4,
    TestFieldBytesProtoFieldNumber = 5,
    TestFieldBytesOptProtoFieldNumber = 6,
    TestFieldStringProtoFieldNumber = 7,
    TestFieldStringOptProtoFieldNumber = 8,
    TestFieldMessageProtoFieldNumber = 9,
    TestFieldMessageOptProtoFieldNumber = 10,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace OptionalMessage_QtProtobufNested
} // namespace qtprotobufnamespace::optional::tests

#endif // QPROTOBUF_OPTIONAL_H
