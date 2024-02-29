#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(
        QWidget *parent,
        std::function<void()> showSearchDetailWidgetBtnClickedCallback);
    int setShowSearchDetailWidgetBtnClickedCallback(
        std::function<void()> showSearchDetailWidgetBtnClickedCallback);

private:
    QLineEdit* searchListLdt_ = nullptr;
    QPushButton* showSearchDetailWidgetBtn_ = nullptr;

private:
    std::function<void()> showSearchDetailWidgetBtnClickedCallback_ = nullptr;

private:
    void initUI(QWidget *parent);
    void showSearchDetailWidgetBtnClicked(bool);

signals:
};

#endif // SEARCHWIDGET_H
