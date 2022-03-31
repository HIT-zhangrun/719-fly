#include "widget.h"

#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/image/719logo.png");
    pixmap.scaled(30,30);
    QSplashScreen splash(pixmap);
    splash.setPixmap(pixmap);
    splash.show();


    a.processEvents();
    QThread::sleep(2);

    Widget w;
    w.show();
    splash.finish(&w);
    return a.exec();
}
