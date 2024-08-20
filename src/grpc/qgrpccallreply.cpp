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
