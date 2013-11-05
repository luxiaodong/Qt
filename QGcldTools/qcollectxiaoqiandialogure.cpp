#include "qcollectxiaoqiandialogure.h"
#include <QString>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QStringList>

QCollectXiaoqianDialogure::QCollectXiaoqianDialogure()
{
}

QString QCollectXiaoqianDialogure::convert(QString& srcStr)
{
    static int dialogureIndex = 220001;
    QString dstStr = srcStr;
    if(srcStr.contains("].desc") == true)
    {
        QStringList list = srcStr.split("=");
        if(list.size() != 2)
        {
            qDebug()<<srcStr;
        }

        if(list.last().count("\"") != 2)
        {
            qDebug()<<srcStr;
        }

        dstStr = QString("%1 = language.get(%2);").arg(list.first()).arg(dialogureIndex);

        QString last = list.last().left(list.last().size() - 1);
        QString language = QString("[%1] =%2,").arg(dialogureIndex).arg(last);
        m_list.append(language);
        dialogureIndex++;
    }

//    [200705] = "吴国城池",
// language.get(40505)
    return dstStr;
}

void QCollectXiaoqianDialogure::parse(QString& filePath)
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
    m_list.clear();
    while(stream.atEnd() == false)
    {
        QString line = stream.readLine().trimmed();
        list.append(convert(line));
        //qDebug()<<convert(line);
    }

    file.close();

    QString newfilePath = filePath;
    newfilePath.replace("constant2.lua", "constant3.lua");
    QFile file2(newfilePath);

    if(file2.open(QIODevice::WriteOnly) == false)
    {
        qDebug()<<"open failed";
        return ;
    }

    QTextStream stream2(&file2);
    foreach(QString single, list)
    {
        if( !(single.contains("function") || single.contains("end")) )
        {
            stream2<<"    ";
        }
        stream2<<single<<"\n";
    }

    stream2<<"\n\n\n\n";
    foreach(QString single, m_list)
    {
        stream2<<single<<"\n";
    }

    file2.close();
    qDebug()<<"ok";
}
