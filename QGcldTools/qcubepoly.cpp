#include "qcubepoly.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

QCubePoly::QCubePoly()
{
    //m_name = "jubenCubePoly";
    //m_name = "tanbaoCubePoly";
    //m_name = "kfWorldCubePoly";
    m_name = "japanCubePoly";
}

QString QCubePoly::convert(QString&line1, QString& line2)
{
    QStringList list1 = line1.split("\t");
    QStringList list2 = line2.split("\t");
    QString city1 = list1.first().remove(".00");
    QString city2 = list2.first().remove(".00");

    QString str;
    str += QString("function cityPath.curve_%1_%2()\n").arg(city1, city2);
    str += QString("    cityPath.data = {}\n");
    str += QString("    cityPath.data.rank = %1\n").arg( 3 );
    str += QString("    cityPath.data.x = {}\n");
    str += QString("    cityPath.data.y = {}\n");

    for(int i = 1; i < list1.size(); ++i)
    {
        str += QString("    cityPath.data.x[%1] = %2\n").arg(i-1).arg(list1.at(i));
        str += QString("    cityPath.data.y[%1] = %2\n").arg(i-1).arg(list2.at(i));
    }

    str += QString("    return cityPath.data\n");
    str += QString("end\n");
    return str;
}

void QCubePoly::parse(QString& filePath)
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
        QString line2 = stream.readLine().trimmed();
        list.append(convert(line1, line2));
        //qDebug()<<convert(line);
    }

    file.close();

    QString newfilePath = filePath;
    newfilePath.replace("polyTable.txt", "cubePoly.lua");
    QFile file2(newfilePath);

    if(file2.open(QIODevice::WriteOnly) == false)
    {
        qDebug()<<"open failed";
        return ;
    }

    QString str;
    str += QString("function cityPath.curve(cityID1, cityID2)\n");
    str += QString("    local funName = \"curve_\" .. tostring(cityID1) .. \"_\" .. tostring(cityID2)\n ");
    str += QString("    if cityID1 > cityID2 then\n");
    str += QString("        funName = \"curve_\" .. tostring(cityID2) .. \"_\" .. tostring(cityID1)\n ");
    str += QString("    end\n");
    str += QString("    if cityPath[funName] == nil then\n");
    str += QString("        return nil\n");
    str += QString("    end\n");
    str += QString("    return cityPath[funName]();\n");
    str += QString("end\n\n");

    str += QString("function cityPath.position(cityID1, cityID2, percent)\n");
    str += QString("    if cityID1 > cityID2 then\n");
    str += QString("        percent = 1.0 - percent;\n");
    str += QString("    end\n\n");
    str += QString("    x = cityPath.data.x[0];\n");
    str += QString("    y = cityPath.data.y[0];\n\n");
    str += QString("    for i =1, cityPath.data.rank do\n");
    str += QString("        x = x*percent + cityPath.data.x[i];\n");
    str += QString("        y = y*percent + cityPath.data.y[i];\n");
    str += QString("    end\n\n");
    str += QString("    return x,y;\n");
    str += QString("end\n");

    QTextStream stream2(&file2);
    stream2.setCodec("UTF-8");
    stream2<<QString("cityPath = {}\n");
    foreach(QString single, list)
    {
        stream2<<single<<"\n";
    }
    stream2<<str;
    file2.close();
    qDebug()<<"ok";
}

QString QCubePoly::convert2(QString&line1, QString& line2)
{
    QStringList list1 = line1.split("\t");
    QStringList list2 = line2.split("\t");
    QString city1 = list1.first().remove(".00");
    QString city2 = list2.first().remove(".00");

    QString str;
    str += QString("function %1.curve_%2_%3()\n").arg(m_name, city1, city2);
    str += QString("    local data = {}\n");
    str += QString("    data.rank = %1\n").arg( 3 );
    str += QString("    data.x = {}\n");
    str += QString("    data.y = {}\n");

    for(int i = 1; i < list1.size(); ++i)
    {
        str += QString("    data.x[%1] = %2\n").arg(i-1).arg(list1.at(i));
        str += QString("    data.y[%1] = %2\n").arg(i-1).arg(list2.at(i));
    }

    str += QString("    return data\n");
    str += QString("end\n");
    return str;
}

void QCubePoly::parse2(QString& filePath)
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
        QString line2 = stream.readLine().trimmed();
        list.append(convert2(line1, line2));
    }

    file.close();

    QString newfilePath = filePath;
    newfilePath.replace("polyTable", m_name);
    newfilePath.replace(".txt", ".lua");
    QFile file2(newfilePath);

    if(file2.open(QIODevice::WriteOnly) == false)
    {
        qDebug()<<"open failed";
        return ;
    }

    QString str;
    str += QString("function %1.curve(cityID1, cityID2)\n").arg(m_name);
    str += QString("    local funName = \"curve_\" .. tostring(cityID1) .. \"_\" .. tostring(cityID2)\n ");
    str += QString("    if cityID1 > cityID2 then\n");
    str += QString("        funName = \"curve_\" .. tostring(cityID2) .. \"_\" .. tostring(cityID1)\n ");
    str += QString("    end\n");
    str += QString("    if %1[funName] == nil then\n").arg(m_name);
    str += QString("        return nil\n");
    str += QString("    end\n");
    str += QString("    return %1[funName]();\n").arg(m_name);
    str += QString("end\n\n");

    str += QString("function %1.position(data, cityID1, cityID2, percent)\n").arg(m_name);
    str += QString("    if cityID1 > cityID2 then\n");
    str += QString("        percent = 1.0 - percent;\n");
    str += QString("    end\n\n");
    str += QString("    x = data.x[0];\n");
    str += QString("    y = data.y[0];\n\n");
    str += QString("    for i =1, data.rank do\n");
    str += QString("        x = x*percent + data.x[i];\n");
    str += QString("        y = y*percent + data.y[i];\n");
    str += QString("    end\n\n");
    str += QString("    return x,y;\n");
    str += QString("end\n");

    QTextStream stream2(&file2);
    stream2.setCodec("UTF-8");
    stream2<<QString("local %1 = {}\n").arg(m_name);
    foreach(QString single, list)
    {
        stream2<<single<<"\n";
    }
    stream2<<str<<"\n";
    stream2<<QString("return %1\n").arg(m_name);
    file2.close();
    qDebug()<<"ok";
}
