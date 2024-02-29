#include "ContactDetailWidget.h"
#include <QVBoxLayout>
#include "Util.h"

ContactDetailWidget::ContactDetailWidget(QWidget *parent)
    : QWidget{parent}
{}

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
