#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

/*
    * C字头城际列车：111个车次
    * D字头动车组：　419个车次
    * G字头高速动车：247个车次
    * Z字头直快列车：43个车次
    * T字头特快列车：168个车次
    * K字头快速列车：770个车次
    * 普快列车：　　 700个车次
    * Y字头旅游专列：8个车次
    * L字头临时列车：4个车次
*/
