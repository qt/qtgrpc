// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "grpcclienttestbase.h"

#include <QtGrpc/QGrpcChannelOptions>

void GrpcClientTestBase::initTestCase_data()
{
    QTest::addColumn<GrpcClientTestBase::Channels>("type");
    QTest::addColumn<std::shared_ptr<QAbstractGrpcChannel>>("channel");

    if (m_channels.testFlag(Channel::Qt)) {
        QTest::newRow("Http2Client")
                << QFlags{ Channel::Qt }
                << std::shared_ptr<QAbstractGrpcChannel>(new QGrpcHttp2Channel(QGrpcChannelOptions{
                           QUrl("http://localhost:50051", QUrl::StrictMode) }));
    }

#if QT_CONFIG(native_grpc)
    if (m_channels.testFlag(Channel::Native)) {
#  ifndef Q_OS_WINDOWS
        QTest::newRow("GrpcSocket") << QFlags{ Channel::Native }
                                    << std::shared_ptr<QAbstractGrpcChannel>(new QGrpcChannel(
                                               QGrpcChannelOptions{ QUrl("unix:///tmp/test.sock") },
                                               QGrpcChannel::InsecureChannelCredentials));
#  endif
        QTest::newRow("GrpcHttp") << QFlags{ Channel::Native }
                                  << std::shared_ptr<QAbstractGrpcChannel>(new QGrpcChannel(
                                             QGrpcChannelOptions{ QUrl("localhost:50051") },
                                             QGrpcChannel::InsecureChannelCredentials));
    }
#endif
}

void GrpcClientTestBase::init()
{
    QFETCH_GLOBAL(GrpcClientTestBase::Channels, type);
    m_channelType = type;
    QFETCH_GLOBAL(std::shared_ptr<QAbstractGrpcChannel>, channel);
    m_client = std::make_shared<qtgrpc::tests::TestService::Client>();
    m_client->attachChannel(std::move(channel));

    if (m_serverProccess.state() != QProcess::ProcessState::Running) {
        qInfo() << "Restarting server";
        m_serverProccess.restart();
        QVERIFY2(m_serverProccess.state() == QProcess::ProcessState::Running,
                 "Precondition failed - Server cannot be started.");
    }
}

std::shared_ptr<qtgrpc::tests::TestService::Client> GrpcClientTestBase::client()
{
    return m_client;
}

GrpcClientTestBase::Channels GrpcClientTestBase::channelType()
{
    return m_channelType;
}
