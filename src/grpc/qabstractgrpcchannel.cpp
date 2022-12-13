// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qabstractgrpcchannel.h"

QT_BEGIN_NAMESPACE

/*!
    \class QAbstractGrpcChannel
    \inmodule QtGRPC
    \brief The QAbstractGrpcChannel class is interface that represents common
    gRPC channel functionality.

    You may implement this interface to create own channels for gRPC transport.
*/

struct QAbstractGrpcChannelPrivate
{
    QAbstractGrpcChannelPrivate() : thread(QThread::currentThread()) { }
    const QThread *thread;
};

QAbstractGrpcChannel::QAbstractGrpcChannel() : dPtr(std::make_unique<QAbstractGrpcChannelPrivate>())
{
}
QAbstractGrpcChannel::~QAbstractGrpcChannel() = default;

const QThread *QAbstractGrpcChannel::thread() const
{
    return dPtr->thread;
}

QT_END_NAMESPACE
