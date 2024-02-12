// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef NAVITHREAD_H
#define NAVITHREAD_H

#include <QtCore/QThread>
#include <memory>
#include <naviservice_client.grpc.qpb.h>

QT_BEGIN_NAMESPACE
class QGrpcServerStream;
QT_END_NAMESPACE

class NaviThread : public QThread
{
    Q_OBJECT

public:
    explicit NaviThread(QObject *parent = nullptr);
    ~NaviThread() override;

    void run() override;
signals:
    void totalDistanceChanged(int distance);
    void remainingDistanceChanged(int distance);
    void directionChanged(qtgrpc::examples::DirectionEnumGadget::DirectionEnum direction);

private:
    std::shared_ptr<QGrpcServerStream> m_stream;
    std::shared_ptr<qtgrpc::examples::NaviService::Client> m_client;
};

#endif // NAVITHREAD_H
