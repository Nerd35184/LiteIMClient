#include "SessInfo.h"


SessInfo::SessInfo(const QString &sessId, const QString &sessName, const QString &to, const QString &avatarUrl) : sessId_(sessId),
    sessName_(sessName),
    avatarUrl_(avatarUrl),
    to_(to)
{
}

const QString &SessInfo::getSessIdR() const
{
    return this->sessId_;
}

const QString &SessInfo::getSessNameR() const
{
    return this->sessName_;
}

const QString &SessInfo::getAvatarUrlR() const
{
    return this->avatarUrl_;
}

const QString &SessInfo::getToR() const
{
    return this->to_;
}

void SessInfo::setAvatarUrl(const QString &avatarUrl)
{
    this->avatarUrl_ = avatarUrl;
    return;
}

void SessInfo::setSessName(const QString &sessName)
{
    this->sessName_ = sessName;
    return;
}

