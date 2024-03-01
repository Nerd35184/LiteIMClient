#include "LogInWidget.h"
#include "ObjGuard.h"
#include "Util.h"
#include <QVBoxLayout>

LogInWidget::LogInWidget(QWidget *parent)
    : QWidget{parent}
{
    this->initUI(this);
    this->setWindowOpacity(0.7);
    this->setAttribute(Qt::WA_StyledBackground);
    connect(this->logInBtn_,&QPushButton::clicked,this,&LogInWidget::logInBtnClicked);
    return ;
}

QString LogInWidget::getUsername() const
{
    return this->usernameEdt_->text();
}

QString LogInWidget::getPassword() const
{
    return this->passwordEdt_->text();
}

int LogInWidget::setlogInBtnClickedCallback(std::function<void (LogInWidget &)> logInBtnClickedCallback)
{
    this->logInBtnClickedCallback_ = logInBtnClickedCallback;
    return 0;
}

void LogInWidget::initUI(QWidget *parent)
{
    ObjGuard g(4);
    this->titleLal_ = CreateQWidget<QLabel>(g,"LogInWidget_titleLal",parent);
    this->titleLal_->setText("log in");
    this->titleLal_->setAlignment(Qt::AlignCenter);

    this->usernameEdt_ = CreateQWidget<QLineEdit>(g,"LogInWidget_usernameEdt",parent);
    this->usernameEdt_->setPlaceholderText("username");
    this->usernameEdt_->setAlignment(Qt::AlignCenter);

    this->passwordEdt_ = CreateQWidget<QLineEdit>(g,"LogInWidget_passwordEdt",parent);
    this->passwordEdt_->setPlaceholderText("password");
    this->passwordEdt_->setAlignment(Qt::AlignCenter);
    this->passwordEdt_->setEchoMode(QLineEdit::Password);

    auto logInBtnWgt = CreateQWidget<QWidget>(g,"",parent);
    this->initBtnWidget(logInBtnWgt);
    auto layout = CreateQLayout<QVBoxLayout>(parent);
    auto s1 = new QSpacerItem(40,40,QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addSpacerItem(s1);
    layout->addWidget(this->titleLal_);
    layout->addWidget(this->usernameEdt_);
    layout->addWidget(this->passwordEdt_);
    layout->addWidget(logInBtnWgt);
    auto s2 = new QSpacerItem(40,40,QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addSpacerItem(s2);
    return ;
}

void LogInWidget::initBtnWidget(QWidget *parent)
{
    ObjGuard g(4);
    this->logInBtn_ = CreateQWidget<QPushButton>(g,"LogInWidget_logInBtn",parent);
    this->logInBtn_->setText("点击登录");
    auto layout = CreateQLayout<QHBoxLayout>(parent);
    auto s1 = new QSpacerItem(40,40,QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addSpacerItem(s1);
    layout->addWidget(this->logInBtn_);
    auto s2 = new QSpacerItem(40,40,QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addSpacerItem(s2);
}

void LogInWidget::logInBtnClicked(bool clicked)
{
    if(this->logInBtnClickedCallback_ == nullptr){
        return ;
    }
    this->logInBtnClickedCallback_(*this);
    return ;
}
