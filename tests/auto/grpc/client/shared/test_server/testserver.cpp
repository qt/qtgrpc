// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "testserverrunner.h"

#include <memory>

int main()
{
    auto server = std::make_unique<TestServer>();
    server->run();
    return 0;
}
