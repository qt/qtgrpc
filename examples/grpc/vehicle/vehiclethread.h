// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef VEHICLETHREAD_H
#define VEHICLETHREAD_H

#include <QtCore/QThread>
#include <memory>

QT_BEGIN_NAMESPACE
class QGrpcServerStream;
QT_END_NAMESPACE

namespace qtgrpc {
namespace examples {
namespace VehicleService {
class Client;
}
}
}

class VehicleThread : public QThread
{
    Q_OBJECT

public:
    explicit VehicleThread(QObject *parent = nullptr);
    ~VehicleThread() override;
    void run() override;
signals:
    void speedChanged(int speed);
    void rpmChanged(int rpm);
    void fuelLevelChanged(int level);
    void connectionError(bool value);

private:
    std::shared_ptr<qtgrpc::examples::VehicleService::Client> m_client;
    std::shared_ptr<QGrpcServerStream> m_streamSpeed;
    std::shared_ptr<QGrpcServerStream> m_streamGear;
};

#endif // VEHICLETHREAD_H
