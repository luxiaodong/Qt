#include "qprotocoldata.h"

QProtocolData::QProtocolData(QString& protocolText)
{
    parseText(protocolText);
}

void QProtocolData::parseText(QString& protocolText)
{
    m_protocolId = this->findValueFromKey(protocolText,"protocol").toInt();
    QString args = this->findValueFromKey(protocolText, "args").remove("\"");
    m_des = this->findValueFromKey(protocolText, "des").remove("\"");
    m_functionName = this->findValueFromKey(protocolText, "function").remove("\"");
    this->parseArgs(args);
}

QString QProtocolData::findValueFromKey(QString& text, QString key)
{
    int index = text.indexOf(key);
    Q_ASSERT(index != -1);

    int leftIndex = text.indexOf("=",index);
    Q_ASSERT(leftIndex != -1);

    int rightIndex = text.indexOf(";",leftIndex);
    Q_ASSERT(rightIndex != -1);

    return text.mid(leftIndex + 1, rightIndex - leftIndex - 1);
}

void QProtocolData::parseArgs(QString& args)
{
    m_args.clear();

    if(args.isEmpty() == true)
    {
        return ;
    }

    QStringList list = args.split(",");
    foreach (QString temp, list)
    {
        QStringList tempList = temp.split(":");
        Q_ASSERT(tempList.size() == 2);
        m_args.append(QString("%1 %2").arg(tempList.last(), tempList.first()));
    }
}

QString QProtocolData::protocolEnumString()
{
    if(m_protocolId%2 == 1)
    {
        return QString("C2S_%1").arg(m_functionName);
    }

    return QString("S2C_%1").arg(m_functionName);
}

QString QProtocolData::upperHeadLetter(QString str)
{
    QString one = str.left(1);
    QString other = str.remove(0,1);
    str = QString("%1%2").arg(one.toUpper(), other);
    return str;
}

