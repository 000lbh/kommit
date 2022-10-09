/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "diffopendialog.h"

#include <KLocalizedString>
#include <QFileDialog>
#include <QSettings>

DiffOpenDialog::DiffOpenDialog(QWidget *parent)
    : AppDialog(parent)
{
    setupUi(this);
    radioButtonCompareFiles->setChecked(true);
    widgetSelectDirectories->hide();
    readSettings();
    // TODO update ok button enabled
}

DiffOpenDialog::~DiffOpenDialog()
{
    readSettings();
}

void DiffOpenDialog::readSettings()
{
    QSettings s;
    s.beginGroup(QStringLiteral("diff"));
    s.setValue("oldFile", lineEditOldFile->text());
    s.setValue("newFile", lineEditNewFile->text());
    s.setValue("oldDir", lineEditOldDirectory->text());
    s.setValue("newDir", lineEditNewDirectory->text());
    s.endGroup();
    s.sync();
}

void DiffOpenDialog::saveSettings()
{
    QSettings s;
    s.beginGroup(QStringLiteral("diff"));
    lineEditOldFile->setText(s.value("oldFile").toString());
    lineEditNewFile->setText(s.value("newFile").toString());
    lineEditOldDirectory->setText(s.value("oldDir").toString());
    lineEditNewDirectory->setText(s.value("newDir").toString());
    s.endGroup();
}

QString DiffOpenDialog::oldFile() const
{
    return lineEditOldFile->text();
}

QString DiffOpenDialog::newFile() const
{
    return lineEditNewFile->text();
}

QString DiffOpenDialog::oldDir() const
{
    return lineEditOldDirectory->text();
}

QString DiffOpenDialog::newDir() const
{
    return lineEditNewDirectory->text();
}

DiffOpenDialog::Mode DiffOpenDialog::mode() const
{
    return radioButtonCompareFiles->isChecked() ? Files : Dirs;
}

void DiffOpenDialog::on_toolButtonBrowseNewFile_clicked()
{
    const auto f = QFileDialog::getOpenFileName(this, i18n("Select new file"));
    if (!f.isEmpty())
        lineEditNewFile->setText(f);
}

void DiffOpenDialog::on_toolButtonBrowseOldFile_clicked()
{
    const auto f = QFileDialog::getOpenFileName(this, i18n("Select old file"));
    if (!f.isEmpty())
        lineEditOldFile->setText(f);
}

void DiffOpenDialog::on_toolButtonBrowseNewDirectory_clicked()
{
    const auto f = QFileDialog::getExistingDirectory(this, i18n("Select new directory"));
    if (!f.isEmpty())
        lineEditNewDirectory->setText(f);
}

void DiffOpenDialog::on_toolButtonBrowseOldDirectory_clicked()
{
    const auto f = QFileDialog::getExistingDirectory(this, i18n("Select old directory"));
    if (!f.isEmpty())
        lineEditOldDirectory->setText(f);
}