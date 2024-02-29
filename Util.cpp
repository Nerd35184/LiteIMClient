#include "Util.h"
#include <QJsonParseError>
#include <QNetworkReply>
#include "JsonStructs.h"

void HandleHttpResponse(QNetworkReply *rsp,const std::function<void(QNetworkReply *)> callback)
{
    Defer d([rsp](){
        rsp->deleteLater();
    });

    if(callback!=nullptr){
        callback(rsp);
    }
}

int DoHttpRequest(
    QNetworkAccessManager &manager,
    const QNetworkAccessManager::Operation method,
    const QString &urlStr,
    const std::map<QString, QString> &headers,
    const QByteArray &body,
    const std::function<void(QNetworkReply *)> responseHandleFunc,
    const std::function<void(QNetworkReply *)> errorHandleFunc)
{
    qDebug("DoHttpRequest %s", urlStr.toStdString().c_str());
    if (responseHandleFunc == nullptr || errorHandleFunc == nullptr)
    {
        return -1;
    }
    if (method != QNetworkAccessManager::Operation::GetOperation &&
        method != QNetworkAccessManager::Operation::PostOperation)
    {
        return -1;
    }

    QUrl url(urlStr);
    QNetworkRequest request;
    request.setUrl(url);
    for (auto &item : headers)
    {
        request.setRawHeader(item.first.toLatin1(), item.second.toLatin1());
    }
    QNetworkReply *reply = nullptr;
    if (method == QNetworkAccessManager::Operation::GetOperation)
    {
        reply = manager.get(request);
    }
    else if (method == QNetworkAccessManager::Operation::PostOperation)
    {
        reply = manager.post(request, body);
    }
    QObject::connect(
        reply, &QNetworkReply::finished, [reply, responseHandleFunc]()
        {
            HandleHttpResponse(reply,responseHandleFunc);
        });

    QObject::connect(
        reply, &QNetworkReply::errorOccurred, [reply, errorHandleFunc]()
        {
            HandleHttpResponse(reply,errorHandleFunc);
        });
    return 0;
}
void PrintHttpError(QNetworkReply *reply) {
    if (reply == nullptr) {
        return;
    }
    qDebug("LiteImClient::printHttpError %d %s", reply->error(),
           reply->errorString().toStdString().c_str());
    return;
}

void PrintCodeMsgFormatError(const QByteArray &responseBody)
{
    qDebug("LiteImClient::printCodeMsgFormatError %s", responseBody.toStdString().c_str());
    return;
}

int GetFileByHttp(QNetworkAccessManager &manager, const QString &url, const std::map<QString, QString> &headers, std::function<void (QNetworkReply *)> callback, std::function<void (QNetworkReply *)> errorCallback)
{
    return DoHttpRequest(
        manager,
        QNetworkAccessManager::Operation::GetOperation,
        url,
        headers,
        nullptr,
        callback,
        errorCallback);
}

void DoHttpRequestHandleCodeMsgDataResponseCallback(QNetworkReply *reply, const std::function<void (const int, const QString &,const QJsonObject&)> responseHandleFunc, const std::function<void (QByteArray &)> fmtErrorHandleFunc){
    if (reply == nullptr)
    {
        qDebug("doHttpRequestHandleCodeMsgDataResponse handleHttpCodeMsgDataResponse");
        return;
    }
    QJsonParseError jsonParseError;
    QByteArray responseByte = reply->readAll();
    CodeMsgData codeMsgData;
    bool ok = codeMsgData.init(responseByte);
    if(!ok){
        if(fmtErrorHandleFunc!=nullptr){
            fmtErrorHandleFunc(responseByte);
        }
        return;
    }
    if(responseHandleFunc == nullptr){
        return ;
    }
    responseHandleFunc(codeMsgData.code_,codeMsgData.msg_,codeMsgData.data_);
}

int DoHttpRequestHandleCodeMsgDataResponse(QNetworkAccessManager &manager, const QNetworkAccessManager::Operation method, const QString &urlStr, const std::map<QString, QString> &headers, const QByteArray &body, const std::function<void (const int, const QString &, const QJsonObject &)> responseHandleFunc, const std::function<void (QByteArray &)> fmtErrorHandleFunc, const std::function<void (QNetworkReply *)> errorHandleFunc)
{
    return DoHttpRequest(
        manager,
        method,
        urlStr,
        headers,
        body,
        [responseHandleFunc,fmtErrorHandleFunc](QNetworkReply *reply){
            DoHttpRequestHandleCodeMsgDataResponseCallback(reply,responseHandleFunc,fmtErrorHandleFunc);
        },
        errorHandleFunc
        );
}
