#include "ContactDetailWidget.h"
#include <QVBoxLayout>
#include "Util.h"

ContactDetailWidget::ContactDetailWidget(QWidget *parent)
    : QWidget{parent}
{
    this->initUI(this);

    connect(this->createSessBtn_,&QPushButton::clicked,this,&ContactDetailWidget::createSessBtnClicked);
}

int ContactDetailWidget::setCreateSessBtnClickedCallback(std::function<void (ContactDetailWidget &)> callback)
{
    this->createSessBtnClickedCallback_ = callback;
    return 0;
}

int ContactDetailWidget::setDetail(
    const QString &userId,
    const QString &nickname,
    const QString &signature,
    const QPixmap &avatar)
{
    this->userId_ = userId;
    this->nicknameLbl_->setText("昵称：" + nickname);
    this->signatureLbl_->setText("个性签名：" + signature);
    this->avatarLbl_->setPixmap(avatar);

    this->avatarLbl_->show();
    this->nicknameLbl_->show();
    this->signatureLbl_->show();
    this->avatarLbl_->show();
    this->createSessBtn_->show();
    return 0;
}

const QString &ContactDetailWidget::getUserIdR() const
{
    return this->userId_;
}

void ContactDetailWidget::initUI(QWidget *parent)
{
    ObjGuard g(3);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->avatarLbl_ = CreateQWidget<QLabel>(g, "ContactDetailWidget_avatarLbl", parent);
    this->avatarLbl_->setScaledContents(true);
    this->nicknameLbl_ = CreateQWidget<QLabel>(g, "ContactDetailWidget_nicknameLbl", parent);
    this->signatureLbl_ = CreateQWidget<QLabel>(g, "ContactDetailWidget_signatureLbl", parent);
    this->createSessBtn_ = CreateQWidget<QPushButton>(g, "ContactDetailWidget_createSessBtn", parent);
    this->createSessBtn_->setText("发送消息");
    this->createSessBtn_->hide();
    auto layout = CreateQLayout<QVBoxLayout>(parent);
    layout->addWidget(this->avatarLbl_);
    layout->addWidget(this->nicknameLbl_);
    layout->addWidget(this->signatureLbl_);
    layout->addWidget(this->createSessBtn_);
    layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    return;
}

void ContactDetailWidget::createSessBtnClicked(bool)
{
    if(this->createSessBtnClickedCallback_==nullptr){
    }
    this->createSessBtnClickedCallback_(*this);
    return;
}
