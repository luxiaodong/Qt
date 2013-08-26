#include "qsildenodeextranumber.h"

#include <QFile>
#include <QTextStream>

QSildeNodeExtraNumber::QSildeNodeExtraNumber()
{
    m_lastError = QString("please drag file");
}

bool QSildeNodeExtraNumber::parseNodes(const QString& filePath)
{
    m_lastError = QString("love is over");
    m_lines.clear();
    QFile file(filePath);
    if(file.exists() == true)
    {
        if( file.open(QIODevice::ReadOnly) == false)
        {
            return false;
        }

        QTextStream out(&file);
        while(out.atEnd() == false)
        {
            m_lines.append(out.readLine());
        }

        file.close();
    }

    if( file.open(QIODevice::WriteOnly) == false)
    {
        return false;
    }

    this->addSlideTag();

    QTextStream in(&file);
    in.setCodec("UTF-8");
    foreach(QString line, m_lines)
    {
        in<<line<<"\n";
    }

    file.close();
    return true;
}

void QSildeNodeExtraNumber::addSlideTag()
{
    int startIndexTag = 15001;
    int lineCycile = 11;

    QRegExp regexp("<string>[ABCDE]S[1234]</string>");
    QRegExp regexp2("<string>[ABCDE][XL][1234]</string>");
    QRegExp regexp3("<string>Beat1</string>");

    int i = 0;
    int size = m_lines.size();

    while(i < size)
    {
        QString line = m_lines.at(i);
        if(line.contains(regexp) == true)
        {
            int nodeExtraNumberLine = i - lineCycile - 6;
            this->replaceExtraNumber(nodeExtraNumberLine, startIndexTag);
            this->replaceExtraNumber(i - 6, startIndexTag);

            while(1)
            {
                i += lineCycile;

                if(i >= size)
                {
                    return;
                }

                QString nextLine = m_lines.at(i);

                if(nextLine.contains(regexp2) == true ||
                   nextLine.contains(regexp3) == true)
                {
                    continue;
                }

                if(nextLine.contains(regexp) == false)
                {
                    startIndexTag++;
                    break;
                }
                else
                {
                    nodeExtraNumberLine = i - 6;
                    this->replaceExtraNumber(nodeExtraNumberLine, startIndexTag);
                }
            }
        }

        i++;
    }
}

void QSildeNodeExtraNumber::replaceExtraNumber(int lineIndex, int TagIndex)
{
    QString extraNumberStr = m_lines.at(lineIndex);
    int leftIndex = extraNumberStr.indexOf(">");
    int rightIndex = extraNumberStr.indexOf("<",leftIndex);
    QString oldStr = extraNumberStr.mid(leftIndex + 1, rightIndex - leftIndex - 1);
    int oldNumber = oldStr.toInt();
    if(oldNumber == 0)
    {
        QString newStr = QString("%1").arg(TagIndex);
        extraNumberStr.replace(oldStr, newStr);
        m_lines.replace(lineIndex, extraNumberStr);
    }
}

