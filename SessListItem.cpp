#include "SessListItem.h"

SessListItem::SessListItem(QWidget *parent)
    : QWidget{parent}
{}

int SessListItem::setAvatar(const QPixmap &avatar)
{
    return 0;
}

const QString SessListItem::getSessIdR() const
{
    return this->sessId_;
}
