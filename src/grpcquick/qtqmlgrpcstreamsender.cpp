// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpcQuick/qtqmlgrpcstreamsender.h>

#include <QtCore/private/qobject_p.h>

QT_BEGIN_NAMESPACE

bool QtQmlGrpcStreamSender::isValidStream(const QGrpcOperation *stream)
{
    Q_ASSERT_X(stream, "writeMessageImpl", "stream is nullptr");

    if (stream->isFinished()) {
        qWarning("Unable to write message; stream is finished");
        return false;
    }
    return true;
}

/*!
    \class QQmlGrpcClientStreamSender
    \internal
*/

class QQmlGrpcClientStreamSenderPrivate : public QObjectPrivate
{
public:
    explicit QQmlGrpcClientStreamSenderPrivate(std::shared_ptr<QGrpcClientStream> stream)
        : m_stream(std::move(stream))
    {
    }
    std::shared_ptr<QGrpcClientStream> m_stream = nullptr;
};

QQmlGrpcClientStreamSender::QQmlGrpcClientStreamSender(std::shared_ptr<QGrpcClientStream> stream,
                                                       QObject *parent)
    : QObject(*new QQmlGrpcClientStreamSenderPrivate(std::move(stream)), parent)
{
}

QQmlGrpcClientStreamSender::~QQmlGrpcClientStreamSender()
    = default;

void QQmlGrpcClientStreamSender::writeMessageImpl(const QProtobufMessage &message) const
{
    Q_D(const QQmlGrpcClientStreamSender);
    if (!QtQmlGrpcStreamSender::isValidStream(d->m_stream.get()))
        return;
    d->m_stream->writeMessage(message);
}

/*!
    \class QQmlGrpcBidirStreamSender
    \internal
*/

class QQmlGrpcBidirStreamSenderPrivate : public QObjectPrivate
{
public:
    explicit QQmlGrpcBidirStreamSenderPrivate(std::shared_ptr<QGrpcBidirStream> stream)
        : m_stream(std::move(stream))
    {
    }
    std::shared_ptr<QGrpcBidirStream> m_stream = nullptr;
};

QQmlGrpcBidirStreamSender::QQmlGrpcBidirStreamSender(std::shared_ptr<QGrpcBidirStream> stream,
                                                     QObject *parent)
    : QObject(*new QQmlGrpcBidirStreamSenderPrivate(std::move(stream)), parent)
{
}

QQmlGrpcBidirStreamSender::~QQmlGrpcBidirStreamSender()
    = default;

void QQmlGrpcBidirStreamSender::writeMessageImpl(const QProtobufMessage &message) const
{
    Q_D(const QQmlGrpcBidirStreamSender);
    if (!QtQmlGrpcStreamSender::isValidStream(d->m_stream.get()))
        return;
    d->m_stream->writeMessage(message);
}

QT_END_NAMESPACE

#include "moc_qtqmlgrpcstreamsender.cpp"
