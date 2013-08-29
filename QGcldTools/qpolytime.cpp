#include "qpolytime.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

QPolyTime::QPolyTime()
{
}

QString QPolyTime::convert(QString&line1)
{
    QStringList list1 = line1.split("\t");
    QString city1 = list1.at(0);
    QString city2 = list1.at(1);

    QString str;
    str += QString("function tracePoints.elapse_%1_%2()\n").arg(city1, city2);
    str += QString("    tracePoints.data = {}\n");
    str += QString("    tracePoints.data.count = %1\n").arg( list1.size() - 2 );
    str += QString("    tracePoints.data.time = {}\n");

    for(int i = 2; i < list1.size(); ++i)
    {
        str += QString("    tracePoints.data.time[%1] = %2\n").arg(i - 1).arg( list1.at(i) );
    }

    str += QString("    return tracePoints.data\n");
    str += QString("end\n");
    return str;
}

void QPolyTime::parse(QString& filePath)
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
        QString line1 = stream.readLine().trimmed();
        list.append(convert(line1));
        //qDebug()<<convert(line);
    }

    file.close();

    QString newfilePath = filePath;
    newfilePath.replace("timeTable.txt", "tracePoints.lua");
    QFile file2(newfilePath);

    if(file2.open(QIODevice::WriteOnly) == false)
    {
        qDebug()<<"open failed";
        return ;
    }

    QString str;
    str += QString("function tracePoints.elapse(cityID1, cityID2)\n");
    str += QString("    local funName = \"elapse_\" .. tostring(cityID1) .. \"_\" .. tostring(cityID2)\n ");
    str += QString("    if cityID1 > cityID2 then\n");
    str += QString("        funName = \"elapse_\" .. tostring(cityID2) .. \"_\" .. tostring(cityID1)\n ");
    str += QString("    end\n");
    str += QString("    if tracePoints[funName] == nil then\n");
    str += QString("        return nil\n");
    str += QString("    end\n");
    str += QString("    return tracePoints[funName]();\n");
    str += QString("end\n\n");

    QTextStream stream2(&file2);
    stream2.setCodec("UTF-8");
    stream2<<QString("tracePoints = {}\n");
    foreach(QString single, list)
    {
        stream2<<single<<"\n";
    }
    stream2<<str;
    file2.close();
    qDebug()<<"ok";
}
