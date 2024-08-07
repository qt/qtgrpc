// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpccallreply.h>
#include <QtGrpc/qgrpcoperationcontext.h>

#include <QtCore/qeventloop.h>
#include <QtCore/qthread.h>

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
    \fn template <typename FinishCallback> void QGrpcCallReply::subscribe(const QObject *receiver, FinishCallback &&finishCallback, Qt::ConnectionType type = Qt::SingleShotConnection)

    Convenience function to connect the \a finishCallback of \a receiver to the
    finished() signal with given connection \a type.

    Calling this function is equivalent to the following:
    \code
        QObject::connect(this, &QGrpcCallReply::finished, receiver,
                         std::forward<FinishCallback>(finishCallback), type);
    \endcode

    \note The finished() signal is emitted only once and in most cases you
    would want to disconnect right after receiving it. An easy way to achieve
    that is by using the Qt::SingleShotConnection \l {Qt::} {ConnectionType}.
*/

QGrpcCallReply::QGrpcCallReply(std::shared_ptr<QGrpcOperationContext> operationContext)
    : QGrpcOperation(std::move(operationContext))
{
}

QGrpcCallReply::~QGrpcCallReply() = default;

bool QGrpcCallReply::event(QEvent *event)
{
    return QObject::event(event);
}

QT_END_NAMESPACE

#include "moc_qgrpccallreply.cpp"
