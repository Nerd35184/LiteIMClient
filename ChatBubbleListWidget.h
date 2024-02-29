#ifndef CHATBUBBLELISTWIDGET_H
#define CHATBUBBLELISTWIDGET_H

#include <QWidget>
#include <QLabel>
#include "Util.h"

#define CHAT_BUBBLE_WIDGET_AVATAR_WIDTH (36)
#define CHAT_BUBBLE_WIDGET_AVATAR_HEIGHT (36)
#define CHAT_BUBBLE_WIDGET_NICKNAME_HEIGHT (18)

class TextDisplayWidget:public QWidget
{
    Q_OBJECT
public:
    explicit TextDisplayWidget(QWidget *parent, const QString &text);
    virtual void paintEvent(QPaintEvent *event);
    const QString &getTextR() const;
    void setText(const QString &text);
    void setAlignment(Qt::AlignmentFlag);

private:
    QString text_;
    Qt::AlignmentFlag alignmentFlag;

signals:
};


class ChatBubbleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatBubbleWidget(
        QWidget *parent,
        const QPixmap &avatar,
        const QString &nickname,
        const QString &content,
        bool left);

    const QString getContentStr() const;
    const int calHeight(int width);

private:
    QLabel *avatarLbl_ = nullptr;
    QLabel *nicknameLbl_ = nullptr;
    TextDisplayWidget *contentWidget_ = nullptr;

private:
    void initUI(QWidget *parent, bool left);
    void initNicknameAndContentWidget(QWidget *parent, bool left);
    void initAvatarLabel(QWidget *parent, bool left);

signals:
};




class ChatBubbleListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatBubbleListWidget(QWidget *parent);
    int addChatBubbleWidget(ChatBubbleWidget *chatBubbleWidget);
    void resizeEvent(QResizeEvent *event);

private:
    QListWidget *bubbleListWidget_;

private:
    void initUI(QWidget *parent);
signals:
};

#endif // CHATBUBBLELISTWIDGET_H
