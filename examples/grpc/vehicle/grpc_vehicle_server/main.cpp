// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "serverrunner.h"

#include <QtCore/QCoreApplication>

#include <memory>

int main()
{
    auto server = std::make_unique<VehicleServer>();
    server->run();
    return 0;
}
