/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef REPEATEDNONPACKEDMESSAGES_QPB_H
#define REPEATEDNONPACKEDMESSAGES_QPB_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>

#include <QtCore/qmetatype.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>


namespace qtprotobufnamespace::tests {
class RepeatedNonPackedIntMessage;
using RepeatedNonPackedIntMessageRepeated = QList<RepeatedNonPackedIntMessage>;
namespace RepeatedNonPackedIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedIntMessage_QtProtobufNested

class RepeatedNonPackedSIntMessage;
using RepeatedNonPackedSIntMessageRepeated = QList<RepeatedNonPackedSIntMessage>;
namespace RepeatedNonPackedSIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedSIntMessage_QtProtobufNested

class RepeatedNonPackedUIntMessage;
using RepeatedNonPackedUIntMessageRepeated = QList<RepeatedNonPackedUIntMessage>;
namespace RepeatedNonPackedUIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedUIntMessage_QtProtobufNested

class RepeatedNonPackedInt64Message;
using RepeatedNonPackedInt64MessageRepeated = QList<RepeatedNonPackedInt64Message>;
namespace RepeatedNonPackedInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedInt64Message_QtProtobufNested

class RepeatedNonPackedSInt64Message;
using RepeatedNonPackedSInt64MessageRepeated = QList<RepeatedNonPackedSInt64Message>;
namespace RepeatedNonPackedSInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedSInt64Message_QtProtobufNested

class RepeatedNonPackedUInt64Message;
using RepeatedNonPackedUInt64MessageRepeated = QList<RepeatedNonPackedUInt64Message>;
namespace RepeatedNonPackedUInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedUInt64Message_QtProtobufNested

class RepeatedNonPackedFixedIntMessage;
using RepeatedNonPackedFixedIntMessageRepeated = QList<RepeatedNonPackedFixedIntMessage>;
namespace RepeatedNonPackedFixedIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedFixedIntMessage_QtProtobufNested

class RepeatedNonPackedSFixedIntMessage;
using RepeatedNonPackedSFixedIntMessageRepeated = QList<RepeatedNonPackedSFixedIntMessage>;
namespace RepeatedNonPackedSFixedIntMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedSFixedIntMessage_QtProtobufNested

class RepeatedNonPackedFixedInt64Message;
using RepeatedNonPackedFixedInt64MessageRepeated = QList<RepeatedNonPackedFixedInt64Message>;
namespace RepeatedNonPackedFixedInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedFixedInt64Message_QtProtobufNested

class RepeatedNonPackedSFixedInt64Message;
using RepeatedNonPackedSFixedInt64MessageRepeated = QList<RepeatedNonPackedSFixedInt64Message>;
namespace RepeatedNonPackedSFixedInt64Message_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedSFixedInt64Message_QtProtobufNested

class RepeatedNonPackedBoolMessage;
using RepeatedNonPackedBoolMessageRepeated = QList<RepeatedNonPackedBoolMessage>;
namespace RepeatedNonPackedBoolMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedBoolMessage_QtProtobufNested

class RepeatedNonPackedDoubleMessage;
using RepeatedNonPackedDoubleMessageRepeated = QList<RepeatedNonPackedDoubleMessage>;
namespace RepeatedNonPackedDoubleMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedDoubleMessage_QtProtobufNested

class RepeatedNonPackedFloatMessage;
using RepeatedNonPackedFloatMessageRepeated = QList<RepeatedNonPackedFloatMessage>;
namespace RepeatedNonPackedFloatMessage_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace RepeatedNonPackedFloatMessage_QtProtobufNested

class NonPackedIntMessageWithExtraMember;
using NonPackedIntMessageWithExtraMemberRepeated = QList<NonPackedIntMessageWithExtraMember>;
namespace NonPackedIntMessageWithExtraMember_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace NonPackedIntMessageWithExtraMember_QtProtobufNested


class RepeatedNonPackedIntMessage_QtProtobufData;
class RepeatedNonPackedIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::int32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedIntMessage();
    ~RepeatedNonPackedIntMessage();
    RepeatedNonPackedIntMessage(const RepeatedNonPackedIntMessage &other);
    RepeatedNonPackedIntMessage &operator =(const RepeatedNonPackedIntMessage &other);
    RepeatedNonPackedIntMessage(RepeatedNonPackedIntMessage &&other) noexcept;
    RepeatedNonPackedIntMessage &operator =(RepeatedNonPackedIntMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedIntMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::int32List testRepeatedInt() const;
    void setTestRepeatedInt(const QtProtobuf::int32List &testRepeatedInt);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedIntMessage &lhs, const RepeatedNonPackedIntMessage &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedIntMessage &lhs, const RepeatedNonPackedIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedIntMessage &lhs, const RepeatedNonPackedIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedIntMessage_QtProtobufData> dptr;
};
namespace RepeatedNonPackedIntMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedIntMessage_QtProtobufNested

class RepeatedNonPackedSIntMessage_QtProtobufData;
class RepeatedNonPackedSIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::sint32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedSIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedSIntMessage();
    ~RepeatedNonPackedSIntMessage();
    RepeatedNonPackedSIntMessage(const RepeatedNonPackedSIntMessage &other);
    RepeatedNonPackedSIntMessage &operator =(const RepeatedNonPackedSIntMessage &other);
    RepeatedNonPackedSIntMessage(RepeatedNonPackedSIntMessage &&other) noexcept;
    RepeatedNonPackedSIntMessage &operator =(RepeatedNonPackedSIntMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedSIntMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::sint32List testRepeatedInt() const;
    void setTestRepeatedInt(const QtProtobuf::sint32List &testRepeatedInt);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedSIntMessage &lhs, const RepeatedNonPackedSIntMessage &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedSIntMessage &lhs, const RepeatedNonPackedSIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedSIntMessage &lhs, const RepeatedNonPackedSIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedSIntMessage_QtProtobufData> dptr;
};
namespace RepeatedNonPackedSIntMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedSIntMessage_QtProtobufNested

class RepeatedNonPackedUIntMessage_QtProtobufData;
class RepeatedNonPackedUIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::uint32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedUIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedUIntMessage();
    ~RepeatedNonPackedUIntMessage();
    RepeatedNonPackedUIntMessage(const RepeatedNonPackedUIntMessage &other);
    RepeatedNonPackedUIntMessage &operator =(const RepeatedNonPackedUIntMessage &other);
    RepeatedNonPackedUIntMessage(RepeatedNonPackedUIntMessage &&other) noexcept;
    RepeatedNonPackedUIntMessage &operator =(RepeatedNonPackedUIntMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedUIntMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::uint32List testRepeatedInt() const;
    void setTestRepeatedInt(const QtProtobuf::uint32List &testRepeatedInt);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedUIntMessage &lhs, const RepeatedNonPackedUIntMessage &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedUIntMessage &lhs, const RepeatedNonPackedUIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedUIntMessage &lhs, const RepeatedNonPackedUIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedUIntMessage_QtProtobufData> dptr;
};
namespace RepeatedNonPackedUIntMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedUIntMessage_QtProtobufNested

class RepeatedNonPackedInt64Message_QtProtobufData;
class RepeatedNonPackedInt64Message : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::int64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedInt64Message();
    ~RepeatedNonPackedInt64Message();
    RepeatedNonPackedInt64Message(const RepeatedNonPackedInt64Message &other);
    RepeatedNonPackedInt64Message &operator =(const RepeatedNonPackedInt64Message &other);
    RepeatedNonPackedInt64Message(RepeatedNonPackedInt64Message &&other) noexcept;
    RepeatedNonPackedInt64Message &operator =(RepeatedNonPackedInt64Message &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedInt64Message &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::int64List testRepeatedInt() const;
    void setTestRepeatedInt(const QtProtobuf::int64List &testRepeatedInt);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedInt64Message &lhs, const RepeatedNonPackedInt64Message &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedInt64Message &lhs, const RepeatedNonPackedInt64Message &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedInt64Message &lhs, const RepeatedNonPackedInt64Message &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedInt64Message_QtProtobufData> dptr;
};
namespace RepeatedNonPackedInt64Message_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedInt64Message_QtProtobufNested

class RepeatedNonPackedSInt64Message_QtProtobufData;
class RepeatedNonPackedSInt64Message : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::sint64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedSInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedSInt64Message();
    ~RepeatedNonPackedSInt64Message();
    RepeatedNonPackedSInt64Message(const RepeatedNonPackedSInt64Message &other);
    RepeatedNonPackedSInt64Message &operator =(const RepeatedNonPackedSInt64Message &other);
    RepeatedNonPackedSInt64Message(RepeatedNonPackedSInt64Message &&other) noexcept;
    RepeatedNonPackedSInt64Message &operator =(RepeatedNonPackedSInt64Message &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedSInt64Message &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::sint64List testRepeatedInt() const;
    void setTestRepeatedInt(const QtProtobuf::sint64List &testRepeatedInt);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedSInt64Message &lhs, const RepeatedNonPackedSInt64Message &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedSInt64Message &lhs, const RepeatedNonPackedSInt64Message &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedSInt64Message &lhs, const RepeatedNonPackedSInt64Message &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedSInt64Message_QtProtobufData> dptr;
};
namespace RepeatedNonPackedSInt64Message_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedSInt64Message_QtProtobufNested

class RepeatedNonPackedUInt64Message_QtProtobufData;
class RepeatedNonPackedUInt64Message : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::uint64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedUInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedUInt64Message();
    ~RepeatedNonPackedUInt64Message();
    RepeatedNonPackedUInt64Message(const RepeatedNonPackedUInt64Message &other);
    RepeatedNonPackedUInt64Message &operator =(const RepeatedNonPackedUInt64Message &other);
    RepeatedNonPackedUInt64Message(RepeatedNonPackedUInt64Message &&other) noexcept;
    RepeatedNonPackedUInt64Message &operator =(RepeatedNonPackedUInt64Message &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedUInt64Message &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::uint64List testRepeatedInt() const;
    void setTestRepeatedInt(const QtProtobuf::uint64List &testRepeatedInt);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedUInt64Message &lhs, const RepeatedNonPackedUInt64Message &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedUInt64Message &lhs, const RepeatedNonPackedUInt64Message &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedUInt64Message &lhs, const RepeatedNonPackedUInt64Message &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedUInt64Message_QtProtobufData> dptr;
};
namespace RepeatedNonPackedUInt64Message_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedUInt64Message_QtProtobufNested

class RepeatedNonPackedFixedIntMessage_QtProtobufData;
class RepeatedNonPackedFixedIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::fixed32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedFixedIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedFixedIntMessage();
    ~RepeatedNonPackedFixedIntMessage();
    RepeatedNonPackedFixedIntMessage(const RepeatedNonPackedFixedIntMessage &other);
    RepeatedNonPackedFixedIntMessage &operator =(const RepeatedNonPackedFixedIntMessage &other);
    RepeatedNonPackedFixedIntMessage(RepeatedNonPackedFixedIntMessage &&other) noexcept;
    RepeatedNonPackedFixedIntMessage &operator =(RepeatedNonPackedFixedIntMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedFixedIntMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::fixed32List testRepeatedInt() const;
    void setTestRepeatedInt(const QtProtobuf::fixed32List &testRepeatedInt);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedFixedIntMessage &lhs, const RepeatedNonPackedFixedIntMessage &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedFixedIntMessage &lhs, const RepeatedNonPackedFixedIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedFixedIntMessage &lhs, const RepeatedNonPackedFixedIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedFixedIntMessage_QtProtobufData> dptr;
};
namespace RepeatedNonPackedFixedIntMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedFixedIntMessage_QtProtobufNested

class RepeatedNonPackedSFixedIntMessage_QtProtobufData;
class RepeatedNonPackedSFixedIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::sfixed32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedSFixedIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedSFixedIntMessage();
    ~RepeatedNonPackedSFixedIntMessage();
    RepeatedNonPackedSFixedIntMessage(const RepeatedNonPackedSFixedIntMessage &other);
    RepeatedNonPackedSFixedIntMessage &operator =(const RepeatedNonPackedSFixedIntMessage &other);
    RepeatedNonPackedSFixedIntMessage(RepeatedNonPackedSFixedIntMessage &&other) noexcept;
    RepeatedNonPackedSFixedIntMessage &operator =(RepeatedNonPackedSFixedIntMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedSFixedIntMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::sfixed32List testRepeatedInt() const;
    void setTestRepeatedInt(const QtProtobuf::sfixed32List &testRepeatedInt);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedSFixedIntMessage &lhs, const RepeatedNonPackedSFixedIntMessage &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedSFixedIntMessage &lhs, const RepeatedNonPackedSFixedIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedSFixedIntMessage &lhs, const RepeatedNonPackedSFixedIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedSFixedIntMessage_QtProtobufData> dptr;
};
namespace RepeatedNonPackedSFixedIntMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedSFixedIntMessage_QtProtobufNested

class RepeatedNonPackedFixedInt64Message_QtProtobufData;
class RepeatedNonPackedFixedInt64Message : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::fixed64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedFixedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedFixedInt64Message();
    ~RepeatedNonPackedFixedInt64Message();
    RepeatedNonPackedFixedInt64Message(const RepeatedNonPackedFixedInt64Message &other);
    RepeatedNonPackedFixedInt64Message &operator =(const RepeatedNonPackedFixedInt64Message &other);
    RepeatedNonPackedFixedInt64Message(RepeatedNonPackedFixedInt64Message &&other) noexcept;
    RepeatedNonPackedFixedInt64Message &operator =(RepeatedNonPackedFixedInt64Message &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedFixedInt64Message &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::fixed64List testRepeatedInt() const;
    void setTestRepeatedInt(const QtProtobuf::fixed64List &testRepeatedInt);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedFixedInt64Message &lhs, const RepeatedNonPackedFixedInt64Message &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedFixedInt64Message &lhs, const RepeatedNonPackedFixedInt64Message &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedFixedInt64Message &lhs, const RepeatedNonPackedFixedInt64Message &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedFixedInt64Message_QtProtobufData> dptr;
};
namespace RepeatedNonPackedFixedInt64Message_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedFixedInt64Message_QtProtobufNested

class RepeatedNonPackedSFixedInt64Message_QtProtobufData;
class RepeatedNonPackedSFixedInt64Message : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::sfixed64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedSFixedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedSFixedInt64Message();
    ~RepeatedNonPackedSFixedInt64Message();
    RepeatedNonPackedSFixedInt64Message(const RepeatedNonPackedSFixedInt64Message &other);
    RepeatedNonPackedSFixedInt64Message &operator =(const RepeatedNonPackedSFixedInt64Message &other);
    RepeatedNonPackedSFixedInt64Message(RepeatedNonPackedSFixedInt64Message &&other) noexcept;
    RepeatedNonPackedSFixedInt64Message &operator =(RepeatedNonPackedSFixedInt64Message &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedSFixedInt64Message &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::sfixed64List testRepeatedInt() const;
    void setTestRepeatedInt(const QtProtobuf::sfixed64List &testRepeatedInt);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedSFixedInt64Message &lhs, const RepeatedNonPackedSFixedInt64Message &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedSFixedInt64Message &lhs, const RepeatedNonPackedSFixedInt64Message &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedSFixedInt64Message &lhs, const RepeatedNonPackedSFixedInt64Message &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedSFixedInt64Message_QtProtobufData> dptr;
};
namespace RepeatedNonPackedSFixedInt64Message_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedSFixedInt64Message_QtProtobufNested

class RepeatedNonPackedBoolMessage_QtProtobufData;
class RepeatedNonPackedBoolMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::boolList testRepeatedBool READ testRepeatedBool WRITE setTestRepeatedBool SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedBoolMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedBoolMessage();
    ~RepeatedNonPackedBoolMessage();
    RepeatedNonPackedBoolMessage(const RepeatedNonPackedBoolMessage &other);
    RepeatedNonPackedBoolMessage &operator =(const RepeatedNonPackedBoolMessage &other);
    RepeatedNonPackedBoolMessage(RepeatedNonPackedBoolMessage &&other) noexcept;
    RepeatedNonPackedBoolMessage &operator =(RepeatedNonPackedBoolMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedBoolMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::boolList testRepeatedBool() const;
    void setTestRepeatedBool(const QtProtobuf::boolList &testRepeatedBool);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedBoolMessage &lhs, const RepeatedNonPackedBoolMessage &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedBoolMessage &lhs, const RepeatedNonPackedBoolMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedBoolMessage &lhs, const RepeatedNonPackedBoolMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedBoolMessage_QtProtobufData> dptr;
};
namespace RepeatedNonPackedBoolMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedBoolProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedBoolMessage_QtProtobufNested

class RepeatedNonPackedDoubleMessage_QtProtobufData;
class RepeatedNonPackedDoubleMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::doubleList testRepeatedDouble READ testRepeatedDouble WRITE setTestRepeatedDouble SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedDoubleMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedDoubleMessage();
    ~RepeatedNonPackedDoubleMessage();
    RepeatedNonPackedDoubleMessage(const RepeatedNonPackedDoubleMessage &other);
    RepeatedNonPackedDoubleMessage &operator =(const RepeatedNonPackedDoubleMessage &other);
    RepeatedNonPackedDoubleMessage(RepeatedNonPackedDoubleMessage &&other) noexcept;
    RepeatedNonPackedDoubleMessage &operator =(RepeatedNonPackedDoubleMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedDoubleMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::doubleList testRepeatedDouble() const;
    void setTestRepeatedDouble(const QtProtobuf::doubleList &testRepeatedDouble);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedDoubleMessage &lhs, const RepeatedNonPackedDoubleMessage &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedDoubleMessage &lhs, const RepeatedNonPackedDoubleMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedDoubleMessage &lhs, const RepeatedNonPackedDoubleMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedDoubleMessage_QtProtobufData> dptr;
};
namespace RepeatedNonPackedDoubleMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedDoubleProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedDoubleMessage_QtProtobufNested

class RepeatedNonPackedFloatMessage_QtProtobufData;
class RepeatedNonPackedFloatMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::floatList testRepeatedFloat READ testRepeatedFloat WRITE setTestRepeatedFloat SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedNonPackedFloatMessage_QtProtobufNested::QtProtobufFieldEnum;
    RepeatedNonPackedFloatMessage();
    ~RepeatedNonPackedFloatMessage();
    RepeatedNonPackedFloatMessage(const RepeatedNonPackedFloatMessage &other);
    RepeatedNonPackedFloatMessage &operator =(const RepeatedNonPackedFloatMessage &other);
    RepeatedNonPackedFloatMessage(RepeatedNonPackedFloatMessage &&other) noexcept;
    RepeatedNonPackedFloatMessage &operator =(RepeatedNonPackedFloatMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedNonPackedFloatMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::floatList testRepeatedFloat() const;
    void setTestRepeatedFloat(const QtProtobuf::floatList &testRepeatedFloat);
    static void registerTypes();

private:
    friend bool comparesEqual(const RepeatedNonPackedFloatMessage &lhs, const RepeatedNonPackedFloatMessage &rhs) noexcept;
    friend bool operator==(const RepeatedNonPackedFloatMessage &lhs, const RepeatedNonPackedFloatMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedNonPackedFloatMessage &lhs, const RepeatedNonPackedFloatMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedNonPackedFloatMessage_QtProtobufData> dptr;
};
namespace RepeatedNonPackedFloatMessage_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedFloatProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedNonPackedFloatMessage_QtProtobufNested

class NonPackedIntMessageWithExtraMember_QtProtobufData;
class NonPackedIntMessageWithExtraMember : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT
    Q_PROPERTY(QtProtobuf::int32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)
    Q_PROPERTY(QString extra READ extra WRITE setExtra SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = NonPackedIntMessageWithExtraMember_QtProtobufNested::QtProtobufFieldEnum;
    NonPackedIntMessageWithExtraMember();
    ~NonPackedIntMessageWithExtraMember();
    NonPackedIntMessageWithExtraMember(const NonPackedIntMessageWithExtraMember &other);
    NonPackedIntMessageWithExtraMember &operator =(const NonPackedIntMessageWithExtraMember &other);
    NonPackedIntMessageWithExtraMember(NonPackedIntMessageWithExtraMember &&other) noexcept;
    NonPackedIntMessageWithExtraMember &operator =(NonPackedIntMessageWithExtraMember &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(NonPackedIntMessageWithExtraMember &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QtProtobuf::int32List testRepeatedInt() const;

    QString extra() const;
    void setTestRepeatedInt(const QtProtobuf::int32List &testRepeatedInt);
    void setExtra(const QString &extra);
    static void registerTypes();

private:
    friend bool comparesEqual(const NonPackedIntMessageWithExtraMember &lhs, const NonPackedIntMessageWithExtraMember &rhs) noexcept;
    friend bool operator==(const NonPackedIntMessageWithExtraMember &lhs, const NonPackedIntMessageWithExtraMember &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const NonPackedIntMessageWithExtraMember &lhs, const NonPackedIntMessageWithExtraMember &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<NonPackedIntMessageWithExtraMember_QtProtobufData> dptr;
};
namespace NonPackedIntMessageWithExtraMember_QtProtobufNested {
Q_NAMESPACE

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
    ExtraProtoFieldNumber = 2,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace NonPackedIntMessageWithExtraMember_QtProtobufNested
} // namespace qtprotobufnamespace::tests

#endif // REPEATEDNONPACKEDMESSAGES_QPB_H
