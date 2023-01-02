// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qabstractgrpcservice.h"
#include <qtgrpcglobal_p.h>

#include <private/qobject_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QAbstractGrpcService
    \inmodule QtGrpc
    \brief The QAbstractGrpcService class is a bridge between gRPC service.

    QAbstractGrpcService provides set of bridge functions for server classes
    generated out of protobuf services.
*/

class QAbstractGrpcServicePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAbstractGrpcService)
public:
    QAbstractGrpcServicePrivate() = default;
};

QAbstractGrpcService::QAbstractGrpcService(QObject *parent)
    : QObject(*new QAbstractGrpcServicePrivate, parent)
{
}

QAbstractGrpcService::~QAbstractGrpcService() = default;

QT_END_NAMESPACE
