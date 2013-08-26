#include "qaddhd.h"

#include <QFile>

QAddHd::QAddHd()
{
    m_lastError = QString("please drag file");
}

bool QAddHd::addHdFile(const QString& filePath)
{
    m_lastError = QString("love is over");
    QString pngFile = filePath;

    if(QFile::exists(pngFile))
    {
        QString newPath = pngFile;
        newPath = newPath.replace(".png","-hd.png");
        QFile::copy(pngFile, newPath);
    }

    QString plistFile = filePath;
    plistFile.replace(".png",".plist");

    if(QFile::exists(plistFile) )
    {
        QString newPath = plistFile;
        newPath = newPath.replace(".plist","-hd.plist");
        QFile::copy(plistFile, newPath);
    }

    return true;
}
