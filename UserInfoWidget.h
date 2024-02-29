#ifndef USERINFOWIDGET_H
#define USERINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class UserInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserInfoWidget(QWidget *parent = nullptr);
    virtual ~UserInfoWidget()
    {
        qDebug("~UserInfoWidget");
    }
    int init(
        const QString &nickname,
        const QString &signatrue,
        const QPixmap &avatar);
    int setSaveBtnClickedCallback(
        std::function<void(UserInfoWidget &userInfoWidget)> saveBtnClickedCallback);

    QString getNickname();
    QString getSignatrue();
    QPixmap getAvatar();
    bool getAvatarChanged();

private:
    QLabel *avatarLbl_ = nullptr;
    QPushButton *uploadAvatarBtn_ = nullptr;
    QLabel *nicknameLbl_ = nullptr;
    QLineEdit *nicknameLdt_ = nullptr;
    QLabel *signatrueLbl_ = nullptr;
    QLineEdit *signatureLdt_ = nullptr;
    QPushButton *saveBtn_ = nullptr;
    QPushButton *cancelBtn_ = nullptr;

private:
    bool avatarChanged_ = false;
    std::function<void(UserInfoWidget &userInfoWidget)> saveBtnClickedCallback_ = nullptr;
    void initUI(QWidget *parent);
    void uploadAvatarButtonClicked(bool);
    void saveBtnClicked(bool);
    void cancelBtnClicked(bool);

signals:

signals:
};

#endif // USERINFOWIDGET_H
