#ifndef QCITYPOSITION_H
#define QCITYPOSITION_H

#include <QString>

class QCityPosition
{
public:
    QCityPosition();
    QString convert(QString&);
    void parse(QString&);
};

#endif // QCITYPOSITION_H
