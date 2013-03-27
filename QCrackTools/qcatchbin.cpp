#include "qcatchbin.h"

QCatchBin::QCatchBin()
{
    m_lastError = "";
}

void QCatchBin::setStartAndEnd(int start, int end)
{
    m_start = start;
    m_end = end;
}

void QCatchBin::setOutPutFileName(QString outPutFileName)
{
    m_outPutFileName = QString("/home/luxiaodong/Desktop/") + outPutFileName;
}

bool QCatchBin::catchBin(QString filePath)
{
    if( filePath.isEmpty() == true )
    {
        m_lastError = "filePath is empty";
        return false;
    }

    if( m_outPutFileName.isEmpty() == true )
    {
        m_lastError = "outPutFileName is empty";
        return false;
    }

    if( m_start >= m_end || m_start < 0 || m_end < 0)
    {
        m_lastError = "start or end offset is error";
        return false;
    }

    QFile file(filePath);
    if( file.open(QIODevice::ReadOnly) == false )
    {
        m_lastError = "read file error";
        return false;
    }

    QDataStream in(&file);
    QByteArray bytes;

    int i = 0;
    while( in.atEnd() == false)
    {
        quint8 t = 0;
        in>>t;

        if(i >= m_start && i < m_end)
        {
            bytes.append(t);
        }

        i++;

        if(i >= m_end)
        {
            break;
        }
    }

    file.close();

    file.setFileName(m_outPutFileName);
    if( file.open(QIODevice::WriteOnly) == false)
    {
        m_lastError = "create file failed";
        return false;
    }

    QDataStream out(&file);
    for(int i = 0; i < bytes.size(); ++i)
    {
        quint8 t = bytes.at(i);
        out<<t;
    }
    file.close();

    return true;
}

bool QCatchBin::catchBin(int start, int end, QString inFilePath, QString outFilePath)
{
    m_start = start;
    m_end = end;
    m_outPutFileName = outFilePath;
    return catchBin(inFilePath);
}


