#ifndef JSONSTRUCT_H
#define JSONSTRUCT_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

class JsonStruct
{
public:
    JsonStruct();
    bool init(const QJsonDocument &doc);
    bool init(const QByteArray &byte);
    bool init(const QJsonObject &obj);
    bool ok();

protected:
    bool ok_;

private:
    virtual bool parseQJsonObject(const QJsonObject &obj) = 0;
};

class CodeMsgData : public JsonStruct
{
public:
    CodeMsgData();
    int code_;
    QString msg_;
    QJsonObject data_;

private:
    virtual bool parseQJsonObject(const QJsonObject &obj);
};

class LoginResponse : public JsonStruct
{
public:
    LoginResponse();
    QString token_;
    QString userId_;
    QString nickname_;
    QString avatar_;
    QString signature_;

private:
    virtual bool parseQJsonObject(const QJsonObject &obj);
};

class GetUserInfoResponse : public JsonStruct
{
public:
    GetUserInfoResponse();
    QString userId_;
    QString nickname_;
    QString avatar_;
    QString signature_;

private:
    virtual bool parseQJsonObject(const QJsonObject &obj);
};

class GetUserInfoByNicknameResponse : public JsonStruct
{
public:
    GetUserInfoByNicknameResponse();
    int count_;
    std::vector<GetUserInfoResponse> items_;

private:
    virtual bool parseQJsonObject(const QJsonObject &obj);
};

class ContactListResponseItem : public JsonStruct
{
public:
    ContactListResponseItem();
    QString userId_;
    QString nickname_;
    QString avatar_;
    QString signature_;
    int contactType_;

private:
    virtual bool parseQJsonObject(const QJsonObject &obj);
};

class GetContactListResponse : public JsonStruct
{
public:
    GetContactListResponse();
    int count_;
    std::vector<ContactListResponseItem> items_;

private:
    virtual bool parseQJsonObject(const QJsonObject &obj);
};

class SetUserInfoResponse : public JsonStruct
{
public:
    SetUserInfoResponse();
    QString nickname_;
    QString avatar_;
    QString signature_;

private:
    virtual bool parseQJsonObject(const QJsonObject &obj);
};

#endif // JSONSTRUCT_H
