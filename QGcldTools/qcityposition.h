#ifndef QCITYPOSITION_H
#define QCITYPOSITION_H

#include <QString>

//将程序里的 city.lua 转成 txt文件给 matlab用
class QCityPosition
{
public:
    QCityPosition();
    QString convert(QString&);
    void parse(QString&);

public:
    int m_mapHeight;
};

#endif // QCITYPOSITION_H
