/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "difftreeview.h"
#include "models/difftreemodel.h"
#include "models/filesmodel.h"

#include <QKeyEvent>
#include <QSortFilterProxyModel>

DiffTreeModel *DiffTreeView::diffModel() const
{
    return _diffModel;
}

void DiffTreeView::setModels(DiffTreeModel *newDiffModel, FilesModel *filesModel)
{
    _diffModel = newDiffModel;
    _filesModel = filesModel;

    _filterModel->setSourceModel(filesModel);
    listView->setModel(_filterModel);

    treeView->setModel(_diffModel);
}

DiffTreeView::DiffTreeView(QWidget *parent) : QWidget(parent)
  , _filterModel(new QSortFilterProxyModel(this))
{
    setupUi(this);
    _filterModel->setFilterKeyColumn(0);
    _filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    listView->setModel(_filterModel);

    connect(checkBoxHideUnchangeds,
            &QAbstractButton::toggled,
            this,
            &DiffTreeView::hideUnchangedsChanged);
    checkBoxHideUnchangeds->hide();

    lineEditFilter->installEventFilter(this);
    listView->installEventFilter(this);
}

void DiffTreeView::on_lineEditFilter_textChanged(QString text)
{
    stackedWidget->setCurrentIndex(text.isEmpty() ? 0 : 1);
    _filterModel->setFilterRegularExpression(".*" + text + ".*");
}

void DiffTreeView::on_treeView_clicked(const QModelIndex &index)
{
    const auto fileName = _diffModel->fullPath(index);
    emit fileSelected(fileName);
}

void DiffTreeView::on_listView_clicked(const QModelIndex &index)
{
    const auto row = _filterModel->mapToSource(index).row();
    const auto fileName = _filesModel->data(_filesModel->index(row, 1), Qt::DisplayRole);
    emit fileSelected(fileName.toString());
}

bool DiffTreeView::hideUnchangeds() const
{
    return checkBoxHideUnchangeds->isChecked();
}

void DiffTreeView::setHideUnchangeds(bool newHideUnchangeds)
{
    if (checkBoxHideUnchangeds->isChecked() == newHideUnchangeds)
        return;
    checkBoxHideUnchangeds->setChecked(newHideUnchangeds);
    emit hideUnchangedsChanged();
}

bool DiffTreeView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == lineEditFilter && event->type() == QEvent::KeyPress) {
        auto ke = dynamic_cast<QKeyEvent*>(event);

        if (ke->key() == Qt::Key_Down) {
            if (stackedWidget->currentIndex() == 0)
                treeView->setFocus();
            else
                listView->setFocus();
        }
    }
    if (watched == listView && event->type() == QEvent::KeyPress) {
        auto ke = static_cast<QKeyEvent*>(event);

        if (ke->key() == Qt::Key_Up && listView->currentIndex().row() == 0)
            lineEditFilter->setFocus();
    }
    return QWidget::eventFilter(watched, event);
}
