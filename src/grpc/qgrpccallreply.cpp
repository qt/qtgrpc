// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QThread>

#include "qgrpccallreply.h"

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcCallReply
    \inmodule QtGrpc

    \brief The QGrpcCallReply class contains data for asynchronous call of gRPC client API.

    It's owned by the client class that created it.
    QGrpcCallReply could be used by QAbstractGrpcChannel implementations to control call work
   flow
    and abort calls if possible in case if QGrpcCallReply::abort method called by library user.
 */

/*!
    \fn template <typename Func1, typename Func2> inline void subscribe(QObject *receiver,
    Func1 finishCallback, Func2 errorCallback, Qt::ConnectionType type = Qt::AutoConnection);

    Convenience function to connect \a receiver \a finishCallback and \a errorCallback to
    QGrpcCallReply::finished and QGrpcCallReply::erroOccurred signals with given connection \a type.

    Calling this function is equivalent to the following:
    \badcode
        QObject::connect(this, &QGrpcCallReply::finished, receiver, finishCallback, type);
        QObject::connect(this, &QGrpcCallReply::errorOccurred, receiver, errorCallback, type);
    \endcode
 */

/*!
    \fn template <typename Func1> inline void subscribe(QObject *receiver,
    Func1 finishCallback, Qt::ConnectionType type = Qt::AutoConnection);

    Convenience function to connect \a receiver \a finishCallback to
    QGrpcCallReply::finished signal with given connection \a type.

    Calling this function is equivalent to the following:
    \badcode
        QObject::connect(this, &QGrpcCallReply::finished, receiver, finishCallback, type);
    \endcode
 */

/*!
    Aborts this reply and try to abort call in channel.
 */
void QGrpcCallReply::abort()
{
    auto abortFunc = [this]() {
        this->setData({});
        errorOccurred({ QGrpcStatus::StatusCode::Aborted,
                        QLatin1StringView("Call aborted by user or timeout") });
    };
    if (thread() != QThread::currentThread())
        QMetaObject::invokeMethod(this, abortFunc, Qt::BlockingQueuedConnection);
    else
        abortFunc();
}
QT_END_NAMESPACE
