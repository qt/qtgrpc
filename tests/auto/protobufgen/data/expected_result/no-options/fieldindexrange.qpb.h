/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_FIELDINDEXRANGE_H
#define QPROTOBUF_FIELDINDEXRANGE_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>

#include <QtCore/qmetatype.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>

#include <memory>


namespace qtprotobufnamespace::tests {
class FieldIndexTest1Message;
using FieldIndexTest1MessageRepeated = QList<FieldIndexTest1Message>;
class FieldIndexTest2Message;
using FieldIndexTest2MessageRepeated = QList<FieldIndexTest2Message>;
class FieldIndexTest3Message;
using FieldIndexTest3MessageRepeated = QList<FieldIndexTest3Message>;
class FieldIndexTest4Message;
using FieldIndexTest4MessageRepeated = QList<FieldIndexTest4Message>;

class FieldIndexTest1Message_QtProtobufData;
class FieldIndexTest1Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(FieldIndexTest1Message)
    Q_PROPERTY(QtProtobuf::sint32 testField READ testField WRITE setTestField SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestFieldProtoFieldNumber = 31,
    };
    Q_ENUM(QtProtobufFieldEnum)

    FieldIndexTest1Message();
    ~FieldIndexTest1Message();
    FieldIndexTest1Message(const FieldIndexTest1Message &other);
    FieldIndexTest1Message &operator =(const FieldIndexTest1Message &other);
    FieldIndexTest1Message(FieldIndexTest1Message &&other) noexcept;
    FieldIndexTest1Message &operator =(FieldIndexTest1Message &&other) noexcept;
    bool operator ==(const FieldIndexTest1Message &other) const;
    bool operator !=(const FieldIndexTest1Message &other) const;

    QtProtobuf::sint32 testField() const;
    void setTestField(const QtProtobuf::sint32 &testField);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<FieldIndexTest1Message_QtProtobufData> dptr;
};

class FieldIndexTest2Message_QtProtobufData;
class FieldIndexTest2Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(FieldIndexTest2Message)
    Q_PROPERTY(QtProtobuf::sint32 testField READ testField WRITE setTestField SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestFieldProtoFieldNumber = 8191,
    };
    Q_ENUM(QtProtobufFieldEnum)

    FieldIndexTest2Message();
    ~FieldIndexTest2Message();
    FieldIndexTest2Message(const FieldIndexTest2Message &other);
    FieldIndexTest2Message &operator =(const FieldIndexTest2Message &other);
    FieldIndexTest2Message(FieldIndexTest2Message &&other) noexcept;
    FieldIndexTest2Message &operator =(FieldIndexTest2Message &&other) noexcept;
    bool operator ==(const FieldIndexTest2Message &other) const;
    bool operator !=(const FieldIndexTest2Message &other) const;

    QtProtobuf::sint32 testField() const;
    void setTestField(const QtProtobuf::sint32 &testField);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<FieldIndexTest2Message_QtProtobufData> dptr;
};

class FieldIndexTest3Message_QtProtobufData;
class FieldIndexTest3Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(FieldIndexTest3Message)
    Q_PROPERTY(QtProtobuf::sint32 testField READ testField WRITE setTestField SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestFieldProtoFieldNumber = 2097151,
    };
    Q_ENUM(QtProtobufFieldEnum)

    FieldIndexTest3Message();
    ~FieldIndexTest3Message();
    FieldIndexTest3Message(const FieldIndexTest3Message &other);
    FieldIndexTest3Message &operator =(const FieldIndexTest3Message &other);
    FieldIndexTest3Message(FieldIndexTest3Message &&other) noexcept;
    FieldIndexTest3Message &operator =(FieldIndexTest3Message &&other) noexcept;
    bool operator ==(const FieldIndexTest3Message &other) const;
    bool operator !=(const FieldIndexTest3Message &other) const;

    QtProtobuf::sint32 testField() const;
    void setTestField(const QtProtobuf::sint32 &testField);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<FieldIndexTest3Message_QtProtobufData> dptr;
};

class FieldIndexTest4Message_QtProtobufData;
class FieldIndexTest4Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(FieldIndexTest4Message)
    Q_PROPERTY(QtProtobuf::sint32 testField READ testField WRITE setTestField SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestFieldProtoFieldNumber = 536870911,
    };
    Q_ENUM(QtProtobufFieldEnum)

    FieldIndexTest4Message();
    ~FieldIndexTest4Message();
    FieldIndexTest4Message(const FieldIndexTest4Message &other);
    FieldIndexTest4Message &operator =(const FieldIndexTest4Message &other);
    FieldIndexTest4Message(FieldIndexTest4Message &&other) noexcept;
    FieldIndexTest4Message &operator =(FieldIndexTest4Message &&other) noexcept;
    bool operator ==(const FieldIndexTest4Message &other) const;
    bool operator !=(const FieldIndexTest4Message &other) const;

    QtProtobuf::sint32 testField() const;
    void setTestField(const QtProtobuf::sint32 &testField);
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<FieldIndexTest4Message_QtProtobufData> dptr;
};
} // namespace qtprotobufnamespace::tests

Q_DECLARE_METATYPE(qtprotobufnamespace::tests::FieldIndexTest1Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::FieldIndexTest2Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::FieldIndexTest3Message)
Q_DECLARE_METATYPE(qtprotobufnamespace::tests::FieldIndexTest4Message)
#endif // QPROTOBUF_FIELDINDEXRANGE_H
