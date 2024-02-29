#include "SessListItem.h"
#include <QMenu>
#include "Util.h"
#include <QVBoxLayout>
#include <QContextMenuEvent>

SessListItem::SessListItem(
    QWidget *parent,
    const QString &sessId,
    const QString &sessName,
    const QString &lastWord,
    const QPixmap &avatar)
    : QWidget{parent}
{
    this->initUI(this);
    this->initAction(this);

    connect(this->deleteAction_, &QAction::triggered, this, &SessListItem::deleteActionTriggered);

    this->sessId_ = sessId;
    this->lastWordLbl_->setText(lastWord);
    this->sessNameLbl_->setText(sessName);
    this->avatarLbl_->setPixmap(avatar);
    return;
}

const QString &SessListItem::getSessIdR() const
{
    return this->sessId_;
}

int SessListItem::setDeleteActionTriggered(std::function<void(SessListItem &)> deleteActionTriCallback)
{
    this->deleteActionTriCallback_ = deleteActionTriCallback;
    return 0;
}

int SessListItem::setAvatar(const QPixmap &avatar)
{
    this->avatarLbl_->setPixmap(avatar);
    return 0;
}

void SessListItem::deleteActionTriggered(bool)
{
    if (this->deleteActionTriCallback_ == nullptr)
    {
        return;
    }

    this->deleteActionTriCallback_(*this);
}

void SessListItem::initSessNameLastWordWidget(QWidget *parent)
{
    ObjGuard g(2);
    this->sessNameLbl_ = CreateQWidget<QLabel>(g, "SessInfoWidget_sessNameLbl", parent);
    this->lastWordLbl_ = CreateQWidget<QLabel>(g, "SessInfoWidget_lastWordLbl", parent);
    auto layout = CreateQLayout<QVBoxLayout>(parent);
    layout->addWidget(this->sessNameLbl_);
    layout->addWidget(this->lastWordLbl_);
    return;
}

void SessListItem::initUI(QWidget *parent)
{
    ObjGuard g(2);
    this->avatarLbl_ = CreateQWidget<QLabel>(g, "SessInfoWidget_avatarLabel", parent);
    this->avatarLbl_->setScaledContents(true);

    auto sessNameLastWordWidget = CreateQWidget<QWidget>(g, "SessInfoWidget_sessNameLastWordWidget", parent);
    sessNameLastWordWidget->setObjectName("SessInfoWidget_sessNameLastWordWidget");
    this->initSessNameLastWordWidget(sessNameLastWordWidget);
    auto layout = CreateQLayout<QHBoxLayout>(parent);
    layout->addWidget(this->avatarLbl_);
    layout->addWidget(sessNameLastWordWidget);
    return;
}

void SessListItem::initAction(QWidget *parent)
{
    ObjGuard g(1);
    this->deleteAction_ = CreateQObject<QAction>(g, "", "删除会话", this);
}

void SessListItem::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;;
    menu.addAction(this->deleteAction_);
    menu.exec(event->globalPos());
    return;
}
