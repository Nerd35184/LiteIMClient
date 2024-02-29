#ifndef USERINFO_H
#define USERINFO_H
#include <QString>

class UserInfo
{
public:
    UserInfo(const QString &userId, const QString &nickname, const QString &signature, const QString &avatarUrl);

    const QString &getUserIdR() const;
    const QString &getNicknameR() const;
    const QString &getSignatureR() const;
    const QString &getAvatarUrlR() const;

private:
    QString userId_;
    QString nickname_;
    QString signature_;
    QString avatarUrl_;
};

#endif // USERINFO_H
