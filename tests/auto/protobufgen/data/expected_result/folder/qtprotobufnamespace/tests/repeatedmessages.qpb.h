/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef REPEATEDMESSAGES_QPB_H
#define REPEATEDMESSAGES_QPB_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>
#include "basicmessages.qpb.h"
#include "tst_qtprotobufgen_gen_exports.qpb.h"

#include <QtCore/qmetatype.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>


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
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QStringList testRepeatedString READ testRepeatedString WRITE setTestRepeatedString SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedStringMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedStringMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedStringMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedStringMessage(const RepeatedStringMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedStringMessage &operator =(const RepeatedStringMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedStringMessage(RepeatedStringMessage &&other) noexcept;
    RepeatedStringMessage &operator =(RepeatedStringMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedStringMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QStringList testRepeatedString() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedString(const QStringList &testRepeatedString);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedStringMessage &lhs, const RepeatedStringMessage &rhs) noexcept;
    friend bool operator==(const RepeatedStringMessage &lhs, const RepeatedStringMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedStringMessage &lhs, const RepeatedStringMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedStringMessage_QtProtobufData> dptr;
};
namespace RepeatedStringMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedStringProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedStringMessage_QtProtobufNested

class RepeatedDoubleMessage_QtProtobufData;
class RepeatedDoubleMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::doubleList testRepeatedDouble READ testRepeatedDouble WRITE setTestRepeatedDouble SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedDoubleMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedDoubleMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedDoubleMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedDoubleMessage(const RepeatedDoubleMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedDoubleMessage &operator =(const RepeatedDoubleMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedDoubleMessage(RepeatedDoubleMessage &&other) noexcept;
    RepeatedDoubleMessage &operator =(RepeatedDoubleMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedDoubleMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::doubleList testRepeatedDouble() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedDouble(const QtProtobuf::doubleList &testRepeatedDouble);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedDoubleMessage &lhs, const RepeatedDoubleMessage &rhs) noexcept;
    friend bool operator==(const RepeatedDoubleMessage &lhs, const RepeatedDoubleMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedDoubleMessage &lhs, const RepeatedDoubleMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedDoubleMessage_QtProtobufData> dptr;
};
namespace RepeatedDoubleMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedDoubleProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedDoubleMessage_QtProtobufNested

class RepeatedBytesMessage_QtProtobufData;
class RepeatedBytesMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QByteArrayList testRepeatedBytes READ testRepeatedBytes WRITE setTestRepeatedBytes SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedBytesMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedBytesMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedBytesMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedBytesMessage(const RepeatedBytesMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedBytesMessage &operator =(const RepeatedBytesMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedBytesMessage(RepeatedBytesMessage &&other) noexcept;
    RepeatedBytesMessage &operator =(RepeatedBytesMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedBytesMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QByteArrayList testRepeatedBytes() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedBytes(const QByteArrayList &testRepeatedBytes);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedBytesMessage &lhs, const RepeatedBytesMessage &rhs) noexcept;
    friend bool operator==(const RepeatedBytesMessage &lhs, const RepeatedBytesMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedBytesMessage &lhs, const RepeatedBytesMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedBytesMessage_QtProtobufData> dptr;
};
namespace RepeatedBytesMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedBytesProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedBytesMessage_QtProtobufNested

class RepeatedFloatMessage_QtProtobufData;
class RepeatedFloatMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::floatList testRepeatedFloat READ testRepeatedFloat WRITE setTestRepeatedFloat SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedFloatMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFloatMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedFloatMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFloatMessage(const RepeatedFloatMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFloatMessage &operator =(const RepeatedFloatMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFloatMessage(RepeatedFloatMessage &&other) noexcept;
    RepeatedFloatMessage &operator =(RepeatedFloatMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedFloatMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::floatList testRepeatedFloat() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedFloat(const QtProtobuf::floatList &testRepeatedFloat);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedFloatMessage &lhs, const RepeatedFloatMessage &rhs) noexcept;
    friend bool operator==(const RepeatedFloatMessage &lhs, const RepeatedFloatMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedFloatMessage &lhs, const RepeatedFloatMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedFloatMessage_QtProtobufData> dptr;
};
namespace RepeatedFloatMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedFloatProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedFloatMessage_QtProtobufNested

class RepeatedComplexMessage_QtProtobufData;
class RepeatedComplexMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(qtprotobufnamespace::tests::ComplexMessageRepeated testRepeatedComplexData READ testRepeatedComplex WRITE setTestRepeatedComplex SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedComplexMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedComplexMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedComplexMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedComplexMessage(const RepeatedComplexMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedComplexMessage &operator =(const RepeatedComplexMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedComplexMessage(RepeatedComplexMessage &&other) noexcept;
    RepeatedComplexMessage &operator =(RepeatedComplexMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedComplexMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ComplexMessageRepeated testRepeatedComplex() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedComplex(const ComplexMessageRepeated &testRepeatedComplex);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedComplexMessage &lhs, const RepeatedComplexMessage &rhs) noexcept;
    friend bool operator==(const RepeatedComplexMessage &lhs, const RepeatedComplexMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedComplexMessage &lhs, const RepeatedComplexMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedComplexMessage_QtProtobufData> dptr;
};
namespace RepeatedComplexMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedComplexProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedComplexMessage_QtProtobufNested

class RepeatedSIntMessage_QtProtobufData;
class RepeatedSIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::sint32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedSIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSIntMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedSIntMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSIntMessage(const RepeatedSIntMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSIntMessage &operator =(const RepeatedSIntMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSIntMessage(RepeatedSIntMessage &&other) noexcept;
    RepeatedSIntMessage &operator =(RepeatedSIntMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedSIntMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::sint32List testRepeatedInt() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedInt(const QtProtobuf::sint32List &testRepeatedInt);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedSIntMessage &lhs, const RepeatedSIntMessage &rhs) noexcept;
    friend bool operator==(const RepeatedSIntMessage &lhs, const RepeatedSIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedSIntMessage &lhs, const RepeatedSIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedSIntMessage_QtProtobufData> dptr;
};
namespace RepeatedSIntMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedSIntMessage_QtProtobufNested

class RepeatedIntMessage_QtProtobufData;
class RepeatedIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::int32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedIntMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedIntMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedIntMessage(const RepeatedIntMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedIntMessage &operator =(const RepeatedIntMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedIntMessage(RepeatedIntMessage &&other) noexcept;
    RepeatedIntMessage &operator =(RepeatedIntMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedIntMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::int32List testRepeatedInt() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedInt(const QtProtobuf::int32List &testRepeatedInt);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedIntMessage &lhs, const RepeatedIntMessage &rhs) noexcept;
    friend bool operator==(const RepeatedIntMessage &lhs, const RepeatedIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedIntMessage &lhs, const RepeatedIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedIntMessage_QtProtobufData> dptr;
};
namespace RepeatedIntMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedIntMessage_QtProtobufNested

class RepeatedUIntMessage_QtProtobufData;
class RepeatedUIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::uint32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedUIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedUIntMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedUIntMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedUIntMessage(const RepeatedUIntMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedUIntMessage &operator =(const RepeatedUIntMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedUIntMessage(RepeatedUIntMessage &&other) noexcept;
    RepeatedUIntMessage &operator =(RepeatedUIntMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedUIntMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::uint32List testRepeatedInt() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedInt(const QtProtobuf::uint32List &testRepeatedInt);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedUIntMessage &lhs, const RepeatedUIntMessage &rhs) noexcept;
    friend bool operator==(const RepeatedUIntMessage &lhs, const RepeatedUIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedUIntMessage &lhs, const RepeatedUIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedUIntMessage_QtProtobufData> dptr;
};
namespace RepeatedUIntMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedUIntMessage_QtProtobufNested

class RepeatedSInt64Message_QtProtobufData;
class RepeatedSInt64Message : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::sint64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedSInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSInt64Message();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedSInt64Message();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSInt64Message(const RepeatedSInt64Message &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSInt64Message &operator =(const RepeatedSInt64Message &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSInt64Message(RepeatedSInt64Message &&other) noexcept;
    RepeatedSInt64Message &operator =(RepeatedSInt64Message &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedSInt64Message &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::sint64List testRepeatedInt() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedInt(const QtProtobuf::sint64List &testRepeatedInt);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedSInt64Message &lhs, const RepeatedSInt64Message &rhs) noexcept;
    friend bool operator==(const RepeatedSInt64Message &lhs, const RepeatedSInt64Message &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedSInt64Message &lhs, const RepeatedSInt64Message &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedSInt64Message_QtProtobufData> dptr;
};
namespace RepeatedSInt64Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedSInt64Message_QtProtobufNested

class RepeatedInt64Message_QtProtobufData;
class RepeatedInt64Message : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::int64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedInt64Message();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedInt64Message();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedInt64Message(const RepeatedInt64Message &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedInt64Message &operator =(const RepeatedInt64Message &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedInt64Message(RepeatedInt64Message &&other) noexcept;
    RepeatedInt64Message &operator =(RepeatedInt64Message &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedInt64Message &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::int64List testRepeatedInt() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedInt(const QtProtobuf::int64List &testRepeatedInt);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedInt64Message &lhs, const RepeatedInt64Message &rhs) noexcept;
    friend bool operator==(const RepeatedInt64Message &lhs, const RepeatedInt64Message &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedInt64Message &lhs, const RepeatedInt64Message &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedInt64Message_QtProtobufData> dptr;
};
namespace RepeatedInt64Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedInt64Message_QtProtobufNested

class RepeatedUInt64Message_QtProtobufData;
class RepeatedUInt64Message : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::uint64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedUInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedUInt64Message();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedUInt64Message();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedUInt64Message(const RepeatedUInt64Message &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedUInt64Message &operator =(const RepeatedUInt64Message &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedUInt64Message(RepeatedUInt64Message &&other) noexcept;
    RepeatedUInt64Message &operator =(RepeatedUInt64Message &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedUInt64Message &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::uint64List testRepeatedInt() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedInt(const QtProtobuf::uint64List &testRepeatedInt);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedUInt64Message &lhs, const RepeatedUInt64Message &rhs) noexcept;
    friend bool operator==(const RepeatedUInt64Message &lhs, const RepeatedUInt64Message &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedUInt64Message &lhs, const RepeatedUInt64Message &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedUInt64Message_QtProtobufData> dptr;
};
namespace RepeatedUInt64Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedUInt64Message_QtProtobufNested

class RepeatedFixedIntMessage_QtProtobufData;
class RepeatedFixedIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::fixed32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedFixedIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFixedIntMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedFixedIntMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFixedIntMessage(const RepeatedFixedIntMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFixedIntMessage &operator =(const RepeatedFixedIntMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFixedIntMessage(RepeatedFixedIntMessage &&other) noexcept;
    RepeatedFixedIntMessage &operator =(RepeatedFixedIntMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedFixedIntMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::fixed32List testRepeatedInt() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedInt(const QtProtobuf::fixed32List &testRepeatedInt);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedFixedIntMessage &lhs, const RepeatedFixedIntMessage &rhs) noexcept;
    friend bool operator==(const RepeatedFixedIntMessage &lhs, const RepeatedFixedIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedFixedIntMessage &lhs, const RepeatedFixedIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedFixedIntMessage_QtProtobufData> dptr;
};
namespace RepeatedFixedIntMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedFixedIntMessage_QtProtobufNested

class RepeatedSFixedIntMessage_QtProtobufData;
class RepeatedSFixedIntMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::sfixed32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedSFixedIntMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSFixedIntMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedSFixedIntMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSFixedIntMessage(const RepeatedSFixedIntMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSFixedIntMessage &operator =(const RepeatedSFixedIntMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSFixedIntMessage(RepeatedSFixedIntMessage &&other) noexcept;
    RepeatedSFixedIntMessage &operator =(RepeatedSFixedIntMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedSFixedIntMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::sfixed32List testRepeatedInt() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedInt(const QtProtobuf::sfixed32List &testRepeatedInt);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedSFixedIntMessage &lhs, const RepeatedSFixedIntMessage &rhs) noexcept;
    friend bool operator==(const RepeatedSFixedIntMessage &lhs, const RepeatedSFixedIntMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedSFixedIntMessage &lhs, const RepeatedSFixedIntMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedSFixedIntMessage_QtProtobufData> dptr;
};
namespace RepeatedSFixedIntMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedSFixedIntMessage_QtProtobufNested

class RepeatedFixedInt64Message_QtProtobufData;
class RepeatedFixedInt64Message : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::fixed64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedFixedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFixedInt64Message();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedFixedInt64Message();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFixedInt64Message(const RepeatedFixedInt64Message &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFixedInt64Message &operator =(const RepeatedFixedInt64Message &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedFixedInt64Message(RepeatedFixedInt64Message &&other) noexcept;
    RepeatedFixedInt64Message &operator =(RepeatedFixedInt64Message &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedFixedInt64Message &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::fixed64List testRepeatedInt() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedInt(const QtProtobuf::fixed64List &testRepeatedInt);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedFixedInt64Message &lhs, const RepeatedFixedInt64Message &rhs) noexcept;
    friend bool operator==(const RepeatedFixedInt64Message &lhs, const RepeatedFixedInt64Message &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedFixedInt64Message &lhs, const RepeatedFixedInt64Message &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedFixedInt64Message_QtProtobufData> dptr;
};
namespace RepeatedFixedInt64Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedFixedInt64Message_QtProtobufNested

class RepeatedSFixedInt64Message_QtProtobufData;
class RepeatedSFixedInt64Message : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::sfixed64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedSFixedInt64Message_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSFixedInt64Message();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedSFixedInt64Message();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSFixedInt64Message(const RepeatedSFixedInt64Message &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSFixedInt64Message &operator =(const RepeatedSFixedInt64Message &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedSFixedInt64Message(RepeatedSFixedInt64Message &&other) noexcept;
    RepeatedSFixedInt64Message &operator =(RepeatedSFixedInt64Message &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedSFixedInt64Message &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::sfixed64List testRepeatedInt() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedInt(const QtProtobuf::sfixed64List &testRepeatedInt);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedSFixedInt64Message &lhs, const RepeatedSFixedInt64Message &rhs) noexcept;
    friend bool operator==(const RepeatedSFixedInt64Message &lhs, const RepeatedSFixedInt64Message &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedSFixedInt64Message &lhs, const RepeatedSFixedInt64Message &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedSFixedInt64Message_QtProtobufData> dptr;
};
namespace RepeatedSFixedInt64Message_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedIntProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedSFixedInt64Message_QtProtobufNested

class RepeatedBoolMessage_QtProtobufData;
class RepeatedBoolMessage : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)
    Q_PROPERTY(QtProtobuf::boolList testRepeatedBool READ testRepeatedBool WRITE setTestRepeatedBool SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = RepeatedBoolMessage_QtProtobufNested::QtProtobufFieldEnum;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedBoolMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT ~RepeatedBoolMessage();
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedBoolMessage(const RepeatedBoolMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedBoolMessage &operator =(const RepeatedBoolMessage &other);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT RepeatedBoolMessage(RepeatedBoolMessage &&other) noexcept;
    RepeatedBoolMessage &operator =(RepeatedBoolMessage &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(RepeatedBoolMessage &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }

    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT QtProtobuf::boolList testRepeatedBool() const;
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT void setTestRepeatedBool(const QtProtobuf::boolList &testRepeatedBool);
    QPB_TST_QTPROTOBUFGEN_GEN_EXPORT static void registerTypes();

private:
    friend QPB_TST_QTPROTOBUFGEN_GEN_EXPORT bool comparesEqual(const RepeatedBoolMessage &lhs, const RepeatedBoolMessage &rhs) noexcept;
    friend bool operator==(const RepeatedBoolMessage &lhs, const RepeatedBoolMessage &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const RepeatedBoolMessage &lhs, const RepeatedBoolMessage &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<RepeatedBoolMessage_QtProtobufData> dptr;
};
namespace RepeatedBoolMessage_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGEN_GEN_EXPORT)

enum class QtProtobufFieldEnum {
    TestRepeatedBoolProtoFieldNumber = 1,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace RepeatedBoolMessage_QtProtobufNested
} // namespace qtprotobufnamespace::tests

#endif // REPEATEDMESSAGES_QPB_H
