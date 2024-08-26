/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef ANYMESSAGES_QPB_H
#define ANYMESSAGES_QPB_H

#include <QtProtobuf/qprotobuflazymessagepointer.h>
#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtProtobufWellKnownTypes/qprotobufanysupport.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qlist.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>

namespace qtproto::tests {
class AnyMessage;
using AnyMessageRepeated = QList<AnyMessage>;
namespace AnyMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace AnyMessage_QtProtobufNested

class RepeatedAnyMessage;
using RepeatedAnyMessageRepeated = QList<RepeatedAnyMessage>;
namespace RepeatedAnyMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedAnyMessage_QtProtobufNested

class TwoAnyMessage;
using TwoAnyMessageRepeated = QList<TwoAnyMessage>;
namespace TwoAnyMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace TwoAnyMessage_QtProtobufNested

class Example;
using ExampleRepeated = QList<Example>;
namespace Example_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace Example_QtProtobufNested

class SimpleMessage;
using SimpleMessageRepeated = QList<SimpleMessage>;
namespace SimpleMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleMessage_QtProtobufNested


class AnyMessage_QtProtobufData;
class AnyMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::Any field READ field WRITE setField SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = AnyMessage_QtProtobufNested::QtProtobufFieldEnum;
    AnyMessage();
    ~AnyMessage();
    AnyMessage(const AnyMessage &other);
    AnyMessage &operator =(const AnyMessage &other);
    AnyMessage(AnyMessage &&other) noexcept;
    AnyMessage &operator =(AnyMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(AnyMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::Any field() const;
    void setField(const QtProtobuf::Any &field);
    void setField(QtProtobuf::Any &&field);
    static void registerTypes();

private:
    friend bool comparesEqual(const AnyMessage &lhs, const AnyMessage &rhs) noexcept;
    friend bool operator==(const AnyMessage &lhs, const AnyMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const AnyMessage &lhs, const AnyMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<AnyMessage_QtProtobufData> dptr;
};
namespace AnyMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    FieldProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace AnyMessage_QtProtobufNested

class RepeatedAnyMessage_QtProtobufData;
class RepeatedAnyMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QList<QtProtobuf::Any> anysData READ anys WRITE setAnys SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedAnyMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedAnyMessage();
    ~RepeatedAnyMessage();
    RepeatedAnyMessage(const RepeatedAnyMessage &other);
    RepeatedAnyMessage &operator =(const RepeatedAnyMessage &other);
    RepeatedAnyMessage(RepeatedAnyMessage &&other) noexcept;
    RepeatedAnyMessage &operator =(RepeatedAnyMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedAnyMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QList<QtProtobuf::Any> anys() const;
    void setAnys(const QList<QtProtobuf::Any> &anys);
    void setAnys(QList<QtProtobuf::Any> &&anys);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedAnyMessage &lhs, const RepeatedAnyMessage &rhs) noexcept;
    friend bool operator==(const RepeatedAnyMessage &lhs, const RepeatedAnyMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedAnyMessage &lhs, const RepeatedAnyMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedAnyMessage_QtProtobufData> dptr;
};
namespace RepeatedAnyMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    AnysProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedAnyMessage_QtProtobufNested

class TwoAnyMessage_QtProtobufData;
class TwoAnyMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::Any one READ one WRITE setOne SCRIPTABLE true)
    Q_PROPERTY(QtProtobuf::Any two READ two WRITE setTwo SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = TwoAnyMessage_QtProtobufNested::QtProtobufFieldEnum;
    TwoAnyMessage();
    ~TwoAnyMessage();
    TwoAnyMessage(const TwoAnyMessage &other);
    TwoAnyMessage &operator =(const TwoAnyMessage &other);
    TwoAnyMessage(TwoAnyMessage &&other) noexcept;
    TwoAnyMessage &operator =(TwoAnyMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(TwoAnyMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::Any one() const;

    QtProtobuf::Any two() const;
    void setOne(const QtProtobuf::Any &one);
    void setOne(QtProtobuf::Any &&one);
    void setTwo(const QtProtobuf::Any &two);
    void setTwo(QtProtobuf::Any &&two);
    static void registerTypes();

private:
    friend bool comparesEqual(const TwoAnyMessage &lhs, const TwoAnyMessage &rhs) noexcept;
    friend bool operator==(const TwoAnyMessage &lhs, const TwoAnyMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const TwoAnyMessage &lhs, const TwoAnyMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<TwoAnyMessage_QtProtobufData> dptr;
};
namespace TwoAnyMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    OneProtoFieldNumber = 1,
    TwoProtoFieldNumber = 2,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace TwoAnyMessage_QtProtobufNested

class Example_QtProtobufData;
class Example : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QString str READ str WRITE setStr SCRIPTABLE true)
    Q_PROPERTY(QtProtobuf::sint32 i READ i WRITE setI SCRIPTABLE true)
    Q_PROPERTY(QtProtobuf::sint32 j READ j WRITE setJ SCRIPTABLE true)
    Q_PROPERTY(QtProtobuf::sint32 h READ h WRITE setH SCRIPTABLE true)
    Q_PROPERTY(QString str2 READ str2 WRITE setStr2 SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = Example_QtProtobufNested::QtProtobufFieldEnum;
    Example();
    ~Example();
    Example(const Example &other);
    Example &operator =(const Example &other);
    Example(Example &&other) noexcept;
    Example &operator =(Example &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(Example &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QString str() const;

    QtProtobuf::sint32 i() const;

    QtProtobuf::sint32 j() const;

    QtProtobuf::sint32 h() const;

    QString str2() const;
    void setStr(const QString &str);
    void setStr(QString &&str);
    void setI(QtProtobuf::sint32 i);
    void setJ(QtProtobuf::sint32 j);
    void setH(QtProtobuf::sint32 h);
    void setStr2(const QString &str2);
    void setStr2(QString &&str2);
    static void registerTypes();

private:
    friend bool comparesEqual(const Example &lhs, const Example &rhs) noexcept;
    friend bool operator==(const Example &lhs, const Example &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const Example &lhs, const Example &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<Example_QtProtobufData> dptr;
};
namespace Example_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    StrProtoFieldNumber = 1,
    IProtoFieldNumber = 2,
    JProtoFieldNumber = 3,
    HProtoFieldNumber = 4,
    Str2ProtoFieldNumber = 5,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace Example_QtProtobufNested

class SimpleMessage_QtProtobufData;
class SimpleMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::int32 i READ i WRITE setI SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleMessage();
    ~SimpleMessage();
    SimpleMessage(const SimpleMessage &other);
    SimpleMessage &operator =(const SimpleMessage &other);
    SimpleMessage(SimpleMessage &&other) noexcept;
    SimpleMessage &operator =(SimpleMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(SimpleMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::int32 i() const;
    void setI(QtProtobuf::int32 i);
    static void registerTypes();

private:
    friend bool comparesEqual(const SimpleMessage &lhs, const SimpleMessage &rhs) noexcept;
    friend bool operator==(const SimpleMessage &lhs, const SimpleMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const SimpleMessage &lhs, const SimpleMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<SimpleMessage_QtProtobufData> dptr;
};
namespace SimpleMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    IProtoFieldNumber = 500000,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleMessage_QtProtobufNested
} // namespace qtproto::tests

#endif // ANYMESSAGES_QPB_H
