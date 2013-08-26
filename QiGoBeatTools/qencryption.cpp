#include "qencryption.h"

#include <QDataStream>
#include <QFile>

QEncryption::QEncryption()
{
    m_lastError = QString("please drag file");
}

bool QEncryption::codeFile(const QString &filePath)
{
    m_lastError = QString("love is over");
    QString inFileName = filePath;
    if(inFileName.isEmpty())
    {
        m_lastError = QString("empty file.");
        return false;
    }

    QFile inFile(inFileName);
    if(inFile.open(QIODevice::ReadOnly) == false)
    {
        m_lastError = QString("open %1 failed!").arg(inFileName);
        return false;
    }

    QString outFileName = inFileName + ".plist";

    QFile outFile(outFileName);
    if(outFile.open(QIODevice::ReadWrite) == false)
    {
        m_lastError = QString("create output %1 failed!").arg(outFileName);
        return false;
    }

    QDataStream in(&inFile);
    QDataStream out(&outFile);

    quint8 mask = 0x47;

    while(in.atEnd() == false)
    {
        quint8 value;
        in>>value;
        value ^= mask;
        mask = (char)((mask<<1) + ( ((mask>>3) & 0x01 ) ^ ((mask>>4) & 0x01 ) ^ ((mask>>5) & 0x01 ) ^ ((mask>>7) & 0x01 )));
        out<<value;
    }

    inFile.close();
    outFile.close();

    return true;
}
