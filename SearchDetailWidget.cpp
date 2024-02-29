#include "SearchDetailWidget.h"
#include <QVBoxLayout>
#include "Util.h"

SearchDetailWidget::SearchDetailWidget(QWidget *parent)
    : QWidget{parent}
{
    this->initUI(this);
    connect(this->searchLineEdit_,&QLineEdit::returnPressed,this,&SearchDetailWidget::returnPressed);

    return ;
}

QString SearchDetailWidget::getSearchLineText() const
{
    return this->searchLineEdit_->text();
}

int SearchDetailWidget::init()
{
    this->searchResultList_->clear();
    this->searchLineEdit_->clear();
    return 0;
}

int SearchDetailWidget::addSearchResultListItem(SearchResultItemWidget *w)
{
    return AddListItem<SearchResultItemWidget>(this->searchResultList_,w,40,60);
}

int SearchDetailWidget::setSearchResultListItemAvatar(const QString &userId, const QPixmap &avatar)
{
    QListWidgetItem *item = nullptr;
    int row = 0;
    int ret = GetListWidgetItem<SearchResultItemWidget>(
        this->searchResultList_,
        [&userId](SearchResultItemWidget *widget)
        {
            return widget->getUserIdR() == userId;
        },
        item,
        row);
    if(ret!=0){
        return ret;
    }
    auto w = qobject_cast<SearchResultItemWidget *>(this->searchResultList_->itemWidget(item));
    w->setAvatar(avatar);
    return 0;
}

int SearchDetailWidget::setAddContantBtnDisable(const QString &userId)
{
    QListWidgetItem *item = nullptr;
    int row = 0;
    int ret = GetListWidgetItem<SearchResultItemWidget>(
        this->searchResultList_,
        [&userId](SearchResultItemWidget *widget)
        {
            return widget->getUserIdR() == userId;
        },
        item,
        row);
    if(ret!=0){
        return ret;
    }
    auto w = qobject_cast<SearchResultItemWidget *>(this->searchResultList_->itemWidget(item));
    return w->setBtnDisable();
}

int SearchDetailWidget::initSearchResultList()
{
    this->searchResultList_->clear();
    return 0;
}

int SearchDetailWidget::setReturnPressedCallback(std::function<void(SearchDetailWidget& w)> callback)
{
    this->returnPressedCallback_ = callback;
    return 0;
}

void SearchDetailWidget::initUI(QWidget *parent)
{
    ObjGuard g(2);
    this->searchLineEdit_ = CreateQWidget<QLineEdit>(g,"SearchDetailWidget_searchLineEdit",parent);
    this->searchResultList_ = CreateQWidget<QListWidget>(g,"SearchDetailWidget_searchResultList",parent);

    auto layout = CreateQLayout<QVBoxLayout>(parent);
    layout->addWidget(this->searchLineEdit_);
    layout->addWidget(this->searchResultList_);

    return ;
}

void SearchDetailWidget::returnPressed()
{
    if(this->returnPressedCallback_ != nullptr){
        this->returnPressedCallback_(*this);
    }
}
