#ifndef QHDWIKIREAD_H
#define QHDWIKIREAD_H

#include <QSqlDatabase>
#include <QString>
#include <QTextCodec>
#include <QSqlTableModel>
#include <QStringList>
#include <QRegExp>

class QHDWikiRead
{
public:
    QHDWikiRead();

public:
    bool tryToConnectDataBase();
    QStringList tryToCatchProtocolText();
    QStringList splitProtocol(QString&);
private:
    QSqlDatabase    m_database;
    QString         m_tableName;
    QString         m_projectName;
};

#endif // QHDWIKIREAD_H
