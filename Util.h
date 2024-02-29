#ifndef UTIL_H
#define UTIL_H

#include <QNetworkAccessManager>
#include "ObjGuard.h"
#include <QListWidget>
#include <QSplitter>

#define HTTP_HEADER_KEY_CONTENT_TYPE ("Content-type")
#define HTTP_HEADER_KEY_AUTHORIZATION ("Authorization")

#define HTTP_HEADER_JSON_CONTENT_TYPE ("application/json")

#define GET_LIST_WIDGET_ITEM_NOT_FOUND_ERROR (-2)

template <class T, class... Args>
T *CreateQObject(ObjGuard &guard, const QString &objName, Args... args)
{
    auto obj = new T(std::forward<Args>(args)...);
    guard.addObj(obj);
    return obj;
}

template <class T, class... Args>
T *CreateQWidget(ObjGuard &guard, const QString &objName, Args... args)
{
    auto obj = new T(std::forward<Args>(args)...);
    obj->setAttribute(Qt::WA_StyledBackground);
    obj->setObjectName(objName);
    guard.addObj(obj);
    return obj;
}

template <class T>
T *CreateQLayout(QWidget *parent)
{
    auto layout = new T(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    return layout;
};

template <class T>
T *CreateQSplitter(QWidget *parent)
{
    auto splitter = new QSplitter(parent);
    splitter->setHandleWidth(0);
    return splitter;
};

template <class T>
int GetListWidgetItem(
    QListWidget *listWidget,
    std::function<bool(T *)> comparator,
    QListWidgetItem* &itemResult,
    int& rowResult)
{
    for (int i = 0; i < listWidget->count(); i++)
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
        auto widget = qobject_cast<T *>(itemWidget);
        if (widget == nullptr)
        {
            return -1;
        }
        if (!comparator(widget))
        {
            continue;
        }
        itemResult = item;
        rowResult = i;
        return 0;
    }
    return GET_LIST_WIDGET_ITEM_NOT_FOUND_ERROR;
}

template <class T>
int TakeListWidgetItem(
    QListWidget *listWidget,
    std::function<bool(T *)> comparator)
{

    QListWidgetItem *item = nullptr;
    int row = 0;
    int ret = GetListWidgetItem<T>(
        listWidget,
        comparator,
        &item,
        &row);

    if (ret != 0 && ret != GET_LIST_WIDGET_ITEM_NOT_FOUND_ERROR)
    {
        return -1;
    }
    if (ret == GET_LIST_WIDGET_ITEM_NOT_FOUND_ERROR)
    {
        return ret;
    }
    listWidget->takeItem(row);
    return 0;
}


template <class T>
int AddListItem(QListWidget *listWidget, T *t,int w,int h)
{
    if (listWidget == nullptr || t == nullptr)
    {
        return -1;
    }
    QListWidgetItem *item = new QListWidgetItem();
    item->setFlags(Qt::NoItemFlags); // 设置item的标志
    item->setFlags(Qt::ItemIsEnabled);
    item->setSizeHint(QSize(w, h)); // 设置item的大小
    listWidget->addItem(item);
    listWidget->setItemWidget(item, t);
    return 0;
}

void PrintHttpError(QNetworkReply *reply);

void PrintCodeMsgFormatError(const QByteArray &responseBody);

int DoHttpRequest(
    QNetworkAccessManager& manager,
    const QNetworkAccessManager::Operation method,
    const QString &urlStr,
    const std::map<QString, QString> &headers,
    const QByteArray &body,
    const std::function<void(QNetworkReply *)> responseHandleFunc,
    const std::function<void(QNetworkReply *)> errorHandleFunc);


void DoHttpRequestHandleCodeMsgDataResponseCallback(
    QNetworkReply *reply,
    const std::function<void(const int code,const QString& msg,const QJsonObject& data)> responseHandleFunc,
    const std::function<void(QByteArray&)> fmtErrorHandleFunc);

int DoHttpRequestHandleCodeMsgDataResponse(
    QNetworkAccessManager& manager,
    const QNetworkAccessManager::Operation method,
    const QString &urlStr,
    const std::map<QString, QString> &headers,
    const QByteArray &body,
    const std::function<void(const int code,const QString& msg,const QJsonObject& data)> responseHandleFunc,
    const std::function<void(QByteArray&)> fmtErrorHandleFunc = nullptr,
    const std::function<void(QNetworkReply *)> errorHandleFunc = nullptr);

int GetFileByHttp(
    QNetworkAccessManager& manager,
    const QString &url,
    const std::map<QString, QString> &headers,
    std::function<void(QNetworkReply *)> callback,
    std::function<void(QNetworkReply *)> errorCallback);


#endif // UTIL_H
