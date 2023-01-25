// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qabstractgrpcchannel.h"
#include "qabstractgrpcchannel_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QAbstractGrpcChannel
    \inmodule QtGRPC
    \brief The QAbstractGrpcChannel class is interface that represents common
    gRPC channel functionality.

    You may implement this interface to create own channels for gRPC transport.
*/

QAbstractGrpcChannel::QAbstractGrpcChannel() : dPtr(std::make_unique<QAbstractGrpcChannelPrivate>())
{
}
QAbstractGrpcChannel::~QAbstractGrpcChannel() = default;

QT_END_NAMESPACE
