#include "SearchWidget.h"
#include "Util.h"
#include <QHBoxLayout>

SearchWidget::SearchWidget(QWidget *parent,std::function<void()> showSearchDetailWidgetBtnClickedCallback)
    : QWidget{parent}
{
    this->initUI(this);
    connect(this->showSearchDetailWidgetBtn_,
            &QPushButton::clicked,this,
            &SearchWidget::showSearchDetailWidgetBtnClicked);
    return ;
}

void SearchWidget::initUI(QWidget *parent)
{
    ObjGuard g(2);
    this->searchListLdt_ = CreateQWidget<QLineEdit>(g, "SearchWidget_searchListLdt", parent);
    this->searchListLdt_->setPlaceholderText("搜索");
    this->showSearchDetailWidgetBtn_ = CreateQWidget<QPushButton>(g, "SearchWidget_showSearchDetailWidgetBtn", parent);
    auto layout = CreateQLayout<QHBoxLayout>(parent);
    layout->addWidget(this->searchListLdt_);
    layout->addWidget(this->showSearchDetailWidgetBtn_);

    return ;
}

void SearchWidget::showSearchDetailWidgetBtnClicked(bool)
{
    if(this->showSearchDetailWidgetBtn_ == nullptr){
        return ;
    }
    this->showSearchDetailWidgetBtnClickedCallback_();
    return ;
}

int SearchWidget::setShowSearchDetailWidgetBtnClickedCallback(std::function<void ()> callback)
{
    this->showSearchDetailWidgetBtnClickedCallback_ = callback;
    return 0;
}
