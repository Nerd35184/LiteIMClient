#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class LogInWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogInWidget(QWidget *parent = nullptr);
private:
signals:
};

#endif // LOGINWIDGET_H
