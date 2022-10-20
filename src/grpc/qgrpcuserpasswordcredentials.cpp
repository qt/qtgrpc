// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgrpcuserpasswordcredentials.h"

QT_BEGIN_NAMESPACE

namespace {
constexpr char defaultUserFieldName[] = "user-name";
constexpr char defaultPasswordFieldName[] = "user-password";
}

/*!
    \class QGrpcUserPasswordCredentials
    \inmodule QtGrpc

    \brief The QGrpcUserPasswordCredentials class is reference
    implementation of simple user-password call authentication.

    You may manually specify UserFieldName and PasswordFieldName in
    class template parameters to change header keys.
    Default values for:
    \code
    constexpr char defaultUserFieldName[] = "user-name";
    constexpr char defaultPasswordFieldName[] = "user-password";
    \endcode
 */

QGrpcUserPasswordCredentials::QGrpcUserPasswordCredentials(const QString &userName,
                                                           const QString &password)
    : QGrpcUserPasswordCredentials(defaultUserFieldName, userName, defaultPasswordFieldName,
                                   password)
{
}

QGrpcUserPasswordCredentials::QGrpcUserPasswordCredentials(const QByteArray &userNameField,
                                                           const QString &userName,
                                                           const QByteArray &passwordField,
                                                           const QString &password)
    : m_map(QGrpcCredentialMap{ { userNameField, QVariant::fromValue(userName) },
                                { passwordField, QVariant::fromValue(password) } })
{
}

QGrpcUserPasswordCredentials::QGrpcUserPasswordCredentials() = default;

QT_END_NAMESPACE
