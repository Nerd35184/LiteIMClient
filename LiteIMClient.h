#ifndef LITEIMCLIENT_H
#define LITEIMCLIENT_H

#include "MainWidget.h"
#include <QString>
#include <QPixmap>
#include <map>
#include "UserInfo.h"
#include "UserInfoWidget.h"
#include <QNetworkAccessManager>
#include <QWebSocket>
#include "ContactListItem.h"

#define LOGIN_URL_FORMAT ("http://%s/api/login")
#define GET_CONTACT_LIST_URL_FORMAT ("http://%s/api/get_contact_list")
#define SEND_MSG_URL_FORMAT ("http://%s/api/send_msg")
#define GET_USER_INFO_BY_NICKNAME_FORMAT ("http://%s/api/get_user_info_by_nickname")
#define SET_USER_INFO_URL_FORMAT ("http://%s/api/set_user_info")
#define UPLOAD_DATA_FORMAT ("http://%s/static/upload/")
#define ADD_CONTANT_URL_FORMAT ("http://%s/api/add_contact")
#define WEB_SOCKET_URL_FORMAT ("ws://%s/ws")

class LiteIMClient
{

public:
    enum ContactType{
        SystemUser = 1,
        RegularUser = 2
    };

public:
    LiteIMClient(const QString& host);
    int logIn(const QString& username,const QString& password);
    int getContactList(int offset,int limit);
    int getQPixmap(const QString &url,
                   std::function<void(const QPixmap &)> callback,
                   std::function<void(QNetworkReply *reply)> errorCallback);
private:
    MainWidget mainWidget_;
    // UserInfoWidget userInfowidget_;
    // AddContantWidget addContantWidget_;
    QString host_;
    QString logInUrl_;
    QString getContactListUrl_;
    QString sendMsgUrl_;
    QString uploadDataUrlFmt_;
    QString setUserInfoUrl_;
    QString getUserInfoByNicknameUrl_;
    QString addContantUrl_;
    QString webSocketUrl_;

    QString token_;
    QNetworkAccessManager networkAccessManager_;
    QWebSocket webSocket_;
    QString userId_;
    QString signature_;
    QString avatarUrl_;
    QString nickName_;

    std::map<QString,std::shared_ptr<UserInfo>> userInfoCache_;
    std::map<QString,std::shared_ptr<QPixmap>> pixmapCache_;

private:
    void handleLogInResponse(
        const int code,
        const QString &msg,
        const QJsonObject &data);
    void handleGetContactListResponse(
        const int code,
        const QString &msg,
        const QJsonObject &data);

    void menuWBtnClickedCallback(MenuWidget&,MenuWidget::Btn);
    void contactListItemDeleteActCallback(ContactListItem& item);
    void contactListItemClickedCallback(ContactListItem& item);
};

#endif // LITEIMCLIENT_H
