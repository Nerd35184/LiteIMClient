#ifndef CONTACTLISTITEM_H
#define CONTACTLISTITEM_H

#include <QWidget>
#include <QLabel>

class ContactListItem : public QWidget
{
    Q_OBJECT
public:
    explicit ContactListItem(
        QWidget *parent,
        const QString& userId,
        const QString& nickname,
        std::function<void(ContactListItem& item)> deleteActCallback
        );
    int setAvatar(const QPixmap& avatar);
    const QString& getUserIdR() const;

private:
    QLabel* avatarLbl_ = nullptr;
    QLabel* nicknameLbl_ = nullptr;
    QAction *deleteAct_ = nullptr;

private:
    QString userId_;
    std::function<void(ContactListItem& item)> deleteActCallback_ = nullptr;

private:
    void initUI(QWidget *parent);
    void initAction(QWidget *parent);
    void deleteActionTriggered(bool);

signals:
};

#endif // CONTACTLISTITEM_H
