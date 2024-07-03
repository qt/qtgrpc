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
    \fn template <typename ReceiverType, typename FinishCallback, typename ErrorCallback> void QGrpcCallReply::subscribe(
    ReceiverType *receiver, FinishCallback &&finishCallback, ErrorCallback &&errorCallback,
    Qt::ConnectionType type = Qt::AutoConnection)

    Convenience function to connect the \a finishCallback and
    \a errorCallback of \a receiver to the QGrpcCallReply::finished signal with
    the given connection \a type. The \a finishCallback is called when the
    QGrpcCallReply is finished with the QGrpcStatus::Ok status code, \a errorCallback is called
    otherwise.
*/

/*!
    \fn template <typename ReceiverType, typename FinishCallback> void QGrpcCallReply::subscribe(
    ReceiverType *receiver, FinishCallback &&finishCallback,
    Qt::ConnectionType type = Qt::AutoConnection)

    Convenience function to connect the \a finishCallback of \a receiver to
    the QGrpcCallReply::finished signal with given connection \a type.

    Calling this function is equivalent to the following:
    \code
        QObject::connect(this, &QGrpcCallReply::finished, receiver,
                         std::forward<FinishCallback>(finishCallback), type);
    \endcode
*/

QGrpcCallReply::QGrpcCallReply(std::shared_ptr<QGrpcOperationContext> operationContext)
    : QGrpcOperation(std::move(operationContext))
{
}

QGrpcCallReply::~QGrpcCallReply() = default;

QT_END_NAMESPACE

#include "moc_qgrpccallreply.cpp"
