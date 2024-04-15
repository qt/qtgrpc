// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "vehiclethread.h"
#include <vehicleservice_client.grpc.qpb.h>

#include <QtCore/QUrl>
#include <QtGrpc/QGrpcChannelOptions>
#include <QtGrpc/QGrpcHttp2Channel>

#include <QDebug>

using namespace qtgrpc::examples;
using namespace google::protobuf;
VehicleThread::VehicleThread(QObject *parent) : QThread(parent)
{
}

VehicleThread::~VehicleThread() = default;

void VehicleThread::run()
{
    if (!m_client) {
        auto channel = std::shared_ptr<
            QAbstractGrpcChannel>(new QGrpcHttp2Channel(QGrpcChannelOptions{
            QUrl("http://localhost:50051", QUrl::StrictMode) }));
        m_client = std::make_shared<qtgrpc::examples::VehicleService::Client>();
        m_client->attachChannel(channel);
    }

    Empty fuelLvlRequest;
    std::shared_ptr<QGrpcCallReply> replyFuel = m_client->getFuelLevel(fuelLvlRequest);
    connect(replyFuel.get(), &QGrpcCallReply::errorOccurred, [this] {
        emit connectionError(true);
        emit fuelLevelChanged(0);
    });

    connect(replyFuel.get(), &QGrpcCallReply::finished, [replyFuel, this] {
        if (const auto fuelLvl = replyFuel->read<FuelLevelMsg>())
            emit fuelLevelChanged(fuelLvl->fuelLevel());
    });

    Empty speedRequest;
    m_streamSpeed = m_client->streamGetSpeedStream(speedRequest);
    connect(m_streamSpeed.get(), &QGrpcServerStream::messageReceived, this, [this] {
        if (const auto speedResponse = m_streamSpeed->read<SpeedMsg>())
            emit speedChanged(speedResponse->speed());
    });

    connect(m_streamSpeed.get(), &QGrpcServerStream::errorOccurred, this,
            [this](const QGrpcStatus &status) {
                emit connectionError(true);
                emit speedChanged(0);
                emit fuelLevelChanged(0);
                qWarning() << "Stream error(" << status.code() << "):" << status.message();
            });

    connect(m_streamSpeed.get(), &QGrpcServerStream::finished, this,
            [this] { emit speedChanged(0); });

    Empty gearRequest;
    m_streamGear = m_client->streamGetGearStream(gearRequest);
    connect(m_streamGear.get(), &QGrpcServerStream::messageReceived, this, [this] {
        if (const auto gearResponse = m_streamGear->read<GearMsg>())
            emit rpmChanged(gearResponse->rpm());
    });

    connect(m_streamGear.get(), &QGrpcServerStream::errorOccurred, this,
            [this] { emit rpmChanged(0); });

    connect(m_streamGear.get(), &QGrpcServerStream::finished, this, [this] { emit rpmChanged(0); });

    QThread::run();
}
