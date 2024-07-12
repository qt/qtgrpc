// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QTQMLGRPCSTREAMSENDER_H
#define QTQMLGRPCSTREAMSENDER_H

#include <QtGrpcQuick/qtgrpcquickexports.h>

#include <QtGrpc/qgrpcstream.h>

#include <QtProtobuf/qprotobufmessage.h>

QT_BEGIN_NAMESPACE

namespace QtQmlGrpcStreamSender {
bool isValidStream(const QGrpcOperation *stream);
}

class QQmlGrpcClientStreamSenderPrivate;
class Q_GRPCQUICK_EXPORT QQmlGrpcClientStreamSender : public QObject
{
    Q_OBJECT
public:
    ~QQmlGrpcClientStreamSender() override;

protected:
    explicit  QQmlGrpcClientStreamSender(std::shared_ptr<QGrpcClientStream> stream,
                                         QObject *parent = nullptr);

    void writeMessageImpl(const QProtobufMessage &message) const;

private:
    Q_DECLARE_PRIVATE(QQmlGrpcClientStreamSender)
    Q_DISABLE_COPY_MOVE(QQmlGrpcClientStreamSender)
};

class QQmlGrpcBidirStreamSenderPrivate;
class Q_GRPCQUICK_EXPORT QQmlGrpcBidirStreamSender : public QObject
{
    Q_OBJECT
public:
    ~QQmlGrpcBidirStreamSender() override;

protected:
    explicit QQmlGrpcBidirStreamSender(std::shared_ptr<QGrpcBidirStream> stream,
                                       QObject *parent = nullptr);

    void writeMessageImpl(const QProtobufMessage &message) const;

private:
    Q_DECLARE_PRIVATE(QQmlGrpcBidirStreamSender)
    Q_DISABLE_COPY_MOVE(QQmlGrpcBidirStreamSender)
};

QT_END_NAMESPACE

#endif // QTQMLGRPCSTREAMSENDER_H
