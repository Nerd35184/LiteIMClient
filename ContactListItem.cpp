#include "ContactListItem.h"
#include "Util.h"
#include <QHBoxLayout>

ContactListItem::ContactListItem(
    QWidget *parent,
    const QString& userId,
    const QString& nickname,
    std::function<void (ContactListItem &)> deleteActCallback):
    QWidget{parent},deleteActCallback_(deleteActCallback),userId_(userId)
{
    this->initUI(this);
    this->initAction(this);
    this->nicknameLbl_->setText(nickname);
    return ;
}

int ContactListItem::setAvatar(const QPixmap &avatar)
{
    this->avatarLbl_->setPixmap(avatar);
    return 0;
}

const QString &ContactListItem::getUserIdR() const
{
    return this->userId_;
}

void ContactListItem::initUI(QWidget *parent)
{
    ObjGuard g(2);

    this->avatarLbl_ = CreateQWidget<QLabel>(g, "ContactInfoWidget_avatarLbl", parent);
    this->avatarLbl_->setScaledContents(true);

    this->nicknameLbl_ = CreateQWidget<QLabel>(g, "ContactInfoWidget_nicknameLbl", parent);

    auto layout = CreateQLayout<QHBoxLayout>(parent);
    layout->addWidget(this->avatarLbl_);
    layout->addWidget(this->nicknameLbl_);
    return ;
}

void ContactListItem::initAction(QWidget *parent)
{
    ObjGuard g(1);
    this->deleteAct_ = CreateQObject<QAction>(g, "", "删除联系人", parent);
    return;
}

void ContactListItem::deleteActionTriggered(bool)
{
    if(this->deleteActCallback_ == nullptr){
        return ;
    }
    this->deleteActCallback_(*this);
    return ;
}
