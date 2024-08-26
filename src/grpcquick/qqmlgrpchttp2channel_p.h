// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QQMLGRPCHTTP2CHANNEL_H
#define QQMLGRPCHTTP2CHANNEL_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGrpcQuick/private/qqmlgrpcchanneloptions_p.h>
#include <QtGrpcQuick/qqmlabstractgrpcchannel.h>
#include <QtGrpcQuick/qtgrpcquickexports.h>

#include <QtQmlIntegration/qqmlintegration.h>

QT_BEGIN_NAMESPACE

class Q_GRPCQUICK_EXPORT QQmlGrpcHttp2Channel : public QQmlAbstractGrpcChannel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(GrpcHttp2Channel)
    QML_ADDED_IN_VERSION(6, 7)
    Q_PROPERTY(QUrl hostUri READ hostUri WRITE setHostUri NOTIFY hostUriChanged REQUIRED)
    Q_PROPERTY(QQmlGrpcChannelOptions *options READ options WRITE setOptions NOTIFY optionsChanged
                   REQUIRED)
    Q_PROPERTY(std::shared_ptr<QAbstractGrpcChannel> channel READ channel NOTIFY channelUpdated)

public:
    explicit QQmlGrpcHttp2Channel(QObject *parent = nullptr);
    ~QQmlGrpcHttp2Channel() override;
    std::shared_ptr<QAbstractGrpcChannel> channel() final;
    QQmlGrpcChannelOptions *options() const { return m_options; }
    void setOptions(QQmlGrpcChannelOptions *options);

    QUrl hostUri() const noexcept { return m_hostUri; }
    void setHostUri(const QUrl &hostUri);
Q_SIGNALS:
    void optionsChanged();
    void channelUpdated();
    void hostUriChanged();

private:
    void updateChannel();

    QQmlGrpcChannelOptions *m_options;
    QUrl m_hostUri;
    std::shared_ptr<QAbstractGrpcChannel> m_channel;
};

QT_END_NAMESPACE

#endif // QQMLGRPCHTTP2CHANNEL_H
