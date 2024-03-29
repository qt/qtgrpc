// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "navithread.h"
#include <naviservice_client.grpc.qpb.h>

#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtGrpc/QGrpcChannelOptions>
#include <QtGrpc/QGrpcHttp2Channel>

using namespace qtgrpc::examples;
using namespace google::protobuf;
NaviThread::NaviThread(QObject *parent) : QThread(parent)
{
}

NaviThread::~NaviThread() = default;

void NaviThread::run()
{
    if (!m_client) {
        auto channel = std::shared_ptr<
            QAbstractGrpcChannel>(new QGrpcHttp2Channel(QGrpcChannelOptions{
            QUrl("http://localhost:50051", QUrl::StrictMode) }));
        m_client = std::make_shared<qtgrpc::examples::NaviService::Client>();
        m_client->attachChannel(channel);
    }

    Empty request;
    m_stream = m_client->streamGetNaviStream(request);
    connect(m_stream.get(), &QGrpcServerStream::messageReceived, this, [this] {
        DistanceMsg result = m_stream->read<DistanceMsg>();
        emit totalDistanceChanged(result.totalDistance());
        emit remainingDistanceChanged(result.remainingDistance());
        emit directionChanged(result.direction());
    });

    connect(m_stream.get(), &QGrpcServerStream::errorOccurred, this,
            [this](const QGrpcStatus &status) {
                emit totalDistanceChanged(0);
                emit remainingDistanceChanged(0);
                emit directionChanged(DirectionEnumGadget::BACKWARD);
                qWarning() << "Stream error(" << status.code() << "):" << status.message();
            });

    connect(m_stream.get(), &QGrpcServerStream::finished, this, [this] {
        emit totalDistanceChanged(0);
        emit remainingDistanceChanged(0);
        emit directionChanged(DirectionEnumGadget::BACKWARD);
    });

    QThread::run();
}
