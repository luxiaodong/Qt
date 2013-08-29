#include "qcityroadmap.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

QCityRoadMap::QCityRoadMap()
{
}

QString QCityRoadMap::convert(QString& str)
{
    if(str.isEmpty() == true)
    {
        return str;
    }

    int index1 = str.indexOf("\"", 0);
    int index2 = str.indexOf("\"", index1 + 1);
    int index3 = str.indexOf("\"", index2 + 1);
    int index4 = str.indexOf("\"", index3 + 1);

    QString city = str.mid(index1 + 1, index2 - index1 - 1);
    QString points = str.mid(index3 + 1, index4 - index3 - 1);
    city.replace("-","_");
    QStringList cityIds = city.split("_");

    bool isInverse = false;
    int value1 = cityIds.first().toInt();
    int value2 = cityIds.last().toInt();

    QString printStr = QString("%1 %2").arg(value1).arg(value2);
    if (value1 > value2)
    {
        city = QString("%1_%2").arg(value2).arg(value1);
        printStr = QString("%1 %2").arg(value2).arg(value1);
        isInverse = true;
    }
    QStringList pts = points.split(";");

    if (isInverse == true)
    {
        QStringList temp = pts;
        pts.clear();
        pts.append( temp.at(1) );
        pts.append( temp.at(0) );
        for(int i = 0; i < temp.size() - 2; ++i)
        {
            QString single = temp.at( temp.size() - 1 - i);
            pts.append(single);
        }
    }

    QString out;
    out += QString("function cityRoad.cityRoad_%1()\n").arg(city);
    out += QString("    cityRoad.data = {}\n");
    out += QString("    cityRoad.data.rank = %1\n").arg(pts.size() - 1);
    out += QString("    cityRoad.data.x = {}\n");
    out += QString("    cityRoad.data.y = {}\n");

    int j = 0;
    for(int i = 0; i < pts.count(); ++i)
    {
        if (i == 1)
        {
            continue;
        }

        QStringList temp = pts.at(i).split("|");
        if(i != 0)
        {
            printStr += QString(" %1 %2").arg(temp[0]).arg(temp[1]);
        }

        out += QString("    cityRoad.data.x[%1] = %2\n").arg(j).arg(temp[0]);
        bool b = false;
        int vlaue = temp[1].toInt(&b);
        if (b == false)
        {
            Q_ASSERT("xxxx");
        }
        out += QString("    cityRoad.data.y[%1] = %2\n").arg(j).arg(3600 - vlaue);
        j++;
    }

    QStringList temp = pts.at(1).split("|");
    out += QString("    cityRoad.data.x[%1] = %2\n").arg(j).arg(temp[0]);
    bool b = false;
    int vlaue = temp[1].toInt(&b);
    if (b == false)
    {
        Q_ASSERT("xxxx");
    }
    out += QString("    cityRoad.data.y[%1] = %2\n").arg(j).arg(3600 - vlaue);

    out += QString("    return cityRoad.data\n");
    out += QString("end\n");
    //printStr += QString(" %1 %2").arg(temp[0]).arg(temp[1]);

qDebug()<<printStr;
    return out;
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
        list.append(convert(line));
        //qDebug()<<convert(line);
    }

    file.close();

    QString newfilePath = filePath;
    newfilePath.replace("cityRoadMap.txt", "cityRoadMap.lua");
    QFile file2(newfilePath);

    if(file2.open(QIODevice::WriteOnly) == false)
    {
        qDebug()<<"open failed";
        return ;
    }

    QString str;
    str += QString("function cityRoad.cityRoad(cityID1, cityID2)\n");
    str += QString("    local funName = \"cityRoad_\" .. tostring(cityID1) .. \"_\" .. tostring(cityID2)\n ");
    str += QString("    if cityID1 > cityID2 then\n");
    str += QString("        funName = \"cityRoad_\" .. tostring(cityID2) .. \"_\" .. tostring(cityID1)\n ");
    str += QString("    end\n");
    str += QString("    if cityRoad[funName] == nil then\n");
    str += QString("        return nil\n");
    str += QString("    end\n");
    str += QString("    return cityRoad[funName]();\n");
    str += QString("end\n");

    QTextStream stream2(&file2);
    stream2.setCodec("UTF-8");
    stream2<<QString("cityRoad = {}");
    foreach(QString single, list)
    {
        stream2<<single<<"\n";
    }
    stream2<<str;
    file2.close();
    qDebug()<<"ok";
}
