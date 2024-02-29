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
#include "SessInfo.h"
#include "SearchDetailWidget.h"
#include "JsonStructs.h"

#define UPLOAD_AVATAR_FMT ("jpg")
#define LOGIN_URL_FORMAT ("http://%s/api/login")
#define GET_CONTACT_LIST_URL_FORMAT ("http://%s/api/get_contact_list")
#define SEND_MSG_URL_FORMAT ("http://%s/api/send_msg")
#define GET_USER_INFO_BY_NICKNAME_FORMAT ("http://%s/api/get_user_info_by_nickname")
#define SET_USER_INFO_URL_FORMAT ("http://%s/api/set_user_info")
#define UPLOAD_DATA_FORMAT ("http://%s/static/upload/")
#define ADD_CONTANT_URL_FORMAT ("http://%s/api/add_contact")
#define WEB_SOCKET_URL_FORMAT ("ws://%s/ws")

class LiteIMClient:public QObject
{
    Q_OBJECT
public:
    enum ContactType{
        SystemUser = 1,
        RegularUser = 2
    };

    enum MsgType{
        TextMsgType = 1
    };

public:
    LiteIMClient(const QString& host);


    int logIn(const QString& username,const QString& password);

    int getContactList(int offset,int limit);

    int getQPixmap(const QString &url,
                   std::function<void(const QPixmap &)> callback,
                   std::function<void(QNetworkReply *reply)> errorCallback);

    QString generateOneToOneChatSessId(const QString &userA, const QString &userB);

    int selectMenu(MenuWidget::Btn btn);

    int setUserInfo(
        const QString &nickname,
        const QString &signature,
        const QString &avatar);


    int upsertSessDetail(
        const SessInfo& sessInfo,
        bool show);

    int sendTextMsg(
        const QString &to,
        ContactType contactType,
        const QString &text);

    int uploadQPixmap(
        const QString &suffixName,
        const QPixmap &data,
        std::function<void(int, const QString &, const QJsonObject &)> callback);

    int uploadData(const QString &suffixName,
                   const QByteArray &data,
                   std::function<void(int code, const QString &msg, const QJsonObject &data)> callback);

    int getUserInfoByNickname(
        const QString &nickname,
        int offset,
        int limit,
        std::function<void(const GetUserInfoByNicknameResponse &rsp)> callback);

    int addContact(
        const QString &userId,
        std::function<void(const int code, const QString &msg, const QJsonObject &data)>);

private:
    MainWidget mainWidget_;
    UserInfoWidget userInfowidget_;
    SearchDetailWidget searchDetailWidget_;

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
    std::map<QString, std::shared_ptr<SessInfo>> sessInfos_;

private:
    void handleLogInResponse(
        const int code,
        const QString &msg,
        const QJsonObject &data);
    void handleGetContactListResponse(
        const int code,
        const QString &msg,
        const QJsonObject &data);

    void handleSetUserInfoResponse(
        const int code,
        const QString &msg,
        const QJsonObject &data);

    void handleGetUserInfoByNicknameResponse(
        const int code,
        const QString &msg,
        const QJsonObject &data,
        std::function<void(const GetUserInfoByNicknameResponse &rsp)> callback);

    void processTextWebSockMsg(
        const QString &sessId,
        int64_t msgId,
        const QString &senderId,
        const QJsonObject &contentObj);

    void menuBtnClickedCallback(MenuWidget&,MenuWidget::Btn);
    void modifyActionTriggeredCallback(MenuWidget::AvatarLabel& lbl);
    void contactListItemDeleteActCallback(ContactListItem& item);
    void contactListItemClickedCallback(ContactListItem& item);
    void sessListItemClickedCallback(SessListItem& item);
    void createSessBtnClickedCallback(ContactDetailWidget& w);
    void sendMsgBtnClickedCallback(ChatDetailWidget &w);
    void webSocketTextMessageReceivedCallback(QString message);
    void webWocketConnectedCallback();
    void webWocketDisconnectedCallback();
    void userInfoWidgetSaveBtnClickedCallback(UserInfoWidget& userInfoWidget);
    void showSearchDetailWidgetBtnClickedCallback(SearchWidget& searchWidget);
    void searchDetailWidgetReturnPressedCallback(SearchDetailWidget &w);
    void addContactBtnClickedCallback(SearchResultItemWidget &w);
    void deleteSessCallback(SessListItem &sessInfoWidget);
    void deleteContactCallback(ContactListItem &contactInfoWidet);
};

#endif // LITEIMCLIENT_H
