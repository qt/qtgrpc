// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qqmlgrpcchanneloptions_p.h"
#include <chrono>

QT_BEGIN_NAMESPACE

QQmlGrpcChannelOptions::QQmlGrpcChannelOptions(QObject *parent)
    : QObject(parent),
      m_options(QUrl())
{
}

QUrl QQmlGrpcChannelOptions::host() const
{
    return m_options.host();
}

void QQmlGrpcChannelOptions::setHost(const QUrl &newUrl)
{
    m_options.withHost(newUrl);
    emit hostChanged();
}

qint64 QQmlGrpcChannelOptions::deadline() const
{
    std::chrono::milliseconds ms = m_options.deadline().value_or(std::chrono::milliseconds(0));
    return ms.count();
}

void QQmlGrpcChannelOptions::setDeadline(qint64 value)
{
    std::chrono::milliseconds ms(value);
    m_options.withDeadline(ms);
    emit deadlineChanged();
}

const QGrpcChannelOptions &QQmlGrpcChannelOptions::options() const
{
    return m_options;
}

QQmlGrpcMetadata *QQmlGrpcChannelOptions::metadata() const
{
    return m_metadata;
}

void QQmlGrpcChannelOptions::setMetadata(QQmlGrpcMetadata *value)
{
    if (m_metadata != value) {
        m_metadata = value;
        if (m_metadata)
            m_options.withMetadata(m_metadata->metadata());
        emit metadataChanged();
    }
}

QT_END_NAMESPACE
