#include "MainWidget.h"
#include "Util.h"
#include <QHBoxLayout>
#include <QListWidget>

MainWidget::MainWidget(QWidget *parent)
    : QWidget{parent}
{
    this->initUI(this);
    this->showDetailWidget(sessDetail);
    this->showItemListWidget(sessionItemList);
}

int MainWidget::showItemListWidget(ItemListWidgetEnum i)
{
    if(i == contactItemList){
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
    if(i == contactDetail){
        this->cttDtlWidget_->show();
        this->sessDtlWidget_->hide();
    }else{
        this->cttDtlWidget_->hide();
        this->sessDtlWidget_->show();
    }
    return 0;
}

int MainWidget::setShowSearchDetailWidgetBtnClickedCallback(std::function<void ()> callback)
{
    return this->searchWidget_->setShowSearchDetailWidgetBtnClickedCallback(callback);
}

int MainWidget::setMenuWidgetAvatar(const QPixmap &avatar)
{
    this->menuWidget_->setAvatar(avatar);
    return 0;
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
