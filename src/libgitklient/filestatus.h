/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include "libgitklient_export.h"
#include <QObject>
#include <QString>

class LIBGITKLIENT_EXPORT FileStatus
{
public:
    enum Status {
        Unknown = 0,
        Unmodified = 1,
        Modified = 2,
        Added = 3,
        Removed = 4,
        Renamed = 5,
        Copied = 6,
        UpdatedButInmerged = 7,
        Ignored = 8,
        Untracked = 9,
        NoGit = 10
    };

    FileStatus();
    FileStatus(QString name, Status status);

    const QString &name() const;
    Q_REQUIRED_RESULT Status status() const;

    void parseStatusLine(const QString &line);
    Q_REQUIRED_RESULT const QString &fullPath() const;

    void setFullPath(const QString &newFullPath);

    void setStatus(const QString &x, const QString &y = QString());
    void setName(const QString &newName);

private:
    QString mFullPath;
    QString mName;
    Status mStatus;

    friend class Manager;
};
Q_DECLARE_METATYPE(FileStatus)
Q_DECLARE_TYPEINFO(FileStatus, Q_MOVABLE_TYPE);
bool operator==(const FileStatus &f1, const FileStatus &f2);