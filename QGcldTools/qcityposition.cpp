#include "qcityposition.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

QCityPosition::QCityPosition()
{
}

QString QCityPosition::convert(QString& line)
{
    int index0 = line.indexOf("_");
    int index1 = line.indexOf("_", index0 + 1);
    int index2 = line.indexOf("=", index1 + 1);
    int index3 = line.indexOf("=", index2 + 1);
    int index4 = line.indexOf(",", index3 + 1);
    int index5 = line.indexOf("=", index4 + 1);
    int index6 = line.indexOf(",", index5 + 1);

    QString s1 = line.mid(index1 + 1, index2 - index1 - 1);
    QString s2 = line.mid(index3 + 1, index4 - index3 - 1);
    QString s3 = line.mid(index5 + 1, index6 - index5 - 1);

    QString str = QString("%1 %2 %3").arg(s1,s2,s3);
    return str;
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
        if (line.contains("world_building_") == true)
        {
            list.append(convert(line));
            //qDebug()<<convert(line);
        }
    }

    file.close();

    QString newfilePath = filePath;
    newfilePath.replace("cityPosition.lua", "cityPosition.txt");
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

