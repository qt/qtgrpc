// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpcoperation.h"

#include "qtgrpcglobal_p.h"
#include "qgrpcchanneloperation.h"

#include <QtCore/qpointer.h>
#include <QtCore/private/qobject_p.h>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

/*!
    \class QGrpcOperation
    \inmodule QtGrpc
    \brief The QGrpcOperation class implements common logic to
           handle the gRPC communication from the client side.
*/

/*!
    \fn template <typename T> T QGrpcOperation::read() const;

    Reads message from raw byte array stored in QGrpcOperation.

    Returns a deserialized message or, on failure, a default-constructed
    message.
    If deserialization is not successful the \l QGrpcOperation::errorOccurred
    signal is emitted.
*/

/*!
    \fn void QGrpcOperation::finished()

    This signal indicates the end of communication for this call.

    If signal emitted by stream this means that stream is successfully
    closed either by client or server.
*/

/*!
    \fn void QGrpcOperation::errorOccurred(const QGrpcStatus &status) const

    This signal indicates the error occurred during serialization.

    This signal is emitted when error with \a status occurs in channel
    or during serialization.
*/

class QGrpcOperationPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QGrpcOperation)
public:
    QGrpcOperationPrivate(std::shared_ptr<QGrpcChannelOperation> _channelOperation,
                          std::shared_ptr<QAbstractProtobufSerializer> _serializer)
        : channelOperation(std::move(_channelOperation)), serializer(std::move(_serializer))
    {
    }

    QByteArray data;
    std::shared_ptr<QGrpcChannelOperation> channelOperation;
    std::shared_ptr<QAbstractProtobufSerializer> serializer;
};

QGrpcOperation::QGrpcOperation(std::shared_ptr<QGrpcChannelOperation> channelOperation,
                               std::shared_ptr<QAbstractProtobufSerializer> serializer)
    : QObject(*new QGrpcOperationPrivate(std::move(channelOperation), std::move(serializer)))
{
    [[maybe_unused]] bool valid =
            QObject::connect(d_func()->channelOperation.get(), &QGrpcChannelOperation::dataReady,
                             this, [this](const QByteArray &data) {
                                 Q_D(QGrpcOperation);
                                 d->data = data;
                             });
    Q_ASSERT_X(valid, "QGrpcOperation::QGrpcOperation",
               "Unable to make connection to the 'dataReady' signal");

    valid = QObject::connect(d_func()->channelOperation.get(),
                             &QGrpcChannelOperation::errorOccurred, this,
                             &QGrpcOperation::errorOccurred);
    Q_ASSERT_X(valid, "QGrpcOperation::QGrpcOperation",
               "Unable to make connection to the 'errorOccurred' signal");

    valid = QObject::connect(d_func()->channelOperation.get(), &QGrpcChannelOperation::finished,
                             this, &QGrpcOperation::finished);
    Q_ASSERT_X(valid, "QGrpcOperation::QGrpcOperation",
               "Unable to make connection to the 'finished' signal");
}

QGrpcOperation::~QGrpcOperation() = default;

/*!
    \internal
    Getter of the data received from the channel.
*/
QByteArray QGrpcOperation::data() const
{
    return d_func()->data;
}

/*!
    Getter of the metadata received from the channel. For the HTTP2 channels it
    usually contains the HTTP headers received from the server.
*/
QGrpcMetadata QGrpcOperation::metadata() const
{
    return d_func()->channelOperation->serverMetadata();
}

/*!
    \internal
    Returns a pointer to the assigned channel-side QGrpcChannelOperation.
*/
const QGrpcChannelOperation *QGrpcOperation::channelOperation() const
{
    return d_func()->channelOperation.get();
}

/*!
    \internal
    Getter of the serializer that QGrpcOperation was constructed with.
*/
std::shared_ptr<QAbstractProtobufSerializer> QGrpcOperation::serializer() const
{
    return d_func()->serializer;
}

void QGrpcOperation::cancel()
{
    emit d_func()->channelOperation->cancelled();
    emit errorOccurred({ QGrpcStatus::Cancelled, "Operation is cancelled by client"_L1 });
}

QGrpcStatus QGrpcOperation::deserializationError() const
{
    QGrpcStatus status;
    switch (d_func()->serializer->deserializationError()) {
    case QAbstractProtobufSerializer::InvalidHeaderError: {
        const QLatin1StringView errStr("Response deserialization failed: invalid field found.");
        status = { QGrpcStatus::InvalidArgument, errStr };
        qGrpcWarning() << errStr;
        emit errorOccurred(status);
    } break;
    case QAbstractProtobufSerializer::NoDeserializerError: {
        const QLatin1StringView errStr("No deserializer was found for a given type.");
        status = { QGrpcStatus::InvalidArgument, errStr };
        qGrpcWarning() << errStr;
        emit errorOccurred(status);
    } break;
    case QAbstractProtobufSerializer::UnexpectedEndOfStreamError: {
        const QLatin1StringView errStr("Invalid size of received buffer.");
        status = { QGrpcStatus::OutOfRange, errStr };
        qGrpcWarning() << errStr;
        emit errorOccurred(status);
    } break;
    case QAbstractProtobufSerializer::NoError:
        Q_FALLTHROUGH();
    default:
        const QLatin1StringView errStr("Deserializing failed, but no error was set.");
        status = { QGrpcStatus::InvalidArgument, errStr };
        qGrpcWarning() << errStr;
        emit errorOccurred(status);
    }
    return status;
}

QT_END_NAMESPACE

#include "moc_qgrpcoperation.cpp"
