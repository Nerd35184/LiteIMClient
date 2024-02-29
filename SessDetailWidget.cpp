#include "SessDetailWidget.h"
#include <QVBoxLayout>

SessDetailWidget::SessDetailWidget(QWidget *parent)
    : QWidget{parent}
{
    this->initUI(this);
}

int SessDetailWidget::upsertSessDetail(ChatDetailWidget *w, bool show)
{
    if (w == nullptr)
    {
        return -1;
    }
    bool found = false;
    for (auto &child : this->children())
    {
        auto widget = qobject_cast<ChatDetailWidget *>(child);
        if (widget == nullptr)
        {
            continue;
        }
        bool equal = false;
        if (widget->getSessIdR() == w->getSessIdR())
        {
            found = true;
            equal = true;
        }
        if (equal && show)
        {
            widget->show();
        }
        if (!equal && show)
        {
            widget->hide();
        }
        if (equal && !show)
        {
            break;
        }
    }
    if (found)
    {
        return 0;
    }
    this->layout_->addWidget(w);
    if (show)
    {
        w->show();
    }
    else
    {
        w->hide();
    }
    return 0;
}

int SessDetailWidget::removeSessDetail(const QString &sessId)
{
    for (auto &child : this->children())
    {
        auto widget = qobject_cast<ChatDetailWidget *>(child);
        if (widget == nullptr)
        {
            continue;
        }
        if (widget->getSessIdR() == sessId)
        {
            auto layout = this->layout_;
            layout->removeWidget(widget);
            widget->deleteLater();
            break;
        }
    }
    return 0;
}

int SessDetailWidget::addChatBubbleWidget(const QString &sessId, ChatBubbleWidget *chatBubbleWidget)
{
    if (chatBubbleWidget == nullptr)
    {
        qDebug("SessDetailsWidget addChatBubbleWidget chatBubbleWidget == nullptr");
        return -1;
    }
    for (auto &child : this->children())
    {
        auto widget = qobject_cast<ChatDetailWidget *>(child);
        if (widget == nullptr)
        {
            continue;
        }
        if (widget->getSessIdR() == sessId)
        {
            widget->addChatBubbleWidget(chatBubbleWidget);
            return 0;
        }
    }
    qDebug("SessDetailsWidget addChatBubbleWidget sess not found %s",sessId.toStdString().c_str());
    return -1;
}

void SessDetailWidget::initUI(QWidget *parent)
{
    this->layout_ = CreateQLayout<QVBoxLayout>(parent);
}
