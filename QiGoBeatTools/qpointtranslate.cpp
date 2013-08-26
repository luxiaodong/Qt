#include "qpointtranslate.h"

#include <QFile>
#include <QTextStream>

QPointTranslate::QPointTranslate()
{
    m_lastError = QString("please drag file");
}

bool QPointTranslate::translateFiles(QStringList &list)
{
    foreach(QString filePath , list)
    {
        if(this->translateFile(filePath) == false)
        {
            return false;
        }
    }

    return true;
}

bool QPointTranslate::translateFile(const QString &filePath)
{
    m_lastError = QString("love is over!");

    if(filePath.contains(".xml") == true)
    {
        return this->translateXmlFile(filePath);
    }
    else if(filePath.contains(".plist") == true)
    {
        return this->translatePlistFile(filePath);
    }

    m_lastError = QString("unKnow format!");
    return false;
}

bool QPointTranslate::translateXmlFile(const QString &filePath)
{
    QString fileName = filePath;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        m_lastError = QString("open %1 failed!").arg(filePath);
        return false;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    QStringList lines;
    lines.clear();

    while(in.atEnd() == false)
    {
        lines.append(in.readLine());
    }

    file.close();
    file.setFileName(fileName);
    if( file.open(QIODevice::WriteOnly) == false)
    {
        m_lastError = QString("open %1 failed!").arg(filePath);
        return false;
    }

    in.setDevice(&file);
    in.setCodec("UTF-8");
    for(int i = 0; i < lines.size(); ++i)
    {
        QString str = lines.at(i);
        int x = 0;
        int y = 0;
        if(str.contains(" x=\"") == true)
        {
            int index = str.indexOf(" x=\"");

            if(index == -1)
            {
                continue;
            }

            int index1 = str.indexOf("\"", index);
            int index2 = str.indexOf("\"", index1 + 1);

            x = str.mid(index1+1, index2 - index1 - 1).toInt();
            x = (x - 384)*160/384 + 160;
            QString xx = QString("%1").arg(x);
            str.replace(index1+1, index2 - index1 - 1, xx);
        }

        if(str.contains(" y=\"") == true)
        {
            int index = str.indexOf(" y=\"");

            if(index == -1)
            {
                continue;
            }

            int index1 = str.indexOf("\"", index);
            int index2 = str.indexOf("\"", index1 + 1);

            y = str.mid(index1+1, index2 - index1 - 1).toInt();
            y = (y - 512)*240/512 + 240;
            QString yy = QString("%1").arg(y);
            str.replace(index1+1, index2 - index1 - 1, yy);
        }

        in<<str<<"\n";
    }
    file.close();
    return true;
}

bool QPointTranslate::translatePlistFile(const QString &filePath)
{
    QString fileName = filePath;
    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly) == false)
    {
        m_lastError = QString("open %1 failed!").arg(filePath);
        return false;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    QStringList lines;
    lines.clear();

    while(in.atEnd() == false)
    {
        lines.append(in.readLine());
    }

    file.close();

    file.setFileName(fileName);
    if( file.open(QIODevice::WriteOnly) == false)
    {
        m_lastError = QString("open %1 failed!").arg(filePath);
        return false;
    }

    in.setDevice(&file);
    in.setCodec("UTF-8");
    bool isXChanged = false;
    bool isYChanged = false;
    for(int i = 0; i < lines.size(); ++i)
    {
        QString str = lines.at(i);

        if(isXChanged == true)
        {
            isXChanged = false;
            int index1 = str.indexOf(">");
            int index2 = str.lastIndexOf("<");

            QString temp = str.mid(index1 + 1, index2 - index1 - 1);
            int x = temp.toInt();
            x = (x - 384)*160/384 + 160;
            str.replace(temp, QString("%1").arg(x));
        }

        if(isYChanged == true)
        {
            isYChanged = false;
            int index1 = str.indexOf(">");
            int index2 = str.lastIndexOf("<");
            QString temp = str.mid(index1 + 1, index2 - index1 - 1);
            int y = temp.toInt();
            y = (y - 512)*240/512 + 240;
            str.replace(temp, QString("%1").arg(y));
        }

        if(str.contains("<key>InitPosX</key>") == true)
        {
            isXChanged = true;
        }

        if(str.contains("<key>InitPosY</key>") == true)
        {
            isYChanged = true;
        }

        in<<str<<"\n";
    }
    file.close();
    return true;
}

