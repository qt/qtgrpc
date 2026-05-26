// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QQmlContext>
#include <QQmlEngine>
#include <QtQuickTest>

#include <message_latency_defs.h>
#include <server_proc_runner.h>

class GrpcClientUnarycallQml : public QObject
{
    Q_OBJECT

public slots:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        engine->rootContext()->setContextProperty("testMessageLatencyWithThreshold",
                                                  QVariant::fromValue(MessageLatencyWithThreshold));
        if (m_serverProcess.state() != QProcess::ProcessState::Running) {
            qInfo() << "Restarting server";
            m_serverProcess.restart();
            QVERIFY2(m_serverProcess.state() == QProcess::ProcessState::Running,
                     "Precondition failed - Server cannot be started.");
        }
    }

private:
    ServerProcRunner m_serverProcess{ TEST_GRPC_SERVER_PATH };
};

QUICK_TEST_MAIN_WITH_SETUP(tst_grpc_client_unarycall_qml, GrpcClientUnarycallQml)

#include "tst_grpc_client_unarycall_qml.moc"
