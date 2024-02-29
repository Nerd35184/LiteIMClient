#ifndef SESSINFO_H
#define SESSINFO_H
#include <QString>

class SessInfo
{
public:
    SessInfo(
        const QString &sessId,
        const QString &sessName,
        const QString &to ,
        const QString &avatarUrl);

    const QString &getSessIdR() const;
    const QString &getSessNameR() const;
    const QString &getAvatarUrlR() const;
    const QString &getToR() const;
    void setAvatarUrl(const QString &avatarUrl);
    void setSessName(const QString &sessName);

private:
    QString sessId_;
    QString sessName_;
    QString avatarUrl_;
    QString to_;
};
#endif // SESSINFO_H
