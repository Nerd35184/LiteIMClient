#include "SessDetailWidget.h"
#include <QVBoxLayout>

SessDetailWidget::SessDetailWidget(QWidget *parent)
    : QWidget{parent}
{
    this->initUI(this);
}

int SessDetailWidget::upsertSessDetail(ChatDetailWidget *w, bool show)
{
    qDebug("tmp 7.2.2");
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
        if (widget->getSessId() == w->getSessId())
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

void SessDetailWidget::initUI(QWidget *parent)
{
    this->layout_ = CreateQLayout<QVBoxLayout>(parent);
}
