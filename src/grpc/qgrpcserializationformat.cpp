// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpcserializationformat.h>

#include <QtProtobuf/qprotobufjsonserializer.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qdebug.h>
#include <QtCore/qvariant.h>

using namespace QtGrpc;

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcSerializationFormat
    \inmodule QtGrpc
    \compares equality
    \brief The QGrpcSerializationFormat class holds the protobuf message
           serializer and the related content type suffix.
    \since 6.8

    Provides a serializer and the content type assigned to it to
    \l QAbstractGrpcChannel implementations.

    To set serializer format, use either one of the existing presets or the user
    serializer.

    \sa QAbstractGrpcChannel
*/

class QGrpcSerializationFormatPrivate : public QSharedData
{
public:
    QGrpcSerializationFormatPrivate(QByteArrayView suffix_,
                                    std::shared_ptr<QAbstractProtobufSerializer> serializer_)
        : suffix(suffix_.toByteArray()), serializer(std::move(serializer_))
    {
    }

    QByteArray suffix;
    std::shared_ptr<QAbstractProtobufSerializer> serializer;
};

QT_DEFINE_QESDP_SPECIALIZATION_DTOR(QGrpcSerializationFormatPrivate)

/*!
    Creates a new QGrpcSerializationFormat object with the given preset
    \a format.

    A \l QtGrpc::SerializationFormat::Default format is used by default.
*/
QGrpcSerializationFormat::QGrpcSerializationFormat(SerializationFormat format)
    : d_ptr(format == SerializationFormat::Json
                ? new QGrpcSerializationFormatPrivate("json",
                                                      std::make_shared<QProtobufJsonSerializer>())
                : new QGrpcSerializationFormatPrivate(format == SerializationFormat::Protobuf
                                                          ? "proto"
                                                          : "",
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
    : d_ptr(new QGrpcSerializationFormatPrivate(suffix, std::move(serializer)))
{
}

/*!
    Constructs a copy of \a other.
*/
QGrpcSerializationFormat::QGrpcSerializationFormat(const QGrpcSerializationFormat &other) = default;

/*!
    Assigns the \a other QGrpcSerializationFormat object to this one.
*/
QGrpcSerializationFormat &
QGrpcSerializationFormat::operator=(const QGrpcSerializationFormat &other) = default;

/*!
    \fn QGrpcSerializationFormat::QGrpcSerializationFormat(QGrpcSerializationFormat &&other) noexcept
    Move-constructs a new QGrpcSerializationFormat from \a other.

    \note The moved-from object \a other is placed in a partially-formed state,
    in which the only valid operations are destruction and assignment of a new
    value.
*/

/*!
    \fn QGrpcSerializationFormat &QGrpcSerializationFormat::operator=(QGrpcSerializationFormat &&other) noexcept
    Move-assigns \a other to this QGrpcSerializationFormat instance and returns
    a reference to it.

    \note The moved-from object \a other is placed in a partially-formed state,
    in which the only valid operations are destruction and assignment of a new
    value.
*/

/*!
    \since 6.8
    \fn void QGrpcSerializationFormat::swap(QGrpcSerializationFormat &other) noexcept
    Swaps this instance with \a other. This operation is very fast and never fails.
*/

/*!
    \since 6.8
    Constructs a new QVariant object from this QGrpcSerializationFormat.
*/
QGrpcSerializationFormat::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

/*!
    Returns the content type suffix for this serialization format.
*/
QByteArrayView QGrpcSerializationFormat::suffix() const noexcept
{
    Q_D(const QGrpcSerializationFormat);
    return d->suffix;
}

/*!
    Returns the serializer for this serialization format.

    /sa QAbstractProtobufSerializer
*/
std::shared_ptr<QAbstractProtobufSerializer> QGrpcSerializationFormat::serializer() const
{
    Q_D(const QGrpcSerializationFormat);
    return d->serializer;
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \since 6.8
    \fn QDebug QGrpcSerializationFormat::operator<<(QDebug debug, const QGrpcSerializationFormat &sfmt)
    Writes \a sfmt to the specified stream \a debug.
*/
QDebug operator<<(QDebug debug, const QGrpcSerializationFormat &sfmt)
{
    const QDebugStateSaver save(debug);
    debug.nospace().noquote();
    debug << "QGrpcSerializationFormat(suffix: " << sfmt.suffix() << ", serializer: { "
          << static_cast<void *>(sfmt.serializer().get())
          << ", useCount: " << sfmt.serializer().use_count() << " })";
    return debug;
}
#endif

bool comparesEqual(const QGrpcSerializationFormat &lhs,
                   const QGrpcSerializationFormat &rhs) noexcept
{
    return lhs.d_func()->suffix == rhs.d_func()->suffix
        && lhs.d_func()->serializer == rhs.d_func()->serializer;
}

size_t qHash(const QGrpcSerializationFormat &key, size_t seed) noexcept
{
    return qHashMulti(seed, key.d_func()->suffix, key.d_func()->serializer.get());
}

QT_END_NAMESPACE
