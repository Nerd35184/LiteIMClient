#include "ChatBubbleListWidget.h"
#include <QPainter>
#include <QHBoxLayout>


TextDisplayWidget::TextDisplayWidget(QWidget *parent, const QString &text)
    : QWidget{parent}, text_(text)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    return ;
}

void TextDisplayWidget::paintEvent(QPaintEvent *event)
{
    auto rect = this->rect();
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); // 消锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));

    QColor col(75, 164, 242);
    painter.setBrush(QBrush(col));
    painter.drawRoundedRect(rect, 4, 4);

    QPen penText;
    penText.setColor(QColor(51, 51, 51));
    painter.setPen(penText);
    QTextOption option(this->alignmentFlag | Qt::AlignTop);
    option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    painter.setFont(this->font());
    painter.drawText(rect, this->text_, option);
    return;
}

const QString &TextDisplayWidget::getTextR() const
{
    return this->text_;
}

void TextDisplayWidget::setText(const QString &text)
{
    this->text_ = text;
    return;
}

void TextDisplayWidget::setAlignment(Qt::AlignmentFlag alignmentFlag)
{
    this->alignmentFlag = alignmentFlag;
    return;
}


ChatBubbleWidget::ChatBubbleWidget(
    QWidget *parent,
    const QPixmap &avatar,
    const QString &nickname,
    const QString &content,
    bool left)
    : QWidget{parent}
{
    this->initUI(this, left);
    this->avatarLbl_->setPixmap(avatar);
    this->nicknameLbl_->setText(nickname);
    this->contentWidget_->setText(content);
    return;
}

const QString ChatBubbleWidget::getContentStr() const
{
    return this->contentWidget_->getTextR();
}

const int ChatBubbleWidget::calHeight(int pWidth)
{
    const QString &content = this->contentWidget_->getTextR();
    QTextDocument doc;
    doc.setPlainText(content);
    auto width = pWidth - CHAT_BUBBLE_WIDGET_AVATAR_WIDTH;
    doc.setTextWidth(width);
    doc.adjustSize();
    auto contentHeight = doc.size().height();
    auto textHeight = contentHeight + CHAT_BUBBLE_WIDGET_NICKNAME_HEIGHT;
    if (textHeight < CHAT_BUBBLE_WIDGET_AVATAR_HEIGHT)
    {
        return CHAT_BUBBLE_WIDGET_AVATAR_HEIGHT;
    }
    return textHeight;
}

void ChatBubbleWidget::initUI(QWidget *parent, bool left)
{
    ObjGuard g(2);
    auto avatarWidget = CreateQWidget<QWidget>(g, "", parent);
    this->initAvatarLabel(avatarWidget, left);

    auto nicknameAndContentWidget = CreateQWidget<QWidget>(g, "", parent);
    this->initNicknameAndContentWidget(nicknameAndContentWidget, left);

    auto layout = CreateQLayout<QHBoxLayout>(parent);
    if (left)
    {
        layout->addWidget(avatarWidget);
        layout->addWidget(nicknameAndContentWidget);
        layout->setAlignment(Qt::AlignLeft);
    }
    else
    {
        layout->addWidget(nicknameAndContentWidget);
        layout->addWidget(avatarWidget);
        layout->setAlignment(Qt::AlignRight);
    }
    return;
}

void ChatBubbleWidget::initNicknameAndContentWidget(QWidget *parent, bool left)
{
    ObjGuard g(2);
    this->nicknameLbl_ = CreateQWidget<QLabel>(
        g, "ChatBubbleWidget_nicknameLbl", parent);

    this->contentWidget_ = CreateQWidget<TextDisplayWidget>(g, "", parent, "");

    if (left)
    {
        this->nicknameLbl_->setAlignment(Qt::AlignLeft);
        this->contentWidget_->setAlignment(Qt::AlignLeft);
    }
    else
    {
        this->nicknameLbl_->setAlignment(Qt::AlignRight);
        this->contentWidget_->setAlignment(Qt::AlignRight);
    }

    auto layout = CreateQLayout<QVBoxLayout>(parent);
    layout->addWidget(this->nicknameLbl_);
    layout->addWidget(this->contentWidget_);
    layout->setAlignment(Qt::AlignTop);
    return;
}

void ChatBubbleWidget::initAvatarLabel(QWidget *parent, bool left)
{
    ObjGuard g(1);
    this->avatarLbl_ = CreateQWidget<QLabel>(g, "ChatBubbleWidget_avatarLbl", parent);
    this->avatarLbl_->setScaledContents(true);
    auto layout = CreateQLayout<QVBoxLayout>(parent);
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(this->avatarLbl_);
    return;
}

ChatBubbleListWidget::ChatBubbleListWidget(QWidget *parent)
    : QWidget{parent}
{
    this->initUI(this);
}

void ChatBubbleListWidget::initUI(QWidget *parent)
{
    ObjGuard g(1);
    this->bubbleListWidget_ = CreateQWidget<QListWidget>(g, "ChatBubbleListWidget_bubbleListWidget", parent);
    this->bubbleListWidget_->setFrameShape(QFrame::NoFrame);
    this->bubbleListWidget_->setSpacing(5);
    auto layout = CreateQLayout<QVBoxLayout>(parent);
    layout->addWidget(this->bubbleListWidget_);
    return;
}

int ChatBubbleListWidget::addChatBubbleWidget(ChatBubbleWidget *chatBubbleWidget)
{
    if (chatBubbleWidget == nullptr)
    {
        return -1;
    }
    QListWidgetItem *item = new QListWidgetItem();
    item->setFlags(Qt::NoItemFlags); // 设置item的标志
    item->setFlags(Qt::ItemIsEnabled);
    int width = this->bubbleListWidget_->width();
    auto height = chatBubbleWidget->calHeight(width);
    chatBubbleWidget->setFixedHeight(height);
    item->setSizeHint(QSize(40, height)); // 设置item的大小
    this->bubbleListWidget_->addItem(item);
    this->bubbleListWidget_->setItemWidget(item, chatBubbleWidget);
    return 0;
}

void ChatBubbleListWidget::resizeEvent(QResizeEvent *event)
{
    if (event == nullptr)
    {
        return;
    }
    for (int i = 0; i < this->bubbleListWidget_->count(); i++)
    {
        auto item = this->bubbleListWidget_->item(i);
        auto widget = qobject_cast<ChatBubbleWidget *>(this->bubbleListWidget_->itemWidget(item));
        if (widget == nullptr)
        {
            continue;
        }
        int width = this->bubbleListWidget_->width();
        auto height = widget->calHeight(width);
        widget->setFixedHeight(height);
        item->setSizeHint(QSize(40, height));
    }
    QWidget::resizeEvent(event);
}

