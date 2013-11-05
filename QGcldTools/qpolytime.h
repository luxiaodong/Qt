#ifndef QPOLYTIME_H
#define QPOLYTIME_H

#include <QString>

//将时间分割点信息转成lua
class QPolyTime
{
public:
    QPolyTime();

    QString convert(QString&);
    void parse(QString&);
};

#endif // QPOLYTIME_H
