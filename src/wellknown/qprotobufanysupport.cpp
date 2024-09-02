// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtProtobufWellKnownTypes/qprotobufanysupport.h>

#include <QtProtobufWellKnownTypes/google/protobuf/any.qpb.h>

#include <QtProtobuf/private/qprotobufmessage_p.h>
#include <QtProtobuf/private/qprotobufregistration_p.h>
#include <QtProtobuf/qabstractprotobufserializer.h>
#include <QtProtobuf/qprotobufpropertyordering.h>

QT_BEGIN_NAMESPACE

namespace {
QAnyStringView defaultUrlPrefix()
{
    // The URL should describe the type of the serialized message.
    // We don't have support for this, so users have to provide the correct URL.
    // We won't check if it's correct.
    return u"type.googleapis.com";
}
}

namespace QtProtobuf {
using namespace QtProtobufPrivate;

class AnyPrivate : public QProtobufMessagePrivate
{
public:
    AnyPrivate(const QMetaObject *metaObject,
               const QtProtobufPrivate::QProtobufPropertyOrdering *ordering);

    static const AnyPrivate *get(const Any *any) { return any->d_func(); }

    QString typeUrl;
    QByteArray value;
};

AnyPrivate::AnyPrivate(const QMetaObject *metaObject,
                       const QtProtobufPrivate::QProtobufPropertyOrdering *ordering)
    : QProtobufMessagePrivate(metaObject, ordering)
{
}

static void serializerProxy(QtProtobufPrivate::MessageFieldSerializer serializer,
                            const void *object, const QProtobufFieldInfo &fieldInfo)
{
    if (object == nullptr)
        return;

    const auto &any = *static_cast<const Any *>(object);
    if (any.typeUrl().isEmpty())
        return;

    google::protobuf::Any realAny;
    realAny.setValue(any.value());
    realAny.setTypeUrl(any.typeUrl());
    serializer(&realAny, fieldInfo);
}

static void listSerializerProxy(QtProtobufPrivate::MessageFieldSerializer serializer,
                                const void *object, const QProtobufFieldInfo &fieldInfo)
{
    if (object == nullptr)
        return;

    const auto &anyList = *static_cast<const QList<Any> *>(object);
    for (const Any &any : anyList) {
        google::protobuf::Any realAny;
        realAny.setValue(any.value());
        realAny.setTypeUrl(any.typeUrl());
        serializer(&realAny, fieldInfo);
    }
}

static void listDeserializerProxy(QtProtobufPrivate::MessageFieldDeserializer deserializer,
                                  void *object)
{
    if (object == nullptr)
        return;

    auto &anyList = *static_cast<QList<Any> *>(object);
    google::protobuf::Any realAny;
    if (deserializer(&realAny)) {
        Any any;
        any.setTypeUrl(realAny.typeUrl());
        any.setValue(realAny.value());
        anyList.append(std::move(any));
    } else {
        return; // unexpected end of data
    }
}

static void deserializerProxy(QtProtobufPrivate::MessageFieldDeserializer deserializer,
                              void *object)
{
    if (object == nullptr)
        return;

    google::protobuf::Any realAny;
    if (deserializer(&realAny)) {
        auto &any = *static_cast<Any *>(object);
        any.setTypeUrl(realAny.typeUrl());
        any.setValue(realAny.value());
    }
}

void Any::registerTypes()
{
    QtProtobufPrivate::registerHandler(QMetaType::fromType<Any>(), &serializerProxy,
                                       &deserializerProxy);
    QtProtobufPrivate::registerHandler(QMetaType::fromType<QList<Any>>(), &listSerializerProxy,
                                       &listDeserializerProxy);
}

/*!
    \class QtProtobuf::Any
    \brief A helper class to simplify working with the protobuf Any type.
    \inmodule QtProtobufWellKnownTypes

    \compares equality

    The Any class enables easy casting to and from other types using as() and
    fromMessage().

    It is the type used in code generated by \l{The qtprotobufgen Tool}
    {qtprotobufgen} when a message contains a field of the \c
    google.protobuf.Any type.
*/

/*!
    Constructs a defaulted, empty, instance of Any.
*/
Any::Any()
    : QProtobufMessage(*new AnyPrivate(&Any::staticMetaObject,
                                       &google::protobuf::Any::staticPropertyOrdering))
{
}

/*!
    Destroys this instance of Any
*/
Any::~Any()
    = default;

/*!
    Constructs a copy of \a other.
*/
Any::Any(const Any &other)
    = default;

/*!
    Copies the data of \a other into this instance.
*/
Any &Any::operator=(const Any &other)
    = default;

/*!
    Returns the type URL of the Any object.
    The URL is meant as a hint for what the contained data really is.

    \note Qt has no support for dynamically obtaining any potential recipes for
    deconstructing types and simply uses the type URL to verify that types
    passed as T to \c{as<T>()} are of the correct type.

    \sa setTypeUrl(), value()
*/
QString Any::typeUrl() const
{
    Q_D(const Any);
    return d->typeUrl;
}

/*!
    Returns the raw bytes that make up the value stored.

    Consult typeUrl() to determine how to interpret these bytes.

    \sa setValue(), typeUrl()
*/
QByteArray Any::value() const
{
    Q_D(const Any);
    return d->value;
}

/*!
    Sets the type URL for the data contained to \a typeUrl.

    \note No verification is performed on the input to test if it's correct or
    in a valid format.

    \sa typeUrl(), fromMessage()
*/
void Any::setTypeUrl(const QString &typeUrl)
{
    Q_D(Any);
    if (d->typeUrl != typeUrl)
        d->typeUrl = typeUrl;
}

/*!
    Sets the raw bytes of the value stored to \a value.

    \a value must be the output of serializing a message.

    \sa value(), fromMessage()
*/
void Any::setValue(const QByteArray &value)
{
    Q_D(Any);
    if (d->value != value)
        d->value = value;
}

/*!
    \fn template <typename T> std::optional<T> Any::unpack(QAbstractProtobufSerializer *serializer) const

    This function compares the message name of T with the value of typeUrl()
    before deserializing the data using \a serializer.

    If the verification or deserialization fails it will return
    \c{std::nullopt}.

    \note T must be a class derived from QProtobufMessage with the
    \c{Q_PROTOBUF_OBJECT} macro or (for a nested Any message) be Any itself.
*/

bool Any::unpackImpl(QAbstractProtobufSerializer *serializer, QProtobufMessage *message) const
{
    Q_ASSERT_X(serializer != nullptr, "Any::unpackImpl", "serializer is null");
    QString tUrl = typeUrl();
    qsizetype lastSegmentIndex = tUrl.lastIndexOf(u'/') + 1;
    if (QStringView(tUrl)
            .mid(lastSegmentIndex)
            .compare(message->propertyOrdering()->messageFullName())
        != 0) {
        return false;
    }
    return message->deserialize(serializer, value());
}

std::optional<Any> Any::unpackAnyImpl(QAbstractProtobufSerializer *serializer) const
{
    google::protobuf::Any realAny;
    if (!unpackImpl(serializer, &realAny))
        return std::nullopt;
    Any any;
    any.setTypeUrl(realAny.typeUrl());
    any.setValue(realAny.value());
    return {std::move(any)};
}

/*!
    \fn template <typename T> static Any Any::fromMessage(QAbstractProtobufSerializer *serializer,
        const T &message, QAnyStringView typeUrlPrefix)

    This function serializes the given \a message as the value of the returned
    Any instance. This instance's typeUrl() is constructed from a prefix, a
    forward slash and the message name obtained from
    \l{QProtobufMessage::propertyOrdering} using \a serializer. If \a
    typeUrlPrefix is supplied, it is used as prefix, otherwise
    \c{"type.googleapis.com"} is used.

    \note T must be a class derived from QProtobufMessage with the
    \c{Q_PROTOBUF_OBJECT} macro or (for a nested Any message) be Any itself.
*/

Any Any::fromMessageImpl(QAbstractProtobufSerializer *serializer, const QProtobufMessage *message,
                         QAnyStringView typeUrlPrefix)
{
    Any any;
    any.setValue(message->serialize(serializer));
    any.setTypeUrl(typeUrlPrefix.toString() + u'/'
                   + message->propertyOrdering()->messageFullName().toString());
    return { any };
}

Any Any::fromMessageImpl(QAbstractProtobufSerializer *serializer, const QProtobufMessage *message)
{
    return fromMessageImpl(serializer, message, defaultUrlPrefix());
}

// Used to handle nested Any messages.
Any Any::fromAnyMessageImpl(QAbstractProtobufSerializer *serializer,
                            const Any *message, QAnyStringView typeUrlPrefix)
{
    using RealAny = google::protobuf::Any;
    RealAny realAny;
    realAny.setTypeUrl(message->typeUrl());
    realAny.setValue(message->value());
    return fromMessageImpl(serializer, &realAny, typeUrlPrefix);
}

// Used to handle nested Any messages.
Any Any::fromAnyMessageImpl(QAbstractProtobufSerializer *serializer,
                            const Any *message)
{
    return fromAnyMessageImpl(serializer, message, defaultUrlPrefix());
}

/*!
    \fn bool Any::operator==(const Any &lhs, const Any &rhs)
    Returns true if the two instances of Any, \a lhs and \a rhs, compare equal.
*/

/*!
    \fn bool Any::operator!=(const Any &lhs, const Any &rhs)
    Returns true if the two instances of Any, \a lhs and \a rhs, are distinct.
*/
}

QT_END_NAMESPACE

#include "moc_qprotobufanysupport.cpp"
