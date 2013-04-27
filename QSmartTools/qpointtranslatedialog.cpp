#include "qpointtranslatedialog.h"
#include "ui_qpointtranslatedialog.h"

#include <QUrl>
#include <QList>
#include <QString>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QMimeData>

QPointTranslateDialog::QPointTranslateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPointTranslateDialog)
{
    ui->setupUi(this);
    ui->selectAll->setChecked(true);

    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOK()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clickClear()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(clickQuit()));
    connect(ui->selectAll,SIGNAL(stateChanged(int)),this,SLOT(selectAll(int)));
    connect(ui->browse, SIGNAL(clicked()), this, SLOT(clickBrowse()));

    this->setAcceptDrops(true);

    m_fileSuffix = QString(".ui");
    m_fileSuffix2 = QString(".xml");
}

QPointTranslateDialog::~QPointTranslateDialog()
{
    delete ui;
}

void QPointTranslateDialog::clickBrowse()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(dirPath.isEmpty())
    {
        return ;
    }

    ui->lineEdit->setText(dirPath);
    this->traverseFiles(dirPath);
    this->createModel();
}

void QPointTranslateDialog::clickOK()
{
    int size = m_fileList.size();
    for(int i = 0; i < size; ++i)
    {
        if(ui->listWidget->item(i)->checkState() == Qt::Checked)
        {
            if(this->convertPoint( m_fileList.at(i) ) == true)
            {
                ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
            }
            else
            {
                break;
            }
        }
    }
}

bool QPointTranslateDialog::convertPoint(QString fileName)
{
    int srcX = ui->srcX->value()/2;
    int srcY = ui->srcY->value()/2;
    int tagX = ui->tagX->value()/2;
    int tagY = ui->tagY->value()/2;

    if(srcX == 0 || srcY == 0 || tagX == 0 || tagY == 0)
    {
        return false;
    }

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly) == false)
    {
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

            y = str.mid(index1+1, index2 - index1 - 1).toInt();
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

            x = str.mid(index1+1, index2 - index1 - 1).toInt();
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

            y = str.mid(index1+1, index2 - index1 - 1).toInt();
            y *= tagY*1.0f/srcY;
            QString yy = QString("%1").arg(y);
            str.replace(index1+1, index2 - index1 - 1, yy);
        }

        if(str.contains(" fontSize=\"") == true)
        {
            float m;
            if((srcX == 512 && tagX == 240) ||
               (srcX == 384  && tagX == 160))
            {
                m = 0.5f;
            }
            else if((srcX == 240  && tagX == 512) ||
                    (srcX == 160  && tagX == 384 ))
            {
                m = 2.0f;
            }
            else
            {
                m = tagY*1.0f/srcY;;
            }

            int index = str.indexOf(" fontSize=\"");

            if(index == -1)
            {
                continue;
            }
            int index1 = str.indexOf("\"", index);
            int index2 = str.indexOf("\"", index1 + 1);

            y = str.mid(index1+1, index2 - index1 - 1).toInt();

            y *= m;
            QString yy = QString("%1").arg(y);
            str.replace(index1+1, index2 - index1 - 1, yy);
        }

        if(str.contains("<label") && !str.contains(" fontSize=\"") && ( tagX == 240 || tagY ==240 ) )
        {
            int index1 = str.indexOf("/>");
            str.replace(index1 , 2 , "fontSize=\"12\" />");
        }

        in<<str<<"\n";
    }
    file.close();
    return true;
}

void QPointTranslateDialog::clickQuit()
{
    this->reject();
}

void QPointTranslateDialog::clickClear()
{
    ui->listWidget->clear();
    m_fileList.clear();
}

void QPointTranslateDialog::selectAll(int state)
{
    int size = ui->listWidget->count();
    for(int i = 0; i < size; ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState((Qt::CheckState)state);
    }
}

void QPointTranslateDialog::createModel()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(this->m_fileList);
    this->selectAll(ui->selectAll->checkState());
}

void QPointTranslateDialog::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void QPointTranslateDialog::dropEvent(QDropEvent* event)
{
    if(event->mimeData()->hasUrls() == true)
    {
        QList<QUrl> urls = event->mimeData()->urls();

        if (urls.isEmpty())
        {
            return;
        }

        QString dirPath = urls.first().toLocalFile();

        QDir dir(dirPath);
        if(dir.exists() == true)
        {
            ui->lineEdit->setText(dirPath);
            this->traverseFiles(dirPath);
        }
        else
        {
            for(int i = 0; i < urls.size(); ++i)
            {
                QString localFile = urls.at(i).toLocalFile();
                if(localFile.contains(m_fileSuffix) || localFile.contains(m_fileSuffix2))
                {
                    if(m_fileList.indexOf(localFile) == -1)
                    {
                        m_fileList.append(localFile);
                    }
                }
            }
        }

        this->createModel();
    }
}

void QPointTranslateDialog::traverseFiles(QString dirPath)
{
    QDir dir(dirPath);

    QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.exists())
        {
            if(fileInfo.isDir())
            {
                this->traverseFiles(fileInfo.absoluteFilePath());
            }
            else if(fileInfo.isFile())
            {
                QString filePath = fileInfo.absoluteFilePath();

                if( (filePath.contains(m_fileSuffix) == true) || (filePath.contains(m_fileSuffix2) == true) )
                {
                    if(m_fileList.indexOf(filePath) == -1)
                    {
                        m_fileList.append(filePath);
                    }
                }
            }
        }
    }
}
