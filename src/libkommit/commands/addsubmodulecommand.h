/*
SPDX-FileCopyrightText: 2021 Hamed Masafi <hamed.masfi@gmail.com>

SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once
#include "abstractcommand.h"
#include "libkommit_export.h"

namespace Git
{

class LIBKOMMIT_EXPORT AddSubmoduleCommand : public AbstractCommand
{
    Q_OBJECT

public:
    explicit AddSubmoduleCommand(Repository *git);

    [[nodiscard]] QStringList generateArgs() const override;

    [[nodiscard]] bool force() const;
    void setForce(bool newForce);

    [[nodiscard]] QString branch() const;
    void setbranch(const QString &newbranch);

    [[nodiscard]] const QString &url() const;
    void setUrl(const QString &newUrl);

    [[nodiscard]] const QString &localPath() const;
    void setLocalPath(const QString &newLocalPath);

private:
    bool mForce{false};
    QString mBranch;
    QString mUrl;
    QString mLocalPath;
};

}
