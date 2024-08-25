// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpcQuick/private/qqmlgrpcmetadata_p.h>
#include <QtGrpcQuick/private/qqmlgrpccalloptions_p.h>

#include <QtCore/private/qobject_p.h>

#include <chrono>

using namespace std::chrono_literals;

QT_BEGIN_NAMESPACE

QQmlGrpcCallOptions::QQmlGrpcCallOptions(QObject *parent)
    : QObject(parent)
{
}

QQmlGrpcCallOptions::~QQmlGrpcCallOptions() = default;

qint64 QQmlGrpcCallOptions::deadlineTimeout() const
{
    std::chrono::milliseconds ms = m_options.deadlineTimeout().value_or(0ms);
    return ms.count();
}

void QQmlGrpcCallOptions::setDeadlineTimeout(qint64 value)
{
    std::chrono::milliseconds ms(value);
    m_options.setDeadlineTimeout(ms);
    emit deadlineTimeoutChanged();
}

const QGrpcCallOptions &QQmlGrpcCallOptions::options() const & noexcept
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
            m_options.setMetadata({});
        emit metadataChanged();
    }
}

QT_END_NAMESPACE

#include "moc_qqmlgrpccalloptions_p.cpp"
