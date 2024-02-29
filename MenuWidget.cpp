#include "MenuWidget.h"
#include "Util.h"
#include <QVBoxLayout>
#include <QMenu>

MenuWidget::AvatarLabel::AvatarLabel(
    QWidget *parent,
    std::function<void(AvatarLabel&)> modifyActionTriggeredCallback):
    QLabel{parent},modifyActionTriggeredCallback_(modifyActionTriggeredCallback)
{
    this->initAction(this);
    connect(this->modifyAction_,&QAction::triggered,this,&AvatarLabel::modifyActionTriggered);
    return ;
}

void MenuWidget::AvatarLabel::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(this->modifyAction_);
    menu.exec(event->globalPos());
    return ;
}

int MenuWidget::AvatarLabel::setModifyActionTriggeredCallback(std::function<void (AvatarLabel &)> modifyActionTriggeredCallback)
{
    qDebug("setModifyActionTriggeredCallback 1");
    this->modifyActionTriggeredCallback_ = modifyActionTriggeredCallback;
    return 0;
}

void MenuWidget::AvatarLabel::initAction(QWidget *parent)
{
    ObjGuard g(1);
    this->modifyAction_ = CreateQObject<QAction>(g,"","修改信息",parent);
    return ;
}

void MenuWidget::AvatarLabel::modifyActionTriggered(bool)
{
    qDebug("setModifyActionTriggeredCallback 2");
    if(this->modifyActionTriggeredCallback_ != nullptr){
        this->modifyActionTriggeredCallback_(*this);
    }
    return ;
}

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget{parent}
{
    this->initUI(this);
    connect(this->sessBtn_,&QPushButton::clicked,this,&MenuWidget::btnClicked);
    connect(this->contactBtn_,&QPushButton::clicked,this,&MenuWidget::btnClicked);
    connect(this->newContactBtn_,&QPushButton::clicked,this,&MenuWidget::btnClicked);

    return ;
}

int MenuWidget::selectBtn(Btn btn)
{
    if(btn == newContactBtn){
        qDebug("MenuWidget selectBtn error");
        return -1;
    }
    this->sessBtn_->setStyleSheet("color: rgba(151, 151, 151,255);");
    this->contactBtn_->setStyleSheet("color: rgba(151, 151, 151,255);");
    if(btn == sessBtn){
        this->sessBtn_->setStyleSheet("color: rgba(7, 193, 96,255);");
    }else if(btn == contactBtn){
        this->contactBtn_->setStyleSheet("color: rgba(7, 193, 96,255);");
    }else{
        qDebug("MenuWidget selectBtn error");
    }
    return 0;
}

int MenuWidget::setAvatar(const QPixmap &map)
{
    this->avatarLbl_->setPixmap(map);
    return 0;
}

int MenuWidget::setBtnClickCallback(std::function<void (MenuWidget &, Btn)> callback)
{
    this->btnClickedCallback_ = callback;
    return 0;
}

int MenuWidget::setModifyActionTriggeredCallback(std::function<void (AvatarLabel &)> callback)
{
    return this->avatarLbl_->setModifyActionTriggeredCallback(callback);
}

void MenuWidget::initUI(QWidget *parent)
{
    ObjGuard g(4);

    this->avatarLbl_ =  CreateQWidget<AvatarLabel>(
        g,"MenuWidget_avatarLbl",
        parent,nullptr);
    this->avatarLbl_->setScaledContents(true);

    this->sessBtn_ = CreateQWidget<QPushButton>(g,"MenuWidget_sessBtn",parent);
    this->sessBtn_->setText("聊天");
    this->contactBtn_ = CreateQWidget<QPushButton>(g,"MenuWidget_contactBtn",parent);
    this->contactBtn_->setText("联系人");
    this->newContactBtn_ = CreateQWidget<QPushButton>(g,"MenuWidget_newContactBtn",parent);
    this->newContactBtn_->setText("新朋友");

    auto layout = CreateQLayout<QVBoxLayout>(parent);
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(this->avatarLbl_);
    layout->addWidget(this->sessBtn_);
    layout->addWidget(this->contactBtn_);
    layout->addWidget(this->newContactBtn_);

    return ;
}

void MenuWidget::btnClicked(bool)
{
    if(this->btnClickedCallback_ == nullptr){
        return ;
    }
    auto s = sender();
    Btn btn;
    if(s == this->sessBtn_){
        btn = sessBtn;
    }else if(s == this->contactBtn_){
        btn = contactBtn;
    }else if(s == this->newContactBtn_){
        btn = newContactBtn;
    }else{
        qDebug("MenuWidget btnClicked error");
        return ;
    }
    this->btnClickedCallback_(*this,btn);
}
