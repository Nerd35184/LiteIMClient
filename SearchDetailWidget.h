#ifndef SEARCHDETAILWIDGET_H
#define SEARCHDETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "SearchResultItemWidget.h"
#include <QLineEdit>
#include <QListWidget>

class SearchDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchDetailWidget(QWidget *parent = nullptr);
    QString getSearchLineText() const;
    int init();
    int setReturnPressedCallback(std::function<void(SearchDetailWidget& w)> callback);
    int addSearchResultListItem(SearchResultItemWidget* w);
    int setSearchResultListItemAvatar(const QString& userId,const QPixmap& avatar);
    int setAddContantBtnDisable(const QString& userId);
    int initSearchResultList();

private:
    QLineEdit* searchLineEdit_;
    QListWidget* searchResultList_;

private:
    std::function<void(SearchDetailWidget& w)> returnPressedCallback_;
    void initUI(QWidget *parent);
    void returnPressed();


signals:

};

#endif // SEARCHDETAILWIDGET_H
