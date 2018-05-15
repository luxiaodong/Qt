#include "qcityposition.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

QCityPosition::QCityPosition()
{
    m_mapHeight = 3600;
}

QString QCityPosition::convert(QString& line)
{
    int index0 = line.indexOf("id=");
    int index1 = line.indexOf("\"", index0+1);
    int index2 = line.indexOf("\"", index1+1);
    QString id = line.mid(index1+1, index2-index1-1);

    index0 = line.indexOf("x=");
    index1 = line.indexOf("\"", index0+1);
    index2 = line.indexOf("\"", index1+1);
    QString x = line.mid(index1+1, index2-index1-1);

    index0 = line.indexOf("y=");
    index1 = line.indexOf("\"", index0+1);
    index2 = line.indexOf("\"", index1+1);
    QString y = line.mid(index1+1, index2-index1-1);
    int iy = 3600 - y.toInt();

    return QString("%1\t%2\t%3").arg(id,x).arg(iy);
}

void QCityPosition::parse(QString& filePath)
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
        if (line.contains("city") && line.contains("id") && line.contains("x") && line.contains("y") )
        {
            list.append(convert(line));
        }
    }

    file.close();

    QString newfilePath = filePath;
    newfilePath.replace("CityInfo.xml", "data_city.txt");
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

