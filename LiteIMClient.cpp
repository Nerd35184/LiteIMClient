#include "LiteIMClient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "Util.h"
#include "JsonStructs.h"
#include "qnetworkreply.h"
#include "ContactListItem.h"
#include <QBuffer>

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
    this->removeContactUrl_ = QString::asprintf(REMOVE_CONTANT_URL_FORMAT, hostChar);

    this->mainWidget_.setMenuBtnClickCallback(
        [this](MenuWidget& w,MenuWidget::Btn btn){
            this->menuBtnClickedCallback(w,btn);
        }
        );

    this->mainWidget_.setModifyActionTriggeredCallback([this](MenuWidget::AvatarLabel &lbl){
        this->modifyActionTriggeredCallback(lbl);
    });

    this->mainWidget_.setContactListItemClickedCallback(
        [this](ContactListItem &item){
            this->contactListItemClickedCallback(item);
        });

    this->mainWidget_.setCreateSessBtnClickedCallback(
        [this](ContactDetailWidget& w)
        {
            this->createSessBtnClickedCallback(w);
        });

    this->mainWidget_.setSessListItemClickedCallback(
        [this](SessListItem& w){
            this->sessListItemClickedCallback(w);
        }
        );

    this->searchDetailWidget_.setReturnPressedCallback(
        [this](SearchDetailWidget &w)
        {
            this->searchDetailWidgetReturnPressedCallback(w);
        });

    this->mainWidget_.setShowSearchDetailWidgetBtnClickedCallback([this](SearchWidget& searchWidget){
        this->showSearchDetailWidgetBtnClickedCallback(searchWidget);
    });
    this->userInfowidget_.setSaveBtnClickedCallback(
        [this](UserInfoWidget& userInfowidget)
        {
            this->userInfoWidgetSaveBtnClickedCallback(userInfowidget);
        });

    this->logInWidget_.setlogInBtnClickedCallback(
        [this](LogInWidget &l){
            this->logInBtnClickedCallback(l);
        });

    this->userInfowidget_.setObjectName("UserInfoWidget");
    this->searchDetailWidget_.setObjectName("SearchDetailWidget");
    this->logInWidget_.setObjectName("LogInWidget");


    QObject::connect(&this->webSocket_, &QWebSocket::connected, this, &LiteIMClient::webWocketConnectedCallback);
    QObject::connect(&this->webSocket_, &QWebSocket::disconnected, this, &LiteIMClient::webWocketDisconnectedCallback);
    this->mainWidget_.showItemListWidget(MainWidget::SessionItemList);
    this->logInWidget_.show();
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
            reply->deleteLater();
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
    qDebug("handleLoginResponse webSocket_ open %s", this->webSocketUrl_.toStdString().c_str());
    this->webSocket_.open(QUrl(this->webSocketUrl_));
    this->mainWidget_.show();
    this->logInWidget_.hide();
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
                this->deleteContactCallback(w);
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

void LiteIMClient::menuBtnClickedCallback(MenuWidget &w, MenuWidget::Btn btn)
{
    this->selectMenu(btn);
}

void LiteIMClient::modifyActionTriggeredCallback(MenuWidget::AvatarLabel &lbl)
{
    qDebug("LiteImClient modifyActionTriggeredCallback");
    auto avatar = this->pixmapCache_[this->avatarUrl_];
    if (avatar == nullptr)
    {
        avatar = std::make_shared<QPixmap>();
    }
    int ret = this->userInfowidget_.init(
        this->nickName_,
        this->signature_,
        *avatar);
    if (ret != 0)
    {
        return;
    }
    this->userInfowidget_.show();
    return ;
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

void LiteIMClient::sessListItemClickedCallback(SessListItem &item)
{
    auto sessInfo = this->sessInfos_[item.getSessIdR()];
    if (sessInfo != nullptr)
    {
        this->mainWidget_.selectSessListItem(sessInfo->getSessIdR());
        this->upsertSessDetail(*sessInfo, true);
        return;
    }
    return ;
}

QString LiteIMClient::generateOneToOneChatSessId(const QString &userA, const QString &userB)
{
    if (userA < userB)
    {
        return userA + "_" + userB;
    }
    return userB + "_" + userA;
}

int LiteIMClient::selectMenu(MenuWidget::Btn btn)
{
    this->mainWidget_.selectMenuBtn(btn);
    if(btn == MenuWidget::contactBtn){
        this->mainWidget_.showItemListWidget(MainWidget::ContactItemList);
        this->mainWidget_.showDetailWidget(MainWidget::ContactDetail);
    }else if(btn == MenuWidget::sessBtn){
        this->mainWidget_.showItemListWidget(MainWidget::SessionItemList);
        this->mainWidget_.showDetailWidget(MainWidget::SessDetail);
    }
    return 0;
}

int LiteIMClient::setUserInfo(const QString &nickname, const QString &signature, const QString &avatar)
{
    QJsonObject requestBody;
    requestBody.insert("nickname", nickname);
    requestBody.insert("signature", signature);
    requestBody.insert("avatar", avatar);
    QJsonDocument requestBodyDocument(requestBody);
    QByteArray requestBodyByte = requestBodyDocument.toJson();
    std::map<QString, QString> headers;
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_CONTENT_TYPE, HTTP_HEADER_JSON_CONTENT_TYPE));
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_AUTHORIZATION, this->token_));
    return DoHttpRequestHandleCodeMsgDataResponse(
        this->networkAccessManager_,
        QNetworkAccessManager::Operation::PostOperation,
        this->setUserInfoUrl_,
        headers,
        requestBodyByte,
        [this](const int code, const QString &msg, const QJsonObject &data)
        {
            this->handleSetUserInfoResponse(code, msg, data);
        },
        PrintCodeMsgFormatError,
        PrintHttpError);
}

void LiteIMClient::handleSetUserInfoResponse(const int code, const QString &msg, const QJsonObject &data)
{
    if (code != 0)
    {
        qDebug("LiteImClient handleSetUserInfoResponse 1 %s", msg.toStdString().c_str());
        return;
    }
    SetUserInfoResponse rsp;
    if (!rsp.init(data))
    {
        qDebug("LiteImClient handleSetUserInfoResponse 2");
        return;
    }
    this->nickName_ = rsp.nickname_;
    this->signature_ = rsp.signature_;
    this->avatarUrl_ = rsp.avatar_;
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
    return;
}

void LiteIMClient::handleGetUserInfoByNicknameResponse(
    const int code,
    const QString &msg,
    const QJsonObject &data,
    std::function<void (const GetUserInfoByNicknameResponse &)> callback)
{
    if (code != 0)
    {
        qDebug("LiteImClient handleGetUserInfoByNicknameResponse %s", msg.toStdString().c_str());
        return;
    }
    GetUserInfoByNicknameResponse rsp;
    bool ok = rsp.init(data);
    if (!ok)
    {
        qDebug("LiteImClient handleGetUserInfoByNicknameResponse %s", msg.toStdString().c_str());
        return;
    }
    if (callback != nullptr)
    {
        callback(rsp);
    }
    qDebug("handleGetUserInfoByNicknameResponse 1");
    return;
}

int LiteIMClient::upsertSessDetail(const SessInfo& sessInfo,bool show)
{
    ObjGuard g(1);
    auto sessWidget = CreateQWidget<ChatDetailWidget>(
        g,
        "ChatSessDetailWidget",
        nullptr,
        sessInfo.getSessIdR(),
        sessInfo.getSessNameR(),
        [this](ChatDetailWidget &chatDetailWidget)
        {
            this->sendMsgBtnClickedCallback(chatDetailWidget);
        });
    int ret = this->mainWidget_.upsertSessDetail(sessWidget, true);
    if (ret != 0)
    {
        qDebug("LiteImClient upsertSessDetail upsertSessDetail error");
        return ret;
    }
    return 0;
}

int LiteIMClient::sendTextMsg(const QString &to, ContactType contactType, const QString &text)
{
    QJsonObject requestBody;
    requestBody.insert("to", to);
    requestBody.insert("contactType", contactType);
    requestBody.insert("msgType", LiteIMClient::TextMsgType);
    QJsonObject content;
    content.insert("text", text);
    requestBody.insert("content", content);
    QJsonDocument requestBodyDocument(requestBody);
    QByteArray requestBodyByte = requestBodyDocument.toJson();
    std::map<QString, QString> headers;
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_CONTENT_TYPE, HTTP_HEADER_JSON_CONTENT_TYPE));
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_AUTHORIZATION, this->token_));
    return DoHttpRequestHandleCodeMsgDataResponse(
        this->networkAccessManager_,
        QNetworkAccessManager::Operation::PostOperation,
        this->sendMsgUrl_,
        headers,
        requestBodyByte,
        [this](const int code, const QString &msg, const QJsonObject &data) {
        },
        PrintCodeMsgFormatError,
        PrintHttpError);
}

int LiteIMClient::uploadQPixmap(const QString &suffixName, const QPixmap &data, std::function<void (int, const QString &, const QJsonObject &)> callback)
{
    QByteArray avatarByte;
    QBuffer buff(&avatarByte);
    bool ok = data.save(&buff, suffixName.toStdString().c_str());
    if (!ok)
    {
        return -1;
    }
    return this->uploadData(
        suffixName,
        avatarByte,
        callback);
}

int LiteIMClient::uploadData(const QString &suffixName, const QByteArray &data, std::function<void (int, const QString &, const QJsonObject &)> callback)
{
    QString url = this->uploadDataUrlFmt_ + suffixName;
    std::map<QString, QString> headers;
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_AUTHORIZATION, this->token_));
    return DoHttpRequestHandleCodeMsgDataResponse(
        this->networkAccessManager_,
        QNetworkAccessManager::Operation::PostOperation,
        url,
        headers,
        data,
        [this, callback](int code, const QString msg, const QJsonObject &data)
        {
            callback(code, msg, data);
        },
        PrintCodeMsgFormatError,
        PrintHttpError);
}

int LiteIMClient::getUserInfoByNickname(const QString &nickname, int offset, int limit, std::function<void (const GetUserInfoByNicknameResponse &)> callback)
{
    QJsonObject requestBody;
    requestBody.insert("nickname", nickname);
    requestBody.insert("offset", offset);
    requestBody.insert("limit", limit);
    QJsonDocument requestBodyDocument(requestBody);
    QByteArray requestBodyByte = requestBodyDocument.toJson();
    std::map<QString, QString> headers;
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_CONTENT_TYPE, HTTP_HEADER_JSON_CONTENT_TYPE));
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_AUTHORIZATION, this->token_));
    return DoHttpRequestHandleCodeMsgDataResponse(
        this->networkAccessManager_,
        QNetworkAccessManager::Operation::PostOperation,
        this->getUserInfoByNicknameUrl_,
        headers,
        requestBodyByte,
        [this, callback](int code, const QString &msg, const QJsonObject &data)
        {
            this->handleGetUserInfoByNicknameResponse(code, msg, data, callback);
        },
        PrintCodeMsgFormatError,
        PrintHttpError);
}

int LiteIMClient::addContact(
    const QString &userId, std::function<void (const int, const QString &, const QJsonObject &)> callback)
{
    QJsonObject requestBody;
    requestBody.insert("userId", userId);
    QJsonDocument requestBodyDocument(requestBody);
    QByteArray requestBodyByte = requestBodyDocument.toJson();
    std::map<QString, QString> headers;
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_AUTHORIZATION, this->token_));
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_CONTENT_TYPE, HTTP_HEADER_JSON_CONTENT_TYPE));
    return DoHttpRequestHandleCodeMsgDataResponse(
        this->networkAccessManager_,
        QNetworkAccessManager::Operation::PostOperation,
        this->addContantUrl_,
        headers,
        requestBodyByte,
        [this, callback](const int code, const QString &msg, const QJsonObject &data)
        {
            if (callback != nullptr)
            {
                callback(code, msg, data);
            }
        },
        PrintCodeMsgFormatError,
        PrintHttpError);
}

int LiteIMClient::removeContact(
    const QString &userId,
    std::function<void (const int, const QString &, const QJsonObject &)> callback)
{
    QJsonObject requestBody;
    requestBody.insert("userId", userId);
    QJsonDocument requestBodyDocument(requestBody);
    QByteArray requestBodyByte = requestBodyDocument.toJson();
    std::map<QString, QString> headers;
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_AUTHORIZATION, this->token_));
    headers.insert(std::pair<QString, QString>(HTTP_HEADER_KEY_CONTENT_TYPE, HTTP_HEADER_JSON_CONTENT_TYPE));
    return DoHttpRequestHandleCodeMsgDataResponse(
        this->networkAccessManager_,
        QNetworkAccessManager::Operation::PostOperation,
        this->removeContactUrl_,
        headers,
        requestBodyByte,
        [this, callback](const int code, const QString &msg, const QJsonObject &data)
        {
            if (callback != nullptr)
            {
                callback(code, msg, data);
            }
        },
        PrintCodeMsgFormatError,
        PrintHttpError);
}

void LiteIMClient::createSessBtnClickedCallback(ContactDetailWidget &w)
{
    auto userInfo = this->userInfoCache_[w.getUserIdR()];
    if (userInfo == nullptr)
    {
        qDebug("LiteImClient::createSessBtnClickedCallback get userinfo error");
        return;
    }
    auto avatar = this->pixmapCache_[userInfo->getAvatarUrlR()];
    if (avatar == nullptr)
    {
        avatar = std::make_shared<QPixmap>();
    }

    this->selectMenu(MenuWidget::sessBtn);

    auto sessId = this->generateOneToOneChatSessId(this->userId_, userInfo->getUserIdR());

    auto sessInfo = this->sessInfos_[sessId];
    if (sessInfo != nullptr)
    {
        this->mainWidget_.selectSessListItem(sessId);
        this->upsertSessDetail(*sessInfo, true);
        return;
    }
    sessInfo = std::make_shared<SessInfo>(
        sessId,
        userInfo->getNicknameR(),
        userInfo->getUserIdR(),
        userInfo->getAvatarUrlR());

    this->sessInfos_[sessId] = sessInfo;
    // todo 这里要考虑失败后除了析构，还需要从sessInfos中删除
    ObjGuard g(1);
    auto sessListItem = CreateQWidget<SessListItem>(
        g,
        "SessInfoWidget",
        nullptr,
        sessId,
        sessInfo->getSessNameR(),
        "",
        *avatar);
    sessListItem->setDeleteActionTriggered(
        [this](SessListItem &sess)
        {
            this->deleteSessCallback(sess);
        });
    int ret = this->mainWidget_.upsertSessListItem(sessListItem,true);
    if (ret != 0)
    {
        qDebug("LiteImClient createSessBtnClickedCallback upsertSessInfoItem error");
        return;
    }
    ret = this->upsertSessDetail(*sessInfo, true);
    if (ret != 0)
    {
        qDebug("LiteImClient createSessBtnClickedCallback upsertSessDetail error");
        return;
    }
    return;
}

void LiteIMClient::sendMsgBtnClickedCallback(ChatDetailWidget &w)
{
    auto sessInfo = this->sessInfos_[w.getSessIdR()];
    if (sessInfo == nullptr)
    {
        return;
    }
    int ret = this->sendTextMsg(sessInfo->getToR(), RegularUser, w.getInputStr());
    if (ret != 0)
    {
        qDebug("LiteImClient sendMsgBtnClickedCallback error");
        return;
    }
    w.cleanInputStr();
    return;
}

void LiteIMClient::processTextWebSockMsg(
    const QString &sessId,
    int64_t msgId,
    const QString &senderId,
    const QJsonObject &contentObj)
{
    if (!contentObj.value("text").isString())
    {
        return;
    }
    QString text = contentObj.value("text").toString();

    std::shared_ptr<QPixmap> avatar = nullptr;
    // todo 这里可以考虑把自己的info也放到缓存中,可能更对称一点
    bool left = false;
    const QString *nickname = &this->nickName_;
    const QString *avtarUrl = &this->avatarUrl_;
    if (senderId != this->userId_)
    {
        left = true;
        auto userInfo = this->userInfoCache_[senderId];
        if (userInfo == nullptr)
        {
            qDebug("processTextWebSockMsg userInfo == nullptr");
            return;
        }
        nickname = &userInfo->getNicknameR();
        avtarUrl = &userInfo->getAvatarUrlR();
    }
    avatar = this->pixmapCache_[*avtarUrl];
    if (avatar == nullptr)
    {
        avatar = std::make_shared<QPixmap>();
    }
    ObjGuard g(1);
    auto chatBubble = CreateQWidget<ChatBubbleWidget>(
        g,
        "ChatBubbleWidget",
        nullptr,
        *avatar,
        *nickname,
        text,
        left);

    int ret = this->mainWidget_.addChatBubbleWidget(sessId, chatBubble);
    if (ret != 0)
    {
        qDebug("processTextWebSockMsg addChatBubbleWidget error");
        return;
    }
    return;
}

void LiteIMClient::webSocketTextMessageReceivedCallback(QString message)
{
    qDebug("LiteImClient webSocketTextMessageReceivedCallback %s", message.toStdString().c_str());
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toStdString().data());
    if (jsonDoc.isNull())
    {
        qDebug("LiteImClient webSocketTextMessageReceivedCallback fromJson error");
        return;
    }
    if (!jsonDoc.isObject())
    {
        qDebug("LiteImClient webSocketTextMessageReceivedCallback jsonDoc error");
        return;
    }
    QJsonObject obj = jsonDoc.object();
    if (!obj.value("sender_id").isString())
    {
        return;
    }
    QString senderId = obj.value("sender_id").toString();
    if (!obj.value("sess_id").isString())
    {
        return;
    }
    QString sessId = obj.value("sess_id").toString();
    if (!obj.value("msg_id").isDouble())
    {
        return;
    }
    int64_t msgId = obj.value("msg_id").toInteger();
    if (!obj.value("msg_type").isDouble())
    {
        return;
    }
    int msgType = obj.value("msg_type").toInt();
    if (!obj.value("content").isObject())
    {
        return;
    }
    QJsonObject contentObj = obj.value("content").toObject();
    switch (msgType)
    {
    case TextMsgType:
        this->processTextWebSockMsg(sessId, msgId, senderId, contentObj);
        break;
    default:
        qDebug("not support msg type %d", msgType);
        return;
    }
    return;
}

void LiteIMClient::webWocketConnectedCallback()
{
    qDebug("LiteImClient webWocketConnectedCallback");
    QObject::connect(&this->webSocket_, &QWebSocket::textMessageReceived,
                     this, &LiteIMClient::webSocketTextMessageReceivedCallback);

    QJsonObject requestBody;
    requestBody.insert("token", this->token_);
    QJsonDocument requestBodyDocument(requestBody);
    QByteArray requestBodyByte = requestBodyDocument.toJson();
    QString requestBodyStr(requestBodyByte);
    this->webSocket_.sendTextMessage(requestBodyStr);
    return ;
}

void LiteIMClient::webWocketDisconnectedCallback()
{
    qDebug("LiteImClient webWocketDisconnectedCallback");
    return ;
}

void LiteIMClient::userInfoWidgetSaveBtnClickedCallback(UserInfoWidget &userInfoWidget)
{
    qDebug("userInfoWidgetSaveBtnClickedCallback");
    QString nickname = userInfoWidget.getNickname();
    if (nickname == "")
    {
        nickname = this->nickName_;
    }
    QString signature = userInfoWidget.getSignatrue();
    if (signature == "")
    {
        signature = this->signature_;
    }
    bool avatarChanged = userInfoWidget.getAvatarChanged();
    if (avatarChanged)
    {
        int ret = this->uploadQPixmap(
            UPLOAD_AVATAR_FMT,
            userInfoWidget.getAvatar(),
            [this, nickname, signature](int code, const QString &msg, const QJsonObject &data)
            {
                if (!data.value("url").isString())
                {
                    qDebug("userInfoWidgetSaveBtnClickedCallback error");
                    return;
                };
                this->setUserInfo(nickname, signature, data.value("url").toString());
            });
        if (ret != 0)
        {
            qDebug("LiteImClient uploadQPixmap");
            return;
        }
        return;
    }

    if (nickname != this->nickName_ || signature != this->signature_)
    {
        int ret = this->setUserInfo(nickname, signature, this->avatarUrl_);
        if (ret != 0)
        {
            qDebug("LiteImClient userInfoWidgetSaveBtnClickedCallback");
            return;
        }
    }
    return ;
}

void LiteIMClient::showSearchDetailWidgetBtnClickedCallback(SearchWidget& searchWidget)
{
    qDebug("setAddContantWidgetShowBtnClickedCallback");
    this->searchDetailWidget_.init();
    this->searchDetailWidget_.show();
}

void LiteIMClient::searchDetailWidgetReturnPressedCallback(SearchDetailWidget &w)
{
    w.initSearchResultList();
    this->getUserInfoByNickname(
        w.getSearchLineText(),
        0,
        65535,
        [this](const GetUserInfoByNicknameResponse &rsp)
        {
            ObjGuard g(rsp.count_);
            for (auto &item : rsp.items_)
            {
                auto w = CreateQWidget<SearchResultItemWidget>(
                    g,
                    "SearchResultItemWidget",
                    nullptr,
                    item.userId_,
                    QPixmap(),
                    item.nickname_,
                    item.signature_,
                    [this](SearchResultItemWidget &w)
                    {
                        this->addContactBtnClickedCallback(w);
                    });
                int ret = this->searchDetailWidget_.addSearchResultListItem(w);
                if (ret != 0)
                {
                    return;
                }
                const QString userId = item.userId_;
                ret = this->getQPixmap(
                    item.avatar_,
                    [this, userId](const QPixmap &pixmap)
                    {
                        this->searchDetailWidget_.setSearchResultListItemAvatar(userId, pixmap);
                    },
                    PrintHttpError);
                if (ret != 0)
                {
                    return;
                }
            }
        });
    return;
}

void LiteIMClient::addContactBtnClickedCallback(SearchResultItemWidget &w)
{
    QString userId = w.getUserIdR();
    this->addContact(
        userId,
        [this, userId](const int code, const QString &msg, const QJsonObject &data)
        {
            if (code != 0)
            {
                qDebug("addContactBtnClickedCallback AddContantSearchResultItemWidget");
                return;
            }
            qDebug("LiteImClient addContactBtnClickedCallback");
            this->searchDetailWidget_.setAddContantBtnDisable(userId);
        });
}

void LiteIMClient::deleteSessCallback(SessListItem &sessInfoWidget)
{
    QString sessID = sessInfoWidget.getSessIdR();
    int ret = this->mainWidget_.removeSess(sessID);
    if (ret != 0)
    {
        return;
    }
    this->sessInfos_.erase(sessID);
    return;
}

void LiteIMClient::deleteContactCallback(ContactListItem &contactInfoWidet)
{
    QString userId = contactInfoWidet.getUserIdR();
    this->removeContact(
        contactInfoWidet.getUserIdR(),
        [this,userId](const int code, const QString &, const QJsonObject &){
            if(code !=0){
                return ;
            }
            int ret = this->mainWidget_.removeContact(userId);
            if (ret != 0)
            {
                qDebug("LiteImClient deleteContactCallback removeContact error");
                return;
            }
            this->userInfoCache_.erase(userId);
        }
        );
    return;
}

void LiteIMClient::logInBtnClickedCallback(LogInWidget &l)
{
    const QString username = l.getUsername();
    const QString password = l.getPassword();
    this->logIn(username,password);
}
