/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "../../dolphinplugins/manager.cpp"
#include "../../dolphinplugins/statuscache.cpp"
#include "../../src/git/filestatus.cpp"
#include "../common/gittestmanager.cpp"
#include "gitglobal.cpp"

#include <QTest>

class OverlayTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test1();
};

void OverlayTest::test1()
{
    GitTestManager tm;

    tm.init();
    tm.addToIgnore("ignored.txt");
    tm.touch("ignored.txt");
    tm.touch("added.txt");
    tm.touch("changed.txt");
    tm.touch("removed.txt");
    tm.add();
    tm.commit("Initial commit");
    tm.touch("changed.txt");
    tm.remove("removed.txt");
    tm.touch("untracked.txt");

    StatusCache cache;

    QCOMPARE(cache.fileStatus(tm.absoluteFilePath("added.txt")), FileStatus::Unmodified);
    QCOMPARE(cache.fileStatus(tm.absoluteFilePath("ignored.txt")), FileStatus::Ignored);
    QCOMPARE(cache.fileStatus(tm.absoluteFilePath("removed.txt")), FileStatus::Removed);
    QCOMPARE(cache.fileStatus(tm.absoluteFilePath("changed.txt")), FileStatus::Modified);
    QCOMPARE(cache.fileStatus(tm.absoluteFilePath("untracked.txt")), FileStatus::Untracked);
}

QTEST_MAIN(OverlayTest)

#include "overlaytest.moc"
