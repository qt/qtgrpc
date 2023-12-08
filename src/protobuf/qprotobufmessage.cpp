// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#include "qprotobufmessage_p.h"
#include "qprotobufmessage.h"

#include <QtCore/qmetaobject.h>

#include <string>

QT_BEGIN_NAMESPACE

static std::string nul_terminate(QLatin1StringView l1) noexcept
{
    return l1.isNull() ? std::string{} : std::string{l1.data(), size_t(l1.size())};
}

/*!
    \class QProtobufMessage
    \inmodule QtProtobuf

    \brief Base class for all protobuf messages.

    Provides access to the properties of a message, using setProperty()
    and property(), without depending on what the message is.
*/

/*!
    \internal
    Used from generated classes to construct the QProtobufMessage base class.
    Internally the \a metaObject is used to query QMetaProperty
*/
QProtobufMessage::QProtobufMessage(const QMetaObject *metaObject)
    : d_ptr(new QProtobufMessagePrivate)
{
    d_ptr->metaObject = metaObject;
}

/*!
    \internal
    The QMetaObject which was passed to the QProtobufMessage constructor.
*/
const QMetaObject *QProtobufMessage::metaObject() const
{
    return d_ptr->metaObject;
}

/*!
    \internal
*/
int QProtobufMessagePrivate::getPropertyIndex(QAnyStringView propertyName) const
{
    return propertyName.visit([this](auto propertyName) {
        if constexpr (std::is_same_v<QStringView, decltype(propertyName)>) {
            return metaObject->indexOfProperty(propertyName.toLatin1().constData());
        } else if constexpr (std::is_same_v<QUtf8StringView, decltype(propertyName)>) {
            return metaObject->indexOfProperty(propertyName.toString().toLatin1().constData());
        } else if constexpr (std::is_same_v<QLatin1StringView, decltype(propertyName)>) {
            return metaObject->indexOfProperty(nul_terminate(propertyName).data());
        }
        return -1;
    });
}

void QProtobufMessagePrivate::storeUnknownEntry(QByteArrayView entry)
{
    ++unknownEntries[entry.toByteArray()];
}

std::optional<QMetaProperty> QProtobufMessagePrivate::metaProperty(QAnyStringView name) const
{
    const int index = getPropertyIndex(name);
    const QMetaProperty property = metaObject->property(index);
    if (property.isValid())
        return property;
    return std::nullopt;
}

/*!
    Set the property \a propertyName to the value stored in \a value.

    If the \a propertyName isn't a part of the known fields then the value will
    not be written and the function returns \c false.

    Returns \c false if it failed to store the \a value on the property.
    Otherwise \c{true}.
*/
bool QProtobufMessage::setProperty(QAnyStringView propertyName, const QVariant &value)
{
    Q_D(QProtobufMessage);

    if (auto mp = d->metaProperty(propertyName))
        return mp->writeOnGadget(this, value);

    return false;
}

/*!
    Get the value of the property \a propertyName.

    If the \a propertyName isn't known then the returned QVariant is invalid.
*/
QVariant QProtobufMessage::property(QAnyStringView propertyName) const
{
    Q_D(const QProtobufMessage);

    if (const auto mp = d->metaProperty(propertyName))
        return mp->readOnGadget(this);
    return {};
}

/*!
    \internal
*/
QProtobufMessage::QProtobufMessage(const QProtobufMessage &other)
    : d_ptr(new QProtobufMessagePrivate(*other.d_ptr))
{
}

/*!
    \internal
*/
QProtobufMessage &QProtobufMessage::operator=(const QProtobufMessage &other)
{
    if (this != &other)
        *d_ptr = *other.d_ptr;
    return *this;
}

/*!
    \internal
*/
QProtobufMessage::~QProtobufMessage()
{
    delete d_ptr;
}

/*!
    \internal
*/
bool QProtobufMessage::isEqual(const QProtobufMessage &lhs, const QProtobufMessage &rhs) noexcept
{
    if (lhs.d_ptr == rhs.d_ptr)
        return true;
    return lhs.d_func()->unknownEntries == rhs.d_func()->unknownEntries;
}

namespace QtProtobufPrivate {
/*!
    \internal
*/
extern QProtobufMessagePointer constructMessageByName(const QString &messageType);
}

/*!
    Constructs QProtobufMessage using \a messageType.
    Returns a pointer to the constructed QProtobufMessage.

    This function attempts to create a message whose type matches \a messageType. If \a messageType
    is unknown, the function returns \nullptr. If the message is not found in the registry, the
    function returns \nullptr.
    Ownership of the constructed message is given to the function caller.
*/
QProtobufMessagePointer QProtobufMessage::constructByName(const QString &messageType)
{
    return QtProtobufPrivate::constructMessageByName(messageType);
}

/*!
    \typedef QProtobufMessagePointer
    \relates QProtobufMessage
    \inmodule QtProtobuf

    Synonym for std::unique_ptr<QProtobufMessage, QProtobufMessageDeleter>.
    Use this to manage the lifetime of dynamically allocated QProtobufMessages,
    such as those created by calling QProtobufMessage::constructByName.
*/

/*!
    \class QProtobufMessageDeleter
    \inmodule QtProtobuf
    \brief Calls the destructor of the child class of a QProtobufMessage.

    This class calls the destructor of a protobuf message using the meta-type
    system. This class is intended to be used with smart pointers, such as
    std::unique_ptr.

    \sa QProtobufMessagePointer
*/

/*!
    Destroys the message pointed to by \a ptr.
    This is intended for use with smart pointers.

    \sa QProtobufMessagePointer
*/
void QProtobufMessageDeleter::operator()(QProtobufMessage *ptr) noexcept
{
    if (!ptr)
        return;
    const QMetaObject *mobj = ptr->metaObject();
    QMetaType type = mobj->metaType();
    type.destroy(ptr);
}

QT_END_NAMESPACE

#include "moc_qprotobufmessage.cpp"
