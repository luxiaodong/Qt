#include <QtGui/QApplication>
#include "qlamp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qlamp w;
    w.show();

    return a.exec();
}
