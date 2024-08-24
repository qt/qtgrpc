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
            QAbstractGrpcChannel>(new QGrpcHttp2Channel(QUrl("http://localhost:50051",
                                                             QUrl::StrictMode)));
        m_client = std::make_shared<qtgrpc::examples::NaviService::Client>();
        m_client->attachChannel(channel);
    }

    Empty request;
    m_stream = m_client->getNaviStream(request);
    connect(m_stream.get(), &QGrpcServerStream::messageReceived, this, [this] {
        const auto result = m_stream->read<DistanceMsg>();
        if (!result)
            return;
        emit totalDistanceChanged(result->totalDistance());
        emit remainingDistanceChanged(result->remainingDistance());
        emit directionChanged(result->direction());
    });

    connect(m_stream.get(), &QGrpcServerStream::finished, this, [this] (const QGrpcStatus &status) {
        if (status.code() != QtGrpc::StatusCode::Ok)
            qWarning() << "Stream error(" << status.code() << "):" << status.message();

        emit totalDistanceChanged(0);
        emit remainingDistanceChanged(0);
        emit directionChanged(DirectionEnumGadget::DirectionEnum::BACKWARD);
    });

    QThread::run();
}
