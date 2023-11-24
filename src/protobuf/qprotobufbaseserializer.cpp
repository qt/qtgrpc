// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qprotobufbaseserializer.h"

#include <QtProtobuf/qprotobufmessage.h>

QT_BEGIN_NAMESPACE

/*!
    \class QProtobufBaseSerializer
    \inmodule QtProtobuf
    \since 6.7
    \brief The QProtobufBaseSerializer class is an interface that represents
           basic functions for serializing/deserializing objects, lists, and enums.

    The QProtobufBaseSerializer class registers serializers/deserializers for
    classes implementing a protobuf message, inheriting QProtobufMessage. These
    classes are generated automatically, based on a .proto file, using the cmake
    build macro qt6_add_protobuf or by running qtprotobufgen directly.

    This class should be used as a base for specific serializers. The handlers
    property contains all message-specific serializers and should be used while
    serialization/deserialization. Inherited classes should reimplement scope of
    virtual methods that used by registered message
    serialization/deserialization functions.
*/

/*!
    Destroys this QProtobufBaseSerializer.
*/
QProtobufBaseSerializer::~QProtobufBaseSerializer() = default;

/*!
    \fn QByteArray QProtobufBaseSerializer::serializeObject(const QProtobufMessage *message,
        const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
        const QtProtobufPrivate::QProtobufPropertyOrderingInfo &fieldInfo) const

    This function serializes a registered Protobuf message \a message
    with defined \a ordering and \a fieldInfo, that is recognized
    like an object, into a QByteArray. \a message must not be \nullptr.

    \sa deserializeObject()
*/

/*!
    \fn bool QProtobufBaseSerializer::deserializeObject(QProtobufMessage *message,
        const QtProtobufPrivate::QProtobufPropertyOrdering &ordering,
        QtProtobufPrivate::QProtobufSelfcheckIterator &it) const

    This function deserializes a registered Protobuf message \a message
    with defined \a ordering from the \a it iterator. \a message must not be \nullptr.
    Returns \c true if deserialization was successful, otherwise \c false.

    \sa serializeObject()
*/

/*!
    \relates QProtobufBaseSerializer
    \fn template<typename T> inline void qRegisterProtobufType()

    Registers a Protobuf type \e T.
    This function is normally called by generated code.
*/

/*!
    \relates QProtobufBaseSerializer
    \fn template<typename K, typename V> inline void qRegisterProtobufMapType();

    Registers a Protobuf map type \c K and \c V.
    \c V must be a QProtobufMessage.
    This function is normally called by generated code.
*/

/*!
    \relates QProtobufBaseSerializer
    \fn template<typename T> inline void qRegisterProtobufEnumType();

    Registers serializers for enumeration type \c T in QtProtobuf global
    serializers registry.

    This function is normally called by generated code.
*/

QT_END_NAMESPACE
