#include "qpackagesongs.h"

#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QDebug>

QPackageSongs::QPackageSongs()
{
    m_lastError = QString("please drag file");
}

bool QPackageSongs::packageSongs(QStringList &list)
{
    foreach(QString filePath, list)
    {
        if(this->packageSong(filePath) == false)
        {
            return false;
        }
    }

    return true;
}

bool QPackageSongs::packageSong(const QString& filePath)
{
    m_lastError = QString("love is over");

    QString currentFileFolder = filePath;
    currentFileFolder.remove(currentFileFolder.size() - 1, 1);
    currentFileFolder = currentFileFolder.split("/").last();

    QString dirPath = filePath;
    QDir dir(dirPath);

    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);

    QString mp3File = QString("");
    QString unKnowPngFile = QString("");
    QString infoPlistFile = QString("");

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.exists())
        {
            QString fileSuffix = fileInfo.suffix().toLower();
            QString fileName = fileInfo.fileName();
            if(fileSuffix == QString("mp3"))
            {
                mp3File = fileName;
            }
            else if(fileSuffix == QString("png"))
            {
                QString coverPng = QString("%1_Cover.png").arg(currentFileFolder);
                QString infoPng = QString("%1_Info.png").arg(currentFileFolder);
                if( fileName != coverPng &&
                    fileName != infoPng )
                {
                    unKnowPngFile = fileName;
                }
            }
            else if(fileSuffix == QString("plist"))
            {
                QString infoPlist = QString("%1_Info.plist").arg(currentFileFolder);
                if(fileName == infoPlist)
                {
                    infoPlistFile = fileName;
                }
            }
        }
    }

    if( mp3File.isEmpty() ||
        unKnowPngFile.isEmpty() ||
        infoPlistFile.isEmpty())
    {
        if(mp3File.isEmpty() == true)
        {
            m_lastError = QString("can't find mp3 in %1").arg(dirPath);
        }
        else if(unKnowPngFile.isEmpty() == true)
        {
            m_lastError = QString("can't find png in %1").arg(unKnowPngFile);
        }
        else if(infoPlistFile.isEmpty() == true)
        {
            m_lastError = QString("can't find plist in %1").arg(infoPlistFile);
        }

        return false;
    }

    QFile::rename(QString("%1%2").arg(filePath, mp3File), QString("%1%2.mp3").arg(filePath,currentFileFolder));
    QFile::rename(QString("%1%2").arg(filePath, unKnowPngFile), QString("%1%2.png").arg(filePath,currentFileFolder));
    QFile::remove(QString("%1%2_Cover.png").arg(filePath, currentFileFolder));
    QFile::rename(QString("%1%2.png").arg(filePath, currentFileFolder), QString("%1%2_Cover.png").arg(filePath,currentFileFolder));
    return this->changeMediaNameInPlist(QString("%1%2").arg(filePath,infoPlistFile), currentFileFolder);
}

bool QPackageSongs::changeMediaNameInPlist(QString plistFile, QString newName, bool isHasMp3)
{
    QString fileName = plistFile;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        m_lastError = QString("open %1 failed!").arg(fileName);
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
        m_lastError = QString("open %1 failed!").arg(fileName);
        return false;
    }

    in.setDevice(&file);
    in.setCodec("UTF-8");
    bool isNeedReplaceMediaName = false;
    bool isNeedReplaceIGBID = false;
    for(int i = 0; i < lines.size(); ++i)
    {
        QString str = lines.at(i);

        if( isNeedReplaceMediaName == true )
        {
            if( str.contains("<string>") && str.contains("</string>") )
            {
                int index1 = str.indexOf(">");
                int index2 = str.indexOf("<", index1);
                str.replace(index1 + 1, index2 - index1 - 1, QString("%1.mp3").arg(newName));
                isNeedReplaceMediaName = false;
            }
        }

        if( isNeedReplaceIGBID == true )
        {
            if( str.contains("<string>") && str.contains("</string>") )
            {
                int index1 = str.indexOf(">");
                int index2 = str.indexOf("<", index1);
                QString tempStr = str.mid(index1+1, index2 - index1 - 1);
                QStringList listStr = tempStr.split("_");
                QString newStr = QString("%1_%2").arg(newName, listStr.last());
                str.replace(tempStr, newStr);
                isNeedReplaceIGBID = false;
            }
        }

        if( str.contains("<key>MediaName</key>") == true )
        {
            if(isHasMp3 == true)
            {
                isNeedReplaceMediaName = true;
            }
        }

        if( str.contains("<key>iGB_ID</key>") == true)
        {
            isNeedReplaceIGBID = true;
        }

        in<<str<<"\n";
    }
    file.close();

    return true;
}




