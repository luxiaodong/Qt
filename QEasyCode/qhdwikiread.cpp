#include "qhdwikiread.h"

#include <QDebug>

QHDWikiRead::QHDWikiRead()
{
    m_tableName = "wiki_doc";
    m_projectName = "LasVesGas%";
}

bool QHDWikiRead::tryToConnectDataBase()
{
    qDebug()<<"support database:"<<QSqlDatabase::drivers();

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    m_database = QSqlDatabase::addDatabase("QMYSQL","hdwiki");
    m_database.setHostName("192.168.12.92");
    m_database.setDatabaseName("hdwiki");
    m_database.setUserName("developer");
    m_database.setPassword("developer2011");

    return m_database.open();
}

QStringList QHDWikiRead::tryToCatchProtocolText()
{
    QSqlTableModel model(0, m_database);
    model.setTable(m_tableName);
    model.select();

    model.setFilter(QString("title LIKE '%1'").arg(m_projectName));

    int rowCount = model.rowCount();
    int columnCount = model.columnCount();

    QStringList list;
    list.clear();
    //for(int j = 0; j < rowCount; ++j)
    int j = rowCount - 1;
    {
        for(int i = 0; i < columnCount; ++i)
        {
            if(i == 6)
            {
                QModelIndex index = model.index(j,i);
                QString str = index.data().toString();
                list += splitProtocol(str);
            }
        }
    }

    return list;
}

QStringList QHDWikiRead::splitProtocol(QString& text)
{
    //QRegExp rx("\\b(&nbsp|</p><p>)\\b");
    QString copyText = text.remove("&nbsp").remove("</p><p>");

    QStringList list;
    list.clear();
    int index = 0;
    while(1)
    {
        int leftIndex = copyText.indexOf("{", index);

        if(leftIndex != -1)
        {
            index = leftIndex++;
            int rightIndex = copyText.indexOf("}", index);

            Q_ASSERT(rightIndex != -1);
            QString str = copyText.mid(leftIndex, rightIndex - leftIndex);
            list.append(str);
            index = rightIndex++;
        }
        else
        {
            break;
        }
    }

    return list;
}


