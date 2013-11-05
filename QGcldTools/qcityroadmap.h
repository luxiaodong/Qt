#ifndef QCITYROADMAP_H
#define QCITYROADMAP_H

#include <QString>
#include <QStringList>

//将matlab生成的路径信息 转成 lua格式
class QCityRoadMap
{
public:
    QCityRoadMap();
    QString convert(QString&);
    void parse(QString&);
};

#endif // QCITYROADMAP_H
