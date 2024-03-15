// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "serverrunner.h"
#include "naviservice.grpc.pb.h"
#include "vehicleservice.grpc.pb.h"

#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <grpc++/grpc++.h>

#include <chrono>
#include <memory>
#include <thread>

namespace {

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using qtgrpc::examples::DistanceMsg;
using qtgrpc::examples::FuelLevelMsg;
using qtgrpc::examples::GearMsg;
using qtgrpc::examples::NaviService;
using qtgrpc::examples::SpeedMsg;
using qtgrpc::examples::VehicleService;

class VehicleServiceServiceImpl final : public qtgrpc::examples::VehicleService::Service
{
    grpc::Status getSpeedStream(grpc::ServerContext *, const ::google::protobuf::Empty *,
                                ServerWriter<SpeedMsg> *writer) override;
    grpc::Status getGearStream(grpc::ServerContext *, const ::google::protobuf::Empty *,
                               ServerWriter<GearMsg> *writer) override;
    grpc::Status getFuelLevel(grpc::ServerContext *, const ::google::protobuf::Empty *,
                              FuelLevelMsg *response) override;

    constexpr static std::chrono::seconds vehicleTimeout = std::chrono::seconds(1);
    int speed = 0;
    int rpm = 100;
};

class NaviServiceServiceImpl final : public qtgrpc::examples::NaviService::Service
{
    grpc::Status getNaviStream(grpc::ServerContext *, const ::google::protobuf::Empty *,
                               ServerWriter<DistanceMsg> *writer) override;
    constexpr static std::chrono::seconds naviTimeout = std::chrono::seconds(2);
    int totaldistance = 2000;
    int remainingdistance = 0;
};
} // namespace

Status NaviServiceServiceImpl::getNaviStream(grpc::ServerContext *,
                                             const ::google::protobuf::Empty *,
                                             ServerWriter<DistanceMsg> *writer)
{
    volatile bool naviRequired = false;
    DistanceMsg msg;
    msg.set_totaldistance(totaldistance);
    std::this_thread::sleep_for(naviTimeout);
    naviRequired = writer->Write(msg);

    while (naviRequired) {
        if (remainingdistance < totaldistance) {
            msg.set_remainingdistance(remainingdistance += 50);
        } else {
            msg.set_remainingdistance(0);
            remainingdistance = 0;
        }
        std::this_thread::sleep_for(naviTimeout);
        naviRequired = writer->Write(msg);

        switch (remainingdistance) {
        case 0: {
            msg.set_direction(qtgrpc::examples::DirectionEnum::STRAIGHT);
        } break;
        case 50: {
            msg.set_direction(qtgrpc::examples::DirectionEnum::RIGHT);
        } break;
        case 900: {
            msg.set_direction(qtgrpc::examples::DirectionEnum::LEFT);
        } break;
        }
        std::this_thread::sleep_for(naviTimeout);
        naviRequired = writer->Write(msg);
    }

    return Status();
}
grpc::Status VehicleServiceServiceImpl::getFuelLevel(grpc::ServerContext *,
                                                     const ::google::protobuf::Empty *,
                                                     FuelLevelMsg *response)
{
    response->set_fuellevel(152);
    return Status();
}

Status VehicleServiceServiceImpl::getSpeedStream(grpc::ServerContext *,
                                                 const ::google::protobuf::Empty *,
                                                 ServerWriter<SpeedMsg> *writer)
{
    SpeedMsg msg;
    volatile bool vehicleRequired = true;
    while (vehicleRequired) {
        if (speed < 180) {
            msg.set_speed(speed += 10);
        } else {
            msg.set_speed(speed -= 10);
        }
        std::this_thread::sleep_for(vehicleTimeout);
        vehicleRequired = writer->Write(msg);
    }
    return Status();
}

Status VehicleServiceServiceImpl::getGearStream(grpc::ServerContext *,
                                                const ::google::protobuf::Empty *,
                                                ServerWriter<GearMsg> *writer)
{
    GearMsg msg;
    volatile bool vehicleRequired = true;
    while (vehicleRequired) {
        if (rpm < 3800) {
            msg.set_rpm(rpm += 200);
        } else {
            msg.set_rpm(rpm -= 200);
        }
        std::this_thread::sleep_for(vehicleTimeout);
        vehicleRequired = writer->Write(msg);
    }

    return Status();
}

void VehicleServer::run()
{
    std::string serverUri("127.0.0.1:50051");
    VehicleServiceServiceImpl vehicleService;
    NaviServiceServiceImpl naviService;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(serverUri, grpc::InsecureServerCredentials());
    builder.RegisterService(&vehicleService);
    builder.RegisterService(&naviService);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    if (!server) {
        qWarning() << "Creating grpc::Server failed.";
        return;
    }

    qDebug() << "Server listening on " << serverUri;
    server->Wait();
}
