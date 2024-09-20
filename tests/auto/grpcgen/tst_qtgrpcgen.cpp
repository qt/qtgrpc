// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/QtTest>

#include <QString>
#include <QProcess>
#include <QCryptographicHash>
#include <qtprotobuftypes.h>

#include "protocplugintestcommon.h"

#define XSTR(x) DEFSTR(x)
#define DEFSTR(x) #x

using namespace Qt::StringLiterals;
using namespace ProtocPluginTest;

const QLatin1StringView cppProtobufGenExtension(".qpb.cpp");
const QLatin1StringView headerProtobufGenExtension(".qpb.h");
const QLatin1StringView cppExtension("_client.grpc.qpb.cpp");
const QLatin1StringView headerExtension("_client.grpc.qpb.h");
const QLatin1StringView grpcGenQtprotobufKey(" --plugin=protoc-gen-qtgrpc=");
const QLatin1StringView optKey(" --qtgrpc_opt=");
const QLatin1StringView outputKey(" --qtgrpc_out=");
const QLatin1StringView includeKey(" -I");
#ifndef PROTOC_EXECUTABLE
#  error PROTOC_EXECUTABLE definition must be set and point to the valid protoc executable
#endif
const QLatin1StringView protocolBufferCompiler(XSTR(PROTOC_EXECUTABLE));
#if defined(Q_OS_WIN)
const QLatin1StringView qtgrpcgen("/qtgrpcgen.exe");
#else
const QLatin1StringView qtgrpcgen("/qtgrpcgen");
#endif

class tst_qtgrpcgen : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();

    //! Test qt_add_grpc() cmake function
    void cmakeGeneratedFile_data();
    void cmakeGeneratedFile();

    //! Test command-line call of qtgrpcgen
    void cmdLineGeneratedFile_data();
    void cmdLineGeneratedFile();

    void cleanupTestCase();

private:
    QString m_grpcgen;
    QString m_cmakeGenerated;
    QString m_qmlCmakeGenerated;
    QString m_commandLineGenerated;
    QString m_expectedResult;
    QString m_protoFiles;
};

void tst_qtgrpcgen::initTestCase()
{
    m_grpcgen = QLibraryInfo::path(QLibraryInfo::LibraryExecutablesPath) + qtgrpcgen;

    m_cmakeGenerated = QFINDTESTDATA("qt_grpc_generated");
    QVERIFY(!m_cmakeGenerated.isEmpty());

#ifdef HAVE_QML
    m_qmlCmakeGenerated = QFINDTESTDATA("qt_grpc_generated_qml");
    QVERIFY(!m_qmlCmakeGenerated.isEmpty());
#endif

    m_expectedResult = QFINDTESTDATA("data/expected_result");
    QVERIFY(!m_expectedResult.isEmpty());

    m_protoFiles = QFINDTESTDATA("../shared/data/proto/");
    QVERIFY(!m_protoFiles.isEmpty());

    QDir testOutputBaseDir(QCoreApplication::applicationDirPath());
    testOutputBaseDir.mkdir(QLatin1StringView("cmd_line_generation"));
    QLatin1StringView folders[] = {"comments"_L1, "extra-namespace"_L1,
                                   "fieldenum"_L1, "folder"_L1, "no-options"_L1};
    for (QLatin1StringView folder : folders)
        testOutputBaseDir.mkdir("cmd_line_generation/"_L1 + folder);

    m_commandLineGenerated = testOutputBaseDir.absolutePath() +
            QLatin1StringView("/cmd_line_generation");
    QVERIFY(!m_commandLineGenerated.isEmpty());
    QVERIFY(protocolCompilerAvailableToRun(protocolBufferCompiler));
}

void tst_qtgrpcgen::cmakeGeneratedFile_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("folder");
    QTest::addColumn<QString>("extension");
    QTest::addColumn<QString>("cmakeGenerationFolder");

    const QLatin1StringView protobufExtensions[] = { cppProtobufGenExtension,
                                                     headerProtobufGenExtension };

    const QLatin1StringView grpcExtensions[] = { cppExtension, headerExtension };

    for (const auto extension : grpcExtensions) {
        QTest::addRow("testservice%s", extension.data())
                << "testservice"
                << "/folder/qtgrpc/tests/"
                << QString(extension)
                << m_cmakeGenerated;

        QTest::addRow("separate/grpc/testservice%s", extension.data())
            << "testservice"
            << "/separate/grpc/qtgrpc/tests/" << QString(extension) << m_cmakeGenerated;
    }

    for (const auto extension : protobufExtensions) {
        QTest::addRow("testservice%s", extension.data())
            << "testservice"
            << "/folder/qtgrpc/tests/" << QString(extension) << m_cmakeGenerated;

        QTest::addRow("separate/protobuf/testservice%s", extension.data())
            << "testservice"
            << "/separate/protobuf/qtgrpc/tests/" << QString(extension) << m_cmakeGenerated;
    }

    QTest::addRow("tst_qtgrpcgen_client_grpc_only_exports.qpb.h")
        << "tst_qtgrpcgen_client_grpc_only_exports.qpb.h"
        << "/separate/grpc/" << QString() << m_cmakeGenerated;

#ifdef HAVE_QML
    const QLatin1StringView qmlExtensions[] = { cppExtension,
                                                headerExtension };

    for (const auto extension : qmlExtensions) {
        QTest::addRow("qmltestservice%s", extension.data())
                << "qmltestservice"
                << "/qml/"
                << QString(extension)
                << m_qmlCmakeGenerated;
    }
#endif
}

void tst_qtgrpcgen::cmakeGeneratedFile()
{
    QFETCH(QString, fileName);
    QFETCH(QString, folder);
    QFETCH(QString, extension);
    QFETCH(QString, cmakeGenerationFolder);

    compareTwoFiles(m_expectedResult + folder + fileName + extension,
                    cmakeGenerationFolder + folder + fileName + extension);
}

void tst_qtgrpcgen::cmdLineGeneratedFile_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("folder");
    QTest::addColumn<QString>("extension");

    const QLatin1StringView extensions[] = { cppExtension, headerExtension };

    for (const auto extension : extensions) {
        QTest::addRow("testservice%s", extension.data())
                << "testservice"
                << "/no-options/"
                << QString(extension);
        QTest::addRow("testserivcenomessages%s", extension.data())
                << "testserivcenomessages"
                << "/no-options/"
                << QString(extension);
    }
}

void tst_qtgrpcgen::cmdLineGeneratedFile()
{
    QFETCH(QString, fileName);
    QFETCH(QString, folder);
    QFETCH(QString, extension);

    QProcess process;
    process.setWorkingDirectory(m_commandLineGenerated);

    /* Call command:
         protoc --plugin=protoc-gen-qtgrpc=<path/to/bin/>qtgrpcgen \
         --qtgrpc_opt=<option> \
         --qtgrpc_out=<output_dir> [-I/extra/proto/include/path] <protofile>.proto */
    process.startCommand(protocolBufferCompiler + QString(" ")
                         + grpcGenQtprotobufKey + m_grpcgen
                         + optKey + outputKey
                         + m_commandLineGenerated + folder
                         + includeKey + m_protoFiles
                         + " " + fileName + ".proto");

    QVERIFY2(process.waitForStarted(), msgProcessStartFailed(process).constData());
    if (!process.waitForFinished()) {
        process.kill();
        QFAIL(msgProcessTimeout(process).constData());
    }
    QVERIFY2(process.exitStatus() == QProcess::NormalExit, msgProcessCrashed(process).constData());
    QVERIFY2(process.exitCode() == 0, msgProcessFailed(process).constData());

    compareTwoFiles(m_expectedResult + folder + fileName + extension,
                    m_commandLineGenerated + folder + fileName + extension);
}

void tst_qtgrpcgen::cleanupTestCase()
{
    // Leave this function at the bottom. It removes generated content.
    // cleanFolder(m_commandLineGenerated);
}

QTEST_MAIN(tst_qtgrpcgen)
#include "tst_qtgrpcgen.moc"
