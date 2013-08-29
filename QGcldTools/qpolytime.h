#ifndef QPOLYTIME_H
#define QPOLYTIME_H

#include <QString>

class QPolyTime
{
public:
    QPolyTime();

    QString convert(QString&);
    void parse(QString&);
};

#endif // QPOLYTIME_H
