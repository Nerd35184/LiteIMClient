#ifndef SESSLISTITEM_H
#define SESSLISTITEM_H

#include <QWidget>
#include <QLabel>

class SessListItem : public QWidget
{
    Q_OBJECT
public:
    explicit SessListItem(
        QWidget *parent,
        const QString &sessId,
        const QString &sessName,
        const QString &lastWord,
        const QPixmap &avatar);

    const QString &getSessIdR() const;
    int setDeleteActionTriggered(std::function<void(SessListItem &)> deleteActionTriCallback);
    int setAvatar(const QPixmap& avatar);
    virtual void contextMenuEvent(QContextMenuEvent *event);

private:
    // 通过qobj控制生命周期的成员
    QLabel *avatarLbl_ = nullptr;
    QLabel *sessNameLbl_ = nullptr;
    QLabel *lastWordLbl_ = nullptr;
    QAction *deleteAction_ = nullptr;

private:
    QString sessId_;
    std::function<void(SessListItem &)> deleteActionTriCallback_ = nullptr;

private:
    void deleteActionTriggered(bool);
    void initUI(QWidget *parent);
    void initAction(QWidget *parent);
    void initSessNameLastWordWidget(QWidget *parent);
signals:
};

#endif // SESSLISTITEM_H
