// Copyright (C) 2023 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QImage>
#include <QBuffer>

#include "chatmessagemodel.h"
#include "simplechat.qpb.h"

namespace {
enum Role {
    Content = Qt::UserRole + 1,
    Type,
    From,
};

QString getImage(const QByteArray &data)
{
    return QString("data:image/png;base64,") + data.toBase64();
}

QString getImageScaled(const QByteArray &data)
{
    QImage img = QImage::fromData(data, "PNG");
    img = img.scaled(300, 300, Qt::KeepAspectRatio);
    QByteArray scaledData;
    QBuffer buffer(&scaledData);
    img.save(&buffer, "PNG");
    return getImage(scaledData);
}

QString getText(const QByteArray &data)
{
    return QString::fromUtf8(data);
}
} // namespace

ChatMessageModel::ChatMessageModel(QObject *parent) : QAbstractListModel(parent) { }

QHash<int, QByteArray> ChatMessageModel::roleNames() const
{
    static QHash<int, QByteArray> s_roleNames;
    if (s_roleNames.isEmpty()) {
        s_roleNames.insert(Content, "content");
        s_roleNames.insert(Type, "type");
        s_roleNames.insert(From, "from");
    }
    return s_roleNames;
}

QVariant ChatMessageModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if (row < 0 || row >= m_container.count() || !m_container.at(row))
        return QVariant();

    auto dataPtr = m_container.at(row).get();

    switch (role) {
    case Content:
        if (dataPtr->type() == qtgrpc::examples::chat::ChatMessage::Image)
            return QVariant::fromValue(getImageScaled(dataPtr->content()));
        else
            return QVariant::fromValue(getText(dataPtr->content()));
    case Type:
        return QVariant::fromValue(dataPtr->type());
    case From:
        return QVariant::fromValue(dataPtr->from());
    }

    return QVariant();
}

int ChatMessageModel::rowCount(const QModelIndex &) const
{
    return m_container.count();
}

void ChatMessageModel::append(
        const QList<std::shared_ptr<qtgrpc::examples::chat::ChatMessage>> &messages)
{
    if (messages.size() > 0) {
        beginInsertRows(QModelIndex(), m_container.size(),
                        m_container.size() + messages.size() - 1);
        m_container.append(messages);
        endInsertRows();
    }
}
