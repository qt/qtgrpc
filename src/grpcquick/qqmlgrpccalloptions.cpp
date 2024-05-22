// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qqmlgrpccalloptions_p.h"

#include <chrono>

QT_BEGIN_NAMESPACE

QQmlGrpcCallOptions::QQmlGrpcCallOptions(QObject *parent)
    : QObject(parent),
      m_metadata(nullptr)
{
}

QQmlGrpcCallOptions::~QQmlGrpcCallOptions() = default;

qint64 QQmlGrpcCallOptions::deadline() const
{
    QGrpcDuration ms = m_options.deadline().value_or(QGrpcDuration(0));
    return ms.count();
}

void QQmlGrpcCallOptions::setDeadline(qint64 value)
{
    QGrpcDuration ms(value);
    m_options.setDeadline(ms);
    emit deadlineChanged();
}

const QGrpcCallOptions &QQmlGrpcCallOptions::options() const
{
    return m_options;
}

QQmlGrpcMetadata *QQmlGrpcCallOptions::metadata() const
{
    return m_metadata;
}

void QQmlGrpcCallOptions::setMetadata(QQmlGrpcMetadata *value)
{
    if (m_metadata != value) {
        m_metadata = value;
        if (m_metadata)
            m_options.setMetadata(m_metadata->metadata());
        else
            m_options.setMetadata(QGrpcMetadata());
        emit metadataChanged();
    }
}

QT_END_NAMESPACE
