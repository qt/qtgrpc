// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/qtest.h>

#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/qcryptographichash.h>
#include <QtCore/qdir.h>
#include <QtCore/qdiriterator.h>
#if QT_CONFIG(process)
#  include <QtCore/qprocess.h>
#endif
#include <QtCore/qstring.h>

using namespace Qt::StringLiterals;

namespace {
#if QT_CONFIG(process)
#  ifndef PROTOC_EXECUTABLE
#    error PROTOC_EXECUTABLE definition must be set and point to the valid protoc executable
#  endif
constexpr QLatin1StringView ProtocPath(PROTOC_EXECUTABLE);

#  ifndef PROTOC_PLUGIN
#    error PROTOC_PLUGIN definition must be set and point to the valid protoc plugin
#  endif
constexpr QLatin1StringView QtprotobufgenPath(PROTOC_PLUGIN);

constexpr QLatin1StringView PluginKey(" --plugin=protoc-gen-qtprotobuf=");
constexpr QLatin1StringView OptKey(" --qtprotobuf_opt=");
constexpr QLatin1StringView OutKey(" --qtprotobuf_out=");
constexpr QLatin1StringView IncludeKey(" -I");
#  ifdef ALLOW_PROTO3_OPTIONAL
const QLatin1StringView allow_proto3_optional(" --experimental_allow_proto3_optional");
#  else
constexpr QLatin1StringView allow_proto3_optional;
#  endif // ALLOW_PROTO3_OPTIONAL

#  define CMD_LINE_GENERATED_DIR "cmd_line_generated"
constexpr QLatin1StringView CmdLineGeneratedDir(CMD_LINE_GENERATED_DIR);
constexpr QLatin1StringView CmdLineGeneratedPathInfix("/" CMD_LINE_GENERATED_DIR "/");

#endif // QT_CONFIG(process)

#  ifndef BINARY_DIR
#    error BINARY_DIR definition must be set
#  endif
constexpr QLatin1StringView BinaryDir(BINARY_DIR);

#ifndef CMAKE_GENERATOR_TESTS
#  define CMAKE_GENERATOR_TESTS
#endif
constexpr QLatin1StringView CMakeGeneratorTests(CMAKE_GENERATOR_TESTS);

#ifndef CMAKE_GENERATED_DIR
#  error CMAKE_GENERATED_DIR definition must be set
#endif
constexpr QLatin1StringView CMakeGeneratedDir(CMAKE_GENERATED_DIR);
constexpr QLatin1StringView CMakeGeneratedPathInfix("/" CMAKE_GENERATED_DIR "/");

#if QT_CONFIG(process)
QByteArray msgProcessStartFailed(const QProcess &p)
{
    const QString result = QLatin1StringView("Could not start \"")
            + QDir::toNativeSeparators(p.program()) + QLatin1StringView("\": ")
            + p.errorString();
    return result.toLocal8Bit();
}

QByteArray msgProcessTimeout(const QProcess &p)
{
    return '"' + QDir::toNativeSeparators(p.program()).toLocal8Bit()
            + "\" timed out.";
}

QByteArray msgProcessCrashed(QProcess &p)
{
    return '"' + QDir::toNativeSeparators(p.program()).toLocal8Bit() + "\" crashed.\n"
        + p.readAll();
}

QByteArray msgProcessFailed(QProcess &p)
{
    return '"' + QDir::toNativeSeparators(p.program()).toLocal8Bit() + "\" returned "
        + QByteArray::number(p.exitCode()) + ":\n" + p.readAll();
}

bool protocolCompilerAvailableToRun()
{
    QProcess protoc;
    protoc.startCommand(ProtocPath + " --version");

    if (!protoc.waitForStarted())
        return false;

    if (!protoc.waitForFinished()) {
        protoc.kill();
        return false;
    }
    return protoc.exitStatus() == QProcess::NormalExit;
}
#endif // QT_CONFIG(process)

QByteArray hash(const QByteArray &fileData)
{
    return QCryptographicHash::hash(fileData, QCryptographicHash::Sha1);
}

QByteArrayList splitToLines(const QByteArray &data)
{
    return data.split('\n');
}

// Return size diff and first NOT equal line;
QByteArray doCompare(const QByteArrayList &actual, const QByteArrayList &expected)
{
    QByteArray ba;

    if (actual.size() != expected.size()) {
        ba.append(QString("Length count different: actual: %1, expected: %2")
                  .arg(actual.size()).arg(expected.size()).toUtf8());
    }

    for (int i = 0, n = expected.size(); i != n; ++i) {
        QString expectedLine = expected.at(i);
        if (expectedLine != actual.at(i)) {
            ba.append("\n<<<<<< ACTUAL\n" + actual.at(i)
                      + "\n======\n" + expectedLine.toUtf8()
                      + "\n>>>>>> EXPECTED\n"
                      );
            break;
        }
    }
    return ba;
}

QByteArray msgCannotReadFile(const QFile &file)
{
    const QString result = QLatin1StringView("Could not read file: ")
            + QDir::toNativeSeparators(file.fileName())
            + QLatin1StringView(": ") + file.errorString();
    return result.toLocal8Bit();
}

void compareTwoFiles(const QString &expectedFileName, const QString &actualFileName)
{
    QFile expectedResultFile(expectedFileName);
    QFile generatedFile(actualFileName);

    QVERIFY2(expectedResultFile.exists(), qPrintable(expectedResultFile.fileName()));
    QVERIFY2(generatedFile.exists(), qPrintable(expectedResultFile.fileName()));

    QVERIFY2(expectedResultFile.open(QIODevice::ReadOnly | QIODevice::Text),
             msgCannotReadFile(expectedResultFile).constData());
    QVERIFY2(generatedFile.open(QIODevice::ReadOnly | QIODevice::Text),
             msgCannotReadFile(generatedFile).constData());

    QByteArray expectedData = expectedResultFile.readAll();
    QByteArray generatedData = generatedFile.readAll();

    expectedResultFile.close();
    generatedFile.close();

    if (hash(expectedData).toHex() != hash(generatedData).toHex()) {
        const QString diff = doCompare(splitToLines(generatedData),
                                       splitToLines(expectedData));
        QCOMPARE_GT(diff.size(), 0); // Hashes can only differ if content does.
        QFAIL(qPrintable(diff));
    }
    // Ensure we do see a failure, even in the unlikely case of a hash collision:
    QVERIFY(generatedData == expectedData);
}

void cleanFolder(const QString &folderName)
{
    QDir dir(folderName);
    dir.removeRecursively();
}

QFileInfoList scanDirectoryRecursively(const QDir &dir)
{
    QFileInfoList result;
    QDirIterator it(dir.path(), QStringList() << "*.cpp" << "*.h", QDir::Files | QDir::NoSymLinks,
                    QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        result.append(it.fileInfo());
    }

    std::sort(result.begin(), result.end(), [&dir](const QFileInfo &lhs, const QFileInfo &rhs) {
        return dir.relativeFilePath(lhs.absoluteFilePath())
            < dir.relativeFilePath(rhs.absoluteFilePath());
    });
    return result;
}

QStringList relativePaths(const QDir &dir, const QFileInfoList &files)
{
    QStringList result;
    for (const auto &file : files) {
        result.append(dir.relativeFilePath(file.absoluteFilePath()));
    }
    return result;
}

bool copyDirectoryRecursively(const QDir &from, QDir to)
{
    if (!from.exists()) {
        qDebug() << "Unable to copy directory" << from << ". Directory doesn't exists.";
        return false;
    }
    for (const auto &item :
         from.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::NoSymLinks)) {
        const auto fileName = item.fileName();
        const auto absolutePath = item.absoluteFilePath();
        if (fileName.isEmpty()) {
            qDebug() << "fileName is empty";
            return false;
        }
        if (item.isFile()) {
            if (!QFile::copy(item.absoluteFilePath(), to.absolutePath() + u'/' + fileName)) {
                qDebug() << "Unable to copy " << item.absoluteFilePath() << "to"
                         << to.absolutePath() + u'/' + fileName;
                return false;
            }
        } else {
            if (!to.mkdir(fileName)) {
                qDebug() << "Unable to create " << to.absolutePath() + u'/' + fileName;
                return false;
            }
            if (!to.cd(fileName)) {
                qDebug() << "Unable to enter " << to.absolutePath() + u'/' + fileName;
                return false;
            }
            if (!copyDirectoryRecursively(QDir(absolutePath), to))
                return false;
            if (!to.cdUp())
                return false;
        }
    }
    return true;
}
} // namespace

class qtprotobufgenTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();

    //! Test qt_add_protobuf() cmake function
    void cmakeGenerated_data();
    void cmakeGenerated();

#if QT_CONFIG(process)
    //! Test command-line call of qtprotobufgen
    void cmdLineGenerated_data();
    void cmdLineGenerated();
    void cmdLineInvalidExportMacro_data();
    void cmdLineInvalidExportMacro();
#endif

    void cleanupTestCase();

private:
    QString m_cmakeGenerated;
    QString m_expectedResult;

#if QT_CONFIG(process)
    QString m_cmdLineGenerated;
#endif

    // Copy test results back to source directory if COPY_TEST_RESULTS environment variable is
    // set to 'true'. Useful when changing the generator to update the test data.
    bool m_copyTestResults = false;
};

void qtprotobufgenTest::initTestCase()
{
    QDir(BinaryDir).mkdir(CmdLineGeneratedDir);
#if QT_CONFIG(process)
    m_cmdLineGenerated = BinaryDir + CmdLineGeneratedPathInfix;
    QVERIFY(!m_cmdLineGenerated.isEmpty());
    QVERIFY(QFile::exists(m_cmdLineGenerated));
#  ifdef Q_OS_MACOS
    if (!protocolCompilerAvailableToRun())
        QSKIP("Protocol buffer compiler is not provisioned for macOS ARM VMs: QTBUG-109130");
#  else
    QVERIFY(protocolCompilerAvailableToRun());
#  endif
#endif

    m_cmakeGenerated = QFINDTESTDATA(CMakeGeneratedDir);
    QVERIFY(!m_cmakeGenerated.isEmpty());

    m_expectedResult = QFINDTESTDATA("data");
    QVERIFY(!m_expectedResult.isEmpty());

    m_copyTestResults = qgetenv("COPY_TEST_RESULTS") == "true"_L1;
}

void qtprotobufgenTest::cmakeGenerated_data()
{
    QTest::addColumn<QString>("testName");
    QTest::addColumn<QString>("filePath");

    const QStringList tests = QString(CMakeGeneratorTests).split(','_L1, Qt::SkipEmptyParts);
    for (const auto &testName : tests) {
        QDir testDir(m_expectedResult + CMakeGeneratedPathInfix + testName);
        const auto testFiles = scanDirectoryRecursively(testDir);
        for (const auto &testFile : testFiles) {
            auto relativePath = testDir.relativeFilePath(testFile.absoluteFilePath());
            QTest::addRow("%s: %s", testName.toUtf8().constData(),
                          relativePath.toUtf8().constData())
                << testName << relativePath;
        }
    }
}

void qtprotobufgenTest::cmakeGenerated()
{
    QFETCH(QString, testName);
    QFETCH(QString, filePath);
    compareTwoFiles(m_expectedResult + CMakeGeneratedPathInfix + testName + '/'_L1 + filePath,
                    m_cmakeGenerated + '/'_L1 + testName + '/'_L1 + filePath);
}

#if QT_CONFIG(process)
void qtprotobufgenTest::cmdLineGenerated_data()
{
    QTest::addColumn<QString>("directory");
    QTest::addColumn<QString>("protoFile");
    QTest::addColumn<QString>("generatorOptions");

    QTest::addRow("no_options") << "no_options"
                                << "qtprotobufgen.proto"
                                << "";

    QTest::addRow("export_macro") << "export_macro"
                                  << "qtprotobufgen.proto"
                                  << "EXPORT_MACRO=TST_QTPROTOBUFGEN_GEN";

    QTest::addRow("generate_package_subfolders") << "generate_package_subfolders"
                                                 << "qtprotobufgen.proto"
                                                 << "GENERATE_PACKAGE_SUBFOLDERS";

    QTest::addRow("copy_comments") << "copy_comments"
                                   << "annotation.proto"
                                   << "COPY_COMMENTS";

    QTest::addRow("extra_namespace") << "extra_namespace"
                                     << "qtprotobufgen.proto"
                                     << "EXTRA_NAMESPACE=MyTopLevelNamespace";

    QTest::addRow("qml") << "qml"
                         << "qtprotobufgen.proto"
                         << "QML";

    QTest::addRow("qml_uri") << "qml_uri"
                             << "qtprotobufgen.proto"
                             << "QML;QML_URI=my.test.uri";

    QTest::addRow("qml_uri_export_macro")
        << "qml_uri_export_macro"
        << "qtprotobufgen.proto"
        << "QML;QML_URI=my.test.uri;EXPORT_MACRO=TST_QTPROTOBUFGEN_GEN";

    QTest::addRow("qml_uri_export_macro_generate_package_subfolders")
        << "qml_uri_export_macro_generate_package_subfolders"
        << "qtprotobufgen.proto"
        << "QML;QML_URI=my.test.uri;EXPORT_MACRO=TST_QTPROTOBUFGEN_GEN;GENERATE_PACKAGE_SUBFOLDERS";

    QTest::addRow("export_macro_custom_file_name")
        << "export_macro_custom_file_name"
        << "qtprotobufgen.proto"
        << "EXPORT_MACRO=EXPORT_MACRO_WITH_FILE:custom_file_name";

    QTest::addRow("export_macro_custom_file_name_force_generate")
        << "export_macro_custom_file_name_force_generate"
        << "qtprotobufgen.proto"
        << "EXPORT_MACRO=EXPORT_MACRO_WITH_FILE:custom_file_name.hpp:true";

    QTest::addRow("export_macro_custom_file_name_skip_generate")
        << "export_macro_custom_file_name_skip_generate"
        << "qtprotobufgen.proto"
        << "EXPORT_MACRO=EXPORT_MACRO_WITH_FILE:custom_file_name.hxx:false";
}

void qtprotobufgenTest::cmdLineGenerated()
{

    QFETCH(QString, directory);
    QFETCH(QString, protoFile);
    QFETCH(QString, generatorOptions);

    const QString fullProtoFilePath(m_expectedResult + '/'_L1 + protoFile);
    QVERIFY2(QFile::exists(fullProtoFilePath),
             qPrintable("Input .proto scheme "_L1 + fullProtoFilePath + " doesn't exists"_L1));

    QDir outputDirectory(m_cmdLineGenerated);
    if (!outputDirectory.exists(directory))
        outputDirectory.mkdir(directory);
    outputDirectory.cd(directory);

    QProcess process;
    process.setWorkingDirectory(m_cmdLineGenerated);
    process.startCommand(ProtocPath + PluginKey + QtprotobufgenPath + OptKey + generatorOptions
                         + OutKey + outputDirectory.absolutePath() + IncludeKey + m_expectedResult
                         + ' '_L1 + fullProtoFilePath + allow_proto3_optional);

    QVERIFY2(process.waitForStarted(), qPrintable(msgProcessStartFailed(process)));
    if (!process.waitForFinished()) {
        process.kill();
        QFAIL(qPrintable(msgProcessTimeout(process)));
    }
    QVERIFY2(process.exitStatus() == QProcess::NormalExit, qPrintable(msgProcessCrashed(process)));
    QVERIFY2(process.exitCode() == 0, qPrintable(msgProcessFailed(process)));

    QDir expectedResultDir(m_expectedResult + "/cmd_line_generated/"_L1 + directory);
    const auto generatedFileList = scanDirectoryRecursively(outputDirectory);
    const auto expectedFileList = scanDirectoryRecursively(expectedResultDir);

    QCOMPARE_EQ(relativePaths(outputDirectory, generatedFileList),
                relativePaths(expectedResultDir, expectedFileList));

    for (qsizetype i = 0; i < expectedFileList.size(); ++i) {
        compareTwoFiles(generatedFileList.at(i).absoluteFilePath(),
                        expectedFileList.at(i).absoluteFilePath());
    }
}

void qtprotobufgenTest::cmdLineInvalidExportMacro_data()
{
    QTest::addColumn<QString>("exportMacro");
    QTest::addColumn<int>("result");

    QTest::addRow("contains_dash") << "TST_QTPROTOBUFGEN-FAIL" << 1;
    QTest::addRow("contains_number_first") << "1Not_ALLoWeD" << 1;
}

void qtprotobufgenTest::cmdLineInvalidExportMacro()
{
    QFETCH(QString, exportMacro);
    QFETCH(int, result);

    static constexpr QLatin1StringView directory("invalid_export_macro");
    QDir outputDirectory(m_cmdLineGenerated);
    if (!outputDirectory.exists(directory))
        outputDirectory.mkdir(directory);
    outputDirectory.cd(directory);
    QString exportMacroCmd = "EXPORT_MACRO=" + exportMacro;

    QProcess process;
    process.setWorkingDirectory(m_cmdLineGenerated);
    process.startCommand(ProtocPath + QString(" ") + PluginKey + QtprotobufgenPath + OptKey
                         + exportMacroCmd + OutKey + outputDirectory.absolutePath() + IncludeKey
                         + m_expectedResult + "qtprotobufgen.proto" + allow_proto3_optional);
    QVERIFY2(process.waitForStarted(), msgProcessStartFailed(process).constData());
    if (!process.waitForFinished()) {
        process.kill();
        QFAIL(msgProcessTimeout(process).constData());
    }
    QVERIFY2(process.exitStatus() == QProcess::NormalExit, msgProcessCrashed(process).constData());
    QVERIFY2(process.exitCode() == result, msgProcessFailed(process).constData());
}
#endif // QT_CONFIG(process)

void qtprotobufgenTest::cleanupTestCase()
{
    if (m_copyTestResults) {
        const QString cmakeExpectedResultDir = m_expectedResult + CMakeGeneratedPathInfix;
        if (QFile::exists(cmakeExpectedResultDir))
            QDir(cmakeExpectedResultDir).removeRecursively();
        QVERIFY(QDir(m_expectedResult).mkdir(CMakeGeneratedDir));

        QVERIFY(copyDirectoryRecursively(QDir(m_cmakeGenerated), QDir(cmakeExpectedResultDir)));

#if QT_CONFIG(process)
        const QString cmdLineExpectedResultDir = m_expectedResult + CmdLineGeneratedPathInfix;
        if (QFile::exists(cmdLineExpectedResultDir))
            QDir(cmdLineExpectedResultDir).removeRecursively();
        QVERIFY(QDir(m_expectedResult).mkdir(CmdLineGeneratedDir));

        QVERIFY(copyDirectoryRecursively(QDir(m_cmdLineGenerated), QDir(cmdLineExpectedResultDir)));
#endif
    }

#if QT_CONFIG(process)
    // Remove content generated by command line tests.
    if (!m_cmdLineGenerated.isEmpty())
        cleanFolder(m_cmdLineGenerated);
#endif
}

QTEST_MAIN(qtprotobufgenTest)
#include "tst_qtprotobufgen.moc"
