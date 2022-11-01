/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef Q_PROTOBUF_basicmessages_H
#define Q_PROTOBUF_basicmessages_H

#include <QtProtobuf/QProtobufMessage>
#include <QtCore/QMetaType>
#include <QtCore/QList>

#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>

#include <memory>

#include <QByteArray>
#include <QString>


namespace qtprotobufnamespace::tests {
class EmptyMessage;
using EmptyMessageRepeated = QList<std::shared_ptr<EmptyMessage>>;
class SimpleBoolMessage;
using SimpleBoolMessageRepeated = QList<std::shared_ptr<SimpleBoolMessage>>;
class SimpleIntMessage;
using SimpleIntMessageRepeated = QList<std::shared_ptr<SimpleIntMessage>>;
class SimpleSIntMessage;
using SimpleSIntMessageRepeated = QList<std::shared_ptr<SimpleSIntMessage>>;
class SimpleUIntMessage;
using SimpleUIntMessageRepeated = QList<std::shared_ptr<SimpleUIntMessage>>;
class SimpleInt64Message;
using SimpleInt64MessageRepeated = QList<std::shared_ptr<SimpleInt64Message>>;
class SimpleSInt64Message;
using SimpleSInt64MessageRepeated = QList<std::shared_ptr<SimpleSInt64Message>>;
class SimpleUInt64Message;
using SimpleUInt64MessageRepeated = QList<std::shared_ptr<SimpleUInt64Message>>;
class SimpleStringMessage;
using SimpleStringMessageRepeated = QList<std::shared_ptr<SimpleStringMessage>>;
class SimpleFloatMessage;
using SimpleFloatMessageRepeated = QList<std::shared_ptr<SimpleFloatMessage>>;
class SimpleDoubleMessage;
using SimpleDoubleMessageRepeated = QList<std::shared_ptr<SimpleDoubleMessage>>;
class SimpleBytesMessage;
using SimpleBytesMessageRepeated = QList<std::shared_ptr<SimpleBytesMessage>>;
class SimpleFixedInt32Message;
using SimpleFixedInt32MessageRepeated = QList<std::shared_ptr<SimpleFixedInt32Message>>;
class SimpleFixedInt64Message;
using SimpleFixedInt64MessageRepeated = QList<std::shared_ptr<SimpleFixedInt64Message>>;
class SimpleSFixedInt32Message;
using SimpleSFixedInt32MessageRepeated = QList<std::shared_ptr<SimpleSFixedInt32Message>>;
class SimpleSFixedInt64Message;
using SimpleSFixedInt64MessageRepeated = QList<std::shared_ptr<SimpleSFixedInt64Message>>;
class ComplexMessage;
using ComplexMessageRepeated = QList<std::shared_ptr<ComplexMessage>>;

class  EmptyMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(EmptyMessage)

public:
    EmptyMessage();
    ~EmptyMessage() override;
    EmptyMessage(const EmptyMessage &other);
    EmptyMessage &operator =(const EmptyMessage &other);
    EmptyMessage(EmptyMessage &&other) noexcept;
    EmptyMessage &operator =(EmptyMessage &&other) noexcept;
    bool operator ==(const EmptyMessage &other) const;
    bool operator !=(const EmptyMessage &other) const;
    static void registerTypes();

private:
};


class  SimpleBoolMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleBoolMessage)
    Q_PROPERTY(bool testFieldBool READ testFieldBool WRITE setTestFieldBool SCRIPTABLE true)

public:
    SimpleBoolMessage();
    ~SimpleBoolMessage() override;
    SimpleBoolMessage(const SimpleBoolMessage &other);
    SimpleBoolMessage &operator =(const SimpleBoolMessage &other);
    SimpleBoolMessage(SimpleBoolMessage &&other) noexcept;
    SimpleBoolMessage &operator =(SimpleBoolMessage &&other) noexcept;
    bool operator ==(const SimpleBoolMessage &other) const;
    bool operator !=(const SimpleBoolMessage &other) const;

    bool testFieldBool() const {
        return m_testFieldBool;
    }

    void setTestFieldBool(const bool &testFieldBool) {
        if (m_testFieldBool != testFieldBool)
            m_testFieldBool = testFieldBool;
    }

    static void registerTypes();

private:
    bool m_testFieldBool;
};


class  SimpleIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleIntMessage)
    Q_PROPERTY(QtProtobuf::int32 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE true)

public:
    SimpleIntMessage();
    ~SimpleIntMessage() override;
    SimpleIntMessage(const SimpleIntMessage &other);
    SimpleIntMessage &operator =(const SimpleIntMessage &other);
    SimpleIntMessage(SimpleIntMessage &&other) noexcept;
    SimpleIntMessage &operator =(SimpleIntMessage &&other) noexcept;
    bool operator ==(const SimpleIntMessage &other) const;
    bool operator !=(const SimpleIntMessage &other) const;

    QtProtobuf::int32 testFieldInt() const {
        return m_testFieldInt;
    }

    void setTestFieldInt(const QtProtobuf::int32 &testFieldInt) {
        if (m_testFieldInt != testFieldInt)
            m_testFieldInt = testFieldInt;
    }

    static void registerTypes();

private:
    QtProtobuf::int32 m_testFieldInt;
};


class  SimpleSIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleSIntMessage)
    Q_PROPERTY(QtProtobuf::sint32 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE true)

public:
    SimpleSIntMessage();
    ~SimpleSIntMessage() override;
    SimpleSIntMessage(const SimpleSIntMessage &other);
    SimpleSIntMessage &operator =(const SimpleSIntMessage &other);
    SimpleSIntMessage(SimpleSIntMessage &&other) noexcept;
    SimpleSIntMessage &operator =(SimpleSIntMessage &&other) noexcept;
    bool operator ==(const SimpleSIntMessage &other) const;
    bool operator !=(const SimpleSIntMessage &other) const;

    QtProtobuf::sint32 testFieldInt() const {
        return m_testFieldInt;
    }

    void setTestFieldInt(const QtProtobuf::sint32 &testFieldInt) {
        if (m_testFieldInt != testFieldInt)
            m_testFieldInt = testFieldInt;
    }

    static void registerTypes();

private:
    QtProtobuf::sint32 m_testFieldInt;
};


class  SimpleUIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleUIntMessage)
    Q_PROPERTY(QtProtobuf::uint32 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE true)

public:
    SimpleUIntMessage();
    ~SimpleUIntMessage() override;
    SimpleUIntMessage(const SimpleUIntMessage &other);
    SimpleUIntMessage &operator =(const SimpleUIntMessage &other);
    SimpleUIntMessage(SimpleUIntMessage &&other) noexcept;
    SimpleUIntMessage &operator =(SimpleUIntMessage &&other) noexcept;
    bool operator ==(const SimpleUIntMessage &other) const;
    bool operator !=(const SimpleUIntMessage &other) const;

    QtProtobuf::uint32 testFieldInt() const {
        return m_testFieldInt;
    }

    void setTestFieldInt(const QtProtobuf::uint32 &testFieldInt) {
        if (m_testFieldInt != testFieldInt)
            m_testFieldInt = testFieldInt;
    }

    static void registerTypes();

private:
    QtProtobuf::uint32 m_testFieldInt;
};


class  SimpleInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleInt64Message)
    Q_PROPERTY(QtProtobuf::int64 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE false)

public:
    SimpleInt64Message();
    ~SimpleInt64Message() override;
    SimpleInt64Message(const SimpleInt64Message &other);
    SimpleInt64Message &operator =(const SimpleInt64Message &other);
    SimpleInt64Message(SimpleInt64Message &&other) noexcept;
    SimpleInt64Message &operator =(SimpleInt64Message &&other) noexcept;
    bool operator ==(const SimpleInt64Message &other) const;
    bool operator !=(const SimpleInt64Message &other) const;

    QtProtobuf::int64 testFieldInt() const {
        return m_testFieldInt;
    }

    void setTestFieldInt(const QtProtobuf::int64 &testFieldInt) {
        if (m_testFieldInt != testFieldInt)
            m_testFieldInt = testFieldInt;
    }

    static void registerTypes();

private:
    QtProtobuf::int64 m_testFieldInt;
};


class  SimpleSInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleSInt64Message)
    Q_PROPERTY(QtProtobuf::sint64 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE false)

public:
    SimpleSInt64Message();
    ~SimpleSInt64Message() override;
    SimpleSInt64Message(const SimpleSInt64Message &other);
    SimpleSInt64Message &operator =(const SimpleSInt64Message &other);
    SimpleSInt64Message(SimpleSInt64Message &&other) noexcept;
    SimpleSInt64Message &operator =(SimpleSInt64Message &&other) noexcept;
    bool operator ==(const SimpleSInt64Message &other) const;
    bool operator !=(const SimpleSInt64Message &other) const;

    QtProtobuf::sint64 testFieldInt() const {
        return m_testFieldInt;
    }

    void setTestFieldInt(const QtProtobuf::sint64 &testFieldInt) {
        if (m_testFieldInt != testFieldInt)
            m_testFieldInt = testFieldInt;
    }

    static void registerTypes();

private:
    QtProtobuf::sint64 m_testFieldInt;
};


class  SimpleUInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleUInt64Message)
    Q_PROPERTY(QtProtobuf::uint64 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE true)

public:
    SimpleUInt64Message();
    ~SimpleUInt64Message() override;
    SimpleUInt64Message(const SimpleUInt64Message &other);
    SimpleUInt64Message &operator =(const SimpleUInt64Message &other);
    SimpleUInt64Message(SimpleUInt64Message &&other) noexcept;
    SimpleUInt64Message &operator =(SimpleUInt64Message &&other) noexcept;
    bool operator ==(const SimpleUInt64Message &other) const;
    bool operator !=(const SimpleUInt64Message &other) const;

    QtProtobuf::uint64 testFieldInt() const {
        return m_testFieldInt;
    }

    void setTestFieldInt(const QtProtobuf::uint64 &testFieldInt) {
        if (m_testFieldInt != testFieldInt)
            m_testFieldInt = testFieldInt;
    }

    static void registerTypes();

private:
    QtProtobuf::uint64 m_testFieldInt;
};


class  SimpleStringMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleStringMessage)
    Q_PROPERTY(QString testFieldString READ testFieldString WRITE setTestFieldString SCRIPTABLE true)

public:
    SimpleStringMessage();
    ~SimpleStringMessage() override;
    SimpleStringMessage(const SimpleStringMessage &other);
    SimpleStringMessage &operator =(const SimpleStringMessage &other);
    SimpleStringMessage(SimpleStringMessage &&other) noexcept;
    SimpleStringMessage &operator =(SimpleStringMessage &&other) noexcept;
    bool operator ==(const SimpleStringMessage &other) const;
    bool operator !=(const SimpleStringMessage &other) const;

    QString testFieldString() const {
        return m_testFieldString;
    }

    void setTestFieldString(const QString &testFieldString);
    static void registerTypes();

private:
    QString m_testFieldString;
};


class  SimpleFloatMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleFloatMessage)
    Q_PROPERTY(float testFieldFloat READ testFieldFloat WRITE setTestFieldFloat SCRIPTABLE true)

public:
    SimpleFloatMessage();
    ~SimpleFloatMessage() override;
    SimpleFloatMessage(const SimpleFloatMessage &other);
    SimpleFloatMessage &operator =(const SimpleFloatMessage &other);
    SimpleFloatMessage(SimpleFloatMessage &&other) noexcept;
    SimpleFloatMessage &operator =(SimpleFloatMessage &&other) noexcept;
    bool operator ==(const SimpleFloatMessage &other) const;
    bool operator !=(const SimpleFloatMessage &other) const;

    float testFieldFloat() const {
        return m_testFieldFloat;
    }

    void setTestFieldFloat(const float &testFieldFloat) {
        if (m_testFieldFloat != testFieldFloat)
            m_testFieldFloat = testFieldFloat;
    }

    static void registerTypes();

private:
    float m_testFieldFloat;
};


class  SimpleDoubleMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleDoubleMessage)
    Q_PROPERTY(double testFieldDouble READ testFieldDouble WRITE setTestFieldDouble SCRIPTABLE true)

public:
    SimpleDoubleMessage();
    ~SimpleDoubleMessage() override;
    SimpleDoubleMessage(const SimpleDoubleMessage &other);
    SimpleDoubleMessage &operator =(const SimpleDoubleMessage &other);
    SimpleDoubleMessage(SimpleDoubleMessage &&other) noexcept;
    SimpleDoubleMessage &operator =(SimpleDoubleMessage &&other) noexcept;
    bool operator ==(const SimpleDoubleMessage &other) const;
    bool operator !=(const SimpleDoubleMessage &other) const;

    double testFieldDouble() const {
        return m_testFieldDouble;
    }

    void setTestFieldDouble(const double &testFieldDouble) {
        if (m_testFieldDouble != testFieldDouble)
            m_testFieldDouble = testFieldDouble;
    }

    static void registerTypes();

private:
    double m_testFieldDouble;
};


class  SimpleBytesMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleBytesMessage)
    Q_PROPERTY(QByteArray testFieldBytes READ testFieldBytes WRITE setTestFieldBytes SCRIPTABLE true)

public:
    SimpleBytesMessage();
    ~SimpleBytesMessage() override;
    SimpleBytesMessage(const SimpleBytesMessage &other);
    SimpleBytesMessage &operator =(const SimpleBytesMessage &other);
    SimpleBytesMessage(SimpleBytesMessage &&other) noexcept;
    SimpleBytesMessage &operator =(SimpleBytesMessage &&other) noexcept;
    bool operator ==(const SimpleBytesMessage &other) const;
    bool operator !=(const SimpleBytesMessage &other) const;

    QByteArray testFieldBytes() const {
        return m_testFieldBytes;
    }

    void setTestFieldBytes(const QByteArray &testFieldBytes);
    static void registerTypes();

private:
    QByteArray m_testFieldBytes;
};


class  SimpleFixedInt32Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleFixedInt32Message)
    Q_PROPERTY(QtProtobuf::fixed32 testFieldFixedInt32 READ testFieldFixedInt32 WRITE setTestFieldFixedInt32 SCRIPTABLE true)

public:
    SimpleFixedInt32Message();
    ~SimpleFixedInt32Message() override;
    SimpleFixedInt32Message(const SimpleFixedInt32Message &other);
    SimpleFixedInt32Message &operator =(const SimpleFixedInt32Message &other);
    SimpleFixedInt32Message(SimpleFixedInt32Message &&other) noexcept;
    SimpleFixedInt32Message &operator =(SimpleFixedInt32Message &&other) noexcept;
    bool operator ==(const SimpleFixedInt32Message &other) const;
    bool operator !=(const SimpleFixedInt32Message &other) const;

    QtProtobuf::fixed32 testFieldFixedInt32() const {
        return m_testFieldFixedInt32;
    }

    void setTestFieldFixedInt32(const QtProtobuf::fixed32 &testFieldFixedInt32) {
        if (m_testFieldFixedInt32 != testFieldFixedInt32)
            m_testFieldFixedInt32 = testFieldFixedInt32;
    }

    static void registerTypes();

private:
    QtProtobuf::fixed32 m_testFieldFixedInt32;
};


class  SimpleFixedInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleFixedInt64Message)
    Q_PROPERTY(QtProtobuf::fixed64 testFieldFixedInt64 READ testFieldFixedInt64 WRITE setTestFieldFixedInt64 SCRIPTABLE false)

public:
    SimpleFixedInt64Message();
    ~SimpleFixedInt64Message() override;
    SimpleFixedInt64Message(const SimpleFixedInt64Message &other);
    SimpleFixedInt64Message &operator =(const SimpleFixedInt64Message &other);
    SimpleFixedInt64Message(SimpleFixedInt64Message &&other) noexcept;
    SimpleFixedInt64Message &operator =(SimpleFixedInt64Message &&other) noexcept;
    bool operator ==(const SimpleFixedInt64Message &other) const;
    bool operator !=(const SimpleFixedInt64Message &other) const;

    QtProtobuf::fixed64 testFieldFixedInt64() const {
        return m_testFieldFixedInt64;
    }

    void setTestFieldFixedInt64(const QtProtobuf::fixed64 &testFieldFixedInt64) {
        if (m_testFieldFixedInt64 != testFieldFixedInt64)
            m_testFieldFixedInt64 = testFieldFixedInt64;
    }

    static void registerTypes();

private:
    QtProtobuf::fixed64 m_testFieldFixedInt64;
};


class  SimpleSFixedInt32Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleSFixedInt32Message)
    Q_PROPERTY(QtProtobuf::sfixed32 testFieldFixedInt32 READ testFieldFixedInt32 WRITE setTestFieldFixedInt32 SCRIPTABLE true)

public:
    SimpleSFixedInt32Message();
    ~SimpleSFixedInt32Message() override;
    SimpleSFixedInt32Message(const SimpleSFixedInt32Message &other);
    SimpleSFixedInt32Message &operator =(const SimpleSFixedInt32Message &other);
    SimpleSFixedInt32Message(SimpleSFixedInt32Message &&other) noexcept;
    SimpleSFixedInt32Message &operator =(SimpleSFixedInt32Message &&other) noexcept;
    bool operator ==(const SimpleSFixedInt32Message &other) const;
    bool operator !=(const SimpleSFixedInt32Message &other) const;

    QtProtobuf::sfixed32 testFieldFixedInt32() const {
        return m_testFieldFixedInt32;
    }

    void setTestFieldFixedInt32(const QtProtobuf::sfixed32 &testFieldFixedInt32) {
        if (m_testFieldFixedInt32 != testFieldFixedInt32)
            m_testFieldFixedInt32 = testFieldFixedInt32;
    }

    static void registerTypes();

private:
    QtProtobuf::sfixed32 m_testFieldFixedInt32;
};


class  SimpleSFixedInt64Message : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleSFixedInt64Message)
    Q_PROPERTY(QtProtobuf::sfixed64 testFieldFixedInt64 READ testFieldFixedInt64 WRITE setTestFieldFixedInt64 SCRIPTABLE false)

public:
    SimpleSFixedInt64Message();
    ~SimpleSFixedInt64Message() override;
    SimpleSFixedInt64Message(const SimpleSFixedInt64Message &other);
    SimpleSFixedInt64Message &operator =(const SimpleSFixedInt64Message &other);
    SimpleSFixedInt64Message(SimpleSFixedInt64Message &&other) noexcept;
    SimpleSFixedInt64Message &operator =(SimpleSFixedInt64Message &&other) noexcept;
    bool operator ==(const SimpleSFixedInt64Message &other) const;
    bool operator !=(const SimpleSFixedInt64Message &other) const;

    QtProtobuf::sfixed64 testFieldFixedInt64() const {
        return m_testFieldFixedInt64;
    }

    void setTestFieldFixedInt64(const QtProtobuf::sfixed64 &testFieldFixedInt64) {
        if (m_testFieldFixedInt64 != testFieldFixedInt64)
            m_testFieldFixedInt64 = testFieldFixedInt64;
    }

    static void registerTypes();

private:
    QtProtobuf::sfixed64 m_testFieldFixedInt64;
};


class  ComplexMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(ComplexMessage)
    Q_PROPERTY(QtProtobuf::int32 testFieldInt READ testFieldInt WRITE setTestFieldInt SCRIPTABLE true)
    Q_PROPERTY(qtprotobufnamespace::tests::SimpleStringMessage *testComplexField READ testComplexField_p WRITE setTestComplexField_p)

public:
    ComplexMessage();
    ~ComplexMessage() override;
    ComplexMessage(const ComplexMessage &other);
    ComplexMessage &operator =(const ComplexMessage &other);
    ComplexMessage(ComplexMessage &&other) noexcept;
    ComplexMessage &operator =(ComplexMessage &&other) noexcept;
    bool operator ==(const ComplexMessage &other) const;
    bool operator !=(const ComplexMessage &other) const;

    QtProtobuf::int32 testFieldInt() const {
        return m_testFieldInt;
    }


    SimpleStringMessage &testComplexField() const;
    void setTestFieldInt(const QtProtobuf::int32 &testFieldInt) {
        if (m_testFieldInt != testFieldInt)
            m_testFieldInt = testFieldInt;
    }

    void setTestComplexField(const SimpleStringMessage &testComplexField);
    static void registerTypes();

private:
    SimpleStringMessage *testComplexField_p() const;
    void setTestComplexField_p(SimpleStringMessage *testComplexField);
    QtProtobuf::int32 m_testFieldInt;
    QtProtobufPrivate::QProtobufLazyMessagePointer<SimpleStringMessage> m_testComplexField;
};

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
#endif // Q_PROTOBUF_basicmessages_H
