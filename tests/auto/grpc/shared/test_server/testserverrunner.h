// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#include <QThread>

class SecureTestServer
{
public:
    void run();
};

class TestServer
{
public:
    void run();
};

#endif // TEST_SERVER_H
