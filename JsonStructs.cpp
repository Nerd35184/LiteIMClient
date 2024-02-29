#include "JsonStructs.h"
#include <QJsonArray>

JsonStruct::JsonStruct() : ok_(false)
{
}

bool JsonStruct::init(const QJsonObject &obj)
{
    this->ok_ = false;
    this->ok_ = this->parseQJsonObject(obj);
    return this->ok_;
}

bool JsonStruct::init(const QJsonDocument &doc)
{
    this->ok_ = false;
    if (doc.isNull())
    {
        return false;
    }
    if (!doc.isObject())
    {
        return false;
    }
    QJsonObject obj = doc.object();
    return this->init(obj);
}

bool JsonStruct::init(const QByteArray &byte)
{
    this->ok_ = false;
    QJsonParseError jsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError)
    {
        return false;
    }
    return this->init(doc);
}

bool JsonStruct::ok()
{
    return this->ok_;
}

CodeMsgData::CodeMsgData()
{
}

bool CodeMsgData::parseQJsonObject(const QJsonObject &obj)
{
    if (!obj.contains("code"))
    {
        return false;
    }
    QJsonValue code = obj.value("code");
    if (!code.isDouble())
    {
        return false;
    }
    QJsonValue msg = obj.value("msg");
    if (!msg.isString())
    {
        return false;
    }
    QJsonValue data = obj.value("data");
    if (!data.isObject() && !data.isNull())
    {
        return false;
    }
    this->code_ = code.toInt();
    this->msg_ = msg.toString();
    this->data_ = data.toObject();
    return true;
}

GetUserInfoResponse::GetUserInfoResponse()
{
}

bool GetUserInfoResponse::parseQJsonObject(const QJsonObject &obj)
{
    QJsonValue userId = obj.value("userId");
    if (!userId.isString())
    {
        return false;
    }
    QJsonValue userNickname = obj.value("nickname");
    if (!userNickname.isString())
    {
        return false;
    }
    QJsonValue userSignature = obj.value("signature");
    if (!userSignature.isString())
    {
        return false;
    }
    QJsonValue userAvatar = obj.value("avatar");
    if (!userAvatar.isString())
    {
        return false;
    }
    this->userId_ = userId.toString();
    this->nickname_ = userNickname.toString();
    this->signature_ = userSignature.toString();
    this->avatar_ = userAvatar.toString();
    return true;
}

GetUserInfoByNicknameResponse::GetUserInfoByNicknameResponse()
{
}

bool GetUserInfoByNicknameResponse::parseQJsonObject(const QJsonObject &obj)
{
    QJsonValue count = obj.value("count");
    if (!count.isDouble())
    {
        return false;
    }
    QJsonValue items = obj.value("items");
    if (!items.isArray() && !items.isNull())
    {
        return false;
    }
    if (items.isNull())
    {
        return true;
    }
    QJsonArray itemArray = items.toArray();
    this->items_.resize(itemArray.count());
    for (int i = 0; i < this->items_.size(); i++)
    {
        QJsonValue itemJsonValue = itemArray[i];
        if (!itemJsonValue.isObject())
        {
            return false;
        }
        bool ok = this->items_[i].init(itemJsonValue.toObject());
        if (!ok)
        {
            return false;
        }
    }
    return true;
}

LoginResponse::LoginResponse()
{
}

bool LoginResponse::parseQJsonObject(const QJsonObject &obj)
{
    QJsonValue token = obj.value("token");
    if (!token.isString())
    {
        return false;
    }
    QJsonValue userId = obj.value("userId");
    if (!userId.isString())
    {
        return false;
    }
    QJsonValue userNickname = obj.value("nickname");
    if (!userNickname.isString())
    {
        return false;
    }
    QJsonValue userSignature = obj.value("signature");
    if (!userSignature.isString())
    {
        return false;
    }

    QJsonValue userAvatar = obj.value("avatar");
    if (!userAvatar.isString())
    {
        return false;
    }
    this->userId_ = userId.toString();
    this->token_ = token.toString();
    this->nickname_ = userNickname.toString();
    this->signature_ = userSignature.toString();
    this->avatar_ = userAvatar.toString();
    return true;
}

ContactListResponseItem::ContactListResponseItem()
{
}

bool ContactListResponseItem::parseQJsonObject(const QJsonObject &obj)
{
    QJsonValue userId = obj.value("userId");
    if (!userId.isString())
    {
        qDebug("parseQJsonObject 6");
        return false;
    }
    QJsonValue userNickname = obj.value("nickname");
    if (!userNickname.isString())
    {
        qDebug("parseQJsonObject 7");
        return false;
    }
    QJsonValue userSignature = obj.value("signature");
    if (!userSignature.isString())
    {
        qDebug("parseQJsonObject 8");
        return false;
    }

    QJsonValue userAvatar = obj.value("avatar");
    if (!userAvatar.isString())
    {
        qDebug("parseQJsonObject 9");
        return false;
    }
    QJsonValue contactType = obj.value("contactType");
    if (!contactType.isDouble())
    {
        qDebug("parseQJsonObject 10");
        return false;
    }
    this->userId_ = userId.toString();
    this->nickname_ = userNickname.toString();
    this->signature_ = userSignature.toString();
    this->avatar_ = userAvatar.toString();
    this->contactType_ = contactType.toInt();
    return true;
}

GetContactListResponse::GetContactListResponse()
{
}

bool GetContactListResponse::parseQJsonObject(const QJsonObject &obj)
{
    QJsonValue count = obj.value("count");
    if (!count.isDouble())
    {
        qDebug("parseQJsonObject 1");
        return false;
    }
    QJsonValue items = obj.value("items");
    if (!items.isArray() && !items.isNull())
    {
        qDebug("parseQJsonObject 2");
        return false;
    }
    if (items.isNull())
    {
        qDebug("parseQJsonObject 3");
        return true;
    }
    QJsonArray itemArray = items.toArray();
    this->items_.resize(itemArray.count());
    for (int i = 0; i < this->items_.size(); i++)
    {
        QJsonValue itemJsonValue = itemArray[i];
        if (!itemJsonValue.isObject())
        {
            qDebug("parseQJsonObject 4");
            return false;
        }
        bool ok = this->items_[i].init(itemJsonValue.toObject());
        if (!ok)
        {
            qDebug("parseQJsonObject 5");
            return false;
        }
    }
    return true;
}

SetUserInfoResponse::SetUserInfoResponse()
{
}

bool SetUserInfoResponse::parseQJsonObject(const QJsonObject &obj)
{
    QJsonValue userNickname = obj.value("nickname");
    if (!userNickname.isString())
    {
        return false;
    }
    QJsonValue userSignature = obj.value("signature");
    if (!userSignature.isString())
    {
        return false;
    }

    QJsonValue userAvatar = obj.value("avatar");
    if (!userAvatar.isString())
    {
        return false;
    }
    this->nickname_ = userNickname.toString();
    this->signature_ = userSignature.toString();
    this->avatar_ = userAvatar.toString();
    return true;
}
