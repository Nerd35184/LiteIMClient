#ifndef CONTACTDETAILWIDGET_H
#define CONTACTDETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class ContactDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContactDetailWidget(QWidget *parent = nullptr);

private:
    QLabel *avatarLbl_ = nullptr;
    QLabel *nicknameLbl_ = nullptr;
    QLabel *signatureLbl_ = nullptr;
    QPushButton *createSessBtn_ = nullptr;
private:
    void initUI(QWidget *parent);
    QString userId_;
    void createSessBtnClicked();
    std::function<void(ContactDetailWidget &w)> createSessBtnClickedCallback_ = nullptr;

    //     int setDetail(
    //         const QString &userId,
    //         const QString &nickname,
    //         const QString &signature,
    //         const QPixmap &avatar);

    //     int setDetail(
    //         const QString &userId,
    //         const QString &nickname,
    //         const QString &signature,
    //         const QString &avatarUrl);

    //     const QString &getUserId() const;
    //     int setCreateSessBtnClickedCallback(
    //         std::function<void(ContactItemDetailWidget &w)> createSessBtnClickedCallback);
    //     int clear();

    //     // 由qobj控制生命周期
    // private:
    //     QLabel *avatarLabel_ = nullptr;
    //     QLabel *nicknameLabel_ = nullptr;
    //     QLabel *signatureLabel_ = nullptr;
    //     QPushButton *createSessBtn_ = nullptr;

    // private:
    //     void initUI(QWidget *parent);
    //     QString userId_;
    //     void createSessBtnClicked();
    //     std::function<void(ContactItemDetailWidget &w)> createSessBtnClickedCallback_ = nullptr;
    // signals:
signals:
};

#endif // CONTACTDETAILWIDGET_H
