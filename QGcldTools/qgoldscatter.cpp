#include "qgoldscatter.h"
#include <QDebug>

QGoldScatter::QGoldScatter()
{
    int gold[11] = {60, 300, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int day[9] = {1, 3, 7, 15, 30, 90, 180, 360, 1000};
    QStringList list;
    for(int j=0; j < 9; ++j)
    {
        for(int i = 0; i < 11; ++i)
        {
            list.append( QString("day%1_gold%2").arg(day[j]).arg(gold[i]) );
        }
    }

    foreach (QString key, list)
    {
        m_map.insert(key, 0);
        m_count.insert(key, 0);
        m_percent.insert(key, 0.0f);
    }

    m_keys = list;
    //qDebug()<<list;
}

void QGoldScatter::loadFile(QString filePath)
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
    QList<QString> yxs;
    QList<QString> serverIds;
    QList<QString> playerIds;
    QList<int> days;
    QList<int> golds;
    int lineNumber = 0;
    while(stream.atEnd() == false)
    {
        QString line = stream.readLine().trimmed();
        QStringList list = line.split("\t");
        yxs.append(list.at(0));
        serverIds.append(list.at(1));
        playerIds.append(list.at(2));
        int day = list.at(3).toInt();
        int gold = list.at(4).toInt();
        days.append(day);
        golds.append(gold);
        lineNumber++;
    }

    int totalGold = 0;
    for(int i=0; i< lineNumber; ++i)
    {
        int day = days.at(i);
        int gold = golds.at(i);
        QString key = this->calculateKey(day, gold);
        int value = m_map.value(key);
        m_map.insert(key, value + gold);

        value = m_count.value(key);
        m_count.insert(key, value + 1);

        if(key == "day1_gold500000")
        {
            qDebug()<<gold<<yxs.at(i)<<serverIds.at(i)<<playerIds.at(i);
        }

        totalGold += gold;
    }

    file.close();
    qDebug()<<totalGold;

    foreach(QString key, m_keys)
    {
//        int value = m_map.value(key);
//        float p = 100.0f*value/totalGold;
//        m_percent.insert(key, p);

        int value = m_count.value(key);
        float p = 100.0f*value/lineNumber;
        m_percent.insert(key, p);

        //qDebug()<<QString("%1 -> %2,%3,%4").arg(key).arg(m_count.value(key)).arg(m_map.value(key)).arg(m_percent.value(key));
        //qDebug()<<QString("%1 -> %2").arg(key).arg(m_percent.value(key));
        qDebug()<<QString("%1").arg(m_percent.value(key));
    }

    qDebug()<<"ok";
}

QString QGoldScatter::calculateKey(int day, int gold)
{
    int days[9] = {1, 3, 7, 15, 30, 90, 180, 360, 1000};
    int golds[11] = {60, 300, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int d = days[8];
    int g = golds[10];

    for(int j = 0; j < 9; ++j)
    {
        if(day <= days[j])
        {
            d = days[j];
            break;
        }
    }

    for(int i = 0; i < 11; ++i)
    {
        if(gold <= golds[i])
        {
            g = golds[i];
            break;
        }
    }

    return QString("day%1_gold%2").arg(d).arg(g);
}
