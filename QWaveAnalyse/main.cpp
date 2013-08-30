#include <QtGui/QApplication>
#include "qwaveanalyse.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWaveAnalyse w;
    w.show();

    return a.exec();
}
