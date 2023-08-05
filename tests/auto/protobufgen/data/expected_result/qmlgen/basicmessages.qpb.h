/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_BASICMESSAGES_H
#define QPROTOBUF_BASICMESSAGES_H

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
class EmptyMessage;
using EmptyMessageRepeated = QList<EmptyMessage>;
namespace EmptyMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace EmptyMessage_QtProtobufNested

class SimpleBoolMessage;
using SimpleBoolMessageRepeated = QList<SimpleBoolMessage>;
namespace SimpleBoolMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleBoolMessage_QtProtobufNested

class SimpleIntMessage;
using SimpleIntMessageRepeated = QList<SimpleIntMessage>;
namespace SimpleIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleIntMessage_QtProtobufNested

class SimpleSIntMessage;
using SimpleSIntMessageRepeated = QList<SimpleSIntMessage>;
namespace SimpleSIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleSIntMessage_QtProtobufNested

class SimpleUIntMessage;
using SimpleUIntMessageRepeated = QList<SimpleUIntMessage>;
namespace SimpleUIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleUIntMessage_QtProtobufNested

class SimpleInt64Message;
using SimpleInt64MessageRepeated = QList<SimpleInt64Message>;
namespace SimpleInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleInt64Message_QtProtobufNested

class SimpleSInt64Message;
using SimpleSInt64MessageRepeated = QList<SimpleSInt64Message>;
namespace SimpleSInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleSInt64Message_QtProtobufNested

class SimpleUInt64Message;
using SimpleUInt64MessageRepeated = QList<SimpleUInt64Message>;
namespace SimpleUInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleUInt64Message_QtProtobufNested

class SimpleStringMessage;
using SimpleStringMessageRepeated = QList<SimpleStringMessage>;
namespace SimpleStringMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleStringMessage_QtProtobufNested

class SimpleFloatMessage;
using SimpleFloatMessageRepeated = QList<SimpleFloatMessage>;
namespace SimpleFloatMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleFloatMessage_QtProtobufNested

class SimpleDoubleMessage;
using SimpleDoubleMessageRepeated = QList<SimpleDoubleMessage>;
namespace SimpleDoubleMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleDoubleMessage_QtProtobufNested

class SimpleBytesMessage;
using SimpleBytesMessageRepeated = QList<SimpleBytesMessage>;
namespace SimpleBytesMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleBytesMessage_QtProtobufNested

class SimpleFixedInt32Message;
using SimpleFixedInt32MessageRepeated = QList<SimpleFixedInt32Message>;
namespace SimpleFixedInt32Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleFixedInt32Message_QtProtobufNested

class SimpleFixedInt64Message;
using SimpleFixedInt64MessageRepeated = QList<SimpleFixedInt64Message>;
namespace SimpleFixedInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleFixedInt64Message_QtProtobufNested

class SimpleSFixedInt32Message;
using SimpleSFixedInt32MessageRepeated = QList<SimpleSFixedInt32Message>;
namespace SimpleSFixedInt32Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleSFixedInt32Message_QtProtobufNested

class SimpleSFixedInt64Message;
using SimpleSFixedInt64MessageRepeated = QList<SimpleSFixedInt64Message>;
namespace SimpleSFixedInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace SimpleSFixedInt64Message_QtProtobufNested

class ComplexMessage;
using ComplexMessageRepeated = QList<ComplexMessage>;
namespace ComplexMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace ComplexMessage_QtProtobufNested


class EmptyMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT EmptyMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(EmptyMessage)
    QML_VALUE_TYPE(emptyMessage)

public:
    using QtProtobufFieldEnum = EmptyMessage_QtProtobufNested::QtProtobufFieldEnum;
    EmptyMessage();
    ~EmptyMessage();
    EmptyMessage(const EmptyMessage &other);
    EmptyMessage &operator =(const EmptyMessage &other);
    EmptyMessage(EmptyMessage &&other) noexcept;
    EmptyMessage &operator =(EmptyMessage &&other) noexcept;
    bool operator ==(const EmptyMessage &other) const;
    bool operator !=(const EmptyMessage &other) const;
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<EmptyMessage_QtProtobufData> dptr;
};
namespace EmptyMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(EmptyMessage)

} // namespace EmptyMessage_QtProtobufNested

class SimpleBoolMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleBoolMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleBoolMessage)
    QML_VALUE_TYPE(simpleBoolMessage)
    Q_PROPERTY(bool testFieldBool READ testFieldBool WRITE setTestFieldBool SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleBoolMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleBoolMessage();
    ~SimpleBoolMessage();
    SimpleBoolMessage(const SimpleBoolMessage &other);
    SimpleBoolMessage &operator =(const SimpleBoolMessage &other);
    SimpleBoolMessage(SimpleBoolMessage &&other) noexcept;
    SimpleBoolMessage &operator =(SimpleBoolMessage &&other) noexcept;
    bool operator ==(const SimpleBoolMessage &other) const;
    bool operator !=(const SimpleBoolMessage &other) const;

    bool testFieldBool() const;
    void setTestFieldBool(const bool &testFieldBool);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleBoolMessage_QtProtobufData> dptr;
};
namespace SimpleBoolMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleBoolMessage)

enum class QtProtobufFieldEnum {
    TestFieldBoolProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleBoolMessage_QtProtobufNested

class SimpleIntMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleIntMessage)
    QML_VALUE_TYPE(simpleIntMessage)
    Q_PROPERTY(QtProtobuf::int32 testFieldInt_p READ testFieldInt WRITE setTestFieldInt SCRIPTABLE false)
    Q_PROPERTY(int testFieldInt READ testFieldInt_p WRITE setTestFieldInt_p SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleIntMessage();
    ~SimpleIntMessage();
    SimpleIntMessage(const SimpleIntMessage &other);
    SimpleIntMessage &operator =(const SimpleIntMessage &other);
    SimpleIntMessage(SimpleIntMessage &&other) noexcept;
    SimpleIntMessage &operator =(SimpleIntMessage &&other) noexcept;
    bool operator ==(const SimpleIntMessage &other) const;
    bool operator !=(const SimpleIntMessage &other) const;

    QtProtobuf::int32 testFieldInt() const;
    void setTestFieldInt(const QtProtobuf::int32 &testFieldInt);
    static void registerTypes();

private:
    int testFieldInt_p() const;
    void setTestFieldInt_p(const int &testFieldInt);
    QExplicitlySharedDataPointer<SimpleIntMessage_QtProtobufData> dptr;
};
namespace SimpleIntMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleIntMessage)

enum class QtProtobufFieldEnum {
    TestFieldIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleIntMessage_QtProtobufNested

class SimpleSIntMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleSIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleSIntMessage)
    QML_VALUE_TYPE(simpleSIntMessage)
    Q_PROPERTY(QtProtobuf::sint32 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleSIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleSIntMessage();
    ~SimpleSIntMessage();
    SimpleSIntMessage(const SimpleSIntMessage &other);
    SimpleSIntMessage &operator =(const SimpleSIntMessage &other);
    SimpleSIntMessage(SimpleSIntMessage &&other) noexcept;
    SimpleSIntMessage &operator =(SimpleSIntMessage &&other) noexcept;
    bool operator ==(const SimpleSIntMessage &other) const;
    bool operator !=(const SimpleSIntMessage &other) const;

    QtProtobuf::sint32 testFieldInt() const;
    void setTestFieldInt(const QtProtobuf::sint32 &testFieldInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleSIntMessage_QtProtobufData> dptr;
};
namespace SimpleSIntMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleSIntMessage)

enum class QtProtobufFieldEnum {
    TestFieldIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleSIntMessage_QtProtobufNested

class SimpleUIntMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleUIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleUIntMessage)
    QML_VALUE_TYPE(simpleUIntMessage)
    Q_PROPERTY(QtProtobuf::uint32 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleUIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleUIntMessage();
    ~SimpleUIntMessage();
    SimpleUIntMessage(const SimpleUIntMessage &other);
    SimpleUIntMessage &operator =(const SimpleUIntMessage &other);
    SimpleUIntMessage(SimpleUIntMessage &&other) noexcept;
    SimpleUIntMessage &operator =(SimpleUIntMessage &&other) noexcept;
    bool operator ==(const SimpleUIntMessage &other) const;
    bool operator !=(const SimpleUIntMessage &other) const;

    QtProtobuf::uint32 testFieldInt() const;
    void setTestFieldInt(const QtProtobuf::uint32 &testFieldInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleUIntMessage_QtProtobufData> dptr;
};
namespace SimpleUIntMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleUIntMessage)

enum class QtProtobufFieldEnum {
    TestFieldIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleUIntMessage_QtProtobufNested

class SimpleInt64Message_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleInt64Message)
    QML_VALUE_TYPE(simpleInt64Message)
    Q_PROPERTY(QtProtobuf::int64 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE false)

public:
    using QtProtobufFieldEnum = SimpleInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    SimpleInt64Message();
    ~SimpleInt64Message();
    SimpleInt64Message(const SimpleInt64Message &other);
    SimpleInt64Message &operator =(const SimpleInt64Message &other);
    SimpleInt64Message(SimpleInt64Message &&other) noexcept;
    SimpleInt64Message &operator =(SimpleInt64Message &&other) noexcept;
    bool operator ==(const SimpleInt64Message &other) const;
    bool operator !=(const SimpleInt64Message &other) const;

    QtProtobuf::int64 testFieldInt() const;
    void setTestFieldInt(const QtProtobuf::int64 &testFieldInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleInt64Message_QtProtobufData> dptr;
};
namespace SimpleInt64Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleInt64Message)

enum class QtProtobufFieldEnum {
    TestFieldIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleInt64Message_QtProtobufNested

class SimpleSInt64Message_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleSInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleSInt64Message)
    QML_VALUE_TYPE(simpleSInt64Message)
    Q_PROPERTY(QtProtobuf::sint64 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE false)

public:
    using QtProtobufFieldEnum = SimpleSInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    SimpleSInt64Message();
    ~SimpleSInt64Message();
    SimpleSInt64Message(const SimpleSInt64Message &other);
    SimpleSInt64Message &operator =(const SimpleSInt64Message &other);
    SimpleSInt64Message(SimpleSInt64Message &&other) noexcept;
    SimpleSInt64Message &operator =(SimpleSInt64Message &&other) noexcept;
    bool operator ==(const SimpleSInt64Message &other) const;
    bool operator !=(const SimpleSInt64Message &other) const;

    QtProtobuf::sint64 testFieldInt() const;
    void setTestFieldInt(const QtProtobuf::sint64 &testFieldInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleSInt64Message_QtProtobufData> dptr;
};
namespace SimpleSInt64Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleSInt64Message)

enum class QtProtobufFieldEnum {
    TestFieldIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleSInt64Message_QtProtobufNested

class SimpleUInt64Message_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleUInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleUInt64Message)
    QML_VALUE_TYPE(simpleUInt64Message)
    Q_PROPERTY(QtProtobuf::uint64 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleUInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    SimpleUInt64Message();
    ~SimpleUInt64Message();
    SimpleUInt64Message(const SimpleUInt64Message &other);
    SimpleUInt64Message &operator =(const SimpleUInt64Message &other);
    SimpleUInt64Message(SimpleUInt64Message &&other) noexcept;
    SimpleUInt64Message &operator =(SimpleUInt64Message &&other) noexcept;
    bool operator ==(const SimpleUInt64Message &other) const;
    bool operator !=(const SimpleUInt64Message &other) const;

    QtProtobuf::uint64 testFieldInt() const;
    void setTestFieldInt(const QtProtobuf::uint64 &testFieldInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleUInt64Message_QtProtobufData> dptr;
};
namespace SimpleUInt64Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleUInt64Message)

enum class QtProtobufFieldEnum {
    TestFieldIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleUInt64Message_QtProtobufNested

class SimpleStringMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleStringMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleStringMessage)
    QML_VALUE_TYPE(simpleStringMessage)
    Q_PROPERTY(QString testFieldString READ testFieldString WRITE setTestFieldString SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleStringMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleStringMessage();
    ~SimpleStringMessage();
    SimpleStringMessage(const SimpleStringMessage &other);
    SimpleStringMessage &operator =(const SimpleStringMessage &other);
    SimpleStringMessage(SimpleStringMessage &&other) noexcept;
    SimpleStringMessage &operator =(SimpleStringMessage &&other) noexcept;
    bool operator ==(const SimpleStringMessage &other) const;
    bool operator !=(const SimpleStringMessage &other) const;

    QString testFieldString() const;
    void setTestFieldString(const QString &testFieldString);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleStringMessage_QtProtobufData> dptr;
};
namespace SimpleStringMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleStringMessage)

enum class QtProtobufFieldEnum {
    TestFieldStringProtoFieldNumber = 6,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleStringMessage_QtProtobufNested

class SimpleFloatMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleFloatMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleFloatMessage)
    QML_VALUE_TYPE(simpleFloatMessage)
    Q_PROPERTY(float testFieldFloat READ testFieldFloat WRITE setTestFieldFloat SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleFloatMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleFloatMessage();
    ~SimpleFloatMessage();
    SimpleFloatMessage(const SimpleFloatMessage &other);
    SimpleFloatMessage &operator =(const SimpleFloatMessage &other);
    SimpleFloatMessage(SimpleFloatMessage &&other) noexcept;
    SimpleFloatMessage &operator =(SimpleFloatMessage &&other) noexcept;
    bool operator ==(const SimpleFloatMessage &other) const;
    bool operator !=(const SimpleFloatMessage &other) const;

    float testFieldFloat() const;
    void setTestFieldFloat(const float &testFieldFloat);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleFloatMessage_QtProtobufData> dptr;
};
namespace SimpleFloatMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleFloatMessage)

enum class QtProtobufFieldEnum {
    TestFieldFloatProtoFieldNumber = 7,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleFloatMessage_QtProtobufNested

class SimpleDoubleMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleDoubleMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleDoubleMessage)
    QML_VALUE_TYPE(simpleDoubleMessage)
    Q_PROPERTY(double testFieldDouble READ testFieldDouble WRITE setTestFieldDouble SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleDoubleMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleDoubleMessage();
    ~SimpleDoubleMessage();
    SimpleDoubleMessage(const SimpleDoubleMessage &other);
    SimpleDoubleMessage &operator =(const SimpleDoubleMessage &other);
    SimpleDoubleMessage(SimpleDoubleMessage &&other) noexcept;
    SimpleDoubleMessage &operator =(SimpleDoubleMessage &&other) noexcept;
    bool operator ==(const SimpleDoubleMessage &other) const;
    bool operator !=(const SimpleDoubleMessage &other) const;

    double testFieldDouble() const;
    void setTestFieldDouble(const double &testFieldDouble);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleDoubleMessage_QtProtobufData> dptr;
};
namespace SimpleDoubleMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleDoubleMessage)

enum class QtProtobufFieldEnum {
    TestFieldDoubleProtoFieldNumber = 8,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleDoubleMessage_QtProtobufNested

class SimpleBytesMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleBytesMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleBytesMessage)
    QML_VALUE_TYPE(simpleBytesMessage)
    Q_PROPERTY(QByteArray testFieldBytes READ testFieldBytes WRITE setTestFieldBytes SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleBytesMessage_QtProtobufNested::QtProtobufFieldEnum;
    SimpleBytesMessage();
    ~SimpleBytesMessage();
    SimpleBytesMessage(const SimpleBytesMessage &other);
    SimpleBytesMessage &operator =(const SimpleBytesMessage &other);
    SimpleBytesMessage(SimpleBytesMessage &&other) noexcept;
    SimpleBytesMessage &operator =(SimpleBytesMessage &&other) noexcept;
    bool operator ==(const SimpleBytesMessage &other) const;
    bool operator !=(const SimpleBytesMessage &other) const;

    QByteArray testFieldBytes() const;
    void setTestFieldBytes(const QByteArray &testFieldBytes);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleBytesMessage_QtProtobufData> dptr;
};
namespace SimpleBytesMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleBytesMessage)

enum class QtProtobufFieldEnum {
    TestFieldBytesProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleBytesMessage_QtProtobufNested

class SimpleFixedInt32Message_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleFixedInt32Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleFixedInt32Message)
    QML_VALUE_TYPE(simpleFixedInt32Message)
    Q_PROPERTY(QtProtobuf::fixed32 testFieldFixedInt32_p READ testFieldFixedInt32 WRITE setTestFieldFixedInt32 SCRIPTABLE false)
    Q_PROPERTY(unsigned int testFieldFixedInt32 READ testFieldFixedInt32_p WRITE setTestFieldFixedInt32_p SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleFixedInt32Message_QtProtobufNested::QtProtobufFieldEnum;
    SimpleFixedInt32Message();
    ~SimpleFixedInt32Message();
    SimpleFixedInt32Message(const SimpleFixedInt32Message &other);
    SimpleFixedInt32Message &operator =(const SimpleFixedInt32Message &other);
    SimpleFixedInt32Message(SimpleFixedInt32Message &&other) noexcept;
    SimpleFixedInt32Message &operator =(SimpleFixedInt32Message &&other) noexcept;
    bool operator ==(const SimpleFixedInt32Message &other) const;
    bool operator !=(const SimpleFixedInt32Message &other) const;

    QtProtobuf::fixed32 testFieldFixedInt32() const;
    void setTestFieldFixedInt32(const QtProtobuf::fixed32 &testFieldFixedInt32);
    static void registerTypes();

private:
    unsigned int testFieldFixedInt32_p() const;
    void setTestFieldFixedInt32_p(const unsigned int &testFieldFixedInt32);
    QExplicitlySharedDataPointer<SimpleFixedInt32Message_QtProtobufData> dptr;
};
namespace SimpleFixedInt32Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleFixedInt32Message)

enum class QtProtobufFieldEnum {
    TestFieldFixedInt32ProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleFixedInt32Message_QtProtobufNested

class SimpleFixedInt64Message_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleFixedInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleFixedInt64Message)
    QML_VALUE_TYPE(simpleFixedInt64Message)
    Q_PROPERTY(QtProtobuf::fixed64 testFieldFixedInt64 READ testFieldFixedInt64 WRITE setTestFieldFixedInt64 SCRIPTABLE false)

public:
    using QtProtobufFieldEnum = SimpleFixedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    SimpleFixedInt64Message();
    ~SimpleFixedInt64Message();
    SimpleFixedInt64Message(const SimpleFixedInt64Message &other);
    SimpleFixedInt64Message &operator =(const SimpleFixedInt64Message &other);
    SimpleFixedInt64Message(SimpleFixedInt64Message &&other) noexcept;
    SimpleFixedInt64Message &operator =(SimpleFixedInt64Message &&other) noexcept;
    bool operator ==(const SimpleFixedInt64Message &other) const;
    bool operator !=(const SimpleFixedInt64Message &other) const;

    QtProtobuf::fixed64 testFieldFixedInt64() const;
    void setTestFieldFixedInt64(const QtProtobuf::fixed64 &testFieldFixedInt64);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleFixedInt64Message_QtProtobufData> dptr;
};
namespace SimpleFixedInt64Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleFixedInt64Message)

enum class QtProtobufFieldEnum {
    TestFieldFixedInt64ProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleFixedInt64Message_QtProtobufNested

class SimpleSFixedInt32Message_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleSFixedInt32Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleSFixedInt32Message)
    QML_VALUE_TYPE(simpleSFixedInt32Message)
    Q_PROPERTY(QtProtobuf::sfixed32 testFieldFixedInt32_p READ testFieldFixedInt32 WRITE setTestFieldFixedInt32 SCRIPTABLE false)
    Q_PROPERTY(int testFieldFixedInt32 READ testFieldFixedInt32_p WRITE setTestFieldFixedInt32_p SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = SimpleSFixedInt32Message_QtProtobufNested::QtProtobufFieldEnum;
    SimpleSFixedInt32Message();
    ~SimpleSFixedInt32Message();
    SimpleSFixedInt32Message(const SimpleSFixedInt32Message &other);
    SimpleSFixedInt32Message &operator =(const SimpleSFixedInt32Message &other);
    SimpleSFixedInt32Message(SimpleSFixedInt32Message &&other) noexcept;
    SimpleSFixedInt32Message &operator =(SimpleSFixedInt32Message &&other) noexcept;
    bool operator ==(const SimpleSFixedInt32Message &other) const;
    bool operator !=(const SimpleSFixedInt32Message &other) const;

    QtProtobuf::sfixed32 testFieldFixedInt32() const;
    void setTestFieldFixedInt32(const QtProtobuf::sfixed32 &testFieldFixedInt32);
    static void registerTypes();

private:
    int testFieldFixedInt32_p() const;
    void setTestFieldFixedInt32_p(const int &testFieldFixedInt32);
    QExplicitlySharedDataPointer<SimpleSFixedInt32Message_QtProtobufData> dptr;
};
namespace SimpleSFixedInt32Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleSFixedInt32Message)

enum class QtProtobufFieldEnum {
    TestFieldFixedInt32ProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleSFixedInt32Message_QtProtobufNested

class SimpleSFixedInt64Message_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT SimpleSFixedInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleSFixedInt64Message)
    QML_VALUE_TYPE(simpleSFixedInt64Message)
    Q_PROPERTY(QtProtobuf::sfixed64 testFieldFixedInt64 READ testFieldFixedInt64 WRITE setTestFieldFixedInt64 SCRIPTABLE false)

public:
    using QtProtobufFieldEnum = SimpleSFixedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    SimpleSFixedInt64Message();
    ~SimpleSFixedInt64Message();
    SimpleSFixedInt64Message(const SimpleSFixedInt64Message &other);
    SimpleSFixedInt64Message &operator =(const SimpleSFixedInt64Message &other);
    SimpleSFixedInt64Message(SimpleSFixedInt64Message &&other) noexcept;
    SimpleSFixedInt64Message &operator =(SimpleSFixedInt64Message &&other) noexcept;
    bool operator ==(const SimpleSFixedInt64Message &other) const;
    bool operator !=(const SimpleSFixedInt64Message &other) const;

    QtProtobuf::sfixed64 testFieldFixedInt64() const;
    void setTestFieldFixedInt64(const QtProtobuf::sfixed64 &testFieldFixedInt64);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<SimpleSFixedInt64Message_QtProtobufData> dptr;
};
namespace SimpleSFixedInt64Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(SimpleSFixedInt64Message)

enum class QtProtobufFieldEnum {
    TestFieldFixedInt64ProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace SimpleSFixedInt64Message_QtProtobufNested

class ComplexMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT ComplexMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(ComplexMessage)
    QML_VALUE_TYPE(complexMessage)
    Q_PROPERTY(QtProtobuf::int32 testFieldInt_p READ testFieldInt WRITE setTestFieldInt SCRIPTABLE false)
    Q_PROPERTY(qtprotobufnamespace::tests::SimpleStringMessage *testComplexField_p READ testComplexField_p WRITE setTestComplexField_p)
    Q_PROPERTY(qtprotobufnamespace::tests::SimpleStringMessage testComplexField READ testComplexField WRITE setTestComplexField)
    Q_PROPERTY(int testFieldInt READ testFieldInt_p WRITE setTestFieldInt_p SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = ComplexMessage_QtProtobufNested::QtProtobufFieldEnum;
    ComplexMessage();
    ~ComplexMessage();
    ComplexMessage(const ComplexMessage &other);
    ComplexMessage &operator =(const ComplexMessage &other);
    ComplexMessage(ComplexMessage &&other) noexcept;
    ComplexMessage &operator =(ComplexMessage &&other) noexcept;
    bool operator ==(const ComplexMessage &other) const;
    bool operator !=(const ComplexMessage &other) const;

    QtProtobuf::int32 testFieldInt() const;

    SimpleStringMessage &testComplexField() const;
    void clearTestComplexField();
    void setTestFieldInt(const QtProtobuf::int32 &testFieldInt);
    void setTestComplexField(const SimpleStringMessage &testComplexField);
    static void registerTypes();

private:
    int testFieldInt_p() const;
    SimpleStringMessage *testComplexField_p() const;
    void setTestFieldInt_p(const int &testFieldInt);
    void setTestComplexField_p(SimpleStringMessage *testComplexField);
    QExplicitlySharedDataPointer<ComplexMessage_QtProtobufData> dptr;
};
namespace ComplexMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_QML_GEN_EXPORT)
QML_NAMED_ELEMENT(ComplexMessage)

enum class QtProtobufFieldEnum {
    TestFieldIntProtoFieldNumber = 1,
    TestComplexFieldProtoFieldNumber = 2,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace ComplexMessage_QtProtobufNested
} // namespace qtprotobufnamespace::tests

Q_DECLARE_METATYPE(qtprotobufnamespace::tests::EmptyMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleBoolMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleSIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleUIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleSInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleUInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleStringMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleFloatMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleDoubleMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleBytesMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleFixedInt32Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleFixedInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleSFixedInt32Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::SimpleSFixedInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::ComplexMessage)
#endif // QPROTOBUF_BASICMESSAGES_H
