// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qqmlgrpcchanneloptions_p.h"
#include "qqmlgrpcmetadata_p.h"
#include <QtGrpc/qgrpcserializationformat.h>
#include <QtCore/private/qobject_p.h>

#include <chrono>

using namespace std::chrono_literals;

QT_BEGIN_NAMESPACE

class QQmlGrpcChannelOptionsPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQmlGrpcChannelOptions)

public:
    QQmlGrpcChannelOptionsPrivate();

    QGrpcChannelOptions m_options;
    QtGrpc::SerializationFormat m_format = QtGrpc::SerializationFormat::Default;
    QQmlGrpcMetadata *m_metadata = nullptr;
#if QT_CONFIG(ssl)
    QQmlSslConfiguration m_configuration;
#endif // QT_CONFIG(ssl)
};

QQmlGrpcChannelOptionsPrivate::QQmlGrpcChannelOptionsPrivate() : QObjectPrivate()
{
}

QQmlGrpcChannelOptions::QQmlGrpcChannelOptions(QObject *parent)
    : QObject(*(new QQmlGrpcChannelOptionsPrivate()), parent)
{
}

qint64 QQmlGrpcChannelOptions::deadlineTimeout() const
{
    std::chrono::milliseconds ms = d_func()->m_options.deadlineTimeout().value_or(0ms);
    return ms.count();
}

void QQmlGrpcChannelOptions::setDeadlineTimeout(qint64 value)
{
    Q_D(QQmlGrpcChannelOptions);
    std::chrono::milliseconds ms(value);
    d->m_options.setDeadlineTimeout(ms);
    emit deadlineTimeoutChanged();
}

const QGrpcChannelOptions &QQmlGrpcChannelOptions::options() const
{
    return d_func()->m_options;
}

QQmlGrpcMetadata *QQmlGrpcChannelOptions::metadata() const
{
    return d_func()->m_metadata;
}

void QQmlGrpcChannelOptions::setMetadata(QQmlGrpcMetadata *value)
{
    Q_D(QQmlGrpcChannelOptions);
    if (d->m_metadata != value) {
        d->m_metadata = value;
        if (d->m_metadata)
            d->m_options.setMetadata(d->m_metadata->metadata());
        else
            d->m_options.setMetadata({});
        emit metadataChanged();
    }
}

QtGrpc::SerializationFormat QQmlGrpcChannelOptions::serializationFormat() const
{
    return d_func()->m_format;
}

void QQmlGrpcChannelOptions::setSerializationFormat(QtGrpc::SerializationFormat format)
{
    Q_D(QQmlGrpcChannelOptions);
    if (d->m_format != format) {
        d->m_format = format;
        d->m_options.setSerializationFormat(format);
        emit serializationFormatChanged();
    }
}

#if QT_CONFIG(ssl)
QQmlSslConfiguration QQmlGrpcChannelOptions::sslConfiguration() const
{
    return d_func()->m_configuration;
}

void QQmlGrpcChannelOptions::setSslConfiguration(const QQmlSslConfiguration &config)
{
    Q_D(QQmlGrpcChannelOptions);
    if (d->m_configuration != config) {
        d->m_configuration = config;
        d->m_options.setSslConfiguration(d->m_configuration.configuration());
        emit sslConfigurationChanged();
    }
}
#endif // QT_CONFIG(ssl)

QT_END_NAMESPACE

#include "moc_qqmlgrpcchanneloptions_p.cpp"
