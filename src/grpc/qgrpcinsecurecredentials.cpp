// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpcinsecurecredentials.h"

/*!
    \class QGrpcInsecureCallCredentials
    \inmodule QtGRPC

    \brief The QGrpcInsecureCallCredentials class is a dummy implementation
    of QGrpcCallCredentials.

    It doesn't provide any extra information to the channel.
    Use it when you don't need any call credentials.
*/

/*!
    \class QGrpcInsecureChannelCredentials
    \inmodule QtGRPC

    \brief The QGrpcInsecureChannelCredentials class is a dummy implementation
    of QGrpcChannelCredentials.

    It doesn't provide any extra information to the channel.
    Use it when you don't need any channel credentials.
*/

QGrpcInsecureCallCredentials::~QGrpcInsecureCallCredentials() = default;

QGrpcCredentialMap QGrpcInsecureCallCredentials::operator()() const
{
    return QGrpcCredentialMap{};
}

QGrpcInsecureChannelCredentials::~QGrpcInsecureChannelCredentials() = default;

QGrpcCredentialMap QGrpcInsecureChannelCredentials::channelCredentials() const
{
    return QGrpcCredentialMap{};
}
