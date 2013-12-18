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

    QString convert2(QString&);
    void parse2(QString&);
};

#endif // QPOLYTIME_H
