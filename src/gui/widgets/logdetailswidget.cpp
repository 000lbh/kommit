/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "logdetailswidget.h"
#include "GitKlientSettings.h"
#include "gitklient_appdebug.h"
#include "gitlog.h"
#include "gitmanager.h"
#include "models/logsmodel.h"

#include <KLocalizedString>

#include <QCalendar>
#include <QDesktopServices>

LogDetailsWidget::LogDetailsWidget(QWidget *parent)
    : QTextBrowser(parent)
{
    connect(this, &QTextBrowser::anchorClicked, this, &LogDetailsWidget::self_anchorClicked);
}

Git::Log *LogDetailsWidget::log() const
{
    return mLog;
}

void LogDetailsWidget::setLog(Git::Log *newLog)
{
    if (mLog == newLog)
        return;

    mLog = newLog;
    createText();
}

void LogDetailsWidget::createText()
{
    if (!mLog) {
        clear();
        return;
    }
    const auto files = Git::Manager::instance()->changedFiles(mLog->commitHash());
    QStringList filesHtml;

    for (auto i = files.constBegin(); i != files.constEnd(); ++i) {
        QString color;
        switch (i.value()) {
        case Git::Manager::Modified:
            color = GitKlientSettings::diffModifiedColor().name();
            break;
        case Git::Manager::Added:
            color = GitKlientSettings::diffAddedColor().name();
            break;
        case Git::Manager::Removed:
            color = GitKlientSettings::diffRemovedColor().name();
            break;

        case Git::Manager::Unknown:
        case Git::Manager::Unmodified:
        case Git::Manager::Renamed:
        case Git::Manager::Copied:
        case Git::Manager::UpdatedButInmerged:
        case Git::Manager::Ignored:
        case Git::Manager::Untracked:
            break;
        }
        if (mEnableFilesLinks)
            filesHtml.append(QStringLiteral("<li><font color=%1>%2</a></li>").arg(color, createFileLink(i.key())));
        else
            filesHtml.append(QStringLiteral("<li><font color=%1>%2</a></li>").arg(color, i.key()));
    }
    QStringList parentHashHtml;
    for (const auto &parent : mLog->parents())
        parentHashHtml.append(createHashLink(parent));

    QStringList childsHashHtml;
    for (const auto &child : mLog->childs())
        childsHashHtml.append(createHashLink(child));

    QString date;
    qCDebug(GITKLIENT_LOG) << "cal=" << GitKlientSettings::calendarType();
    QCalendar cal(GitKlientSettings::calendarType());
    /*switch (GitKlientSettings::calendarType()) {
    case SettingsHelper::CalendarType::Gregorian:
        cal = QCalendar(QCalendar::System::Gregorian);
        break;
    case SettingsHelper::CalendarType::Jalali:
        cal = QCalendar(QCalendar::System::Julian);
        break;
    case SettingsHelper::CalendarType::Milankovic:
        cal = QCalendar(QCalendar::System::Milankovic);
        break;
    case SettingsHelper::CalendarType::IslamicCivil:
        cal = QCalendar(QCalendar::System::IslamicCivil);
        break;
    }*/
    if (cal.isValid())
        date = mLog->commitDate().toLocalTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"), cal);
    else
        date = mLog->commitDate().toLocalTime().toString();

    clear();
    QString html;
    appendHeading(html, mLog->subject());
    if (!mLog->refLog().isEmpty())
        appendParagraph(html, i18n("Ref"), mLog->refLog());

    if (mEnableEmailsLinks) {
        appendParagraph(html, i18n("Committer"), QStringLiteral(R"(<a href="mailto:%2">%1 &lt;%2&gt;</a>)").arg(mLog->committerName(), mLog->committerEmail()));
        appendParagraph(html, i18n("Author"), QStringLiteral(R"(<a href="mailto:%2">%1 &lt;%2&gt;</a>)").arg(mLog->authorName(), mLog->authorEmail()));
    } else {
        appendParagraph(html, i18n("Committer"), QStringLiteral(R"(%1 &lt;%2&gt;)").arg(mLog->committerName(), mLog->committerEmail()));
        appendParagraph(html, i18n("Author"), QStringLiteral(R"(%1 &lt;%2&gt;)").arg(mLog->authorName(), mLog->authorEmail()));
    }
    appendParagraph(html, i18n("Date"), date);
    appendParagraph(html, i18n("Hash"), mLog->commitHash());

    if (!mLog->parents().empty())
        appendParagraph(html, mLog->parents().size() == 1 ? i18n("Parent") : i18n("Parents"), parentHashHtml.join(QStringLiteral(", ")));
    if (!mLog->childs().empty())
        appendParagraph(html, mLog->childs().size() == 1 ? i18n("Child") : i18n("Children"), childsHashHtml.join(QStringLiteral(", ")));

    appendParagraph(html, i18n("Changed files"), filesHtml);

    setHtml(html);
}

void LogDetailsWidget::appendHeading(QString &html, const QString &title, const short level)
{
    html.append(QStringLiteral("<h%2>%1</h%2>").arg(title).arg(level));
}

void LogDetailsWidget::appendParagraph(QString &html, const QString &text)
{
    html.append(QStringLiteral("<p>%1</p>").arg(text));
}

void LogDetailsWidget::appendParagraph(QString &html, const QString &name, const QString &value)
{
    html.append(QStringLiteral("<p><b>%1:</b> %2</p>").arg(name, value));
}

void LogDetailsWidget::appendParagraph(QString &html, const QString &name, const QStringList &list)
{
    if (list.isEmpty())
        return;

    html.append(QStringLiteral("<p><b>%1</b><ul>").arg(name));
    for (const auto &l : list)
        html.append(QStringLiteral("<li>%1</li>").arg(l));
    html.append(QStringLiteral("</ul>"));
}

QString LogDetailsWidget::createHashLink(const QString &hash) const
{
    auto log = Git::Manager::instance()->logsModel()->findLogByHash(hash);
    if (!log)
        return {};

    if (mEnableCommitsLinks)
        return QStringLiteral(R"(<a href="hash:%1">%2</a> )").arg(log->commitHash(), log->subject());

    return log->subject();
}

QString LogDetailsWidget::createFileLink(const QString &file)
{
    return QStringLiteral(R"(<a href="file:%1">%1</a> )").arg(file);
}

void LogDetailsWidget::self_anchorClicked(const QUrl &url)
{
    const auto scheme = url.scheme().toLower();

    if (scheme == QStringLiteral("hash"))
        Q_EMIT hashClicked(url.path());
    else if (scheme == QStringLiteral("file"))
        Q_EMIT fileClicked(url.path());
    else if (scheme == QStringLiteral("mailto"))
        QDesktopServices::openUrl(url);
}

bool LogDetailsWidget::enableCommitsLinks() const
{
    return mEnableCommitsLinks;
}

void LogDetailsWidget::setEnableCommitsLinks(bool newEnableCommitsLinks)
{
    if (mEnableCommitsLinks == newEnableCommitsLinks)
        return;
    mEnableCommitsLinks = newEnableCommitsLinks;
    Q_EMIT enableCommitsLinksChanged();
}

bool LogDetailsWidget::enableEmailsLinks() const
{
    return mEnableEmailsLinks;
}

void LogDetailsWidget::setEnableEmailsLinks(bool newEnableEmailsLinks)
{
    if (mEnableEmailsLinks == newEnableEmailsLinks)
        return;
    mEnableEmailsLinks = newEnableEmailsLinks;
    Q_EMIT enableEmailsLinksChanged();
}

bool LogDetailsWidget::enableFilesLinks() const
{
    return mEnableFilesLinks;
}

void LogDetailsWidget::setEnableFilesLinks(bool newEnableFilesLinks)
{
    if (mEnableFilesLinks == newEnableFilesLinks)
        return;
    mEnableFilesLinks = newEnableFilesLinks;
    Q_EMIT enableFilesLinksChanged();
}
