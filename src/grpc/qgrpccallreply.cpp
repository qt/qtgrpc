// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpccallreply.h"
#include "qgrpcchanneloperation.h"

#include <QtCore/QThread>
#include <QtCore/QEventLoop>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

/*!
    \class QGrpcCallReply
    \inmodule QtGrpc

    \brief The QGrpcCallReply class implements logic to handle gRPC calls
    from the gRPC client side.

    The QGrpcCallReply object is owned by the client object that created it.
*/

/*!
    \fn template <typename Func1, typename Func2> void QGrpcCallReply::subscribe(QObject *receiver,
    Func1 &&finishCallback, Func2 &&errorCallback, Qt::ConnectionType type = Qt::AutoConnection);

    Convenience function to connect the \a finishCallback and
    \a errorCallback of \a receiver to the QGrpcCallReply::finished and
    the QGrpcCallReply::errorOccurred signals with the given connection \a type.

    Calling this function is equivalent to the following:
    \code
        QObject::connect(this, &QGrpcCallReply::finished, receiver,
                         std::forward<Func1>(finishCallback), type);
        QObject::connect(this, &QGrpcCallReply::errorOccurred, receiver,
                         std::forward<Func2>(errorCallback), type);
    \endcode
*/

/*!
    \fn template <typename Func1> void QGrpcCallReply::subscribe(QObject *receiver,
    Func1 &&finishCallback, Qt::ConnectionType type = Qt::AutoConnection);

    Convenience function to connect the \a finishCallback of \a receiver to
    the QGrpcCallReply::finished signal with given connection \a type.

    Calling this function is equivalent to the following:
    \code
        QObject::connect(this, &QGrpcCallReply::finished, receiver,
                         std::forward<Func1>(finishCallback), type);
    \endcode
*/

QGrpcCallReply::QGrpcCallReply(std::shared_ptr<QGrpcChannelOperation> channelOperation,
                               std::shared_ptr<QAbstractProtobufSerializer> serializer)
    : QGrpcOperation(std::move(channelOperation), std::move(serializer))
{
}

QGrpcCallReply::~QGrpcCallReply() = default;

/*!
    Waits for the call either finished or returned the error. Returns the
    resulting QGrpcStatus of the call. If the call was successful, the received
    response can be read using the QGrpcCallReply::read method.

    To control the maximum waiting time, use \c QGrpcChannelOptions or
    \c QGrpcCallOptions, otherwise the call may be suspended indefinitely.
*/
QGrpcStatus QGrpcCallReply::waitForFinished() const
{
    QEventLoop loop;
    QGrpcStatus status;
    QObject::connect(this, &QGrpcCallReply::errorOccurred,
                     [&status, &loop](const QGrpcStatus &error) {
                         status = error;
                         loop.quit();
                     });
    QObject::connect(this, &QGrpcCallReply::finished, &loop, &QEventLoop::quit);

    loop.exec();
    return status;
}

QT_END_NAMESPACE

#include "moc_qgrpccallreply.cpp"
