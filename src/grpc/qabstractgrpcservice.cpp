// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <qtgrpcglobal_p.h>
#include "qabstractgrpcservice.h"

QT_BEGIN_NAMESPACE

/*!
    \class QAbstractGrpcService
    \inmodule QtGrpc
    \brief The QAbstractGrpcService class is bridge between gRPC service.

    QAbstractGrpcService provides set of bridge functions for server classes generated out of
    protobuf services.
 */

class QAbstractGrpcServicePrivate final
{
public:
    QAbstractGrpcServicePrivate() = default;
};

QAbstractGrpcService::QAbstractGrpcService(QObject *parent)
    : QObject(parent), dPtr(std::make_unique<QAbstractGrpcServicePrivate>())
{
}

QAbstractGrpcService::~QAbstractGrpcService()
{
}

QT_END_NAMESPACE
