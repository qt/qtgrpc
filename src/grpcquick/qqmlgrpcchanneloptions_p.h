// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQMLGRPCCHANNELOPTIONS_P_H
#define QQMLGRPCCHANNELOPTIONS_P_H

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

#include "qtgrpcquickexports.h"
#include "qqmlgrpcmetadata_p.h"

#include <QtCore/QMetaType>
#include <QtCore/QtGlobal>
#include <QtQml/qqml.h>
#include <QtGrpc/qgrpcchanneloptions.h>

QT_BEGIN_NAMESPACE

class Q_GRPCQUICK_EXPORT QQmlGrpcChannelOptions : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(GrpcChannelOptions)
    QML_ADDED_IN_VERSION(6, 7)

    Q_PROPERTY(QUrl host READ host WRITE setHost NOTIFY hostChanged REQUIRED)
    Q_PROPERTY(qint64 deadline READ deadline WRITE setDeadline NOTIFY deadlineChanged)
    Q_PROPERTY(QQmlGrpcMetadata *metadata READ metadata WRITE setMetadata NOTIFY metadataChanged)

public:
    QQmlGrpcChannelOptions(QObject *parent = nullptr);

    const QGrpcChannelOptions &options() const;
    QUrl host() const;
    void setHost(const QUrl &newUrl);
    qint64 deadline() const;
    void setDeadline(qint64 value);
    QQmlGrpcMetadata *metadata() const;
    void setMetadata(QQmlGrpcMetadata *value);

signals:
    void hostChanged();
    void deadlineChanged();
    void metadataChanged();

private:
    QGrpcChannelOptions m_options;
    QQmlGrpcMetadata *m_metadata;
};

QT_END_NAMESPACE

#endif // QQMLGRPCCHANNELOPTIONS_P_H
