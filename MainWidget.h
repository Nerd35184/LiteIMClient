#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "MenuWidget.h"
#include "SearchWidget.h"
#include "SessDetailWidget.h"
#include "ContactDetailWidget.h"
#include <QListWidget>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    enum ItemListWidgetEnum{
        contactItemList,
        sessionItemList
    };

    enum ItemDetailWidgetEnum{
        contactDetail,
        sessDetail
    };

public:
    explicit MainWidget(QWidget *parent = nullptr);

    int addContactListItem();
    int addSessListWidget();
    int showItemListWidget(ItemListWidgetEnum i);
    int showDetailWidget(ItemDetailWidgetEnum i);
    int setShowSearchDetailWidgetBtnClickedCallback(
        std::function<void()> showSearchDetailWidgetBtnClickedCallback);
    int setMenuWidgetAvatar(const QPixmap& avatar);

private:
    MenuWidget* menuWidget_;
    SearchWidget* searchWidget_;
    QListWidget* contactListWidget_;
    QListWidget* sessListWidget_;
    QWidget* titleWidget_;
    SessDetailWidget* sessDtlWidget_;
    ContactDetailWidget* cttDtlWidget_;



private:
    void initUI(QWidget *parent);
    void initSearchAndItemListWidget(QWidget *parent);
    void initTitlAndItemDetailWidget(QWidget *parent);
    void showSearchDetailWidgetBtnClicked(bool);

signals:
};

#endif // MAINWIDGET_H
