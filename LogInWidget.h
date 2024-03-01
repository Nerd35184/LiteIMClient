#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LogInWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogInWidget(QWidget *parent = nullptr);
    QString getUsername() const;
    QString getPassword() const;
    int setlogInBtnClickedCallback(std::function<void(LogInWidget& w)> logInBtnClickedCallback);
private:
    QLabel* titleLal_ = nullptr;
    QLineEdit* usernameEdt_ = nullptr;
    QLineEdit* passwordEdt_ = nullptr;
    QPushButton* logInBtn_ = nullptr;
private:
    std::function<void(LogInWidget& w)> logInBtnClickedCallback_ = nullptr;
private:
    void initUI(QWidget *parent);
    void initBtnWidget(QWidget *parent);
    void logInBtnClicked(bool clicked);
signals:
};

#endif // LOGINWIDGET_H
