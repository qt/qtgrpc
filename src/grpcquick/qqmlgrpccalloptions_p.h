// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QQMLGRPCCALLOPTIONS_P_H
#define QQMLGRPCCALLOPTIONS_P_H

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

#include <QtGrpcQuick/qtgrpcquickexports.h>
#include <QtGrpcQuick/private/qqmlgrpcmetadata_p.h>

#include <QtGrpc/qgrpccalloptions.h>

#include <QtQmlIntegration/qqmlintegration.h>

#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

class Q_GRPCQUICK_EXPORT QQmlGrpcCallOptions : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(GrpcCallOptions)
    QML_ADDED_IN_VERSION(6, 7)

    Q_PROPERTY(qint64 deadlineTimeout READ deadlineTimeout WRITE setDeadlineTimeout NOTIFY deadlineTimeoutChanged)
    Q_PROPERTY(QQmlGrpcMetadata *metadata READ metadata WRITE setMetadata NOTIFY metadataChanged)

public:
    explicit QQmlGrpcCallOptions(QObject *parent = nullptr);
    ~QQmlGrpcCallOptions() override;

    const QGrpcCallOptions &options() const & noexcept;
    void options() && = delete;

    qint64 deadlineTimeout() const;
    void setDeadlineTimeout(qint64 value);
    QQmlGrpcMetadata *metadata() const;
    void setMetadata(QQmlGrpcMetadata *value);

Q_SIGNALS:
    void deadlineTimeoutChanged();
    void metadataChanged();

private:
    QGrpcCallOptions m_options;
    QQmlGrpcMetadata *m_metadata;
};

QT_END_NAMESPACE

#endif // QQMLGRPCCALLOPTIONS_P_H
