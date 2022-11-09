/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include "core/appdialog.h"
#include "gitglobal.h"
#include "ui_commitpushdialog.h"

namespace Git
{
class Manager;
}
class ChangedFileActions;
class ChangedFilesModel;
class CommitPushDialog : public AppDialog, private Ui::CommitPushDialog
{
    Q_OBJECT

public:
    explicit CommitPushDialog(Git::Manager *git, QWidget *parent = nullptr);

private Q_SLOTS:
    void slotPushButtonCommitClicked();
    void slotPushButtonPushClicked();
    void slotToolButtonAddAllClicked();
    void slotToolButtonAddNoneClicked();
    void slotToolButtonAddIndexedClicked();
    void slotToolButtonAddAddedClicked();
    void slotToolButtonAddRemovedClicked();
    void slotToolButtonAddModifiedClicked();
    void slotListWidgetItemDoubleClicked(const QModelIndex &index);
    void slotGroupBoxMakeCommitToggled(bool);
    void slotListWidgetCustomContextMenuRequested(const QPoint &pos);
    void checkButtonsEnable();

private:
    enum Roles { StatusRole = Qt::UserRole + 1 };
    void addFiles();
    void reload();
    ChangedFileActions *mActions = nullptr;
    ChangedFilesModel *const mModel;
};
