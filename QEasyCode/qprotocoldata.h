#ifndef QPROTOCOLDATA_H
#define QPROTOCOLDATA_H

#include <QString>
#include <QStringList>
#include <QMap>

class QProtocolData
{
public:
    QProtocolData(QString&);

public:
    int protocolId(){return m_protocolId;}
    QStringList args(){return m_args;}
    QString des(){return m_des;}
    QString functionName(){return m_functionName;}
    QString protocolEnumString();
    QString upperHeadLetter(QString);

private:
    void parseText(QString&);
    QString findValueFromKey(QString& text, QString key);
    void parseArgs(QString&);

private:
    int m_protocolId;
    QStringList  m_args;  //"int playerId"
    QString m_des;
    QString m_functionName;
};

#endif // QPROTOCOLDATA_H
