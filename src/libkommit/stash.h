/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once
#include "libkommit_export.h"
#include <QDateTime>
#include <QString>

#include <git2/types.h>

namespace Git
{

class Manager;
class LIBKOMMIT_EXPORT Stash
{
public:
    explicit Stash(Git::Manager *git, QString name);
    Stash(size_t index, git_repository *repo, const char *message, const git_oid *stash_id);

    void apply();
    void drop();
    void pop();
    Q_REQUIRED_RESULT const QString &name() const;
    Q_REQUIRED_RESULT const QString &authorName() const;
    Q_REQUIRED_RESULT const QString &authorEmail() const;
    Q_REQUIRED_RESULT const QString &subject() const;
    Q_REQUIRED_RESULT const QString &branch() const;
    Q_REQUIRED_RESULT const QDateTime &pushTime() const;

    friend class Manager;
    friend class StashesModel;

private:
    Git::Manager *mGit = nullptr;
    QString mName;
    QString mAuthorName;
    QString mAuthorEmail;
    QString mSubject;
    QString mBranch;
    QDateTime mPushTime;
    size_t mIndex;
};

} // namespace Git
