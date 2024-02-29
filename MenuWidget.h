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
    enum Btn{
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
        int setModifyActionTriggeredCallback(std::function<void(AvatarLabel&)> modifyActionTriggeredCallback);
    private:
        QAction *modifyAction_ = nullptr;

    private:
        void initAction(QWidget* parent);
        void modifyActionTriggered(bool);
        std::function<void(AvatarLabel&)> modifyActionTriggeredCallback_ = nullptr;
    };

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    int selectBtn(Btn btn);
    int setAvatar(const QPixmap& map);
    int setBtnClickCallback(std::function<void(MenuWidget&,Btn)> callback);
    int setModifyActionTriggeredCallback(
        std::function<void(AvatarLabel&)> callback);

private:
    AvatarLabel *avatarLbl_ = nullptr;
    QPushButton *sessBtn_ = nullptr;
    QPushButton *contactBtn_ = nullptr;
    QPushButton *newContactBtn_ = nullptr;

private:
    void initUI(QWidget* parent);
    void btnClicked(bool);
    std::function<void(MenuWidget&,Btn)> btnClickedCallback_;

signals:
};

#endif // MENUWIDGET_H
