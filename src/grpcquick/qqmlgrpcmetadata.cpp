// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qqmlgrpcmetadata_p.h"

QT_BEGIN_NAMESPACE

QQmlGrpcMetadata::QQmlGrpcMetadata(QObject *parent)
    : QObject(parent)
{
}

QQmlGrpcMetadata::~QQmlGrpcMetadata() = default;

void QQmlGrpcMetadata::setData(const QVariantMap &value)
{
    if (m_variantdata != value) {
        m_metadata.clear();
        m_variantdata = value;
        for (const QString &key : m_variantdata.keys()) {
            // Case how we trying to cover "multi" values with the same key
            QStringList valueList = m_variantdata.value(key).toString().split(u',');
            for (const QString &val : valueList)
                m_metadata.insert(std::make_pair(key.toUtf8(), val.toUtf8()));
        }
        emit dataChanged();
    }
}

QT_END_NAMESPACE
