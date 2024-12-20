/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include "libkommitgui_export.h"
#include <QMap>
#include <QObject>
#include <QString>

enum ParseReturnType {
    ExecApp,
    ReturnCode
};

struct ArgParserReturn {
    ParseReturnType type;
    int code{0};

    ArgParserReturn() = default;
    ArgParserReturn(int code)
        : type{ReturnCode}
        , code{code}
    {
    }
    ArgParserReturn(ParseReturnType type)
        : type{type}
        , code{0}
    {
    }
};

namespace Git
{
class Repository;
}

#define HelpText(name, text) Q_CLASSINFO("help." #name, text)
class LIBKOMMITGUI_EXPORT CommandArgsParser : public QObject
{
    Q_OBJECT
public:
    CommandArgsParser();
    ~CommandArgsParser() override;
    ArgParserReturn run(const QStringList &args);

public Q_SLOTS:
    ArgParserReturn help();

    ArgParserReturn clone();
    ArgParserReturn clone(const QString &path);
    ArgParserReturn init(const QString &path);
    ArgParserReturn pull(const QString &path);
    ArgParserReturn fetch(const QString &path);
    ArgParserReturn push(const QString &path);
    ArgParserReturn merge(const QString &path);
    ArgParserReturn changes();
    ArgParserReturn changes(const QString &path);

    ArgParserReturn create_tag(const QString &path);
    ArgParserReturn diff();
    ArgParserReturn diff(const QString &file);
    ArgParserReturn diff(const QString &file1, const QString &file2);
    ArgParserReturn diff(const QString &path, const QString &file1, const QString &file2);
    ArgParserReturn blame(const QString &file);
    ArgParserReturn history(const QString &file);
    ArgParserReturn merge();
    ArgParserReturn merge(const QString &base, const QString &local, const QString &remote, const QString &result);
    ArgParserReturn ignore(const QString &path);
    ArgParserReturn cleanup(const QString &path);
    ArgParserReturn switch_checkout(const QString &path);
    ArgParserReturn diff_branches(const QString &path);
    ArgParserReturn browse(const QString &place);
    ArgParserReturn browse(const QString &path, const QString &place);
    ArgParserReturn conflicts(const QString &path);

    ArgParserReturn add(const QString &path);
    ArgParserReturn remove(const QString &path);

    ArgParserReturn main();
    ArgParserReturn main(const QString &path);

private:
    [[nodiscard]] bool checkGitPath(const QString &path);
    Git::Repository *const mGit;
    QMap<QString, QString> mHelpTexts;
};
