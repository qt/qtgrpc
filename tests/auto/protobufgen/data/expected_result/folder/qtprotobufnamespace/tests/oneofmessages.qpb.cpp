/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#include "qtprotobufnamespace/tests/oneofmessages.qpb.h"
#include <QtProtobuf/qprotobufregistration.h>
#include <cmath>

namespace qtprotobufnamespace::tests {

class OneofSimpleMessage_QtProtobufData : public QSharedData
{
public:
    OneofSimpleMessage_QtProtobufData()
        : QSharedData()
    {
    }

    OneofSimpleMessage_QtProtobufData(const OneofSimpleMessage_QtProtobufData &other)
        : QSharedData(other),
          m_testOneof(other.m_testOneof)
    {
    }

    QtProtobufPrivate::QProtobufOneof m_testOneof;
};

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
        uint(QtProtobufPrivate::FieldFlag::Oneof | QtProtobufPrivate::FieldFlag::ExplicitPresence), /* = testOneofFieldInt */
        uint(QtProtobufPrivate::FieldFlag::Oneof | QtProtobufPrivate::FieldFlag::ExplicitPresence), /* = testOneofFieldSecondInt */
    },
    // char_data
    /* metadata char_data: */
    "qtprotobufnamespace.tests.OneofSimpleMessage\0" /* = full message name */
    /* field char_data: */
    "testOneofFieldInt\0testOneofFieldSecondInt\0"
};

const QtProtobufPrivate::QProtobufPropertyOrdering OneofSimpleMessage::staticPropertyOrdering = {
    &qt_protobuf_OneofSimpleMessage_metadata.data
};

void OneofSimpleMessage::registerTypes()
{
    qRegisterMetaType<OneofSimpleMessage>();
    qRegisterMetaType<OneofSimpleMessageRepeated>();
}

OneofSimpleMessage::OneofSimpleMessage()
    : QProtobufMessage(&OneofSimpleMessage::staticMetaObject, &OneofSimpleMessage::staticPropertyOrdering),
      dptr(new OneofSimpleMessage_QtProtobufData)
{
}

OneofSimpleMessage::OneofSimpleMessage(const OneofSimpleMessage &other)
    : QProtobufMessage(other),
      dptr(other.dptr)
{
}
OneofSimpleMessage &OneofSimpleMessage::operator =(const OneofSimpleMessage &other)
{
    QProtobufMessage::operator=(other);
    dptr = other.dptr;
    return *this;
}
OneofSimpleMessage::OneofSimpleMessage(OneofSimpleMessage &&other) noexcept
    : QProtobufMessage(std::move(other)),
      dptr(std::move(other.dptr))
{
}
OneofSimpleMessage &OneofSimpleMessage::operator =(OneofSimpleMessage &&other) noexcept
{
    QProtobufMessage::operator=(std::move(other));
    dptr.swap(other.dptr);
    return *this;
}
bool comparesEqual(const OneofSimpleMessage &lhs, const OneofSimpleMessage &rhs) noexcept
{
    return operator ==(static_cast<const QProtobufMessage&>(lhs),
                       static_cast<const QProtobufMessage&>(rhs))
        && lhs.dptr->m_testOneof == rhs.dptr->m_testOneof;
}

QtProtobuf::int32 OneofSimpleMessage::testOneofFieldInt_p() const
{
    return dptr->m_testOneof.holdsField(1) ?
        dptr->m_testOneof.value<QtProtobuf::int32>() : QtProtobuf::int32(0);
}

bool OneofSimpleMessage::hasTestOneofFieldInt() const
{
    return dptr->m_testOneof.holdsField(1);
}
QtProtobuf::int32 OneofSimpleMessage::testOneofFieldInt() const
{
    Q_ASSERT(dptr->m_testOneof.holdsField(1));
    return dptr->m_testOneof.value<QtProtobuf::int32>();
}

QtProtobuf::int32 OneofSimpleMessage::testOneofFieldSecondInt_p() const
{
    return dptr->m_testOneof.holdsField(2) ?
        dptr->m_testOneof.value<QtProtobuf::int32>() : QtProtobuf::int32(0);
}

bool OneofSimpleMessage::hasTestOneofFieldSecondInt() const
{
    return dptr->m_testOneof.holdsField(2);
}
QtProtobuf::int32 OneofSimpleMessage::testOneofFieldSecondInt() const
{
    Q_ASSERT(dptr->m_testOneof.holdsField(2));
    return dptr->m_testOneof.value<QtProtobuf::int32>();
}

void OneofSimpleMessage::setTestOneofFieldInt(const QtProtobuf::int32 &testOneofFieldInt)
{
    if (!dptr->m_testOneof.isEqual(testOneofFieldInt, 1)) {
        dptr.detach();
        dptr->m_testOneof.setValue(testOneofFieldInt, 1);
    }
}

void OneofSimpleMessage::setTestOneofFieldInt_p(QtProtobuf::int32 testOneofFieldInt)
{
    if (!dptr->m_testOneof.isEqual(testOneofFieldInt, 1)) {
        dptr.detach();
        dptr->m_testOneof.setValue(testOneofFieldInt, 1);
    }
}

void OneofSimpleMessage::setTestOneofFieldSecondInt(const QtProtobuf::int32 &testOneofFieldSecondInt)
{
    if (!dptr->m_testOneof.isEqual(testOneofFieldSecondInt, 2)) {
        dptr.detach();
        dptr->m_testOneof.setValue(testOneofFieldSecondInt, 2);
    }
}

void OneofSimpleMessage::setTestOneofFieldSecondInt_p(QtProtobuf::int32 testOneofFieldSecondInt)
{
    if (!dptr->m_testOneof.isEqual(testOneofFieldSecondInt, 2)) {
        dptr.detach();
        dptr->m_testOneof.setValue(testOneofFieldSecondInt, 2);
    }
}

OneofSimpleMessage::TestOneofFields OneofSimpleMessage::testOneofField() const
{
    return static_cast<TestOneofFields>(dptr->m_testOneof.fieldNumber());
}
void OneofSimpleMessage::clearTestOneof()
{
    if (dptr->m_testOneof.fieldNumber() != QtProtobuf::InvalidFieldNumber) {
        dptr.detach();
        dptr->m_testOneof = QtProtobufPrivate::QProtobufOneof();
    }
}

class OneofComplexMessage_QtProtobufData : public QSharedData
{
public:
    OneofComplexMessage_QtProtobufData()
        : QSharedData(),
          m_testFieldInt(0)
    {
    }

    OneofComplexMessage_QtProtobufData(const OneofComplexMessage_QtProtobufData &other)
        : QSharedData(other),
          m_testFieldInt(other.m_testFieldInt),
          m_testOneof(other.m_testOneof),
          m_secondOneof(other.m_secondOneof)
    {
    }

    QtProtobuf::int32 m_testFieldInt;
    QtProtobufPrivate::QProtobufOneof m_testOneof;
    QtProtobufPrivate::QProtobufOneof m_secondOneof;
};

OneofComplexMessage::~OneofComplexMessage() = default;

static constexpr struct {
    QtProtobufPrivate::QProtobufPropertyOrdering::Data data;
    const std::array<uint, 33> qt_protobuf_OneofComplexMessage_uint_data;
    const char qt_protobuf_OneofComplexMessage_char_data[206];
} qt_protobuf_OneofComplexMessage_metadata {
    // data
    {
        0, /* = version */
        8, /* = num fields */
        9, /* = field number offset */
        17, /* = property index offset */
        25, /* = field flags offset */
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
        186, /* = testSnakeCaseField */
        205, /* = end-of-string-marker */
        // Field numbers:
        1, /* = testFieldInt */
        42, /* = testOneofFieldInt */
        3, /* = testOneofComplexField */
        4, /* = testOneofSecondComplexField */
        43, /* = secondFieldInt */
        5, /* = secondComplexField */
        6, /* = secondSecondComplexField */
        7, /* = testSnakeCaseField */
        // Property indices:
        0, /* = testFieldInt */
        1, /* = testOneofFieldInt */
        3, /* = testOneofComplexField */
        5, /* = testOneofSecondComplexField */
        7, /* = secondFieldInt */
        9, /* = secondComplexField */
        11, /* = secondSecondComplexField */
        13, /* = testSnakeCaseField */
        // Field flags:
        uint(QtProtobufPrivate::FieldFlag::NoFlags), /* = testFieldInt */
        uint(QtProtobufPrivate::FieldFlag::Oneof | QtProtobufPrivate::FieldFlag::ExplicitPresence), /* = testOneofFieldInt */
        uint(QtProtobufPrivate::FieldFlag::Oneof | QtProtobufPrivate::FieldFlag::ExplicitPresence | QtProtobufPrivate::FieldFlag::Message), /* = testOneofComplexField */
        uint(QtProtobufPrivate::FieldFlag::Oneof | QtProtobufPrivate::FieldFlag::ExplicitPresence | QtProtobufPrivate::FieldFlag::Message), /* = testOneofSecondComplexField */
        uint(QtProtobufPrivate::FieldFlag::Oneof | QtProtobufPrivate::FieldFlag::ExplicitPresence), /* = secondFieldInt */
        uint(QtProtobufPrivate::FieldFlag::Oneof | QtProtobufPrivate::FieldFlag::ExplicitPresence | QtProtobufPrivate::FieldFlag::Message), /* = secondComplexField */
        uint(QtProtobufPrivate::FieldFlag::Oneof | QtProtobufPrivate::FieldFlag::ExplicitPresence | QtProtobufPrivate::FieldFlag::Message), /* = secondSecondComplexField */
        uint(QtProtobufPrivate::FieldFlag::Oneof | QtProtobufPrivate::FieldFlag::ExplicitPresence), /* = testSnakeCaseField */
    },
    // char_data
    /* metadata char_data: */
    "qtprotobufnamespace.tests.OneofComplexMessage\0" /* = full message name */
    /* field char_data: */
    "testFieldInt\0testOneofFieldInt\0testOneofComplexField\0testOneofSecondComplexField\0secondFieldInt\0"
    "secondComplexField\0secondSecondComplexField\0testSnakeCaseField\0"
};

const QtProtobufPrivate::QProtobufPropertyOrdering OneofComplexMessage::staticPropertyOrdering = {
    &qt_protobuf_OneofComplexMessage_metadata.data
};

void OneofComplexMessage::registerTypes()
{
    qRegisterMetaType<OneofComplexMessage>();
    qRegisterMetaType<OneofComplexMessageRepeated>();
}

OneofComplexMessage::OneofComplexMessage()
    : QProtobufMessage(&OneofComplexMessage::staticMetaObject, &OneofComplexMessage::staticPropertyOrdering),
      dptr(new OneofComplexMessage_QtProtobufData)
{
}

OneofComplexMessage::OneofComplexMessage(const OneofComplexMessage &other)
    : QProtobufMessage(other),
      dptr(other.dptr)
{
}
OneofComplexMessage &OneofComplexMessage::operator =(const OneofComplexMessage &other)
{
    QProtobufMessage::operator=(other);
    dptr = other.dptr;
    return *this;
}
OneofComplexMessage::OneofComplexMessage(OneofComplexMessage &&other) noexcept
    : QProtobufMessage(std::move(other)),
      dptr(std::move(other.dptr))
{
}
OneofComplexMessage &OneofComplexMessage::operator =(OneofComplexMessage &&other) noexcept
{
    QProtobufMessage::operator=(std::move(other));
    dptr.swap(other.dptr);
    return *this;
}
bool comparesEqual(const OneofComplexMessage &lhs, const OneofComplexMessage &rhs) noexcept
{
    return operator ==(static_cast<const QProtobufMessage&>(lhs),
                       static_cast<const QProtobufMessage&>(rhs))
        && lhs.dptr->m_testFieldInt == rhs.dptr->m_testFieldInt
        && lhs.dptr->m_testOneof == rhs.dptr->m_testOneof
        && lhs.dptr->m_secondOneof == rhs.dptr->m_secondOneof;
}

QtProtobuf::int32 OneofComplexMessage::testFieldInt() const
{
    return dptr->m_testFieldInt;
}

QtProtobuf::int32 OneofComplexMessage::testOneofFieldInt_p() const
{
    return dptr->m_testOneof.holdsField(42) ?
        dptr->m_testOneof.value<QtProtobuf::int32>() : QtProtobuf::int32(0);
}

bool OneofComplexMessage::hasTestOneofFieldInt() const
{
    return dptr->m_testOneof.holdsField(42);
}
QtProtobuf::int32 OneofComplexMessage::testOneofFieldInt() const
{
    Q_ASSERT(dptr->m_testOneof.holdsField(42));
    return dptr->m_testOneof.value<QtProtobuf::int32>();
}

ComplexMessage *OneofComplexMessage::testOneofComplexField_p()
{
    if (!dptr->m_testOneof.holdsField(3))
        dptr.detach();
    return dptr->m_testOneof.message<ComplexMessage>();
}

bool OneofComplexMessage::hasTestOneofComplexField() const
{
    return dptr->m_testOneof.holdsField(3);
}
ComplexMessage &OneofComplexMessage::testOneofComplexField() const
{
    Q_ASSERT(dptr->m_testOneof.holdsField(3));
    return *(dptr->m_testOneof.message<ComplexMessage>());
}

ComplexMessage *OneofComplexMessage::testOneofSecondComplexField_p()
{
    if (!dptr->m_testOneof.holdsField(4))
        dptr.detach();
    return dptr->m_testOneof.message<ComplexMessage>();
}

bool OneofComplexMessage::hasTestOneofSecondComplexField() const
{
    return dptr->m_testOneof.holdsField(4);
}
ComplexMessage &OneofComplexMessage::testOneofSecondComplexField() const
{
    Q_ASSERT(dptr->m_testOneof.holdsField(4));
    return *(dptr->m_testOneof.message<ComplexMessage>());
}

QtProtobuf::int32 OneofComplexMessage::secondFieldInt_p() const
{
    return dptr->m_secondOneof.holdsField(43) ?
        dptr->m_secondOneof.value<QtProtobuf::int32>() : QtProtobuf::int32(0);
}

bool OneofComplexMessage::hasSecondFieldInt() const
{
    return dptr->m_secondOneof.holdsField(43);
}
QtProtobuf::int32 OneofComplexMessage::secondFieldInt() const
{
    Q_ASSERT(dptr->m_secondOneof.holdsField(43));
    return dptr->m_secondOneof.value<QtProtobuf::int32>();
}

ComplexMessage *OneofComplexMessage::secondComplexField_p()
{
    if (!dptr->m_secondOneof.holdsField(5))
        dptr.detach();
    return dptr->m_secondOneof.message<ComplexMessage>();
}

bool OneofComplexMessage::hasSecondComplexField() const
{
    return dptr->m_secondOneof.holdsField(5);
}
ComplexMessage &OneofComplexMessage::secondComplexField() const
{
    Q_ASSERT(dptr->m_secondOneof.holdsField(5));
    return *(dptr->m_secondOneof.message<ComplexMessage>());
}

ComplexMessage *OneofComplexMessage::secondSecondComplexField_p()
{
    if (!dptr->m_secondOneof.holdsField(6))
        dptr.detach();
    return dptr->m_secondOneof.message<ComplexMessage>();
}

bool OneofComplexMessage::hasSecondSecondComplexField() const
{
    return dptr->m_secondOneof.holdsField(6);
}
ComplexMessage &OneofComplexMessage::secondSecondComplexField() const
{
    Q_ASSERT(dptr->m_secondOneof.holdsField(6));
    return *(dptr->m_secondOneof.message<ComplexMessage>());
}

QtProtobuf::int32 OneofComplexMessage::testSnakeCaseField_p() const
{
    return dptr->m_secondOneof.holdsField(7) ?
        dptr->m_secondOneof.value<QtProtobuf::int32>() : QtProtobuf::int32(0);
}

bool OneofComplexMessage::hasTestSnakeCaseField() const
{
    return dptr->m_secondOneof.holdsField(7);
}
QtProtobuf::int32 OneofComplexMessage::testSnakeCaseField() const
{
    Q_ASSERT(dptr->m_secondOneof.holdsField(7));
    return dptr->m_secondOneof.value<QtProtobuf::int32>();
}

void OneofComplexMessage::setTestFieldInt(const QtProtobuf::int32 &testFieldInt)
{
    if (dptr->m_testFieldInt != testFieldInt) {
        dptr.detach();
        dptr->m_testFieldInt = testFieldInt;
    }
}

void OneofComplexMessage::setTestOneofFieldInt(const QtProtobuf::int32 &testOneofFieldInt)
{
    if (!dptr->m_testOneof.isEqual(testOneofFieldInt, 42)) {
        dptr.detach();
        dptr->m_testOneof.setValue(testOneofFieldInt, 42);
    }
}

void OneofComplexMessage::setTestOneofFieldInt_p(QtProtobuf::int32 testOneofFieldInt)
{
    if (!dptr->m_testOneof.isEqual(testOneofFieldInt, 42)) {
        dptr.detach();
        dptr->m_testOneof.setValue(testOneofFieldInt, 42);
    }
}

void OneofComplexMessage::setTestOneofComplexField(const ComplexMessage &testOneofComplexField)
{
    if (!dptr->m_testOneof.isEqual(testOneofComplexField, 3)) {
        dptr.detach();
        dptr->m_testOneof.setValue(testOneofComplexField, 3);
    }
}

void OneofComplexMessage::setTestOneofComplexField_p(ComplexMessage *testOneofComplexField)
{
    const ComplexMessage &value = *testOneofComplexField;
    if (!dptr->m_testOneof.isEqual(value, 3)) {
        dptr.detach();
        dptr->m_testOneof.setValue(value, 3);
    }
}

void OneofComplexMessage::setTestOneofSecondComplexField(const ComplexMessage &testOneofSecondComplexField)
{
    if (!dptr->m_testOneof.isEqual(testOneofSecondComplexField, 4)) {
        dptr.detach();
        dptr->m_testOneof.setValue(testOneofSecondComplexField, 4);
    }
}

void OneofComplexMessage::setTestOneofSecondComplexField_p(ComplexMessage *testOneofSecondComplexField)
{
    const ComplexMessage &value = *testOneofSecondComplexField;
    if (!dptr->m_testOneof.isEqual(value, 4)) {
        dptr.detach();
        dptr->m_testOneof.setValue(value, 4);
    }
}

void OneofComplexMessage::setSecondFieldInt(const QtProtobuf::int32 &secondFieldInt)
{
    if (!dptr->m_secondOneof.isEqual(secondFieldInt, 43)) {
        dptr.detach();
        dptr->m_secondOneof.setValue(secondFieldInt, 43);
    }
}

void OneofComplexMessage::setSecondFieldInt_p(QtProtobuf::int32 secondFieldInt)
{
    if (!dptr->m_secondOneof.isEqual(secondFieldInt, 43)) {
        dptr.detach();
        dptr->m_secondOneof.setValue(secondFieldInt, 43);
    }
}

void OneofComplexMessage::setSecondComplexField(const ComplexMessage &secondComplexField)
{
    if (!dptr->m_secondOneof.isEqual(secondComplexField, 5)) {
        dptr.detach();
        dptr->m_secondOneof.setValue(secondComplexField, 5);
    }
}

void OneofComplexMessage::setSecondComplexField_p(ComplexMessage *secondComplexField)
{
    const ComplexMessage &value = *secondComplexField;
    if (!dptr->m_secondOneof.isEqual(value, 5)) {
        dptr.detach();
        dptr->m_secondOneof.setValue(value, 5);
    }
}

void OneofComplexMessage::setSecondSecondComplexField(const ComplexMessage &secondSecondComplexField)
{
    if (!dptr->m_secondOneof.isEqual(secondSecondComplexField, 6)) {
        dptr.detach();
        dptr->m_secondOneof.setValue(secondSecondComplexField, 6);
    }
}

void OneofComplexMessage::setSecondSecondComplexField_p(ComplexMessage *secondSecondComplexField)
{
    const ComplexMessage &value = *secondSecondComplexField;
    if (!dptr->m_secondOneof.isEqual(value, 6)) {
        dptr.detach();
        dptr->m_secondOneof.setValue(value, 6);
    }
}

void OneofComplexMessage::setTestSnakeCaseField(const QtProtobuf::int32 &testSnakeCaseField)
{
    if (!dptr->m_secondOneof.isEqual(testSnakeCaseField, 7)) {
        dptr.detach();
        dptr->m_secondOneof.setValue(testSnakeCaseField, 7);
    }
}

void OneofComplexMessage::setTestSnakeCaseField_p(QtProtobuf::int32 testSnakeCaseField)
{
    if (!dptr->m_secondOneof.isEqual(testSnakeCaseField, 7)) {
        dptr.detach();
        dptr->m_secondOneof.setValue(testSnakeCaseField, 7);
    }
}

OneofComplexMessage::TestOneofFields OneofComplexMessage::testOneofField() const
{
    return static_cast<TestOneofFields>(dptr->m_testOneof.fieldNumber());
}
void OneofComplexMessage::clearTestOneof()
{
    if (dptr->m_testOneof.fieldNumber() != QtProtobuf::InvalidFieldNumber) {
        dptr.detach();
        dptr->m_testOneof = QtProtobufPrivate::QProtobufOneof();
    }
}
OneofComplexMessage::SecondOneofFields OneofComplexMessage::secondOneofField() const
{
    return static_cast<SecondOneofFields>(dptr->m_secondOneof.fieldNumber());
}
void OneofComplexMessage::clearSecondOneof()
{
    if (dptr->m_secondOneof.fieldNumber() != QtProtobuf::InvalidFieldNumber) {
        dptr.detach();
        dptr->m_secondOneof = QtProtobufPrivate::QProtobufOneof();
    }
}
} // namespace qtprotobufnamespace::tests

#include "moc_oneofmessages.qpb.cpp"
