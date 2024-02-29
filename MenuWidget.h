#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QContextMenuEvent>

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    enum MenuWidgetBtn{
        sessBtn,
        contactBtn,
        newContactBtn
    };

    class AvatarLabel:public QLabel
    {
    public:
        AvatarLabel(
            QWidget *parent,
            std::function<void(AvatarLabel&)> modifyActionTriggeredCallback
            );
        virtual void contextMenuEvent(QContextMenuEvent *event);
    private:
        QAction *modifyAction_ = nullptr;

    private:
        void initAction(QWidget* parent);
        void modifyActionTriggered(bool);
        std::function<void(AvatarLabel&)> modifyActionTriggeredCallback_ = nullptr;
    };

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    int selectBtn(MenuWidgetBtn btn);
    int setAvatar(const QPixmap& map);
    int setBtnClickCallback(std::function<void(MenuWidget&,MenuWidgetBtn)> callback);

private:
    AvatarLabel *avatarLbl_ = nullptr;
    QPushButton *sessBtn_ = nullptr;
    QPushButton *contactBtn_ = nullptr;
    QPushButton *newContactBtn_ = nullptr;

private:
    void initUI(QWidget* parent);
    void btnClicked(bool);
    std::function<void(MenuWidget&,MenuWidgetBtn)> btnClickedCallback_;
    void avatarLblmodifyActionTriggered(AvatarLabel& lbl);
    std::function<void(AvatarLabel&)> avatarLblmodifyActionTriggeredCallback_;

signals:
};

#endif // MENUWIDGET_H
