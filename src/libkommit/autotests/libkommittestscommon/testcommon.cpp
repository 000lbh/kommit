/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "testcommon.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QResource>
#include <QStandardPaths>
#include <QUuid>
#include <repository.h>

namespace TestCommon
{

QString touch(const QString &fileName)
{
    QFileInfo fi{fileName};
    fi.absoluteDir().mkpath(fi.absolutePath());

    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return {};

    auto content = QUuid::createUuid().toString(QUuid::Id128);
    f.write(content.toLatin1());
    f.close();
    return content;
}

QString getTempPath(bool create)
{
    const auto path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QLatin1Char('/') + QUuid::createUuid().toString(QUuid::Id128);
    if (create) {
        QDir d;
        d.mkpath(path);
    }
    return path;
}

bool cleanPath(Git::Repository *manager)
{
    if (!manager->isValid())
        return false;
    QDir d{manager->path()};
    return d.removeRecursively();
}

QString readFile(const QString &fileName)
{
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};
    const auto buffer = f.readAll();
    f.close();
    return buffer;
}

bool makePath(Git::Repository *manager, const QString &path)
{
    QDir d{};
    return d.mkpath(manager->path() + QLatin1Char('/') + path);
}

QString touch(Git::Repository *manager, const QString &fileName)
{
    QString content;
    if (fileName.startsWith(QLatin1Char('/')))
        content = touch(manager->path() + fileName);
    else
        content = touch(manager->path() + QLatin1Char('/') + fileName);
    manager->addFile(fileName);
    return content;
}

void initSignature(Git::Repository *manager)
{
    manager->setConfig("user.name", "kommit test user", Git::Repository::ConfigLocal);
    manager->setConfig("user.email", "kommit@kde.org", Git::Repository::ConfigLocal);
}

bool writeFile(Git::Repository *manager, const QString &fileName, const QString &content)
{
    QFile f(manager->path() + QLatin1Char('/') + fileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;
    f.write(content.toUtf8());
    f.flush();
    f.close();
    return true;
}

bool copyFolder(const QString &srcPath, const QString &destPath)
{
    QDir sourceDir(srcPath);
    if (!sourceDir.exists()) {
        qDebug() << "Source directory does not exist:" << srcPath;
        return false;
    }

    QDir targetDir(destPath);
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(destPath)) {
            qDebug() << "Failed to create destination directory:" << destPath;
            return false;
        }
    }

    // Get the list of files and directories in the source folder
    auto fileList = sourceDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);

    for (auto &fileInfo : fileList) {
        QString srcFilePath = fileInfo.filePath();
        QString destFilePath = destPath + QDir::separator() + fileInfo.fileName();

        if (fileInfo.isDir()) {
            // Recursively copy subdirectory
            if (!copyFolder(srcFilePath, destFilePath)) {
                return false;
            }
        } else {
            // Copy the file
            if (!QFile::copy(srcFilePath, destFilePath)) {
                qDebug() << "Failed to copy file:" << srcFilePath << "to" << destFilePath;
                return false;
            }
        }
    }

    return true;
}

bool extractSampleRepo(const QString &path)
{
    return copyFolder(SAMPLES_PATH, path);
}

}
