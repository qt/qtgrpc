/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_REPEATEDMESSAGES_H
#define QPROTOBUF_REPEATEDMESSAGES_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>
#include "basicmessages.qpb.h"

#include <QtCore/qmetatype.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>

#include <memory>


namespace qtprotobufnamespace::tests {
class RepeatedStringMessage;
using RepeatedStringMessageRepeated = QList<RepeatedStringMessage>;
namespace RepeatedStringMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedStringMessage_QtProtobufNested

class RepeatedDoubleMessage;
using RepeatedDoubleMessageRepeated = QList<RepeatedDoubleMessage>;
namespace RepeatedDoubleMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedDoubleMessage_QtProtobufNested

class RepeatedBytesMessage;
using RepeatedBytesMessageRepeated = QList<RepeatedBytesMessage>;
namespace RepeatedBytesMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedBytesMessage_QtProtobufNested

class RepeatedFloatMessage;
using RepeatedFloatMessageRepeated = QList<RepeatedFloatMessage>;
namespace RepeatedFloatMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedFloatMessage_QtProtobufNested

class RepeatedComplexMessage;
using RepeatedComplexMessageRepeated = QList<RepeatedComplexMessage>;
namespace RepeatedComplexMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedComplexMessage_QtProtobufNested

class RepeatedSIntMessage;
using RepeatedSIntMessageRepeated = QList<RepeatedSIntMessage>;
namespace RepeatedSIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedSIntMessage_QtProtobufNested

class RepeatedIntMessage;
using RepeatedIntMessageRepeated = QList<RepeatedIntMessage>;
namespace RepeatedIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedIntMessage_QtProtobufNested

class RepeatedUIntMessage;
using RepeatedUIntMessageRepeated = QList<RepeatedUIntMessage>;
namespace RepeatedUIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedUIntMessage_QtProtobufNested

class RepeatedSInt64Message;
using RepeatedSInt64MessageRepeated = QList<RepeatedSInt64Message>;
namespace RepeatedSInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedSInt64Message_QtProtobufNested

class RepeatedInt64Message;
using RepeatedInt64MessageRepeated = QList<RepeatedInt64Message>;
namespace RepeatedInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedInt64Message_QtProtobufNested

class RepeatedUInt64Message;
using RepeatedUInt64MessageRepeated = QList<RepeatedUInt64Message>;
namespace RepeatedUInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedUInt64Message_QtProtobufNested

class RepeatedFixedIntMessage;
using RepeatedFixedIntMessageRepeated = QList<RepeatedFixedIntMessage>;
namespace RepeatedFixedIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedFixedIntMessage_QtProtobufNested

class RepeatedSFixedIntMessage;
using RepeatedSFixedIntMessageRepeated = QList<RepeatedSFixedIntMessage>;
namespace RepeatedSFixedIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedSFixedIntMessage_QtProtobufNested

class RepeatedFixedInt64Message;
using RepeatedFixedInt64MessageRepeated = QList<RepeatedFixedInt64Message>;
namespace RepeatedFixedInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedFixedInt64Message_QtProtobufNested

class RepeatedSFixedInt64Message;
using RepeatedSFixedInt64MessageRepeated = QList<RepeatedSFixedInt64Message>;
namespace RepeatedSFixedInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedSFixedInt64Message_QtProtobufNested

class RepeatedBoolMessage;
using RepeatedBoolMessageRepeated = QList<RepeatedBoolMessage>;
namespace RepeatedBoolMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedBoolMessage_QtProtobufNested


class RepeatedStringMessage_QtProtobufData;
class RepeatedStringMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QStringList testRepeatedString READ testRepeatedString WRITE setTestRepeatedString SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedStringMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedStringMessage();
    ~RepeatedStringMessage();
    RepeatedStringMessage(const RepeatedStringMessage &other);
    RepeatedStringMessage &operator =(const RepeatedStringMessage &other);
    RepeatedStringMessage(RepeatedStringMessage &&other) noexcept;
    RepeatedStringMessage &operator =(RepeatedStringMessage &&other) noexcept;
    bool operator ==(const RepeatedStringMessage &other) const;
    bool operator !=(const RepeatedStringMessage &other) const;

    QStringList testRepeatedString() const;
    QStringList &testRepeatedString();
    void setTestRepeatedString(const QStringList &testRepeatedString);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedStringMessage_QtProtobufData> dptr;
};
namespace RepeatedStringMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedStringProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedStringMessage_QtProtobufNested

class RepeatedDoubleMessage_QtProtobufData;
class RepeatedDoubleMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::doubleList testRepeatedDouble READ testRepeatedDouble WRITE setTestRepeatedDouble SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedDoubleMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedDoubleMessage();
    ~RepeatedDoubleMessage();
    RepeatedDoubleMessage(const RepeatedDoubleMessage &other);
    RepeatedDoubleMessage &operator =(const RepeatedDoubleMessage &other);
    RepeatedDoubleMessage(RepeatedDoubleMessage &&other) noexcept;
    RepeatedDoubleMessage &operator =(RepeatedDoubleMessage &&other) noexcept;
    bool operator ==(const RepeatedDoubleMessage &other) const;
    bool operator !=(const RepeatedDoubleMessage &other) const;

    QtProtobuf::doubleList testRepeatedDouble() const;
    QtProtobuf::doubleList &testRepeatedDouble();
    void setTestRepeatedDouble(const QtProtobuf::doubleList &testRepeatedDouble);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedDoubleMessage_QtProtobufData> dptr;
};
namespace RepeatedDoubleMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedDoubleProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedDoubleMessage_QtProtobufNested

class RepeatedBytesMessage_QtProtobufData;
class RepeatedBytesMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QByteArrayList testRepeatedBytes READ testRepeatedBytes WRITE setTestRepeatedBytes SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedBytesMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedBytesMessage();
    ~RepeatedBytesMessage();
    RepeatedBytesMessage(const RepeatedBytesMessage &other);
    RepeatedBytesMessage &operator =(const RepeatedBytesMessage &other);
    RepeatedBytesMessage(RepeatedBytesMessage &&other) noexcept;
    RepeatedBytesMessage &operator =(RepeatedBytesMessage &&other) noexcept;
    bool operator ==(const RepeatedBytesMessage &other) const;
    bool operator !=(const RepeatedBytesMessage &other) const;

    QByteArrayList testRepeatedBytes() const;
    QByteArrayList &testRepeatedBytes();
    void setTestRepeatedBytes(const QByteArrayList &testRepeatedBytes);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedBytesMessage_QtProtobufData> dptr;
};
namespace RepeatedBytesMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedBytesProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedBytesMessage_QtProtobufNested

class RepeatedFloatMessage_QtProtobufData;
class RepeatedFloatMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::floatList testRepeatedFloat READ testRepeatedFloat WRITE setTestRepeatedFloat SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedFloatMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedFloatMessage();
    ~RepeatedFloatMessage();
    RepeatedFloatMessage(const RepeatedFloatMessage &other);
    RepeatedFloatMessage &operator =(const RepeatedFloatMessage &other);
    RepeatedFloatMessage(RepeatedFloatMessage &&other) noexcept;
    RepeatedFloatMessage &operator =(RepeatedFloatMessage &&other) noexcept;
    bool operator ==(const RepeatedFloatMessage &other) const;
    bool operator !=(const RepeatedFloatMessage &other) const;

    QtProtobuf::floatList testRepeatedFloat() const;
    QtProtobuf::floatList &testRepeatedFloat();
    void setTestRepeatedFloat(const QtProtobuf::floatList &testRepeatedFloat);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedFloatMessage_QtProtobufData> dptr;
};
namespace RepeatedFloatMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedFloatProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedFloatMessage_QtProtobufNested

class RepeatedComplexMessage_QtProtobufData;
class RepeatedComplexMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(qtprotobufnamespace::tests::ComplexMessageRepeated testRepeatedComplexData READ testRepeatedComplex WRITE setTestRepeatedComplex SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedComplexMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedComplexMessage();
    ~RepeatedComplexMessage();
    RepeatedComplexMessage(const RepeatedComplexMessage &other);
    RepeatedComplexMessage &operator =(const RepeatedComplexMessage &other);
    RepeatedComplexMessage(RepeatedComplexMessage &&other) noexcept;
    RepeatedComplexMessage &operator =(RepeatedComplexMessage &&other) noexcept;
    bool operator ==(const RepeatedComplexMessage &other) const;
    bool operator !=(const RepeatedComplexMessage &other) const;

    ComplexMessageRepeated testRepeatedComplex() const;
    ComplexMessageRepeated &testRepeatedComplex();
    void setTestRepeatedComplex(const ComplexMessageRepeated &testRepeatedComplex);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedComplexMessage_QtProtobufData> dptr;
};
namespace RepeatedComplexMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedComplexProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedComplexMessage_QtProtobufNested

class RepeatedSIntMessage_QtProtobufData;
class RepeatedSIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::sint32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedSIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedSIntMessage();
    ~RepeatedSIntMessage();
    RepeatedSIntMessage(const RepeatedSIntMessage &other);
    RepeatedSIntMessage &operator =(const RepeatedSIntMessage &other);
    RepeatedSIntMessage(RepeatedSIntMessage &&other) noexcept;
    RepeatedSIntMessage &operator =(RepeatedSIntMessage &&other) noexcept;
    bool operator ==(const RepeatedSIntMessage &other) const;
    bool operator !=(const RepeatedSIntMessage &other) const;

    QtProtobuf::sint32List testRepeatedInt() const;
    QtProtobuf::sint32List &testRepeatedInt();
    void setTestRepeatedInt(const QtProtobuf::sint32List &testRepeatedInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedSIntMessage_QtProtobufData> dptr;
};
namespace RepeatedSIntMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedSIntMessage_QtProtobufNested

class RepeatedIntMessage_QtProtobufData;
class RepeatedIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::int32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedIntMessage();
    ~RepeatedIntMessage();
    RepeatedIntMessage(const RepeatedIntMessage &other);
    RepeatedIntMessage &operator =(const RepeatedIntMessage &other);
    RepeatedIntMessage(RepeatedIntMessage &&other) noexcept;
    RepeatedIntMessage &operator =(RepeatedIntMessage &&other) noexcept;
    bool operator ==(const RepeatedIntMessage &other) const;
    bool operator !=(const RepeatedIntMessage &other) const;

    QtProtobuf::int32List testRepeatedInt() const;
    QtProtobuf::int32List &testRepeatedInt();
    void setTestRepeatedInt(const QtProtobuf::int32List &testRepeatedInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedIntMessage_QtProtobufData> dptr;
};
namespace RepeatedIntMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedIntMessage_QtProtobufNested

class RepeatedUIntMessage_QtProtobufData;
class RepeatedUIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::uint32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedUIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedUIntMessage();
    ~RepeatedUIntMessage();
    RepeatedUIntMessage(const RepeatedUIntMessage &other);
    RepeatedUIntMessage &operator =(const RepeatedUIntMessage &other);
    RepeatedUIntMessage(RepeatedUIntMessage &&other) noexcept;
    RepeatedUIntMessage &operator =(RepeatedUIntMessage &&other) noexcept;
    bool operator ==(const RepeatedUIntMessage &other) const;
    bool operator !=(const RepeatedUIntMessage &other) const;

    QtProtobuf::uint32List testRepeatedInt() const;
    QtProtobuf::uint32List &testRepeatedInt();
    void setTestRepeatedInt(const QtProtobuf::uint32List &testRepeatedInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedUIntMessage_QtProtobufData> dptr;
};
namespace RepeatedUIntMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedUIntMessage_QtProtobufNested

class RepeatedSInt64Message_QtProtobufData;
class RepeatedSInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::sint64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedSInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedSInt64Message();
    ~RepeatedSInt64Message();
    RepeatedSInt64Message(const RepeatedSInt64Message &other);
    RepeatedSInt64Message &operator =(const RepeatedSInt64Message &other);
    RepeatedSInt64Message(RepeatedSInt64Message &&other) noexcept;
    RepeatedSInt64Message &operator =(RepeatedSInt64Message &&other) noexcept;
    bool operator ==(const RepeatedSInt64Message &other) const;
    bool operator !=(const RepeatedSInt64Message &other) const;

    QtProtobuf::sint64List testRepeatedInt() const;
    QtProtobuf::sint64List &testRepeatedInt();
    void setTestRepeatedInt(const QtProtobuf::sint64List &testRepeatedInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedSInt64Message_QtProtobufData> dptr;
};
namespace RepeatedSInt64Message_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedSInt64Message_QtProtobufNested

class RepeatedInt64Message_QtProtobufData;
class RepeatedInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::int64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedInt64Message();
    ~RepeatedInt64Message();
    RepeatedInt64Message(const RepeatedInt64Message &other);
    RepeatedInt64Message &operator =(const RepeatedInt64Message &other);
    RepeatedInt64Message(RepeatedInt64Message &&other) noexcept;
    RepeatedInt64Message &operator =(RepeatedInt64Message &&other) noexcept;
    bool operator ==(const RepeatedInt64Message &other) const;
    bool operator !=(const RepeatedInt64Message &other) const;

    QtProtobuf::int64List testRepeatedInt() const;
    QtProtobuf::int64List &testRepeatedInt();
    void setTestRepeatedInt(const QtProtobuf::int64List &testRepeatedInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedInt64Message_QtProtobufData> dptr;
};
namespace RepeatedInt64Message_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedInt64Message_QtProtobufNested

class RepeatedUInt64Message_QtProtobufData;
class RepeatedUInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::uint64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedUInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedUInt64Message();
    ~RepeatedUInt64Message();
    RepeatedUInt64Message(const RepeatedUInt64Message &other);
    RepeatedUInt64Message &operator =(const RepeatedUInt64Message &other);
    RepeatedUInt64Message(RepeatedUInt64Message &&other) noexcept;
    RepeatedUInt64Message &operator =(RepeatedUInt64Message &&other) noexcept;
    bool operator ==(const RepeatedUInt64Message &other) const;
    bool operator !=(const RepeatedUInt64Message &other) const;

    QtProtobuf::uint64List testRepeatedInt() const;
    QtProtobuf::uint64List &testRepeatedInt();
    void setTestRepeatedInt(const QtProtobuf::uint64List &testRepeatedInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedUInt64Message_QtProtobufData> dptr;
};
namespace RepeatedUInt64Message_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedUInt64Message_QtProtobufNested

class RepeatedFixedIntMessage_QtProtobufData;
class RepeatedFixedIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::fixed32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedFixedIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedFixedIntMessage();
    ~RepeatedFixedIntMessage();
    RepeatedFixedIntMessage(const RepeatedFixedIntMessage &other);
    RepeatedFixedIntMessage &operator =(const RepeatedFixedIntMessage &other);
    RepeatedFixedIntMessage(RepeatedFixedIntMessage &&other) noexcept;
    RepeatedFixedIntMessage &operator =(RepeatedFixedIntMessage &&other) noexcept;
    bool operator ==(const RepeatedFixedIntMessage &other) const;
    bool operator !=(const RepeatedFixedIntMessage &other) const;

    QtProtobuf::fixed32List testRepeatedInt() const;
    QtProtobuf::fixed32List &testRepeatedInt();
    void setTestRepeatedInt(const QtProtobuf::fixed32List &testRepeatedInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedFixedIntMessage_QtProtobufData> dptr;
};
namespace RepeatedFixedIntMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedFixedIntMessage_QtProtobufNested

class RepeatedSFixedIntMessage_QtProtobufData;
class RepeatedSFixedIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::sfixed32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedSFixedIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedSFixedIntMessage();
    ~RepeatedSFixedIntMessage();
    RepeatedSFixedIntMessage(const RepeatedSFixedIntMessage &other);
    RepeatedSFixedIntMessage &operator =(const RepeatedSFixedIntMessage &other);
    RepeatedSFixedIntMessage(RepeatedSFixedIntMessage &&other) noexcept;
    RepeatedSFixedIntMessage &operator =(RepeatedSFixedIntMessage &&other) noexcept;
    bool operator ==(const RepeatedSFixedIntMessage &other) const;
    bool operator !=(const RepeatedSFixedIntMessage &other) const;

    QtProtobuf::sfixed32List testRepeatedInt() const;
    QtProtobuf::sfixed32List &testRepeatedInt();
    void setTestRepeatedInt(const QtProtobuf::sfixed32List &testRepeatedInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedSFixedIntMessage_QtProtobufData> dptr;
};
namespace RepeatedSFixedIntMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedSFixedIntMessage_QtProtobufNested

class RepeatedFixedInt64Message_QtProtobufData;
class RepeatedFixedInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::fixed64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedFixedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedFixedInt64Message();
    ~RepeatedFixedInt64Message();
    RepeatedFixedInt64Message(const RepeatedFixedInt64Message &other);
    RepeatedFixedInt64Message &operator =(const RepeatedFixedInt64Message &other);
    RepeatedFixedInt64Message(RepeatedFixedInt64Message &&other) noexcept;
    RepeatedFixedInt64Message &operator =(RepeatedFixedInt64Message &&other) noexcept;
    bool operator ==(const RepeatedFixedInt64Message &other) const;
    bool operator !=(const RepeatedFixedInt64Message &other) const;

    QtProtobuf::fixed64List testRepeatedInt() const;
    QtProtobuf::fixed64List &testRepeatedInt();
    void setTestRepeatedInt(const QtProtobuf::fixed64List &testRepeatedInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedFixedInt64Message_QtProtobufData> dptr;
};
namespace RepeatedFixedInt64Message_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedFixedInt64Message_QtProtobufNested

class RepeatedSFixedInt64Message_QtProtobufData;
class RepeatedSFixedInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::sfixed64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedSFixedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedSFixedInt64Message();
    ~RepeatedSFixedInt64Message();
    RepeatedSFixedInt64Message(const RepeatedSFixedInt64Message &other);
    RepeatedSFixedInt64Message &operator =(const RepeatedSFixedInt64Message &other);
    RepeatedSFixedInt64Message(RepeatedSFixedInt64Message &&other) noexcept;
    RepeatedSFixedInt64Message &operator =(RepeatedSFixedInt64Message &&other) noexcept;
    bool operator ==(const RepeatedSFixedInt64Message &other) const;
    bool operator !=(const RepeatedSFixedInt64Message &other) const;

    QtProtobuf::sfixed64List testRepeatedInt() const;
    QtProtobuf::sfixed64List &testRepeatedInt();
    void setTestRepeatedInt(const QtProtobuf::sfixed64List &testRepeatedInt);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedSFixedInt64Message_QtProtobufData> dptr;
};
namespace RepeatedSFixedInt64Message_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedSFixedInt64Message_QtProtobufNested

class RepeatedBoolMessage_QtProtobufData;
class RepeatedBoolMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::boolList testRepeatedBool READ testRepeatedBool WRITE setTestRepeatedBool SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedBoolMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedBoolMessage();
    ~RepeatedBoolMessage();
    RepeatedBoolMessage(const RepeatedBoolMessage &other);
    RepeatedBoolMessage &operator =(const RepeatedBoolMessage &other);
    RepeatedBoolMessage(RepeatedBoolMessage &&other) noexcept;
    RepeatedBoolMessage &operator =(RepeatedBoolMessage &&other) noexcept;
    bool operator ==(const RepeatedBoolMessage &other) const;
    bool operator !=(const RepeatedBoolMessage &other) const;

    QtProtobuf::boolList testRepeatedBool() const;
    QtProtobuf::boolList &testRepeatedBool();
    void setTestRepeatedBool(const QtProtobuf::boolList &testRepeatedBool);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<RepeatedBoolMessage_QtProtobufData> dptr;
};
namespace RepeatedBoolMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedBoolProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedBoolMessage_QtProtobufNested
} // namespace qtprotobufnamespace::tests

Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedStringMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedDoubleMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedBytesMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedFloatMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedComplexMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedSIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedUIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedSInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedUInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedFixedIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedSFixedIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedFixedInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedSFixedInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedBoolMessage)
#endif // QPROTOBUF_REPEATEDMESSAGES_H
