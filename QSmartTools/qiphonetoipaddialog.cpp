#include "qiphonetoipaddialog.h"
#include "ui_qiphonetoipaddialog.h"

#include <QFileDialog>
#include <QPixmap>
#include <QDebug>

QIPhoneToIPadDialog::QIPhoneToIPadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QIPhoneToIPadDialog)
{
    ui->setupUi(this);

    connect(ui->resourcePath, SIGNAL(clicked()), this, SLOT(clickResourcePath()));
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOK()));
}

QIPhoneToIPadDialog::~QIPhoneToIPadDialog()
{
    delete ui;
}

void QIPhoneToIPadDialog::clickResourcePath()
{
    QString filePath = QFileDialog::getExistingDirectory(this);

    if(filePath.isEmpty() == true)
    {
        return ;
    }

    ui->lineEdit->setText(filePath);
}

void QIPhoneToIPadDialog::clickOK()
{
    QString filePath = ui->lineEdit->text();

    if(filePath.isEmpty() == true)
    {
        return ;
    }

    m_resourceRootPath = filePath;

    QString iPadPath = m_resourceRootPath + "/iPad";
    QDir dir(iPadPath);
    if(dir.exists() == false)
    {
        dir.mkdir(iPadPath);
    }

    m_allFiles.clear();
    m_allDirs.clear();

    this->traverseDir(m_resourceRootPath);

    ui->textBrowser->clear();
    foreach(QString file, m_allFiles)
    {
        ui->textBrowser->append(file);
        this->convert(file);
    }

//    foreach(QString file, m_allDirs)
//    {
//        ui->textBrowser->append(file);
//    }
}

void QIPhoneToIPadDialog::convert(QString filePath)
{
    QString tagFile = this->targetPath(filePath);
    QString suffix = this->suffix(tagFile);

    int index = tagFile.lastIndexOf("/");
    QString dirPath = tagFile.left(index);
    if( this->createDir(dirPath) == false)
    {
        qDebug()<<dirPath;
        Q_ASSERT(0);
    }

    ui->textBrowser->append(suffix);

    if(suffix == "ui")
    {
        this->convertUI(filePath, tagFile);
    }
    else if(suffix == "plist")
    {
        //this->convertPlist(filePath, tagFile);
    }
    else if(suffix == "xml")
    {
        this->convertXml(filePath, tagFile);
    }
    else if(suffix == "png")
    {
        //this->convertPng(filePath, tagFile);
    }

    ui->textBrowser->append(tagFile);
}

void QIPhoneToIPadDialog::convertUI(QString srcPath, QString tagPath)
{
    //QFile::copy(srcPath, tagPath);

    QFile file(srcPath);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        return ;
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

    file.setFileName(tagPath);
    if( file.open(QIODevice::WriteOnly) == false)
    {
        return ;
    }

    in.setDevice(&file);
    in.setCodec("UTF-8");

    int srcX = 0;
    int srcY = 0;
    int tagX = 0;
    int tagY = 0;
    bool isCanConvert = false;

    for(int i = 0; i < lines.size(); ++i)
    {
        QString str = lines.at(i);

        if(str.contains("<layer") == true)
        {
            str.remove("isHD=\"true\"");

            int index = str.indexOf(" width=\"");
            int index1 = str.indexOf("\"", index);
            int index2 = str.indexOf("\"", index1 + 1);
            int index3 = str.indexOf("\"", index2 + 1);
            int index4 = str.indexOf("\"", index3 + 1);

            srcX = str.mid(index1+1, index2 - index1 - 1).toInt();
            srcY = str.mid(index3+1, index4 - index3 - 1).toInt();

            if( srcX == 480 && srcY == 320 )
            {
                tagX = 1024;
                tagY = 768;
                isCanConvert = true;
            }
            else if(srcX == 320 && srcY == 480)
            {
                tagX = 768;
                tagY = 1024;
                isCanConvert = true;
            }
        }

        if(str.contains("=\"iPhone/") == true)
        {
            str.replace("=\"iPhone/", "=\"iPad/");
        }

        if(isCanConvert == true)
        {
            if(str.contains(" x=\"") == true)
            {
                int index = str.indexOf(" x=\"");

                if(index == -1)
                {
                    continue;
                }

                int index1 = str.indexOf("\"", index);
                int index2 = str.indexOf("\"", index1 + 1);

                int x = str.mid(index1+1, index2 - index1 - 1).toInt();
                x = (x - srcX)*tagX/srcX + tagX;
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

                int y = str.mid(index1+1, index2 - index1 - 1).toInt();
                y = (y - srcY)*tagY/srcY + tagY;
                QString yy = QString("%1").arg(y);
                str.replace(index1+1, index2 - index1 - 1, yy);
            }

            if(str.contains(" width=\"") == true)
            {
                int index = str.indexOf(" width=\"");

                if(index == -1)
                {
                    continue;
                }

                int index1 = str.indexOf("\"", index);
                int index2 = str.indexOf("\"", index1 + 1);

                int x = str.mid(index1+1, index2 - index1 - 1).toInt();
                x *= tagX*1.0f/srcX;
                QString xx = QString("%1").arg(x);
                str.replace(index1+1, index2 - index1 - 1, xx);
            }

            if(str.contains(" height=\"") == true)
            {
                int index = str.indexOf(" height=\"");

                if(index == -1)
                {
                    continue;
                }

                int index1 = str.indexOf("\"", index);
                int index2 = str.indexOf("\"", index1 + 1);

                int y = str.mid(index1+1, index2 - index1 - 1).toInt();
                y *= tagY*1.0f/srcY;
                QString yy = QString("%1").arg(y);
                str.replace(index1+1, index2 - index1 - 1, yy);
            }
        }

        in<<str<<"\n";
    }

    file.close();
}

void QIPhoneToIPadDialog::convertXml(QString srcPath, QString tagPath)
{
    QFile::copy(srcPath, tagPath);

//    if(srcPath.contains("animation.xml") == true)
//    {
//        return this->replaceAnimationPath(tagPath);
//    }
}

void QIPhoneToIPadDialog::convertPlist(QString srcPath, QString tagPath)
{
    float scale = 0.5f;
    QFile file(srcPath);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        return ;
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

    file.setFileName(tagPath);
    if( file.open(QIODevice::WriteOnly) == false)
    {
        return ;
    }

    in.setDevice(&file);
    in.setCodec("UTF-8");
    bool isValueChanged = false;

    for(int i = 0; i < lines.size(); ++i)
    {
        QString str = lines.at(i);

        if(isValueChanged == true)
        {
            isValueChanged = false;
            int index1 = str.indexOf(">");
            int index2 = str.indexOf("<", index1);
            int value = str.mid(index1 + 1, index2 - index1 - 1).toInt();
            if(value != -1)
            {
                str.replace(index1 + 1, index2 - index1 - 1, QString("%1").arg(value*scale));
            }
        }
        else if( (str.contains("<key>x</key>")) ||
                 (str.contains("<key>y</key>")) ||
                 (str.contains("<key>width</key>")) ||
                 (str.contains("<key>height</key>")) ||
                 (str.contains("<key>offsetX</key>")) ||
                 (str.contains("<key>offsetY</key>")) ||
                 (str.contains("<key>originalWidth</key>")) ||
                 (str.contains("<key>originalHeight</key>")) )
        {
            isValueChanged = true;
        }

        in<<str<<"\n";
    }

    file.close();
}

void QIPhoneToIPadDialog::convertPng(QString srcPath, QString tagPath)
{
    float scale = 0.5f;
    QString fileName = srcPath;

    if(QFile::exists(fileName) == true)
    {
        if(fileName.contains(".png") == true)
        {
            QPixmap image(fileName, "PNG");

            int w = image.width()*scale;
            int h = image.height()*scale;

            QPixmap newImage = image.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
            newImage.save(tagPath, "PNG", 1);
        }
    }
}

// /Users/luxiaodong/Code/CatchGhost/CatchGhost/Resources

QString QIPhoneToIPadDialog::suffix(QString filePath)
{
    QFileInfo info(filePath);
    return info.suffix();
}

bool QIPhoneToIPadDialog::createDir(QString filePath)
{
    if(filePath.isEmpty() == true)
    {
        return false;
    }

    if(filePath.contains(".") == true)
    {
        return false;
    }

    QDir dir(filePath);
    if(dir.exists() == true)
    {
        return true;
    }

    return dir.mkdir(filePath);
}

QString QIPhoneToIPadDialog::targetPath(QString srcPath)
{
    QString srcFilePath = srcPath;
    QString tagFilePath = srcPath;

    if(srcFilePath.contains("iPhone/") == true)
    {
        tagFilePath.replace("iPhone/", "iPad/");
    }

    return tagFilePath;
}

void QIPhoneToIPadDialog::traverseDir(QString rootDir)
{
    QDir dir(rootDir);

    QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries |  QDir::NoSymLinks | QDir::NoDotAndDotDot);

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.exists())
        {
            if(fileInfo.isDir())
            {
                QString filePath = fileInfo.absoluteFilePath();
                m_allDirs.append(filePath);
                this->traverseDir(filePath);
            }
            else if(fileInfo.isFile())
            {
                QString filePath = fileInfo.absoluteFilePath();
                QString temp = filePath;
                temp.remove( m_resourceRootPath + QString("/"));
                QStringList tempList = temp.split("/");

                if(tempList.size() == 1)
                {
                    continue;
                }

                QString first = tempList.first();
                if( first == "iPhone" )
                {
                    if( (filePath.contains(".ui") == true) ||
                        (filePath.contains(".xml")) == true )
                    {
                        m_allFiles.append(filePath);
                    }
                }
            }
        }
    }
}

