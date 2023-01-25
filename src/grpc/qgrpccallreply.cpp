// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpccallreply.h"
#include <QtCore/QThread>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

/*!
    \class QGrpcCallReply
    \inmodule QtGrpc

    \brief The QGrpcCallReply class contains data for asynchronous call
    of gRPC client API.

    It's owned by the client class that created it.
    QGrpcCallReply could be used by QAbstractGrpcChannel implementations
    to control call workflow and abort calls if possible in the event
    of QGrpcCallReply::abort being called by a library user.
*/

/*!
    \fn template <typename Func1, typename Func2> void subscribe(QObject *receiver,
    Func1 &&finishCallback, Func2 &&errorCallback, Qt::ConnectionType type = Qt::AutoConnection);

    Convenience function to connect the \a finishCallback and
    \a errorCallback of \a receiver to the QGrpcCallReply::finished and
    the QGrpcCallReply::errorOccurred signals with the given connection \a type.

    Calling this function is equivalent to the following:
    \code
        QObject::connect(this, &QGrpcCallReply::finished, receiver, finishCallback, type);
        QObject::connect(this, &QGrpcCallReply::errorOccurred, receiver, errorCallback, type);
    \endcode
*/

/*!
    \fn template <typename Func1> void subscribe(QObject *receiver,
    Func1 &&finishCallback, Qt::ConnectionType type = Qt::AutoConnection);

    Convenience function to connect the \a finishCallback of \a receiver to
    the QGrpcCallReply::finished signal with given connection \a type.

    Calling this function is equivalent to the following:
    \code
        QObject::connect(this, &QGrpcCallReply::finished, receiver, finishCallback, type);
    \endcode
*/

QGrpcCallReply::QGrpcCallReply(QAbstractGrpcClient *parent) : QGrpcOperation(parent)
{
}

QGrpcCallReply::~QGrpcCallReply() = default;

/*!
    Aborts this reply and try to abort call in channel.
*/
void QGrpcCallReply::abort()
{
    auto abortFunc = [&] {
        setData({});
        errorOccurred({ QGrpcStatus::StatusCode::Aborted, "Call aborted by user or timeout"_L1 });
    };
    if (thread() != QThread::currentThread())
        QMetaObject::invokeMethod(this, abortFunc, Qt::BlockingQueuedConnection);
    else
        abortFunc();
}

QT_END_NAMESPACE

#include "moc_qgrpccallreply.cpp"
