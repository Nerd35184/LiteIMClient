#ifndef SESSDETAILWIDGET_H
#define SESSDETAILWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "ChatDetailWidget.h"

class SessDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SessDetailWidget(QWidget *parent = nullptr);

    int upsertSessDetail(ChatDetailWidget* w,bool show);
    // int removeSessDetail(const QString &sessId);
    // int addChatBubbleWidget(const QString &sessId, ChatBubbleWidget *chatBubbleWidget);

private:
    void initUI(QWidget *parent);
    QVBoxLayout *layout_;

signals:
};

#endif // SESSDETAILWIDGET_H
