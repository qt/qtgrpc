// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufregistration.h>
#include <QtProtobuf/qprotobufrepeatediterator.h>

/*!
    \class QProtobufRepeatedIterator
    \inmodule QtProtobuf
    \since 6.8

    \brief Allows iterating over repeated protobuf types.

    Allows iterating over repeated protobuf types and access the repeated field elements as
    reference to QProtobufMessage.
*/

/*!
    Constructs iterator from \a data pointer.
*/
QProtobufRepeatedIterator::QProtobufRepeatedIterator(QtProtobufPrivate::AbstractRepeatedIterator
                                                         *data) noexcept
    : m_data(data)
{
}

/*!
    Destroys iterator.
*/
QProtobufRepeatedIterator::~QProtobufRepeatedIterator() noexcept
{
    delete m_data;
}

/*!
    \fn QProtobufRepeatedIterator::QProtobufRepeatedIterator(QProtobufRepeatedIterator &&other) noexcept

    Move-constructs a new QProtobufRepeatedIterator from \a other.
*/

/*!
    \fn QProtobufRepeatedIterator &QProtobufRepeatedIterator::operator=(QProtobufRepeatedIterator &&other) noexcept

    Move-assigns \a other to this QProtobufRepeatedIterator and returns a reference to
    the updated object.
*/

/*!
    \fn bool QProtobufRepeatedIterator::isValid() const noexcept

    Returns \c true if the iterator points to a valid data object.
*/

/*!
    Returns \c true if the iterator can read the next element from the repeated field.
*/
bool QProtobufRepeatedIterator::hasNext() const noexcept
{
    return m_data->hasNext();
}

/*!
    Returns the next element under from the repeated field.
*/
QProtobufMessage &QProtobufRepeatedIterator::next() const noexcept
{
    return m_data->next();
}

/*!
    Returns the reference to the new temporary element in the repeated field.
*/
QProtobufMessage &QProtobufRepeatedIterator::addNext() noexcept
{
    return m_data->addNext();
}

/*!
    Adds the element, created by addNext function, to the repeated field.
*/
void QProtobufRepeatedIterator::push() const noexcept
{
    return m_data->push();
}
