#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("gb2312");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
