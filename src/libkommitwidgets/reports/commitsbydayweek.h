/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include "abstractreport.h"
#include "libkommitwidgets_export.h"

class LIBKOMMITWIDGETS_EXPORT CommitsByDayWeek : public AbstractReport
{
    Q_OBJECT

public:
    explicit CommitsByDayWeek(Git::Repository *git, QObject *parent = nullptr);

    void reload() override;
    [[nodiscard]] QString name() const override;

    [[nodiscard]] int columnCount() const override;
    [[nodiscard]] QStringList headerData() const override;

    [[nodiscard]] bool supportChart() const override;
    [[nodiscard]] QString axisXTitle() const override;
    [[nodiscard]] QString axisYTitle() const override;

    [[nodiscard]] int labelsAngle() const override;

private:
    enum CommitsByDayWeekRoles {
        DayOfWeek,
        Commits,
        LastColumn,
    };
};
