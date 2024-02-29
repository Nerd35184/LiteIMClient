#ifndef SESSLISTITEM_H
#define SESSLISTITEM_H

#include <QWidget>

class SessListItem : public QWidget
{
    Q_OBJECT
public:
    explicit SessListItem(QWidget *parent = nullptr);
    int setAvatar(const QPixmap& avatar);
    const QString getSessIdR() const;
private:
    QString sessId_;
signals:
};

#endif // SESSLISTITEM_H
