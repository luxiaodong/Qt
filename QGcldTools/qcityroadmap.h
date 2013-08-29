#ifndef QCITYROADMAP_H
#define QCITYROADMAP_H

#include <QString>
#include <QStringList>


class QCityRoadMap
{
public:
    QCityRoadMap();
    QString convert(QString&);
    void parse(QString&);
};

#endif // QCITYROADMAP_H
