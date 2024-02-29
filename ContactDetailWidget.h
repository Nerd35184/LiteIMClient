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
    int setDetail(
        const QString &userId,
        const QString &nickname,
        const QString &signature,
        const QPixmap &avatar);
    const QString& getUserIdR() const;

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
signals:
};

#endif // CONTACTDETAILWIDGET_H
