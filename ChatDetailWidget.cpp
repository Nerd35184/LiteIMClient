#include "ChatDetailWidget.h"
#include <QVBoxLayout>



ChatDetailWidget::ChatDetailWidget(
    QWidget *parent,
    const QString &sessId,
    const QString &sessName,
    std::function<void(ChatDetailWidget &)> sendMsgBtnClickedCallback)
    : QWidget{parent}
{
    this->initUI(this);
    this->sessId_ = sessId;
    this->titleLbl_->setText(sessName);
    this->sendMsgBtnClickedCallback_ = sendMsgBtnClickedCallback;
    connect(this->sendMsgBtn_, &QPushButton::clicked, this, &ChatDetailWidget::sendMsgBtnClicked);
}

const QString &ChatDetailWidget::getSessId() const
{
    return this->sessId_;
}

const QString ChatDetailWidget::getInputStr() const
{
    return this->inputTextEidt_->toPlainText();
}

void ChatDetailWidget::cleanInputStr()
{
    this->inputTextEidt_->setText("");
    return;
}

int ChatDetailWidget::addChatBubbleWidget(ChatBubbleWidget *chatBubbleWidget)
{
    return this->chatBubbleListWidget_->addChatBubbleWidget(chatBubbleWidget);
}

void ChatDetailWidget::initUI(QWidget *parent)
{
    ObjGuard g(5);
    auto titleWidget = CreateQWidget<QWidget>(g, "ChatDetailWidget_titleWidget", parent);
    this->initTitleWidget(titleWidget);

    this->chatBubbleListWidget_ = CreateQWidget<ChatBubbleListWidget>(
        g, "ChatDetailWidget_chatBubbleListWidget", parent);
    this->toolBarWidget_ = CreateQWidget<QWidget>(g, "ChatDetailWidget_toolBarWidget", parent);
    this->inputTextEidt_ = CreateQWidget<QTextEdit>(g, "ChatDetailWidget_inputTextEidt", parent);
    auto sendMsgWidget = CreateQWidget<QWidget>(g, "", parent);
    this->initSendMsgBtn(sendMsgWidget);

    auto splitter = CreateQSplitter<QSplitter>(parent);
    splitter->setOrientation(Qt::Vertical);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(titleWidget);
    // splitter->addWidget(this->chatBubbleList_);
    splitter->addWidget(this->chatBubbleListWidget_);
    splitter->addWidget(this->toolBarWidget_);
    splitter->addWidget(this->inputTextEidt_);
    splitter->addWidget(sendMsgWidget);

    auto layout = CreateQLayout<QVBoxLayout>(parent);
    layout->addWidget(splitter);
    return;
}

void ChatDetailWidget::initTitleWidget(QWidget *parent)
{
    ObjGuard g(1);
    this->titleLbl_ = CreateQWidget<QLabel>(g, "ChatDetailWidget_titleLbl", parent);
    auto layout = CreateQLayout<QHBoxLayout>(parent);
    layout->addWidget(this->titleLbl_);
    layout->setAlignment(Qt::AlignLeft);
    return;
}

void ChatDetailWidget::initSendMsgBtn(QWidget *parent)
{
    ObjGuard g(1);
    this->sendMsgBtn_ = CreateQWidget<QPushButton>(g, "ChatDetailWidget_sendMsgBtn", parent);
    this->sendMsgBtn_->setText("发送");
    auto layout = CreateQLayout<QHBoxLayout>(parent);
    layout->addWidget(this->sendMsgBtn_);
    layout->setAlignment(Qt::AlignRight);
    return;
}

void ChatDetailWidget::sendMsgBtnClicked()
{
    if (this->sendMsgBtnClickedCallback_ == nullptr)
    {
        return;
    }
    this->sendMsgBtnClickedCallback_(*this);
    return;
}
