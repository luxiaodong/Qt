#include <QApplication>
#include "mainwindow.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_WS_WIN
    QTranslator ts;
    ts.load("cocos2dUIDesign.qm");
    a.installTranslator(&ts);
#endif
    MainWindow w;
    w.show();
    a.setQuitOnLastWindowClosed(true);
    return a.exec();
}
