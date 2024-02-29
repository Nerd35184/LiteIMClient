#include "UserInfo.h"
const QString &UserInfo::getUserIdR() const
{
    return this->userId_;
}

const QString &UserInfo::getNicknameR() const
{
    return this->nickname_;
}

const QString &UserInfo::getSignatureR() const
{
    return this->signature_;
}

const QString &UserInfo::getAvatarUrlR() const
{
    return this->avatarUrl_;
}

UserInfo::UserInfo(
    const QString &userId,
    const QString &nickname,
    const QString &signature,
    const QString &avatarUrl)
    : userId_(userId),
    nickname_(nickname),
    signature_(signature),
    avatarUrl_(avatarUrl)
{
}

