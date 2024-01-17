/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#include "extranamespace.qpb.h"
#include <QProtobufSerializer>

namespace qtprotobufnamespace::tests {
EmptyMessage::~EmptyMessage() = default;

static constexpr struct {
    QtProtobufPrivate::QProtobufPropertyOrdering::Data data;
    const std::array<uint, 1> qt_protobuf_EmptyMessage_uint_data;
    const char qt_protobuf_EmptyMessage_char_data[1];
} qt_protobuf_EmptyMessage_metadata {
    // data
    {
        0, /* = version */
        0, /* = num fields */
        1, /* = field number offset */
        1, /* = property index offset */
    },
    // uint_data
    {
        // JSON name offsets:
        0, /* = end-of-string-marker */
        // Field numbers:
        // Property indices:
    },
    // char_data
    ""
};

const QtProtobufPrivate::QProtobufPropertyOrdering EmptyMessage::staticPropertyOrdering = {
    &qt_protobuf_EmptyMessage_metadata.data
};

void EmptyMessage::registerTypes()
{
    qRegisterMetaType<EmptyMessage>();
    qRegisterMetaType<EmptyMessageRepeated>();
}
EmptyMessage::EmptyMessage(QObject *parent) : QObject(parent)
{
}
EmptyMessage::EmptyMessage(const EmptyMessage &) : QObject() {}
EmptyMessage &EmptyMessage::operator =(const EmptyMessage &) { return *this; }
EmptyMessage::EmptyMessage(EmptyMessage &&) noexcept : QObject() {}
EmptyMessage &EmptyMessage::operator =(EmptyMessage &&) noexcept { return *this; }
bool EmptyMessage::operator ==(const EmptyMessage &) const
{
    return true;
}

bool EmptyMessage::operator !=(const EmptyMessage &other) const
{
    return !this->operator ==(other);
}

SimpleStringMessage::~SimpleStringMessage() = default;

static constexpr struct {
    QtProtobufPrivate::QProtobufPropertyOrdering::Data data;
    const std::array<uint, 4> qt_protobuf_SimpleStringMessage_uint_data;
    const char qt_protobuf_SimpleStringMessage_char_data[17];
} qt_protobuf_SimpleStringMessage_metadata {
    // data
    {
        0, /* = version */
        1, /* = num fields */
        2, /* = field number offset */
        3, /* = property index offset */
    },
    // uint_data
    {
        // JSON name offsets:
        0, /* = testFieldString */
        16, /* = end-of-string-marker */
        // Field numbers:
        6, /* = testFieldString */
        // Property indices:
        1, /* = testFieldString */
    },
    // char_data
    "testFieldString\0"
};

const QtProtobufPrivate::QProtobufPropertyOrdering SimpleStringMessage::staticPropertyOrdering = {
    &qt_protobuf_SimpleStringMessage_metadata.data
};

void SimpleStringMessage::registerTypes()
{
    qRegisterMetaType<SimpleStringMessage>();
    qRegisterMetaType<SimpleStringMessageRepeated>();
}
SimpleStringMessage::SimpleStringMessage(QObject *parent) : QObject(parent)
{
}
SimpleStringMessage::SimpleStringMessage(const SimpleStringMessage &other) : QObject()
{
    setTestFieldString(other.m_testFieldString);
}
SimpleStringMessage &SimpleStringMessage::operator =(const SimpleStringMessage &other)
{
    setTestFieldString(other.m_testFieldString);
    return *this;
}
SimpleStringMessage::SimpleStringMessage(SimpleStringMessage &&other) noexcept : QObject()
{
    m_testFieldString = std::move(other.m_testFieldString);
    other.testFieldStringChanged();
}
SimpleStringMessage &SimpleStringMessage::operator =(SimpleStringMessage &&other) noexcept
{
    if (m_testFieldString != other.m_testFieldString) {
        m_testFieldString = std::move(other.m_testFieldString);
        testFieldStringChanged();
        other.testFieldStringChanged();
    }
    return *this;
}
bool SimpleStringMessage::operator ==(const SimpleStringMessage &other) const
{
    return m_testFieldString == other.m_testFieldString;
}
bool SimpleStringMessage::operator !=(const SimpleStringMessage &other) const
{
    return !this->operator ==(other);
}

void SimpleStringMessage::setTestFieldString(const QString &testFieldString)
{
    if (m_testFieldString != testFieldString) {
        m_testFieldString = testFieldString;
        testFieldStringChanged();
    }
}

ComplexMessage::~ComplexMessage() = default;

static constexpr struct {
    QtProtobufPrivate::QProtobufPropertyOrdering::Data data;
    const std::array<uint, 7> qt_protobuf_ComplexMessage_uint_data;
    const char qt_protobuf_ComplexMessage_char_data[31];
} qt_protobuf_ComplexMessage_metadata {
    // data
    {
        0, /* = version */
        2, /* = num fields */
        3, /* = field number offset */
        5, /* = property index offset */
    },
    // uint_data
    {
        // JSON name offsets:
        0, /* = testFieldInt */
        13, /* = testComplexField */
        30, /* = end-of-string-marker */
        // Field numbers:
        1, /* = testFieldInt */
        2, /* = testComplexField */
        // Property indices:
        1, /* = testFieldInt */
        2, /* = testComplexField */
    },
    // char_data
    "testFieldInt\0testComplexField\0"
};

const QtProtobufPrivate::QProtobufPropertyOrdering ComplexMessage::staticPropertyOrdering = {
    &qt_protobuf_ComplexMessage_metadata.data
};

void ComplexMessage::registerTypes()
{
    qRegisterMetaType<ComplexMessage>();
    qRegisterMetaType<ComplexMessageRepeated>();
}
ComplexMessage::ComplexMessage(QObject *parent) : QObject(parent),
    m_testFieldInt(0),
    m_testComplexField(nullptr)
{
}
ComplexMessage::ComplexMessage(const ComplexMessage &other) : QObject(),
    m_testComplexField(nullptr)
{
    setTestFieldInt(other.m_testFieldInt);
    if (m_testComplexField != other.m_testComplexField) {
        *m_testComplexField = *other.m_testComplexField;
    }
}
ComplexMessage &ComplexMessage::operator =(const ComplexMessage &other)
{
    setTestFieldInt(other.m_testFieldInt);
    if (m_testComplexField != other.m_testComplexField) {
        *m_testComplexField = *other.m_testComplexField;
        testComplexFieldChanged();
    }
    return *this;
}
ComplexMessage::ComplexMessage(ComplexMessage &&other) noexcept : QObject(),
    m_testComplexField(nullptr)
{
    setTestFieldInt(std::exchange(other.m_testFieldInt, 0));
    other.testFieldIntChanged();
    if (m_testComplexField != other.m_testComplexField) {
        *m_testComplexField = std::move(*other.m_testComplexField);
    }
}
ComplexMessage &ComplexMessage::operator =(ComplexMessage &&other) noexcept
{
    setTestFieldInt(std::exchange(other.m_testFieldInt, 0));
    other.testFieldIntChanged();
    if (m_testComplexField != other.m_testComplexField) {
        *m_testComplexField = std::move(*other.m_testComplexField);
        testComplexFieldChanged();
        other.testComplexFieldChanged();
    }
    return *this;
}
bool ComplexMessage::operator ==(const ComplexMessage &other) const
{
    return m_testFieldInt == other.m_testFieldInt
        && (m_testComplexField == other.m_testComplexField
            || *m_testComplexField == *other.m_testComplexField)
;
}
bool ComplexMessage::operator !=(const ComplexMessage &other) const
{
    return !this->operator ==(other);
}

SimpleStringMessage *ComplexMessage::testComplexField_p() const
{
    return m_testComplexField.get();
}

SimpleStringMessage &ComplexMessage::testComplexField() const
{
    return *m_testComplexField;
}

void ComplexMessage::setTestComplexField_p(SimpleStringMessage *testComplexField)
{
    if (m_testComplexField.get() != testComplexField) {
        m_testComplexField.reset(testComplexField);
        testComplexFieldChanged();
    }
}

void ComplexMessage::setTestComplexField(const SimpleStringMessage &testComplexField)
{
    if (*m_testComplexField != testComplexField) {
        *m_testComplexField = testComplexField;
        testComplexFieldChanged();
    }
}

} // namespace qtprotobufnamespace::tests

#include "moc_extranamespace.qpb.cpp"
