
#include <QApplication>
#include <QFile>
#include "LiteIMClient.h"
#include "Util.h"
#include "LogInWidget.h"
#define APPLIACITON_ASS_PATH (":/qss/Application.qss")

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
    return a.exec();
}
