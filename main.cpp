
#include <QApplication>
#include <QFile>
#include "LiteIMClient.h"
#include "Util.h"
#define APPLIACITON_ASS_PATH (":/qss/Application.qss")


int test(QApplication& a){
    ObjGuard g(1);
    auto w = CreateQWidget<MenuWidget>(g,"MainWidget_menuWidget",nullptr);
    // QPixmap m(1024,1024);
    // m.fill(Qt::red);
    // w->setAvatar(m);
    w->show();
    return a.exec();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qFile(APPLIACITON_ASS_PATH);
    bool ok = qFile.open(QFile::ReadOnly);
    if (!ok)
    {
        qDebug("some error happen when open");
        return -1;
    }
    a.setStyleSheet(qFile.readAll());
    qFile.close();

    LiteIMClient client("127.0.0.1:8080");
    client.logIn("username1", "password1");
    // test(a);
    return a.exec();
}
