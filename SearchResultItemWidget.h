#ifndef SEARCHRESULTITEMWIDGET_H
#define SEARCHRESULTITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class SearchResultItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchResultItemWidget(
        QWidget *parent,
        const QString& userId,
        const QPixmap& avatar,
        const QString& nickname,
        const QString& signature,
        std::function<void(SearchResultItemWidget& w)> callback
        );
    const QString& getUserIdR() const;
    int setAvatar(const QPixmap& avatar) ;
    int setNickname(const QString& nickanme) ;
    int setSignatrue(const QString& signatrue) ;
    int setAddContantBtnClickedCallback(std::function<void(SearchResultItemWidget& w)> callback);
    int setBtnDisable();

private:
    QLabel* avatarLabel_ = nullptr;
    QLabel* nicknameLabel_ = nullptr;
    QLabel* signatureLabel_ = nullptr;
    QPushButton* addContantBtn_ = nullptr;

private:
    QString userId_;
    std::function<void(SearchResultItemWidget& w)> addContantBtnClickedCallback_ = nullptr;

private:
    void addContantBtnClicked(bool);
    void initUI(QWidget* parent);
signals:
};

#endif // SEARCHRESULTITEMWIDGET_H
