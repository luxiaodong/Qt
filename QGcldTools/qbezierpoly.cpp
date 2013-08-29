#include "qbezierpoly.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

QBezierPoly::QBezierPoly()
{
}

QString QBezierPoly::convert(QString& str)
{
    if (str.isEmpty() == true)
    {
        return str;
    }

    QStringList list;
    int indexRight = 0;
    while(1)
    {
        int indexLeft = str.indexOf("(", indexRight);
        if(indexLeft == -1)
        {
            break;
        }

        indexRight = str.indexOf(")", indexLeft);
        QString sub = str.mid(indexLeft + 1, indexRight - indexLeft - 1);
        list.append(sub);
    }
    list.append("p0");

    int size = list.size();
    for(int i = size - 1; i >= 0; --i)
    {
        QString src = QString("p%1").arg(i);
        QString dst = QString("p[%1]").arg(i);

        for(int j = 0; j < size; ++j)
        {
            QString temp1 = list.at(j);
            QString temp2 = temp1.replace(src,dst);
            list.replace(j, temp2);
        }
    }

    QString out;
    out += QString("function bezier.bezierpoly_%1(p)\n").arg(size - 1);
    //out += QString("{\n");
    out += QString("    bezier.c = {}\n");
    for(int i = 0; i < size; ++i)
    {
        out += QString("    bezier.c[%1] = %2;\n").arg(i).arg(list.at(size - 1 - i));
    }
    out += QString("    return bezier.c\n");
    out += QString("end\n");
    return out;
}

void QBezierPoly::parse(QString& filePath)
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
    }

    file.close();

    QString newfilePath = filePath;
    newfilePath.replace("bezierpoly.txt", "bezierpoly.lua");
    QFile file2(newfilePath);

    if(file2.open(QIODevice::WriteOnly) == false)
    {
        qDebug()<<"open failed";
        return ;
    }

    QString str;
    str += QString("function bezier.bezierpoly(rank, p)\n");
    str += QString("    local funName = \"bezierpoly_\" .. tostring(rank)\n");
    str += QString("    return bezier[funName](p)\n");
    str += QString("end\n");

    QTextStream stream2(&file2);
    stream2.setCodec("UTF-8");
    stream2<<QString("bezier = {}\n");
    foreach(QString single, list)
    {
        stream2<<single<<"\n";
    }

    stream2<<str<<"\n";
    file2.close();
    qDebug()<<"ok";
}
