#include "LiteIMClient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "Util.h"
#include "JsonStructs.h"
#include "qnetworkreply.h"
#include "ContactListItem.h"

LiteIMClient::LiteIMClient(const QString &host)
{
    this->host_ = host;
    const char* hostChar = host.toStdString().c_str();
    this->logInUrl_ = QString::asprintf(LOGIN_URL_FORMAT, hostChar);
    this->getContactListUrl_ = QString::asprintf(GET_CONTACT_LIST_URL_FORMAT, hostChar);
    this->sendMsgUrl_ = QString::asprintf(SEND_MSG_URL_FORMAT, hostChar);
    this->webSocketUrl_ = QString::asprintf(WEB_SOCKET_URL_FORMAT, hostChar);
    this->setUserInfoUrl_ = QString::asprintf(SET_USER_INFO_URL_FORMAT, hostChar);
    this->uploadDataUrlFmt_ = QString::asprintf(UPLOAD_DATA_FORMAT, hostChar);
    this->getUserInfoByNicknameUrl_ = QString::asprintf(GET_USER_INFO_BY_NICKNAME_FORMAT, hostChar);
    this->addContantUrl_ = QString::asprintf(ADD_CONTANT_URL_FORMAT, hostChar);

    this->mainWidget_.setMenuBtnClickCallback(
        [this](MenuWidget& w,MenuWidget::Btn btn){
            this->menuWBtnClickedCallback(w,btn);
        }
        );

    this->mainWidget_.setContactListItemClickedCallback([this](ContactListItem &item){
        this->contactListItemClickedCallback(item);
    });


    this->mainWidget_.showItemListWidget(MainWidget::SessionItemList);
    this->mainWidget_.hide();
}

int LiteIMClient::getQPixmap(const QString &url,
                             std::function<void(const QPixmap &)> callback,
                             std::function<void(QNetworkReply *reply)> errorCallback)
{

    if (callback == nullptr || errorCallback == nullptr)
    {
        return -1;
    }
    auto iter = this->pixmapCache_.find(url);
    if (iter != this->pixmapCache_.end())
    {
        callback(*iter->second);
        return 0;
    }
    std::map<QString, QString> headers;
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_CONTENT_TYPE, HTTP_HEADER_JSON_CONTENT_TYPE));
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_AUTHORIZATION, this->token_));
    return GetFileByHttp(
        this->networkAccessManager_,
        url,
        headers,
        [this, url, callback](QNetworkReply *reply)
        {
            QByteArray body = reply->readAll();
            auto iconPtr = std::make_shared<QPixmap>();
            auto ok = iconPtr->loadFromData(body);
            if (!ok)
            {
                qDebug("getQPixmapByHttp loadFromData error");
                return;
            }
            this->pixmapCache_[url] = iconPtr;
            callback(*iconPtr);
        },
        errorCallback);
}

int LiteIMClient::logIn(const QString &username, const QString &password)
{
    QJsonObject requestBody;
    requestBody.insert("username", username);
    requestBody.insert("password", password);
    QJsonDocument requestBodyDocument(requestBody);
    QByteArray requestBodyByte = requestBodyDocument.toJson();
    std::map<QString, QString> headers;
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_CONTENT_TYPE, HTTP_HEADER_JSON_CONTENT_TYPE));
    return DoHttpRequestHandleCodeMsgDataResponse(
        this->networkAccessManager_,
        QNetworkAccessManager::Operation::PostOperation,
        this->logInUrl_,
        headers,
        requestBodyByte,
        [this](const int code, const QString &msg, const QJsonObject &data)
        {
            this->handleLogInResponse(code, msg, data);
        },
        PrintCodeMsgFormatError,
        PrintHttpError);
}

int LiteIMClient::getContactList(int offset, int limit)
{
    QJsonObject requestBody;
    requestBody.insert("offset", offset);
    requestBody.insert("limit", limit);
    // todo 暂时只支持一种类型
    requestBody.insert("contactType", RegularUser);
    QJsonDocument requestBodyDocument(requestBody);
    QByteArray requestBodyByte = requestBodyDocument.toJson();
    std::map<QString, QString> headers;
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_CONTENT_TYPE, HTTP_HEADER_JSON_CONTENT_TYPE));
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_AUTHORIZATION, this->token_));
    return DoHttpRequestHandleCodeMsgDataResponse(
        this->networkAccessManager_,
        QNetworkAccessManager::Operation::PostOperation,
        this->getContactListUrl_,
        headers,
        requestBodyByte,
        [this](const int code, const QString &msg, const QJsonObject &data)
        {
            this->handleGetContactListResponse(code, msg, data);
        },
        PrintCodeMsgFormatError,
        PrintHttpError);
}

void LiteIMClient::handleLogInResponse(const int code, const QString &msg, const QJsonObject &data)
{
    if(code != 0){
        qDebug("LiteIMClient handleLogInResponse 1 error %d %s",code ,msg.toStdString().c_str());
        return ;
    }
    LoginResponse r;
    bool ok = r.init(data);
    if (!ok){
        qDebug("LiteIMClient handleLogInResponse 2 error");
        return ;
    }
    this->userId_ =r.userId_;
    this->nickName_ = r.nickname_;
    this->signature_ = r.signature_;
    this->token_ = r.token_;
    this->signature_ =r.signature_;
    this->avatarUrl_ = r.avatar_;


    int ret = this->getQPixmap(
        this->avatarUrl_,
        [this](const QPixmap &pixmap)
        {
            this->mainWidget_.setMenuWidgetAvatar(pixmap);
        },
        PrintHttpError);
    if (ret != 0)
    {
        qDebug("handleLoginResponse get getQPixmap error");
        return;
    }
    ret = this->getContactList(0, 65535);
    if (ret != 0)
    {
        qDebug("handleLoginResponse get getContactList error");
        return;
    }
    // qDebug("handleLoginResponse webSocket_ open %s", this->webSocketUrl_.toStdString().c_str());
    // this->webSocket_.open(QUrl(this->webSocketUrl_));

    this->mainWidget_.show();
    return ;
}

void LiteIMClient::handleGetContactListResponse(const int code, const QString &msg, const QJsonObject &data)
{
    if(code != 0){
        qDebug("LiteIMClient handleGetContactListResponse err:%s",msg.toStdString().c_str());
        return ;
    }
    GetContactListResponse r;
    bool ok = r.init(data);
    if(!ok){
        qDebug("LiteIMClient handleGetContactListResponse init err");
        return ;
    }
    ObjGuard g(r.count_);
    for(auto& item:r.items_){

        auto userInfo = std::make_shared<UserInfo>(
            item.userId_,
            item.nickname_,
            item.signature_,
            item.avatar_
            );
        this->userInfoCache_[item.userId_] = userInfo;

        ContactListItem* w = CreateQWidget<ContactListItem>(
            g,
            "ContactListItem",
            nullptr,
            item.userId_,
            item.nickname_,
            [this](ContactListItem &w){
                this->contactListItemDeleteActCallback(w);
            });
        int ret = this->mainWidget_.addContactListItem(w);
        if(ret != 0){
            qDebug("LiteIMClient handleGetContactListResponse addContactListItem err");
            return ;
        }
        QString userId = item.userId_;
        this->getQPixmap(
            item.avatar_,
            [this,userId](const QPixmap &pixmap)
            {
                this->mainWidget_.setContactListItemAvatar(userId,pixmap);
            },
            PrintHttpError);
    }
    return ;
}

void LiteIMClient::menuWBtnClickedCallback(MenuWidget &w, MenuWidget::Btn btn)
{
    this->mainWidget_.selectMenuBtn(btn);
    if(btn == MenuWidget::contactBtn){
        this->mainWidget_.showItemListWidget(MainWidget::ContactItemList);
        this->mainWidget_.showDetailWidget(MainWidget::ContactDetail);
    }else if(btn == MenuWidget::sessBtn){
        this->mainWidget_.showItemListWidget(MainWidget::SessionItemList);
        this->mainWidget_.showDetailWidget(MainWidget::SessDetail);
    }
}

void LiteIMClient::contactListItemDeleteActCallback(ContactListItem &item)
{
    qDebug("LiteIMClient contactListItemDeleteActCallback");
}

void LiteIMClient::contactListItemClickedCallback(ContactListItem &item)
{
    auto userInfo = this->userInfoCache_[item.getUserIdR()];
    if(userInfo == nullptr){
        qDebug("LiteIMClient contactListItemClickedCallback userInfo not found");
        return ;
    }
    auto avatar = this->pixmapCache_[userInfo->getAvatarUrlR()];
    if(avatar == nullptr){
        avatar = std::make_shared<QPixmap>();
    }
    this->mainWidget_.setContactDetail(
        userInfo->getUserIdR(),
        userInfo->getNicknameR(),
        userInfo->getSignatureR(),
        *avatar
        );
    return ;
}
