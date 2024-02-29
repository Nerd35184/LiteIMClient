#ifndef CHATDETAILWIDGET_H
#define CHATDETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include "ChatBubbleListWidget.h"
#include <QTextEdit>
#include <QPushButton>

class ChatDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatDetailWidget(
        QWidget *parent,
        const QString &sessId,
        const QString &sessName,
        std::function<void(ChatDetailWidget &)> sendMsgBtnClickedCallback);

    const QString &getSessId() const;
    const QString getInputStr() const;

    void cleanInputStr();
    int addChatBubbleWidget(ChatBubbleWidget *chatBubbleWidget);
    virtual ~ChatDetailWidget()
    {
        qDebug("~ChatDetailWidget");
    }

private:
    QLabel *titleLbl_ = nullptr;
    ChatBubbleListWidget *chatBubbleListWidget_ = nullptr;
    QWidget *toolBarWidget_ = nullptr;
    QTextEdit *inputTextEidt_ = nullptr; // todo 名字修改
    QPushButton *sendMsgBtn_ = nullptr;

private:
    QString sessId_;
    QString to_;
    std::function<void(ChatDetailWidget &w)> sendMsgBtnClickedCallback_ = nullptr;

private:
    void initUI(QWidget *parent);
    void initTitleWidget(QWidget *parent);
    void initSendMsgBtn(QWidget *parent);
    void sendMsgBtnClicked();

signals:
};

#endif // CHATDETAILWIDGET_H
