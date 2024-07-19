// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qqmlgrpchttp2channel_p.h"
#include <QtGrpc/qgrpchttp2channel.h>

#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

QQmlGrpcHttp2Channel::QQmlGrpcHttp2Channel(QObject *parent)
    : QQmlAbstractGrpcChannel(parent),
      m_options(nullptr)
{
    connect(this, &QQmlGrpcHttp2Channel::optionsChanged,
            this, &QQmlGrpcHttp2Channel::updateChannel);
}

QQmlGrpcHttp2Channel::~QQmlGrpcHttp2Channel() = default;

std::shared_ptr<QAbstractGrpcChannel> QQmlGrpcHttp2Channel::getChannel()
{
    return m_channel;
}

void QQmlGrpcHttp2Channel::setOptions(QQmlGrpcChannelOptions *options)
{
    if (options == nullptr || m_options == options)
        return;

    m_options = options;
    emit optionsChanged();
}

void QQmlGrpcHttp2Channel::updateChannel()
{
    if (m_hostUri.isEmpty())
        return;

    if (!m_hostUri.isValid()) {
        qWarning() << "Unable to initialize the channel. The host URI is not valid.";
        return;
    }

    if (m_channel)
        m_channel.reset();

    if (m_hostUri.isValid()) {
        m_channel = m_options ? std::make_shared<QGrpcHttp2Channel>(m_hostUri, m_options->options())
                              : std::make_shared<QGrpcHttp2Channel>(m_hostUri);
    }
    emit channelUpdated();
}

void QQmlGrpcHttp2Channel::setHostUri(const QUrl &hostUri)
{
    if (hostUri == m_hostUri)
        return;

    if (m_channel) {
        qWarning() << "Changing the host URI is not supported.";
        return;
    }

    m_hostUri = hostUri;

    emit hostUriChanged();
    updateChannel();
}

QT_END_NAMESPACE

#include "moc_qqmlgrpchttp2channel_p.cpp"
