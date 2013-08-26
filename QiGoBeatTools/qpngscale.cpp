#include "qpngscale.h"

#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <QImage>

QPngScale::QPngScale()
{
    m_lastError = QString("please drag file");
}

bool QPngScale::scale(const QString& filePath)
{
    m_lastError = QString("love is over");
    QString fileName = filePath;

    if(QFile::exists(fileName) == true)
    {
        if(fileName.contains(".png") == true)
        {
            QPixmap image(fileName, "PNG");

            int w = image.width()*m_scale;
            int h = image.height()*m_scale;

            QPixmap newImage = image.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
            newImage.save(fileName, "PNG", 1);
            return true;
        }
    }

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
    bool isFrameChanged = false;
    bool isOffsetChanged = false;
    bool isSourceColorRectChanged = false;
    bool isSourceSizeChanged = false;
    bool isSizeChanged = false;
    for(int i = 0; i < lines.size(); ++i)
    {
        QString str = lines.at(i);

        if(isFrameChanged == true)
        {
            isFrameChanged = false;
            int index1 = str.indexOf("{");
            int index2 = str.lastIndexOf("}");
            QString temp = str.mid(index1 + 1, index2 - index1 - 1);
            QStringList tempList = temp.remove("{").remove("}").split(",");
            int x = tempList.at(0).trimmed().toInt();
            int y = tempList.at(1).trimmed().toInt();
            int w = tempList.at(2).trimmed().toInt();
            int h = tempList.at(3).trimmed().toInt();
            x = x*m_scale;
            y = y*m_scale;
            w = w*m_scale;
            h = h*m_scale;
            str.replace(str.mid(index1 + 1, index2 - index1 - 1), QString("{%1, %2}, {%3, %4}").arg(x).arg(y).arg(w).arg(h));
        }

        if(isOffsetChanged == true)
        {
            isOffsetChanged = false;
            int index1 = str.indexOf("{");
            int index2 = str.lastIndexOf("}");
            QString temp = str.mid(index1 + 1, index2 - index1 - 1);
            QStringList tempList = temp.remove("{").remove("}").split(",");
            int x = tempList.at(0).trimmed().toInt();
            int y = tempList.at(1).trimmed().toInt();
            x = x*m_scale;
            y = y*m_scale;
            str.replace(str.mid(index1 + 1, index2 - index1 - 1), QString("%1, %2").arg(x).arg(y));
        }

        if(isSourceColorRectChanged == true)
        {
            isSourceColorRectChanged = false;
            int index1 = str.indexOf("{");
            int index2 = str.lastIndexOf("}");
            QString temp = str.mid(index1 + 1, index2 - index1 - 1);
            QStringList tempList = temp.remove("{").remove("}").split(",");
            int x = tempList.at(0).trimmed().toInt();
            int y = tempList.at(1).trimmed().toInt();
            int w = tempList.at(2).trimmed().toInt();
            int h = tempList.at(3).trimmed().toInt();
            x = x*m_scale;
            y = y*m_scale;
            w = w*m_scale;
            h = h*m_scale;
            str.replace(str.mid(index1 + 1, index2 - index1 - 1), QString("{%1, %2}, {%3, %4}").arg(x).arg(y).arg(w).arg(h));
        }

        if(isSourceSizeChanged == true)
        {
            isSourceSizeChanged = false;
            int index1 = str.indexOf("{");
            int index2 = str.lastIndexOf("}");
            QString temp = str.mid(index1 + 1, index2 - index1 - 1);
            QStringList tempList = temp.remove("{").remove("}").split(",");
            int x = tempList.at(0).trimmed().toInt();
            int y = tempList.at(1).trimmed().toInt();
            x = x*m_scale;
            y = y*m_scale;
            str.replace(str.mid(index1 + 1, index2 - index1 - 1), QString("%1, %2").arg(x).arg(y));
        }

        if(isSizeChanged == true)
        {
            isSizeChanged = false;
            int index1 = str.indexOf("{");
            int index2 = str.lastIndexOf("}");
            QString temp = str.mid(index1 + 1, index2 - index1 - 1);
            QStringList tempList = temp.remove("{").remove("}").split(",");
            int x = tempList.at(0).trimmed().toInt();
            int y = tempList.at(1).trimmed().toInt();
            x = x*m_scale;
            y = y*m_scale;
            str.replace(str.mid(index1 + 1, index2 - index1 - 1), QString("%1, %2").arg(x).arg(y));
        }

        if(str.contains("<key>frame</key>") == true)
        {
            isFrameChanged = true;
        }

        if(str.contains("<key>offset</key>") == true)
        {
            isOffsetChanged = true;
        }

        if(str.contains("<key>sourceColorRect</key>") == true)
        {
            isSourceColorRectChanged = true;
        }

        if(str.contains("<key>sourceSize</key>") == true)
        {
            isSourceSizeChanged = true;
        }

        if(str.contains("<key>size</key>") == true)
        {
            isSizeChanged = true;
        }

        in<<str<<"\n";
    }

    file.close();
    return true;
}
