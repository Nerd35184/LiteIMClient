#include "MainWidget.h"
#include "Util.h"
#include <QHBoxLayout>
#include <QListWidget>

MainWidget::MainWidget(QWidget *parent)
    : QWidget{parent}
{
    this->initUI(this);
    this->showDetailWidget(SessDetail);
    this->showItemListWidget(SessionItemList);

    connect(
        this->contactListWidget_,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem* item){
            this->contactListItemClicked(item);
        });

    connect(
        this->sessListWidget_,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem* item){
            this->sessListItemClicked(item);
        });
}

int MainWidget::showItemListWidget(ItemListWidgetEnum i)
{
    if(i == ContactItemList){
        this->contactListWidget_->show();
        this->sessListWidget_->hide();
    }else{
        this->contactListWidget_->hide();
        this->sessListWidget_->show();
    }
    return 0;
}

int MainWidget::showDetailWidget(ItemDetailWidgetEnum i)
{
    if(i == ContactDetail){
        this->cttDtlWidget_->show();
        this->sessDtlWidget_->hide();
    }else{
        this->cttDtlWidget_->hide();
        this->sessDtlWidget_->show();
    }
    return 0;
}

int MainWidget::setShowSearchDetailWidgetBtnClickedCallback(std::function<void (SearchWidget&)> callback)
{
    return this->searchWidget_->setShowSearchDetailWidgetBtnClickedCallback(callback);
}

int MainWidget::setMenuWidgetAvatar(const QPixmap &avatar)
{
    this->menuWidget_->setAvatar(avatar);
    return 0;
}

int MainWidget::selectMenuBtn(MenuWidget::Btn btn)
{
    this->menuWidget_->selectBtn(btn);
    return 0;
}

int MainWidget::setMenuBtnClickCallback(std::function<void (MenuWidget &, MenuWidget::Btn)> callback)
{
    return this->menuWidget_->setBtnClickCallback(callback);
}

int MainWidget::setModifyActionTriggeredCallback(std::function<void (MenuWidget::AvatarLabel &)> callback)
{
    return this->menuWidget_->setModifyActionTriggeredCallback(callback);
}

int MainWidget::clearList(ItemListWidgetEnum i)
{
    if(i == SessionItemList){
        this->sessListWidget_->clear();
    }else if(i == ContactItemList){
        this->contactListWidget_->clear();
    }else{
        return -1;
    }
    return 0;
}

int MainWidget::addContactListItem(ContactListItem *item)
{
    return AddListItem(this->contactListWidget_,item,40,60);
}

int MainWidget::setContactListItemAvatar(const QString &userId, const QPixmap &avatar)
{
    QListWidgetItem *item = nullptr;
    int row = 0;
    int ret = GetListWidgetItem<ContactListItem>(
        this->contactListWidget_,
        [&userId](ContactListItem *widget)
        {
            return widget->getUserIdR() == userId;
        },
        item,
        row);
    if(ret != 0){
        return ret;
    }
    auto w = qobject_cast<ContactListItem *>(this->contactListWidget_->itemWidget(item));
    w->setAvatar(avatar);
    return 0;
}

int MainWidget::setContactListItemClickedCallback(std::function<void (ContactListItem &)> callback)
{
    this->contactListItemClickedCallback_ =callback;
    return 0;
}

int MainWidget::selectContactListItem(const QString &userId)
{
    return this->selectListItem<ContactListItem>(
        this->contactListWidget_,
        [&userId](const ContactListItem& item){
            return item.getUserIdR() == userId;
        });
}

int MainWidget::setContactDetail(const QString &userId, const QString &nickname, const QString &signature, const QPixmap &avatar)
{
    return this->cttDtlWidget_->setDetail(userId,nickname,signature,avatar);
}

int MainWidget::setCreateSessBtnClickedCallback(std::function<void (ContactDetailWidget &)> callback)
{
    return this->cttDtlWidget_->setCreateSessBtnClickedCallback(callback);
}

int MainWidget::addSessListWidget(SessListItem *item)
{
    return AddListItem(this->sessListWidget_,item,40,60);
}

int MainWidget::setSessListItemAvatar(const QString &sessId, const QPixmap &avatar)
{
    QListWidgetItem *item = nullptr;
    int row = 0;
    int ret = GetListWidgetItem<SessListItem>(
        this->sessListWidget_,
        [&sessId](SessListItem *widget)
        {
            return widget->getSessIdR() == sessId;
        },
        item,
        row);
    if(ret != 0){
        return ret;
    }
    auto w = qobject_cast<SessListItem *>(this->contactListWidget_->itemWidget(item));
    w->setAvatar(avatar);
    return 0;
}

int MainWidget::setSessListItemClickedCallback(std::function<void (SessListItem &)> callback)
{
    this->sessListItemClickedCallback_ = callback;
    return 0;
}

int MainWidget::selectSessListItem(const QString &sessId)
{
    return this->selectListItem<SessListItem>(
        this->sessListWidget_,
        [&sessId](const SessListItem& item){
            return item.getSessIdR() == sessId;
        });
}

int MainWidget::upsertSessListItem(SessListItem *newSessItem,bool select)
{
    if(newSessItem == nullptr){
        return -1;
    }
    auto listWidget = this->sessListWidget_;
    bool found = false;
    int i = 0;

    for (i = 0; i < listWidget->count(); i++)
    {
        auto item = listWidget->item(i);
        if (item == nullptr)
        {
            return -1;
        }
        auto itemWidget = listWidget->itemWidget(item);
        if (itemWidget == nullptr)
        {
            return -1;
        }
        if(found && select){
            itemWidget->setStyleSheet("background-color: rgba(230, 230, 230,255);");
            continue;
        }
        auto olsSessItem = qobject_cast<SessListItem*>(itemWidget);
        if (olsSessItem == nullptr)
        {
            return -1;
        }
        if(newSessItem->getSessIdR() == olsSessItem->getSessIdR()){
            found = true;
        }

        if(found && !select){
            break;
        }
        if(found && select){
            olsSessItem->setStyleSheet("background-color: rgba(200, 198, 197,255);");
            break;
        }

        if(!found && select){
            olsSessItem->setStyleSheet("background-color: rgba(230, 230, 230,255);");
        }
    }
    if(!found){
        if(select){
            newSessItem->setStyleSheet("background-color: rgba(200, 198, 197,255);");
        }
        return this->addSessListWidget(newSessItem);
    }
    return 0;
}

int MainWidget::upsertSessDetail(ChatDetailWidget *w, bool show)
{
    if(w == nullptr){
        return -1;
    }
    return this->sessDtlWidget_->upsertSessDetail(w,show);
}

int MainWidget::removeSess(const QString &sessId)
{
    int ret = this->sessDtlWidget_->removeSessDetail(sessId);
    if(ret != 0){
        return ret;
    }
    return TakeListWidgetItem<SessListItem>(
        this->sessListWidget_,
        [&sessId](SessListItem *widget)
        {
            return widget->getSessIdR() == sessId;
        });
}

int MainWidget::removeContact(const QString &userId)
{
    int ret = this->cttDtlWidget_->clearIfEqual(userId);
    if(ret != 0){
        return 0;
    }
    return  TakeListWidgetItem<ContactListItem>(
        this->contactListWidget_,
        [&userId](ContactListItem *widget)
        {
            return widget->getUserIdR() == userId;
        });
}

int MainWidget::addChatBubbleWidget(const QString &sessId, ChatBubbleWidget *chatBubbleWidget)
{
    return this->sessDtlWidget_->addChatBubbleWidget(sessId,chatBubbleWidget);
}

void MainWidget::initUI(QWidget *parent)
{
    ObjGuard g(3);
    this->setAttribute(Qt::WA_StyledBackground);
    this->menuWidget_ = CreateQWidget<MenuWidget>(g, "MainWidget_menuWidget", parent);

    auto searchAndItemListWidget = CreateQWidget<QWidget>(g,"MainWidget_searchAndItemListWidget",parent);
    this->initSearchAndItemListWidget(searchAndItemListWidget);

    auto titleAndDetailWidget = CreateQWidget<QWidget>(g,"MainWidget_titleAndDetailWidget",parent);
    this->initTitlAndItemDetailWidget(titleAndDetailWidget);

    auto splitter = CreateQSplitter<QSplitter>(parent);
    splitter->setOrientation(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(this->menuWidget_);
    splitter->addWidget(searchAndItemListWidget);
    splitter->addWidget(titleAndDetailWidget);
    auto layout = CreateQLayout<QHBoxLayout>(parent);
    layout->addWidget(splitter);
    this->resize(700, 500);
    return ;
}

void MainWidget::initSearchAndItemListWidget(QWidget *parent)
{
    ObjGuard g(3);
    this->searchWidget_ = CreateQWidget<SearchWidget>(g,"MainWidget_searchWidget", parent, nullptr);
    this->sessListWidget_ = CreateQWidget<QListWidget>(g,"MainWidget_sessListWidget", parent);
    this->sessListWidget_->setFrameShape(QListWidget::NoFrame);
    this->sessListWidget_->hide();
    this->contactListWidget_ = CreateQWidget<QListWidget>(g,"MainWidget_contactListWidget", parent);
    this->contactListWidget_->setFrameShape(QListWidget::NoFrame);
    this->contactListWidget_->hide();
    auto layout = CreateQLayout<QVBoxLayout>(parent);
    layout->addWidget(this->searchWidget_);
    layout->addWidget(this->sessListWidget_);
    layout->addWidget(this->contactListWidget_);
    return ;
}

void MainWidget::initTitlAndItemDetailWidget(QWidget *parent)
{
    ObjGuard g(3);
    this->titleWidget_ = CreateQWidget<QWidget>(g,"MainWidget_titleWidget",parent);
    this->sessDtlWidget_ = CreateQWidget<SessDetailWidget>(g,"MainWidget_sessDtlWidget",parent);
    this->cttDtlWidget_ = CreateQWidget<ContactDetailWidget>(g,"MainWidget_contactListWidget",parent);

    auto layout = CreateQLayout<QVBoxLayout>(parent);
    layout->addWidget(this->titleWidget_);
    layout->addWidget(this->sessDtlWidget_);
    layout->addWidget(this->cttDtlWidget_);
    return;
}

void MainWidget::contactListItemClicked(QListWidgetItem *itemClicked)
{
    this->ListItemClicked<ContactListItem>(
        this->contactListWidget_,
        itemClicked,
        this->contactListItemClickedCallback_);
}

void MainWidget::sessListItemClicked(QListWidgetItem *itemClicked)
{
    this->ListItemClicked<SessListItem>(
        this->sessListWidget_,
        itemClicked,
        this->sessListItemClickedCallback_);
}
