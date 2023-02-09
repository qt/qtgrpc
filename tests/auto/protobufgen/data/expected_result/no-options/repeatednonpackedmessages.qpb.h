/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef Q_PROTOBUF_repeatednonpackedmessages_H
#define Q_PROTOBUF_repeatednonpackedmessages_H

#include <QtProtobuf/QProtobufMessage>
#include <QtCore/QMetaType>
#include <QtCore/QList>

#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>

#include <memory>

#include <QByteArray>
#include <QString>


namespace qtprotobufnamespace::tests {
class RepeatedNonPackedIntMessage;
using RepeatedNonPackedIntMessageRepeated = QList<std::shared_ptr<RepeatedNonPackedIntMessage>>;
class RepeatedNonPackedSIntMessage;
using RepeatedNonPackedSIntMessageRepeated = QList<std::shared_ptr<RepeatedNonPackedSIntMessage>>;
class RepeatedNonPackedUIntMessage;
using RepeatedNonPackedUIntMessageRepeated = QList<std::shared_ptr<RepeatedNonPackedUIntMessage>>;
class RepeatedNonPackedInt64Message;
using RepeatedNonPackedInt64MessageRepeated = QList<std::shared_ptr<RepeatedNonPackedInt64Message>>;
class RepeatedNonPackedSInt64Message;
using RepeatedNonPackedSInt64MessageRepeated = QList<std::shared_ptr<RepeatedNonPackedSInt64Message>>;
class RepeatedNonPackedUInt64Message;
using RepeatedNonPackedUInt64MessageRepeated = QList<std::shared_ptr<RepeatedNonPackedUInt64Message>>;
class RepeatedNonPackedFixedIntMessage;
using RepeatedNonPackedFixedIntMessageRepeated = QList<std::shared_ptr<RepeatedNonPackedFixedIntMessage>>;
class RepeatedNonPackedSFixedIntMessage;
using RepeatedNonPackedSFixedIntMessageRepeated = QList<std::shared_ptr<RepeatedNonPackedSFixedIntMessage>>;
class RepeatedNonPackedFixedInt64Message;
using RepeatedNonPackedFixedInt64MessageRepeated = QList<std::shared_ptr<RepeatedNonPackedFixedInt64Message>>;
class RepeatedNonPackedSFixedInt64Message;
using RepeatedNonPackedSFixedInt64MessageRepeated = QList<std::shared_ptr<RepeatedNonPackedSFixedInt64Message>>;
class RepeatedNonPackedBoolMessage;
using RepeatedNonPackedBoolMessageRepeated = QList<std::shared_ptr<RepeatedNonPackedBoolMessage>>;
class RepeatedNonPackedDoubleMessage;
using RepeatedNonPackedDoubleMessageRepeated = QList<std::shared_ptr<RepeatedNonPackedDoubleMessage>>;
class RepeatedNonPackedFloatMessage;
using RepeatedNonPackedFloatMessageRepeated = QList<std::shared_ptr<RepeatedNonPackedFloatMessage>>;
class NonPackedIntMessageWithExtraMember;
using NonPackedIntMessageWithExtraMemberRepeated = QList<std::shared_ptr<NonPackedIntMessageWithExtraMember>>;

class  RepeatedNonPackedIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedIntMessage)
    Q_PROPERTY(QtProtobuf::int32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedIntMessage();
    ~RepeatedNonPackedIntMessage();
    RepeatedNonPackedIntMessage(const RepeatedNonPackedIntMessage &other);
    RepeatedNonPackedIntMessage &operator =(const RepeatedNonPackedIntMessage &other);
    RepeatedNonPackedIntMessage(RepeatedNonPackedIntMessage &&other) noexcept;
    RepeatedNonPackedIntMessage &operator =(RepeatedNonPackedIntMessage &&other) noexcept;
    bool operator ==(const RepeatedNonPackedIntMessage &other) const;
    bool operator !=(const RepeatedNonPackedIntMessage &other) const;

    QtProtobuf::int32List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::int32List &testRepeatedInt() {
        return m_testRepeatedInt;
    }

    void setTestRepeatedInt(const QtProtobuf::int32List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    static void registerTypes();

private:
    QtProtobuf::int32List m_testRepeatedInt;
};


class  RepeatedNonPackedSIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedSIntMessage)
    Q_PROPERTY(QtProtobuf::sint32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedSIntMessage();
    ~RepeatedNonPackedSIntMessage();
    RepeatedNonPackedSIntMessage(const RepeatedNonPackedSIntMessage &other);
    RepeatedNonPackedSIntMessage &operator =(const RepeatedNonPackedSIntMessage &other);
    RepeatedNonPackedSIntMessage(RepeatedNonPackedSIntMessage &&other) noexcept;
    RepeatedNonPackedSIntMessage &operator =(RepeatedNonPackedSIntMessage &&other) noexcept;
    bool operator ==(const RepeatedNonPackedSIntMessage &other) const;
    bool operator !=(const RepeatedNonPackedSIntMessage &other) const;

    QtProtobuf::sint32List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::sint32List &testRepeatedInt() {
        return m_testRepeatedInt;
    }

    void setTestRepeatedInt(const QtProtobuf::sint32List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    static void registerTypes();

private:
    QtProtobuf::sint32List m_testRepeatedInt;
};


class  RepeatedNonPackedUIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedUIntMessage)
    Q_PROPERTY(QtProtobuf::uint32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedUIntMessage();
    ~RepeatedNonPackedUIntMessage();
    RepeatedNonPackedUIntMessage(const RepeatedNonPackedUIntMessage &other);
    RepeatedNonPackedUIntMessage &operator =(const RepeatedNonPackedUIntMessage &other);
    RepeatedNonPackedUIntMessage(RepeatedNonPackedUIntMessage &&other) noexcept;
    RepeatedNonPackedUIntMessage &operator =(RepeatedNonPackedUIntMessage &&other) noexcept;
    bool operator ==(const RepeatedNonPackedUIntMessage &other) const;
    bool operator !=(const RepeatedNonPackedUIntMessage &other) const;

    QtProtobuf::uint32List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::uint32List &testRepeatedInt() {
        return m_testRepeatedInt;
    }

    void setTestRepeatedInt(const QtProtobuf::uint32List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    static void registerTypes();

private:
    QtProtobuf::uint32List m_testRepeatedInt;
};


class  RepeatedNonPackedInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedInt64Message)
    Q_PROPERTY(QtProtobuf::int64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedInt64Message();
    ~RepeatedNonPackedInt64Message();
    RepeatedNonPackedInt64Message(const RepeatedNonPackedInt64Message &other);
    RepeatedNonPackedInt64Message &operator =(const RepeatedNonPackedInt64Message &other);
    RepeatedNonPackedInt64Message(RepeatedNonPackedInt64Message &&other) noexcept;
    RepeatedNonPackedInt64Message &operator =(RepeatedNonPackedInt64Message &&other) noexcept;
    bool operator ==(const RepeatedNonPackedInt64Message &other) const;
    bool operator !=(const RepeatedNonPackedInt64Message &other) const;

    QtProtobuf::int64List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::int64List &testRepeatedInt() {
        return m_testRepeatedInt;
    }

    void setTestRepeatedInt(const QtProtobuf::int64List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    static void registerTypes();

private:
    QtProtobuf::int64List m_testRepeatedInt;
};


class  RepeatedNonPackedSInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedSInt64Message)
    Q_PROPERTY(QtProtobuf::sint64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedSInt64Message();
    ~RepeatedNonPackedSInt64Message();
    RepeatedNonPackedSInt64Message(const RepeatedNonPackedSInt64Message &other);
    RepeatedNonPackedSInt64Message &operator =(const RepeatedNonPackedSInt64Message &other);
    RepeatedNonPackedSInt64Message(RepeatedNonPackedSInt64Message &&other) noexcept;
    RepeatedNonPackedSInt64Message &operator =(RepeatedNonPackedSInt64Message &&other) noexcept;
    bool operator ==(const RepeatedNonPackedSInt64Message &other) const;
    bool operator !=(const RepeatedNonPackedSInt64Message &other) const;

    QtProtobuf::sint64List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::sint64List &testRepeatedInt() {
        return m_testRepeatedInt;
    }

    void setTestRepeatedInt(const QtProtobuf::sint64List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    static void registerTypes();

private:
    QtProtobuf::sint64List m_testRepeatedInt;
};


class  RepeatedNonPackedUInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedUInt64Message)
    Q_PROPERTY(QtProtobuf::uint64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedUInt64Message();
    ~RepeatedNonPackedUInt64Message();
    RepeatedNonPackedUInt64Message(const RepeatedNonPackedUInt64Message &other);
    RepeatedNonPackedUInt64Message &operator =(const RepeatedNonPackedUInt64Message &other);
    RepeatedNonPackedUInt64Message(RepeatedNonPackedUInt64Message &&other) noexcept;
    RepeatedNonPackedUInt64Message &operator =(RepeatedNonPackedUInt64Message &&other) noexcept;
    bool operator ==(const RepeatedNonPackedUInt64Message &other) const;
    bool operator !=(const RepeatedNonPackedUInt64Message &other) const;

    QtProtobuf::uint64List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::uint64List &testRepeatedInt() {
        return m_testRepeatedInt;
    }

    void setTestRepeatedInt(const QtProtobuf::uint64List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    static void registerTypes();

private:
    QtProtobuf::uint64List m_testRepeatedInt;
};


class  RepeatedNonPackedFixedIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedFixedIntMessage)
    Q_PROPERTY(QtProtobuf::fixed32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedFixedIntMessage();
    ~RepeatedNonPackedFixedIntMessage();
    RepeatedNonPackedFixedIntMessage(const RepeatedNonPackedFixedIntMessage &other);
    RepeatedNonPackedFixedIntMessage &operator =(const RepeatedNonPackedFixedIntMessage &other);
    RepeatedNonPackedFixedIntMessage(RepeatedNonPackedFixedIntMessage &&other) noexcept;
    RepeatedNonPackedFixedIntMessage &operator =(RepeatedNonPackedFixedIntMessage &&other) noexcept;
    bool operator ==(const RepeatedNonPackedFixedIntMessage &other) const;
    bool operator !=(const RepeatedNonPackedFixedIntMessage &other) const;

    QtProtobuf::fixed32List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::fixed32List &testRepeatedInt() {
        return m_testRepeatedInt;
    }

    void setTestRepeatedInt(const QtProtobuf::fixed32List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    static void registerTypes();

private:
    QtProtobuf::fixed32List m_testRepeatedInt;
};


class  RepeatedNonPackedSFixedIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedSFixedIntMessage)
    Q_PROPERTY(QtProtobuf::sfixed32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedSFixedIntMessage();
    ~RepeatedNonPackedSFixedIntMessage();
    RepeatedNonPackedSFixedIntMessage(const RepeatedNonPackedSFixedIntMessage &other);
    RepeatedNonPackedSFixedIntMessage &operator =(const RepeatedNonPackedSFixedIntMessage &other);
    RepeatedNonPackedSFixedIntMessage(RepeatedNonPackedSFixedIntMessage &&other) noexcept;
    RepeatedNonPackedSFixedIntMessage &operator =(RepeatedNonPackedSFixedIntMessage &&other) noexcept;
    bool operator ==(const RepeatedNonPackedSFixedIntMessage &other) const;
    bool operator !=(const RepeatedNonPackedSFixedIntMessage &other) const;

    QtProtobuf::sfixed32List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::sfixed32List &testRepeatedInt() {
        return m_testRepeatedInt;
    }

    void setTestRepeatedInt(const QtProtobuf::sfixed32List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    static void registerTypes();

private:
    QtProtobuf::sfixed32List m_testRepeatedInt;
};


class  RepeatedNonPackedFixedInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedFixedInt64Message)
    Q_PROPERTY(QtProtobuf::fixed64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedFixedInt64Message();
    ~RepeatedNonPackedFixedInt64Message();
    RepeatedNonPackedFixedInt64Message(const RepeatedNonPackedFixedInt64Message &other);
    RepeatedNonPackedFixedInt64Message &operator =(const RepeatedNonPackedFixedInt64Message &other);
    RepeatedNonPackedFixedInt64Message(RepeatedNonPackedFixedInt64Message &&other) noexcept;
    RepeatedNonPackedFixedInt64Message &operator =(RepeatedNonPackedFixedInt64Message &&other) noexcept;
    bool operator ==(const RepeatedNonPackedFixedInt64Message &other) const;
    bool operator !=(const RepeatedNonPackedFixedInt64Message &other) const;

    QtProtobuf::fixed64List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::fixed64List &testRepeatedInt() {
        return m_testRepeatedInt;
    }

    void setTestRepeatedInt(const QtProtobuf::fixed64List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    static void registerTypes();

private:
    QtProtobuf::fixed64List m_testRepeatedInt;
};


class  RepeatedNonPackedSFixedInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedSFixedInt64Message)
    Q_PROPERTY(QtProtobuf::sfixed64List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedSFixedInt64Message();
    ~RepeatedNonPackedSFixedInt64Message();
    RepeatedNonPackedSFixedInt64Message(const RepeatedNonPackedSFixedInt64Message &other);
    RepeatedNonPackedSFixedInt64Message &operator =(const RepeatedNonPackedSFixedInt64Message &other);
    RepeatedNonPackedSFixedInt64Message(RepeatedNonPackedSFixedInt64Message &&other) noexcept;
    RepeatedNonPackedSFixedInt64Message &operator =(RepeatedNonPackedSFixedInt64Message &&other) noexcept;
    bool operator ==(const RepeatedNonPackedSFixedInt64Message &other) const;
    bool operator !=(const RepeatedNonPackedSFixedInt64Message &other) const;

    QtProtobuf::sfixed64List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::sfixed64List &testRepeatedInt() {
        return m_testRepeatedInt;
    }

    void setTestRepeatedInt(const QtProtobuf::sfixed64List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    static void registerTypes();

private:
    QtProtobuf::sfixed64List m_testRepeatedInt;
};


class  RepeatedNonPackedBoolMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedBoolMessage)
    Q_PROPERTY(QtProtobuf::boolList testRepeatedBool READ testRepeatedBool WRITE setTestRepeatedBool SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedBoolProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedBoolMessage();
    ~RepeatedNonPackedBoolMessage();
    RepeatedNonPackedBoolMessage(const RepeatedNonPackedBoolMessage &other);
    RepeatedNonPackedBoolMessage &operator =(const RepeatedNonPackedBoolMessage &other);
    RepeatedNonPackedBoolMessage(RepeatedNonPackedBoolMessage &&other) noexcept;
    RepeatedNonPackedBoolMessage &operator =(RepeatedNonPackedBoolMessage &&other) noexcept;
    bool operator ==(const RepeatedNonPackedBoolMessage &other) const;
    bool operator !=(const RepeatedNonPackedBoolMessage &other) const;

    QtProtobuf::boolList testRepeatedBool() const {
        return m_testRepeatedBool;
    }

    QtProtobuf::boolList &testRepeatedBool() {
        return m_testRepeatedBool;
    }

    void setTestRepeatedBool(const QtProtobuf::boolList &testRepeatedBool) {
        if (m_testRepeatedBool != testRepeatedBool)
            m_testRepeatedBool = testRepeatedBool;
    }

    static void registerTypes();

private:
    QtProtobuf::boolList m_testRepeatedBool;
};


class  RepeatedNonPackedDoubleMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedDoubleMessage)
    Q_PROPERTY(QtProtobuf::doubleList testRepeatedDouble READ testRepeatedDouble WRITE setTestRepeatedDouble SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedDoubleProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedDoubleMessage();
    ~RepeatedNonPackedDoubleMessage();
    RepeatedNonPackedDoubleMessage(const RepeatedNonPackedDoubleMessage &other);
    RepeatedNonPackedDoubleMessage &operator =(const RepeatedNonPackedDoubleMessage &other);
    RepeatedNonPackedDoubleMessage(RepeatedNonPackedDoubleMessage &&other) noexcept;
    RepeatedNonPackedDoubleMessage &operator =(RepeatedNonPackedDoubleMessage &&other) noexcept;
    bool operator ==(const RepeatedNonPackedDoubleMessage &other) const;
    bool operator !=(const RepeatedNonPackedDoubleMessage &other) const;

    QtProtobuf::doubleList testRepeatedDouble() const {
        return m_testRepeatedDouble;
    }

    QtProtobuf::doubleList &testRepeatedDouble() {
        return m_testRepeatedDouble;
    }

    void setTestRepeatedDouble(const QtProtobuf::doubleList &testRepeatedDouble) {
        if (m_testRepeatedDouble != testRepeatedDouble)
            m_testRepeatedDouble = testRepeatedDouble;
    }

    static void registerTypes();

private:
    QtProtobuf::doubleList m_testRepeatedDouble;
};


class  RepeatedNonPackedFloatMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(RepeatedNonPackedFloatMessage)
    Q_PROPERTY(QtProtobuf::floatList testRepeatedFloat READ testRepeatedFloat WRITE setTestRepeatedFloat SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedFloatProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    RepeatedNonPackedFloatMessage();
    ~RepeatedNonPackedFloatMessage();
    RepeatedNonPackedFloatMessage(const RepeatedNonPackedFloatMessage &other);
    RepeatedNonPackedFloatMessage &operator =(const RepeatedNonPackedFloatMessage &other);
    RepeatedNonPackedFloatMessage(RepeatedNonPackedFloatMessage &&other) noexcept;
    RepeatedNonPackedFloatMessage &operator =(RepeatedNonPackedFloatMessage &&other) noexcept;
    bool operator ==(const RepeatedNonPackedFloatMessage &other) const;
    bool operator !=(const RepeatedNonPackedFloatMessage &other) const;

    QtProtobuf::floatList testRepeatedFloat() const {
        return m_testRepeatedFloat;
    }

    QtProtobuf::floatList &testRepeatedFloat() {
        return m_testRepeatedFloat;
    }

    void setTestRepeatedFloat(const QtProtobuf::floatList &testRepeatedFloat) {
        if (m_testRepeatedFloat != testRepeatedFloat)
            m_testRepeatedFloat = testRepeatedFloat;
    }

    static void registerTypes();

private:
    QtProtobuf::floatList m_testRepeatedFloat;
};


class  NonPackedIntMessageWithExtraMember : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(NonPackedIntMessageWithExtraMember)
    Q_PROPERTY(QtProtobuf::int32List testRepeatedInt READ testRepeatedInt WRITE setTestRepeatedInt SCRIPTABLE true)
    Q_PROPERTY(QString extra READ extra WRITE setExtra SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestRepeatedIntProtoFieldNumber = 1,
        ExtraProtoFieldNumber = 2,
    };
    Q_ENUM(QtProtobufFieldEnum)

    NonPackedIntMessageWithExtraMember();
    ~NonPackedIntMessageWithExtraMember();
    NonPackedIntMessageWithExtraMember(const NonPackedIntMessageWithExtraMember &other);
    NonPackedIntMessageWithExtraMember &operator =(const NonPackedIntMessageWithExtraMember &other);
    NonPackedIntMessageWithExtraMember(NonPackedIntMessageWithExtraMember &&other) noexcept;
    NonPackedIntMessageWithExtraMember &operator =(NonPackedIntMessageWithExtraMember &&other) noexcept;
    bool operator ==(const NonPackedIntMessageWithExtraMember &other) const;
    bool operator !=(const NonPackedIntMessageWithExtraMember &other) const;

    QtProtobuf::int32List testRepeatedInt() const {
        return m_testRepeatedInt;
    }

    QtProtobuf::int32List &testRepeatedInt() {
        return m_testRepeatedInt;
    }


    QString extra() const {
        return m_extra;
    }

    void setTestRepeatedInt(const QtProtobuf::int32List &testRepeatedInt) {
        if (m_testRepeatedInt != testRepeatedInt)
            m_testRepeatedInt = testRepeatedInt;
    }

    void setExtra(const QString &extra);
    static void registerTypes();

private:
    QtProtobuf::int32List m_testRepeatedInt;
    QString m_extra;
};

} // namespace qtprotobufnamespace::tests

Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedSIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedUIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedSInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedUInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedFixedIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedSFixedIntMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedFixedInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedSFixedInt64Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedBoolMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedDoubleMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::RepeatedNonPackedFloatMessage)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::NonPackedIntMessageWithExtraMember)
#endif // Q_PROTOBUF_repeatednonpackedmessages_H
