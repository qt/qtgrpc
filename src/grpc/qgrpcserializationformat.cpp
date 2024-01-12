// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpcserializationformat.h"

#include "QtProtobuf/qprotobufjsonserializer.h"
#include "QtProtobuf/qprotobufserializer.h"

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcSerializationFormat
    \inmodule QtGrpc
    \brief The QGrpcSerializationFormat class holds the protobuf message
           serializer and the related content type suffix.
    \since 6.8

    Provides a serializer and the content type assigned to it to
    \l QAbstractGrpcChannel implementations.

    To set serializer format, use either one of the existing presets or the user
    serializer.

    \sa QAbstractGrpcChannel
*/

/*!
    \enum QGrpcSerializationFormat::Format

    Holds presets for setting the protobuf serializer format.

    \value Default   The default QGrpcSerializationFormat preset. This preset
                     constructs \l QProtobufSerializer with an empty content
                     type suffix.
    \value Protobuf  The embedded protobuf QGrpcSerializationFormat preset. This
                     preset constructs \l QProtobufSerializer with the \c proto
                     content type suffix.
    \value Json      The embedded JSON QGrpcSerializationFormat preset. This
                     preset constructs \l QProtobufJsonSerializer with the
                     \c json content type suffix.
    \sa suffix(), serializer()
*/

class QGrpcSerializationFormatPrivate
{
public:
    QGrpcSerializationFormatPrivate(QByteArrayView _suffix,
                                    std::shared_ptr<QAbstractProtobufSerializer> _serializer)
        : suffix(_suffix.toByteArray()), serializer(std::move(_serializer))
    {
    }
    QByteArray suffix;
    std::shared_ptr<QAbstractProtobufSerializer> serializer;
};

/*!
    Creates a new QGrpcSerializationFormat object with the given preset
    \a format.

    The \l QGrpcSerializationFormat::Default format is used by default.
*/
QGrpcSerializationFormat::QGrpcSerializationFormat(Format format)
    : dPtr(format == Format::Json
               ? std::make_unique<
                   QGrpcSerializationFormatPrivate>("json",
                                                    std::make_shared<QProtobufJsonSerializer>())
               : std::make_unique<
                   QGrpcSerializationFormatPrivate>(format == Format::Protobuf ? "proto" : "",
                                                    std::make_shared<QProtobufSerializer>()))
{
}

/*!
    Destroys the QGrpcSerializationFormat object.
*/
QGrpcSerializationFormat::~QGrpcSerializationFormat() = default;

/*!
    Creates a new QGrpcSerializationFormat object with the custom content type
    \a suffix and \a serializer.
*/
QGrpcSerializationFormat::QGrpcSerializationFormat(QByteArrayView suffix,
                                                   std::shared_ptr<QAbstractProtobufSerializer>
                                                       serializer)
    : dPtr(std::make_unique<QGrpcSerializationFormatPrivate>(suffix, std::move(serializer)))
{
}

/*!
    Constructs a copy of \a other.
*/
QGrpcSerializationFormat::QGrpcSerializationFormat(const QGrpcSerializationFormat &other)
    : dPtr(std::make_unique<QGrpcSerializationFormatPrivate>(*other.dPtr))
{
}

/*!
    Move-constructs a QGrpcSerializationFormat instance from \a other.
*/
QGrpcSerializationFormat::QGrpcSerializationFormat(QGrpcSerializationFormat &&other)
    : dPtr(std::exchange(other.dPtr, nullptr))
{
}

/*!
    Assigns the \a other QGrpcSerializationFormat object to this one.
*/
QGrpcSerializationFormat &QGrpcSerializationFormat::operator=(const QGrpcSerializationFormat &other)
{
    if (this != &other)
        *dPtr = *other.dPtr;
    return *this;
}

/*!
    Moves the \a other QGrpcSerializationFormat object to this one.
*/
QGrpcSerializationFormat &QGrpcSerializationFormat::operator=(QGrpcSerializationFormat &&other)
{
    if (this != &other)
        dPtr = std::exchange(other.dPtr, nullptr);
    return *this;
}

/*!
    Returns the content type suffix for this serialization format.
*/
QByteArray QGrpcSerializationFormat::suffix() const
{
    return dPtr->suffix;
}

/*!
    Returns the serializer for this serialization format.

    /sa QAbstractProtobufSerializer
*/
std::shared_ptr<QAbstractProtobufSerializer> QGrpcSerializationFormat::serializer() const
{
    return dPtr->serializer;
}

QT_END_NAMESPACE
