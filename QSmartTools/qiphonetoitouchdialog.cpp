#include "qiphonetoitouchdialog.h"
#include "ui_qiphonetoitouchdialog.h"

#include <QFileDialog>
#include <QPixmap>
#include <QDebug>

QIPhoneToITouchDialog::QIPhoneToITouchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QIPhoneToITouchDialog)
{
    ui->setupUi(this);

    connect(ui->resourcePath, SIGNAL(clicked()), this, SLOT(clickResourcePath()));
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOK()));
}

QIPhoneToITouchDialog::~QIPhoneToITouchDialog()
{
    delete ui;
}

void QIPhoneToITouchDialog::clickResourcePath()
{
    QString filePath = QFileDialog::getExistingDirectory(this);

    if(filePath.isEmpty() == true)
    {
        return ;
    }

    ui->lineEdit->setText(filePath);
}

void QIPhoneToITouchDialog::clickOK()
{
    QString filePath = ui->lineEdit->text();

    if(filePath.isEmpty() == true)
    {
        return ;
    }

    m_resourceRootPath = filePath;

    QString iTouchPath = m_resourceRootPath + "/iTouch";
    QDir dir(iTouchPath);
    if(dir.exists() == false)
    {
        dir.mkdir(iTouchPath);
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

void QIPhoneToITouchDialog::convert(QString filePath)
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
        this->convertPlist(filePath, tagFile);
    }
    else if(suffix == "xml")
    {
        this->convertXml(filePath, tagFile);
    }
    else if(suffix == "png")
    {
        this->convertPng(filePath, tagFile);
    }

    ui->textBrowser->append(tagFile);
}

void QIPhoneToITouchDialog::convertUI(QString srcPath, QString tagPath)
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

    for(int i = 0; i < lines.size(); ++i)
    {
        QString str = lines.at(i);

        if(str.contains("<layer") == true)
        {
            str.remove("isHD=\"true\"");
        }

        if(str.contains("=\"iPhone/") == true)
        {
            str.replace("=\"iPhone/", "=\"iTouch/");
        }

        if(str.contains("=\"picture/") == true)
        {
            str.replace("=\"picture/", "=\"iTouch/picture/");
        }

        in<<str<<"\n";
    }

    file.close();
}

void QIPhoneToITouchDialog::convertXml(QString srcPath, QString tagPath)
{
    QFile::copy(srcPath, tagPath);

    if(srcPath.contains("animation.xml") == true)
    {
        return this->replaceAnimationPath(tagPath);
    }
}

void QIPhoneToITouchDialog::convertPlist(QString srcPath, QString tagPath)
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

void QIPhoneToITouchDialog::convertPng(QString srcPath, QString tagPath)
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

void QIPhoneToITouchDialog::replaceAnimationPath(QString fileName)
{
    QFile file(fileName);
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
    file.setFileName(fileName);
    if( file.open(QIODevice::WriteOnly) == false)
    {
        return ;
    }

    in.setDevice(&file);
    in.setCodec("UTF-8");
    for(int i = 0; i < lines.size(); ++i)
    {
        QString str = lines.at(i);
        if(str.contains("animation/") == true)
        {
            str.replace("animation/", "iTouch/animation/");
        }
        in<<str<<"\n";
    }
    file.close();
}

// /Users/luxiaodong/Code/CatchGhost/CatchGhost/Resources

QString QIPhoneToITouchDialog::suffix(QString filePath)
{
    QFileInfo info(filePath);
    return info.suffix();
}

bool QIPhoneToITouchDialog::createDir(QString filePath)
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

QString QIPhoneToITouchDialog::targetPath(QString srcPath)
{
    QString srcFilePath = srcPath;
    QString tagFilePath = srcPath;

    if(srcFilePath.contains("animation/") == true)
    {
        tagFilePath.replace("animation/", "iTouch/animation/");
    }
    else if(srcFilePath.contains("iPhone/") == true)
    {
        tagFilePath.replace("iPhone/", "iTouch/");
    }
    else if(srcFilePath.contains("picture/") && (!srcFilePath.contains("iPhone/")))
    {
        tagFilePath.replace("picture/", "iTouch/picture/");
    }

    return tagFilePath;
}

void QIPhoneToITouchDialog::traverseDir(QString rootDir)
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
                if( (first == "animation") ||
                    (first == "iPhone") ||
                    (first == "picture"))
                {
                    if( (filePath.contains(".ui") == true) ||
                        (filePath.contains(".xml")) == true ||
                        (filePath.contains(".png") == true) ||
                        (filePath.contains(".plist") == true))
                    {
                        m_allFiles.append(filePath);
                    }
                }
            }
        }
    }
}
