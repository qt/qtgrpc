// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "clusterdatamanager.h"
#include "navithread.h"
#include "vehiclethread.h"

using namespace qtgrpc::examples;
ClusterDataManager::ClusterDataManager(QObject *parent) : QObject(parent)
{
    startVehicleClient();
    startNaviClient();
}

ClusterDataManager::~ClusterDataManager()
{
    if (m_vehicleThread->isRunning()) {
        m_vehicleThread->quit();
        m_vehicleThread->wait();
    }
    if (m_naviThread && m_naviThread->isRunning()) {
        m_naviThread->quit();
        m_naviThread->wait();
    }
}

void ClusterDataManager::startNaviClient()
{
    m_naviThread = std::make_unique<NaviThread>();
    connect(m_naviThread.get(), &NaviThread::totalDistanceChanged, this,
            &ClusterDataManager::totalDistanceChanged, Qt::QueuedConnection);
    connect(m_naviThread.get(), &NaviThread::remainingDistanceChanged, this,
            &ClusterDataManager::remainingDistanceChanged, Qt::QueuedConnection);
    connect(
        m_naviThread.get(), &NaviThread::directionChanged, this,
        [this](qtgrpc::examples::DirectionEnumGadget::DirectionEnum direction) {
            ClusterDataManager::directionChanged(qToUnderlying(direction));
        },
        Qt::QueuedConnection);

    m_naviThread->start();
}

void ClusterDataManager::startVehicleClient()
{
    m_vehicleThread = std::make_unique<VehicleThread>();
    connect(m_vehicleThread.get(), &VehicleThread::speedChanged, this,
            &ClusterDataManager::speedChanged, Qt::QueuedConnection);
    connect(m_vehicleThread.get(), &VehicleThread::fuelLevelChanged, this,
            &ClusterDataManager::fuelLevelChanged, Qt::QueuedConnection);
    connect(m_vehicleThread.get(), &VehicleThread::rpmChanged, this,
            &ClusterDataManager::rpmChanged, Qt::QueuedConnection);
    connect(m_vehicleThread.get(), &VehicleThread::connectionError, this,
            &ClusterDataManager::setThreadsAvailable, Qt::QueuedConnection);

    m_vehicleThread->start();
}

void ClusterDataManager::setThreadsAvailable(bool value)
{
    if (m_threadsAvailable != value) {
        m_threadsAvailable = value;
        emit showRestartClient(m_threadsAvailable);
    }
}

void ClusterDataManager::restart()
{
    setThreadsAvailable(false);
    if (m_vehicleThread->isRunning()) {
        m_vehicleThread->quit();
        m_vehicleThread->wait();
        m_vehicleThread->start();
    }

    if (m_naviThread->isRunning()) {
        m_naviThread->quit();
        m_naviThread->wait();
        m_naviThread->start();
    }
}
