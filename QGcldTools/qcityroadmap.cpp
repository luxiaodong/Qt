#include "qcityroadmap.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

QCityRoadMap::QCityRoadMap()
{
}

QString QCityRoadMap::convert(QString& line)
{
    int index0 = line.indexOf("city=");
    int index1 = line.indexOf("\"", index0+1);
    int index2 = line.indexOf("\"", index1+1);
    QString city = line.mid(index1+1, index2-index1-1);
    QStringList citys = city.split("-");
    int citySrc = citys.first().toInt();
    int cityDst = citys.last().toInt();

    index0 = line.indexOf("points=");
    index1 = line.indexOf("\"", index0+1);
    index2 = line.indexOf("\"", index1+1);
    QString point = line.mid(index1+1, index2-index1-1);
    QStringList points = point.split(";");

    if(points.count() > 2)
    {
        points.removeAt(0);
        points.removeAt(0);
    }

    QString str;
    if(citySrc < cityDst)
    {
        str = QString("%1\t%2").arg(citySrc).arg(cityDst);
        for(int i = 0; i < points.count(); ++i)
        {
            QStringList temp = points.at(i).split("|");
            int x = temp.first().toInt();
            int y = temp.last().toInt();
            str += QString("\t%1\t%2").arg(x).arg(y);
        }
    }
    else
    {
        str = QString("%1\t%2").arg(cityDst).arg(citySrc);
        for(int i = points.count() - 1; i >=0 ; --i)
        {
            QStringList temp = points.at(i).split("|");
            int x = temp.first().toInt();
            int y = temp.last().toInt();
            str += QString("\t%1\t%2").arg(x).arg(y);
        }
    }

    qDebug()<<str;
    return str;
}

void QCityRoadMap::parse(QString& filePath)
{
    if(filePath.isEmpty() == true)
    {
        return ;
    }

    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly) == false)
    {
        return ;
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    QStringList list;
    while(stream.atEnd() == false)
    {
        QString line = stream.readLine().trimmed();
        if(line.contains("<road") && line.contains("city=") && line.contains("points="))
        {
            list.append(this->convert(line));
        }
    }

    file.close();

    QString newfilePath = filePath;
    newfilePath.replace("Road.xml", "data_path.txt");
    QFile file2(newfilePath);

    if(file2.open(QIODevice::WriteOnly) == false)
    {
        qDebug()<<"open failed";
        return ;
    }

    QTextStream stream2(&file2);
    stream2.setCodec("UTF-8");
    foreach(QString single, list)
    {
        stream2<<single<<"\n";
    }
    file2.close();
    qDebug()<<"ok";
}
