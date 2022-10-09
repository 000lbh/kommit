/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include "ui_branchesstatuswidget.h"
#include "widgetbase.h"

namespace Git
{
class Manager;
class BranchesModel;
}
class BranchActions;
class BranchesStatusWidget : public WidgetBase, private Ui::BranchesStatusWidget
{
    Q_OBJECT

public:
    explicit BranchesStatusWidget(QWidget *parent = nullptr);
    explicit BranchesStatusWidget(Git::Manager *git, AppWindow *parent = nullptr);

    void saveState(QSettings &settings) const override;
    void restoreState(QSettings &settings) override;

    void init(Git::Manager *git);

private Q_SLOTS:
    void on_comboBoxReferenceBranch_currentIndexChanged(const QString &selectedBranch);
    void on_pushButtonRemoveSelected_clicked();
    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void reload() override;

private:
    BranchActions *mActions = nullptr;
    Git::BranchesModel *mModel = nullptr;
};