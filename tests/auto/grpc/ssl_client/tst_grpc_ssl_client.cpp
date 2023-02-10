// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qtgrpcglobal.h>

#if QT_CONFIG(native_grpc)
#  include <QGrpcChannel>
#  include <grpcpp/security/credentials.h>
#endif
#include <QGrpcHttp2Channel>
#include <QGrpcInsecureCallCredentials>
#include <QGrpcInsecureChannelCredentials>
#include <QGrpcSslCredentials>

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QProcess>
#include <QSignalSpy>
#include <QTest>
#include <QtCore/QTimer>
#include <QtCore/qsysinfo.h>
#include <QtNetwork/QSslConfiguration>

#include "testservice_client.grpc.qpb.h"

using namespace qtgrpc::tests;

class QtGrpcSslClientTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        startServer();
        qRegisterProtobufTypes();
    }

    void cleanupTestCase()
    {
        if (serverProc && serverProc->state() == QProcess::ProcessState::Running) {
            serverProc->kill();
            serverProc->waitForFinished();
        }
    }

    void init()
    {
        if (QSysInfo::productVersion() == "8.6" && QSysInfo::productType().contains("rhel")) {
            QSKIP("Test case disabled on RHEL due to QTBUG-111098");
        }

        if (serverProc->state() != QProcess::ProcessState::Running) {
            qInfo() << "Restarting server";
            startServer();
            QVERIFY2(serverProc->state() == QProcess::ProcessState::Running,
                     "Precondition failed - Server cannot be started.");
        }
    }

    void Http2ChannelIncorrectSecureCredentialsTest();
#if QT_CONFIG(native_grpc)
    void GrpcHttpChannelIncorrectSecureCredentialsTest();
#  ifndef Q_OS_WINDOWS
    void GrpcSocketChannelIncorrectSecureCredentialsTest();
#  endif
#endif

private:
    void startServer()
    {
        serverProc = std::make_unique<QProcess>();
        QString serverPath = QFINDTESTDATA("../shared/test_server/securetestserver");
        QVERIFY2(!serverPath.isEmpty(), "securetestserver binary is missing");
        serverProc->start(serverPath);
        serverProc->waitForStarted();
        // Extra time for the server to setup
        QTest::qSleep(1000);
    }

    std::unique_ptr<QProcess> serverProc;
};

void QtGrpcSslClientTest::Http2ChannelIncorrectSecureCredentialsTest()
{
    // Prepare ssl configuration
    QSslConfiguration conf = QSslConfiguration::defaultConfiguration();
    conf.setProtocol(QSsl::TlsV1_2);
    // NOTE: CA certificate is not setup on purpose to induce the ssl handshake error
    //  QFile certFile("cert.pem");
    //  certFile.open(QIODevice::ReadOnly);
    //  QByteArray cert = certFile.readAll();
    //  conf.setCaCertificates({QSslCertificate(cert)});

    TestService::Client testClient;
    testClient.attachChannel(std::make_shared<QGrpcHttp2Channel>(
            QUrl("https://localhost:60051", QUrl::StrictMode),
            QGrpcInsecureCallCredentials() | QGrpcSslCredentials(conf)));

    SimpleStringMessage expected;
    expected.setTestFieldString("Hello Qt!");
    auto result = std::make_shared<SimpleStringMessage>();
    QVERIFY(testClient.testMethod(expected, result.get()) != QGrpcStatus::Ok);
}

#if QT_CONFIG(native_grpc)
void QtGrpcSslClientTest::GrpcHttpChannelIncorrectSecureCredentialsTest()
{
    TestService::Client testClient;
    grpc::SslCredentialsOptions options = grpc::SslCredentialsOptions();
    QStringList optionsList;
    optionsList.append(QString::fromStdString(options.pem_root_certs));
    optionsList.append(QString::fromStdString(options.pem_private_key));
    optionsList.append(QString::fromStdString(options.pem_cert_chain));
    testClient.attachChannel(std::make_shared<QGrpcChannel>(QUrl("localhost:60051"),
                                                            QGrpcChannel::SslDefaultCredentials,
                                                            optionsList));

    SimpleStringMessage expected;
    expected.setTestFieldString("Hello Qt!");
    auto result = std::make_shared<SimpleStringMessage>();
    QVERIFY(testClient.testMethod(expected, result.get()) != QGrpcStatus::Ok);
}

#  ifndef Q_OS_WINDOWS
void QtGrpcSslClientTest::GrpcSocketChannelIncorrectSecureCredentialsTest()
{
    TestService::Client testClient;
    testClient.attachChannel(std::make_shared<QGrpcChannel>(QUrl("unix:///tmp/test.sock"),
                                                            QGrpcChannel::SslDefaultCredentials));

    SimpleStringMessage expected;
    expected.setTestFieldString("Hello Qt!");
    auto result = std::make_shared<SimpleStringMessage>();
    QVERIFY(testClient.testMethod(expected, result.get()) != QGrpcStatus::Ok);
}
#  endif
#endif

QTEST_MAIN(QtGrpcSslClientTest)
#include "tst_grpc_ssl_client.moc"
