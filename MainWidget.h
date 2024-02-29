#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "MenuWidget.h"
#include "SearchWidget.h"
#include "SessDetailWidget.h"
#include "ContactDetailWidget.h"
#include "ContactListItem.h"
#include <QListWidget>
#include "SessListItem.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    enum ItemListWidgetEnum{
        ContactItemList,
        SessionItemList
    };

    enum ItemDetailWidgetEnum{
        ContactDetail,
        SessDetail
    };

public:
    explicit MainWidget(QWidget *parent = nullptr);

    int showItemListWidget(ItemListWidgetEnum i);
    int showDetailWidget(ItemDetailWidgetEnum i);
    int setShowSearchDetailWidgetBtnClickedCallback(
        std::function<void()> showSearchDetailWidgetBtnClickedCallback);
    int setMenuWidgetAvatar(const QPixmap& avatar);
    int selectMenuBtn(MenuWidget::Btn btn);
    int setMenuBtnClickCallback(std::function<void(MenuWidget&,MenuWidget::Btn)> callback);
    int clearList(ItemListWidgetEnum i);

    int addContactListItem(ContactListItem* item);
    int setContactListItemAvatar(const QString& userId,const QPixmap& avatar);
    int setContactListItemClickedCallback(std::function<void(ContactListItem& item)> callback);
    int setContactDetail(
        const QString &userId,
        const QString &nickname,
        const QString &signature,
        const QPixmap &avatar);

    int addSessListWidget(SessListItem* item);
    int setSessListItemAvatar(const QString& userId,const QPixmap& avatar);
    int setSessListItemClickedCallback(std::function<void(SessListItem& item)> callback);


private:
    MenuWidget* menuWidget_= nullptr;
    SearchWidget* searchWidget_= nullptr;
    QListWidget* contactListWidget_= nullptr;
    QListWidget* sessListWidget_= nullptr;
    QWidget* titleWidget_ = nullptr;
    SessDetailWidget* sessDtlWidget_= nullptr;
    ContactDetailWidget* cttDtlWidget_ = nullptr;

private:
    std::function<void(ContactListItem& item)> contactListItemClickedCallback_ = nullptr;
    std::function<void(SessListItem& item)> sessListItemClickedCallback_ = nullptr;

private:
    void initUI(QWidget *parent);
    void initSearchAndItemListWidget(QWidget *parent);
    void initTitlAndItemDetailWidget(QWidget *parent);
    void showSearchDetailWidgetBtnClicked(bool);
    void ContactListWidgetClicked(ContactListItem& item);
    void SessListWidgetClicked(SessListItem& item);

    void contactListItemClicked(QListWidgetItem *itemClicked);
    void sessListItemClicked(QListWidgetItem *itemClicked);


    template<class T>
    int ListItemClicked(QListWidget *listWidget, QListWidgetItem *itemClicked,std::function<void(T&)> callback){
        if (listWidget == nullptr)
        {
            qDebug("ItemInfoListWidget ContactListItemClicked");
            return -1;
        }
        for (int i = 0; i < listWidget->count(); i++)
        {
            auto item = listWidget->item(i);
            auto infoWidget = qobject_cast<T *>(listWidget->itemWidget(item));
            if (infoWidget == nullptr)
            {
                qDebug("ItemInfoListWidget::ContactListItemClicked qobject_cast error");
                return -1;
            }
            if (item == itemClicked)
            {
                infoWidget->setStyleSheet("background-color: rgba(200, 198, 197,255);");
                if (callback != nullptr)
                {
                    callback(*infoWidget);
                }
            }
            else
            {
                infoWidget->setStyleSheet("background-color: rgba(230, 230, 230,255);");
            }
        }
        return 0;
    }

signals:
};

#endif // MAINWIDGET_H
