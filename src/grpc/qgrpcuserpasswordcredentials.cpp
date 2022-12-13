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
    \inmodule QtGRPC

    \brief The QGrpcUserPasswordCredentials class is a reference
    implementation of simple user-password call authentication.

    You may manually specify UserFieldName and PasswordFieldName in
    class template parameters to change header keys.
    The \c userNameField defaults to \c{"user-name"},
    the \c passwordField defaults to \c{"user-password"}.
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

QGrpcUserPasswordCredentials::~QGrpcUserPasswordCredentials() = default;

QGrpcCredentialMap QGrpcUserPasswordCredentials::operator()() const
{
    return m_map;
}

QT_END_NAMESPACE
