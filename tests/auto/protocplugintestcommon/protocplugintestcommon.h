// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef PROTOCPLUGINTESTCOMMON_H
#define PROTOCPLUGINTESTCOMMON_H

#include <QtCore/qdir.h>
#include <QtCore/qstring.h>

QT_FORWARD_DECLARE_CLASS(QProcess)

namespace ProtocPluginTest {
QByteArray msgCannotReadFile(const QFile &file);
#if QT_CONFIG(process)
QByteArray msgProcessStartFailed(const QProcess &p);
QByteArray msgProcessTimeout(const QProcess &p);
QByteArray msgProcessCrashed(QProcess &p);
QByteArray msgProcessFailed(QProcess &p);
bool protocolCompilerAvailableToRun(const QString &protocPath);
#endif // QT_CONFIG(process)

void compareTwoFiles(const QString &expectedFileName, const QString &actualFileName);
void cleanFolder(const QString &folderName);
QFileInfoList scanDirectoryRecursively(const QDir &dir);
QStringList relativePaths(const QDir &dir, const QFileInfoList &files);
bool copyDirectoryRecursively(const QDir &from, QDir to);
} // namespace ProtocPluginTest

#endif // PROTOCPLUGINTESTCOMMON_H
