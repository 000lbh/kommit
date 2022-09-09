/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "commitswidget.h"

#include "actions/branchactions.h"
#include "dialogs/filestreedialog.h"
#include "models/treemodel.h"
#include "git/gitmanager.h"


CommitsWidget::CommitsWidget(QWidget *parent) : WidgetBase(parent)
{
    setupUi(this);
    init();
}

CommitsWidget::CommitsWidget(Git::Manager *git, AppWindow *parent) : WidgetBase(git, parent)
{
    setupUi(this);
    init();
}

void CommitsWidget::reload()
{
    mRepoModel->clear();
    auto branches = git()->branches();
    mRepoModel->addData(branches);

    if (branches.contains("master"))
        mMainBranch = QStringLiteral("master");
    else if (branches.contains("main"))
        mMainBranch = QStringLiteral("main");

    widgetCommitsView->setBranch(QString());
}

void CommitsWidget::saveState(QSettings &settings) const
{
    save(settings, splitter);
    save(settings, treeViewRepo);
    widgetCommitsView->saveState(settings);
}

void CommitsWidget::restoreState(QSettings &settings)
{
    restore(settings, splitter);
    restore(settings, treeViewRepo);
    widgetCommitsView->restoreState(settings);
}

void CommitsWidget::on_treeViewRepo_itemActivated(const QModelIndex &index)
{
    auto key = mRepoModel->key(index);
    if (!key.isEmpty())
        widgetCommitsView->setBranch(key);
}

void CommitsWidget::on_treeViewRepo_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    auto branchName = mRepoModel->fullPath(treeViewRepo->currentIndex());
    mActions->setBranchName(branchName);
    mActions->popup();
}

void CommitsWidget::init()
{
    mRepoModel = new TreeModel(this);
    treeViewRepo->setModel(mRepoModel);

    mActions = new BranchActions(_git, this);
}
