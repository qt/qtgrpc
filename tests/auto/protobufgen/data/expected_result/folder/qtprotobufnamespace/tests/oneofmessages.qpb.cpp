/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#include "qtprotobufnamespace/tests/oneofmessages.qpb.h"
#include <QtProtobuf/qprotobufserializer.h>

namespace qtprotobufnamespace::tests {
OneofSimpleMessage::~OneofSimpleMessage() = default;

static constexpr struct {
    QtProtobufPrivate::QProtobufPropertyOrdering::Data data;
    const std::array<uint, 9> qt_protobuf_OneofSimpleMessage_uint_data;
    const char qt_protobuf_OneofSimpleMessage_char_data[88];
} qt_protobuf_OneofSimpleMessage_metadata {
    // data
    {
        0, /* = version */
        2, /* = num fields */
        3, /* = field number offset */
        5, /* = property index offset */
        7, /* = field flags offset */
        44, /* = message full name length */
    },
    // uint_data
    {
        // JSON name offsets:
        45, /* = testOneofFieldInt */
        63, /* = testOneofFieldSecondInt */
        87, /* = end-of-string-marker */
        // Field numbers:
        1, /* = testOneofFieldInt */
        2, /* = testOneofFieldSecondInt */
        // Property indices:
        0, /* = testOneofFieldInt */
        2, /* = testOneofFieldSecondInt */
        // Field flags:
        QtProtobufPrivate::Oneof, /* = testOneofFieldInt */
        QtProtobufPrivate::Oneof, /* = testOneofFieldSecondInt */
    },
    // char_data
    /* metadata char_data: */
    "qtprotobufnamespace.tests.OneofSimpleMessage\0" /* = full message name */
    /* field char_data: */
    "testOneofFieldInt\0testOneofFieldSecondInt\0"
};

const QtProtobufPrivate::QProtobufPropertyOrdering OneofSimpleMessage::propertyOrdering = {
    &qt_protobuf_OneofSimpleMessage_metadata.data
};

void OneofSimpleMessage::registerTypes()
{
    qRegisterMetaType<OneofSimpleMessage>();
    qRegisterMetaType<OneofSimpleMessageRepeated>();
}

OneofSimpleMessage::OneofSimpleMessage()
    : QProtobufMessage(&OneofSimpleMessage::staticMetaObject)
{
}

OneofSimpleMessage::OneofSimpleMessage(const OneofSimpleMessage &other)
    : QProtobufMessage(other),
      m_testOneof(other.m_testOneof)
{
}

OneofSimpleMessage &OneofSimpleMessage::operator =(const OneofSimpleMessage &other)
{
    QProtobufMessage::operator=(other);
    m_testOneof = other.m_testOneof;
    return *this;
}

OneofSimpleMessage::OneofSimpleMessage(OneofSimpleMessage &&other) noexcept
    : QProtobufMessage(std::move(other)),
    m_testOneof(std::move(other.m_testOneof))

{
}

OneofSimpleMessage &OneofSimpleMessage::operator =(OneofSimpleMessage &&other) noexcept
{
    QProtobufMessage::operator=(std::move(other));
    m_testOneof = std::move(other.m_testOneof);
    return *this;
}

bool OneofSimpleMessage::operator ==(const OneofSimpleMessage &other) const
{
    return QProtobufMessage::isEqual(*this, other)
        && m_testOneof == other.m_testOneof
;
}

bool OneofSimpleMessage::operator !=(const OneofSimpleMessage &other) const
{
    return !this->operator ==(other);
}

QtProtobuf::int32 OneofSimpleMessage::testOneofFieldInt_p() const
{
    return m_testOneof.holdsField(1) ?
        m_testOneof.value<QtProtobuf::int32>() : QtProtobuf::int32(0);
}

bool OneofSimpleMessage::hasTestOneofFieldInt() const
{
    return m_testOneof.holdsField(1);
}
QtProtobuf::int32 OneofSimpleMessage::testOneofFieldInt() const
{
    Q_ASSERT(m_testOneof.holdsField(1));
    return m_testOneof.value<QtProtobuf::int32>();
}

QtProtobuf::int32 OneofSimpleMessage::testOneofFieldSecondInt_p() const
{
    return m_testOneof.holdsField(2) ?
        m_testOneof.value<QtProtobuf::int32>() : QtProtobuf::int32(0);
}

bool OneofSimpleMessage::hasTestOneofFieldSecondInt() const
{
    return m_testOneof.holdsField(2);
}
QtProtobuf::int32 OneofSimpleMessage::testOneofFieldSecondInt() const
{
    Q_ASSERT(m_testOneof.holdsField(2));
    return m_testOneof.value<QtProtobuf::int32>();
}

void OneofSimpleMessage::setTestOneofFieldInt(const QtProtobuf::int32 &testOneofFieldInt)
{
    if (!m_testOneof.isEqual(testOneofFieldInt, 1)) {
        m_testOneof.setValue(testOneofFieldInt, 1);
    }
}

void OneofSimpleMessage::setTestOneofFieldInt_p(QtProtobuf::int32 testOneofFieldInt)
{
    if (!m_testOneof.isEqual(testOneofFieldInt, 1))
        m_testOneof.setValue(testOneofFieldInt, 1);
}

void OneofSimpleMessage::setTestOneofFieldSecondInt(const QtProtobuf::int32 &testOneofFieldSecondInt)
{
    if (!m_testOneof.isEqual(testOneofFieldSecondInt, 2)) {
        m_testOneof.setValue(testOneofFieldSecondInt, 2);
    }
}

void OneofSimpleMessage::setTestOneofFieldSecondInt_p(QtProtobuf::int32 testOneofFieldSecondInt)
{
    if (!m_testOneof.isEqual(testOneofFieldSecondInt, 2))
        m_testOneof.setValue(testOneofFieldSecondInt, 2);
}

int OneofSimpleMessage::testOneofField() const
{
    return m_testOneof.fieldNumber();
}
void OneofSimpleMessage::clearTestOneof()
{
    m_testOneof = QtProtobufPrivate::QProtobufOneof();
}
OneofComplexMessage::~OneofComplexMessage() = default;

static constexpr struct {
    QtProtobufPrivate::QProtobufPropertyOrdering::Data data;
    const std::array<uint, 29> qt_protobuf_OneofComplexMessage_uint_data;
    const char qt_protobuf_OneofComplexMessage_char_data[187];
} qt_protobuf_OneofComplexMessage_metadata {
    // data
    {
        0, /* = version */
        7, /* = num fields */
        8, /* = field number offset */
        15, /* = property index offset */
        22, /* = field flags offset */
        45, /* = message full name length */
    },
    // uint_data
    {
        // JSON name offsets:
        46, /* = testFieldInt */
        59, /* = testOneofFieldInt */
        77, /* = testOneofComplexField */
        99, /* = testOneofSecondComplexField */
        127, /* = secondFieldInt */
        142, /* = secondComplexField */
        161, /* = secondSecondComplexField */
        186, /* = end-of-string-marker */
        // Field numbers:
        1, /* = testFieldInt */
        42, /* = testOneofFieldInt */
        3, /* = testOneofComplexField */
        4, /* = testOneofSecondComplexField */
        43, /* = secondFieldInt */
        5, /* = secondComplexField */
        6, /* = secondSecondComplexField */
        // Property indices:
        0, /* = testFieldInt */
        1, /* = testOneofFieldInt */
        3, /* = testOneofComplexField */
        5, /* = testOneofSecondComplexField */
        7, /* = secondFieldInt */
        9, /* = secondComplexField */
        11, /* = secondSecondComplexField */
        // Field flags:
        QtProtobufPrivate::NoFlags, /* = testFieldInt */
        QtProtobufPrivate::Oneof, /* = testOneofFieldInt */
        QtProtobufPrivate::Oneof, /* = testOneofComplexField */
        QtProtobufPrivate::Oneof, /* = testOneofSecondComplexField */
        QtProtobufPrivate::Oneof, /* = secondFieldInt */
        QtProtobufPrivate::Oneof, /* = secondComplexField */
        QtProtobufPrivate::Oneof, /* = secondSecondComplexField */
    },
    // char_data
    /* metadata char_data: */
    "qtprotobufnamespace.tests.OneofComplexMessage\0" /* = full message name */
    /* field char_data: */
    "testFieldInt\0testOneofFieldInt\0testOneofComplexField\0testOneofSecondComplexField\0secondFieldInt\0"
    "secondComplexField\0secondSecondComplexField\0"
};

const QtProtobufPrivate::QProtobufPropertyOrdering OneofComplexMessage::propertyOrdering = {
    &qt_protobuf_OneofComplexMessage_metadata.data
};

void OneofComplexMessage::registerTypes()
{
    qRegisterMetaType<OneofComplexMessage>();
    qRegisterMetaType<OneofComplexMessageRepeated>();
}

OneofComplexMessage::OneofComplexMessage()
    : QProtobufMessage(&OneofComplexMessage::staticMetaObject),
      m_testFieldInt(0)
{
}

OneofComplexMessage::OneofComplexMessage(const OneofComplexMessage &other)
    : QProtobufMessage(other),
      m_testFieldInt(other.m_testFieldInt),
      m_testOneof(other.m_testOneof),
      m_secondOneof(other.m_secondOneof)
{
}

OneofComplexMessage &OneofComplexMessage::operator =(const OneofComplexMessage &other)
{
    QProtobufMessage::operator=(other);
    setTestFieldInt(other.m_testFieldInt);
    m_testOneof = other.m_testOneof;
    m_secondOneof = other.m_secondOneof;
    return *this;
}

OneofComplexMessage::OneofComplexMessage(OneofComplexMessage &&other) noexcept
    : QProtobufMessage(std::move(other)),
    m_testOneof(std::move(other.m_testOneof))
,
    m_secondOneof(std::move(other.m_secondOneof))

{
    setTestFieldInt(std::exchange(other.m_testFieldInt, 0));
}

OneofComplexMessage &OneofComplexMessage::operator =(OneofComplexMessage &&other) noexcept
{
    QProtobufMessage::operator=(std::move(other));
    setTestFieldInt(std::exchange(other.m_testFieldInt, 0));
    m_testOneof = std::move(other.m_testOneof);
    m_secondOneof = std::move(other.m_secondOneof);
    return *this;
}

bool OneofComplexMessage::operator ==(const OneofComplexMessage &other) const
{
    return QProtobufMessage::isEqual(*this, other)
        && m_testFieldInt == other.m_testFieldInt
        && m_testOneof == other.m_testOneof

        && m_secondOneof == other.m_secondOneof
;
}

bool OneofComplexMessage::operator !=(const OneofComplexMessage &other) const
{
    return !this->operator ==(other);
}

QtProtobuf::int32 OneofComplexMessage::testOneofFieldInt_p() const
{
    return m_testOneof.holdsField(42) ?
        m_testOneof.value<QtProtobuf::int32>() : QtProtobuf::int32(0);
}

bool OneofComplexMessage::hasTestOneofFieldInt() const
{
    return m_testOneof.holdsField(42);
}
QtProtobuf::int32 OneofComplexMessage::testOneofFieldInt() const
{
    Q_ASSERT(m_testOneof.holdsField(42));
    return m_testOneof.value<QtProtobuf::int32>();
}

ComplexMessage *OneofComplexMessage::testOneofComplexField_p() const
{
    return m_testOneof.holdsField(3) ?
        m_testOneof.value<ComplexMessage>() : nullptr;
}

bool OneofComplexMessage::hasTestOneofComplexField() const
{
    return m_testOneof.holdsField(3);
}
ComplexMessage &OneofComplexMessage::testOneofComplexField() const
{
    Q_ASSERT(m_testOneof.holdsField(3));
    return *(m_testOneof.value<ComplexMessage>());
}

ComplexMessage *OneofComplexMessage::testOneofSecondComplexField_p() const
{
    return m_testOneof.holdsField(4) ?
        m_testOneof.value<ComplexMessage>() : nullptr;
}

bool OneofComplexMessage::hasTestOneofSecondComplexField() const
{
    return m_testOneof.holdsField(4);
}
ComplexMessage &OneofComplexMessage::testOneofSecondComplexField() const
{
    Q_ASSERT(m_testOneof.holdsField(4));
    return *(m_testOneof.value<ComplexMessage>());
}

QtProtobuf::int32 OneofComplexMessage::secondFieldInt_p() const
{
    return m_secondOneof.holdsField(43) ?
        m_secondOneof.value<QtProtobuf::int32>() : QtProtobuf::int32(0);
}

bool OneofComplexMessage::hasSecondFieldInt() const
{
    return m_secondOneof.holdsField(43);
}
QtProtobuf::int32 OneofComplexMessage::secondFieldInt() const
{
    Q_ASSERT(m_secondOneof.holdsField(43));
    return m_secondOneof.value<QtProtobuf::int32>();
}

ComplexMessage *OneofComplexMessage::secondComplexField_p() const
{
    return m_secondOneof.holdsField(5) ?
        m_secondOneof.value<ComplexMessage>() : nullptr;
}

bool OneofComplexMessage::hasSecondComplexField() const
{
    return m_secondOneof.holdsField(5);
}
ComplexMessage &OneofComplexMessage::secondComplexField() const
{
    Q_ASSERT(m_secondOneof.holdsField(5));
    return *(m_secondOneof.value<ComplexMessage>());
}

ComplexMessage *OneofComplexMessage::secondSecondComplexField_p() const
{
    return m_secondOneof.holdsField(6) ?
        m_secondOneof.value<ComplexMessage>() : nullptr;
}

bool OneofComplexMessage::hasSecondSecondComplexField() const
{
    return m_secondOneof.holdsField(6);
}
ComplexMessage &OneofComplexMessage::secondSecondComplexField() const
{
    Q_ASSERT(m_secondOneof.holdsField(6));
    return *(m_secondOneof.value<ComplexMessage>());
}

void OneofComplexMessage::setTestOneofFieldInt(const QtProtobuf::int32 &testOneofFieldInt)
{
    if (!m_testOneof.isEqual(testOneofFieldInt, 42)) {
        m_testOneof.setValue(testOneofFieldInt, 42);
    }
}

void OneofComplexMessage::setTestOneofFieldInt_p(QtProtobuf::int32 testOneofFieldInt)
{
    if (!m_testOneof.isEqual(testOneofFieldInt, 42))
        m_testOneof.setValue(testOneofFieldInt, 42);
}

void OneofComplexMessage::setTestOneofComplexField(const ComplexMessage &testOneofComplexField)
{
    if (!m_testOneof.isEqual(testOneofComplexField, 3)) {
        m_testOneof.setValue(testOneofComplexField, 3);
    }
}

void OneofComplexMessage::setTestOneofComplexField_p(ComplexMessage *testOneofComplexField)
{
    const ComplexMessage &value = *testOneofComplexField;    if (!m_testOneof.isEqual(value, 3))
        m_testOneof.setValue(value, 3);
}

void OneofComplexMessage::setTestOneofSecondComplexField(const ComplexMessage &testOneofSecondComplexField)
{
    if (!m_testOneof.isEqual(testOneofSecondComplexField, 4)) {
        m_testOneof.setValue(testOneofSecondComplexField, 4);
    }
}

void OneofComplexMessage::setTestOneofSecondComplexField_p(ComplexMessage *testOneofSecondComplexField)
{
    const ComplexMessage &value = *testOneofSecondComplexField;    if (!m_testOneof.isEqual(value, 4))
        m_testOneof.setValue(value, 4);
}

void OneofComplexMessage::setSecondFieldInt(const QtProtobuf::int32 &secondFieldInt)
{
    if (!m_secondOneof.isEqual(secondFieldInt, 43)) {
        m_secondOneof.setValue(secondFieldInt, 43);
    }
}

void OneofComplexMessage::setSecondFieldInt_p(QtProtobuf::int32 secondFieldInt)
{
    if (!m_secondOneof.isEqual(secondFieldInt, 43))
        m_secondOneof.setValue(secondFieldInt, 43);
}

void OneofComplexMessage::setSecondComplexField(const ComplexMessage &secondComplexField)
{
    if (!m_secondOneof.isEqual(secondComplexField, 5)) {
        m_secondOneof.setValue(secondComplexField, 5);
    }
}

void OneofComplexMessage::setSecondComplexField_p(ComplexMessage *secondComplexField)
{
    const ComplexMessage &value = *secondComplexField;    if (!m_secondOneof.isEqual(value, 5))
        m_secondOneof.setValue(value, 5);
}

void OneofComplexMessage::setSecondSecondComplexField(const ComplexMessage &secondSecondComplexField)
{
    if (!m_secondOneof.isEqual(secondSecondComplexField, 6)) {
        m_secondOneof.setValue(secondSecondComplexField, 6);
    }
}

void OneofComplexMessage::setSecondSecondComplexField_p(ComplexMessage *secondSecondComplexField)
{
    const ComplexMessage &value = *secondSecondComplexField;    if (!m_secondOneof.isEqual(value, 6))
        m_secondOneof.setValue(value, 6);
}

int OneofComplexMessage::testOneofField() const
{
    return m_testOneof.fieldNumber();
}
void OneofComplexMessage::clearTestOneof()
{
    m_testOneof = QtProtobufPrivate::QProtobufOneof();
}
int OneofComplexMessage::secondOneofField() const
{
    return m_secondOneof.fieldNumber();
}
void OneofComplexMessage::clearSecondOneof()
{
    m_secondOneof = QtProtobufPrivate::QProtobufOneof();
}
} // namespace qtprotobufnamespace::tests

#include "moc_oneofmessages.qpb.cpp"
