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
    int setCreateSessBtnClickedCallback(std::function<void(ContactDetailWidget &w)> callback);
    int setDetail(
        const QString &userId,
        const QString &nickname,
        const QString &signature,
        const QPixmap &avatar);
    int clearIfEqual(const QString &userId);
    const QString& getUserIdR() const;
    int clear();

private:
    QLabel *avatarLbl_ = nullptr;
    QLabel *nicknameLbl_ = nullptr;
    QLabel *signatureLbl_ = nullptr;
    QPushButton *createSessBtn_ = nullptr;

private:
    std::function<void(ContactDetailWidget &w)> createSessBtnClickedCallback_ = nullptr;
    QString userId_;

private:
    void initUI(QWidget *parent);
    void createSessBtnClicked(bool);
signals:
};

#endif // CONTACTDETAILWIDGET_H
