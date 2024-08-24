// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CLUSTERDATAMANAGER_H
#define CLUSTERDATAMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QtTypeTraits>
#include <QtQml/QQmlEngine>
#include <naviservice_client.grpc.qpb.h>

#include <memory>

class VehicleThread;
class NaviThread;
class ClusterDataManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool threadsAvailable READ threadsAvailable WRITE setThreadsAvailable NOTIFY
                   showRestartClient FINAL)
public:
    enum NaviDirection {
        RIGHT = qToUnderlying(qtgrpc::examples::DirectionEnumGadget::DirectionEnum::RIGHT),
        LEFT = qToUnderlying(qtgrpc::examples::DirectionEnumGadget::DirectionEnum::LEFT),
        STRAIGHT = qToUnderlying(qtgrpc::examples::DirectionEnumGadget::DirectionEnum::STRAIGHT),
        BACKWARD = qToUnderlying(qtgrpc::examples::DirectionEnumGadget::DirectionEnum::BACKWARD)
    };

    Q_ENUM(NaviDirection);

    explicit ClusterDataManager(QObject *parent = nullptr);
    ~ClusterDataManager() override;

    Q_INVOKABLE void restart();

    bool threadsAvailable() const { return m_threadsAvailable; }
    void setThreadsAvailable(bool value);

signals:
    void speedChanged(int speed);
    void rpmChanged(int rpm);
    void fuelLevelChanged(int level);

    void totalDistanceChanged(int distance);
    void remainingDistanceChanged(int distance);
    void directionChanged(int direction);

    void showRestartClient(bool value);

private:
    void startVehicleClient();
    void startNaviClient();

    std::unique_ptr<NaviThread> m_naviThread;
    std::unique_ptr<VehicleThread> m_vehicleThread;

    bool m_threadsAvailable = false;
};

#endif // CLUSTERDATAMANAGER_H
