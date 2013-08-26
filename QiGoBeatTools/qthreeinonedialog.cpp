#include "qthreeinonedialog.h"
#include "ui_qthreeinonedialog.h"

#include <QList>
#include <QUrl>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <qDebug>
#include <QMimeData>

#include "qpackagesongs.h"
#include "qsildenodeextranumber.h"

QThreeInOneDialog::QThreeInOneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QThreeInOneDialog)
{
    ui->setupUi(this);
    ui->selectAll->setChecked(true);

    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOK()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clickClear()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(clickQuit()));
    connect(ui->selectAll,SIGNAL(stateChanged(int)),this,SLOT(selectAll(int)));

    this->setAcceptDrops(true);
}

QThreeInOneDialog::~QThreeInOneDialog()
{
    delete ui;
}

void QThreeInOneDialog::clickOK()
{
    if(ui->lineEdit->text().isEmpty() == true)
    {
        QMessageBox::information(this,"3In1Dialog", "iGB1_ is empty");
        return ;
    }

    QString path = m_dirPath;
    path = path.remove(path.size() - 1, 1);
    QString oldFolderString = path.split("/").last();
    QString newFolderString = QString("iGB2_%1").arg(ui->lineEdit->text());
    int index1 = path.lastIndexOf("/");
    QString newDirPath = QString("%1/%2/").arg(path.left(index1), newFolderString);

    QDir tempDir(newDirPath);

    if(tempDir.exists() == true)
    {
        QMessageBox::warning(this,"3In1Dialog", "The dir has been exist");
        return ;
    }

    QString nodeCoverPng = QString("");
    QString nodeInfoPng = QString("");
    QString nodeInfoPlist = QString("");
    QString nodeMp3 = QString("");
    QString unKnowMp3 = QString("");
    QString easyPlist = QString("");
    QString normalPlist = QString("");
    QString hardPlist = QString("");

    QDir dir(m_dirPath);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        QString fileSuffix = fileInfo.suffix().toLower();
        QString fileName = fileInfo.fileName();

        if(fileSuffix == QString("mp3"))
        {
            if(fileName == QString("%1.mp3").arg(oldFolderString))
            {
                nodeMp3 = fileName;
            }
            else
            {
                unKnowMp3 = fileName;
            }
        }
        else if(fileSuffix == QString("png"))
        {
            if(fileName == QString("%1_Cover.png").arg(oldFolderString))
            {
                nodeCoverPng = fileName;
            }
            else if(fileName == QString("%1_Info.png").arg(oldFolderString))
            {
                nodeInfoPng = fileName;
            }
        }
        else if(fileSuffix == QString("plist"))
        {
            if(fileName == QString("%1_Info.plist").arg(oldFolderString))
            {
                nodeInfoPlist = fileName;
            }
            else if(fileName == QString("%1_E.plist").arg(oldFolderString))
            {
                easyPlist = fileName;
            }
            else if(fileName == QString("%1_N.plist").arg(oldFolderString))
            {
                normalPlist = fileName;
            }
            else if(fileName == QString("%1_H.plist").arg(oldFolderString))
            {
                hardPlist = fileName;
            }
        }
    }

    if( nodeCoverPng.isEmpty() ||
        nodeInfoPng.isEmpty() ||
        nodeInfoPlist.isEmpty())
    {
        QString errorString = QString();
        if(nodeCoverPng.isEmpty())
        {
            errorString = QString("can't find _Cover.png");
        }
        else if(nodeInfoPng.isEmpty())
        {
            errorString = QString("can't find _Info.png");
        }
        else if(nodeInfoPlist.isEmpty())
        {
            errorString = QString("can't find _Info.plist");
        }

        QMessageBox::information(this,"3In1Dialog", errorString);
        return ;
    }

    QPackageSongs packageSongs;
    if(nodeMp3.isEmpty() == false || unKnowMp3.isEmpty() == false)
    {
        packageSongs.changeMediaNameInPlist(QString("%1%2").arg(m_dirPath, nodeInfoPlist), newFolderString, true);
    }
    else
    {
        packageSongs.changeMediaNameInPlist(QString("%1%2").arg(m_dirPath, nodeInfoPlist), newFolderString, false);
    }

    QSildeNodeExtraNumber sildeNodeExtra;
    if(easyPlist.isEmpty() == false)
    {
        sildeNodeExtra.parseNodes(QString("%1%2").arg(m_dirPath, easyPlist));
    }

    if(normalPlist.isEmpty() == false)
    {
        sildeNodeExtra.parseNodes(QString("%1%2").arg(m_dirPath, normalPlist));
    }

    if(hardPlist.isEmpty() == false)
    {
        sildeNodeExtra.parseNodes(QString("%1%2").arg(m_dirPath, hardPlist));
    }

    if(nodeMp3.isEmpty() == false && unKnowMp3.isEmpty() == false)
    {
        QFile::remove(QString("%1%2").arg(m_dirPath, nodeMp3));
    }

    if(unKnowMp3.isEmpty() == false)
    {
        QFile::rename(QString("%1%2").arg(m_dirPath, unKnowMp3), QString("%1%2.mp3").arg(m_dirPath, newFolderString));
    }

    //qDebug()<<m_dirPath;

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        QString fileName = fileInfo.fileName();
        QString newFileName = fileName;
        newFileName.replace(oldFolderString, newFolderString);
        //qDebug()<<fileName<<oldFolderString<<newFolderString;
        QFile::rename(QString("%1%2").arg(m_dirPath,fileName), QString("%1%2").arg(m_dirPath,newFileName));
    }

    dir.rename(m_dirPath, newDirPath);
    m_dirPath = newDirPath;

    QProcess proc;
    int index2 = newDirPath.remove(newDirPath.size() - 1, 1).lastIndexOf("/");
    proc.setWorkingDirectory(m_dirPath.left(index2));
    QStringList arguments;
    QString fullStr = QString("");
    if(nodeMp3.isEmpty() == false || unKnowMp3.isEmpty() == false)
    {
        fullStr = QString("_Full");
    }
    arguments << "-r" << newFolderString + fullStr + QString(".zip") << newFolderString;
    proc.start("zip", arguments);

    if (!proc.waitForStarted())
    {
        qDebug()<<"启动失败\n";
    }

    proc.closeWriteChannel();

    while (false == proc.waitForFinished())
    {
       ;
    }

    QMessageBox::information(this,"3In1Dialog", "love is over");
}

void QThreeInOneDialog::clickQuit()
{
    this->reject();
}

void QThreeInOneDialog::clickClear()
{
    ui->listWidget->clear();
    m_dirPath.clear();
}

void QThreeInOneDialog::selectAll(int state)
{
    int size = ui->listWidget->count();
    for(int i = 0; i < size; ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState((Qt::CheckState)state);
    }
}

void QThreeInOneDialog::createModel()
{
    ui->listWidget->clear();
    ui->listWidget->addItem(this->m_dirPath);
    this->selectAll(ui->selectAll->checkState());
}

void QThreeInOneDialog::dropEvent( QDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void QThreeInOneDialog::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls() == true)
    {
        QList<QUrl> urls = event->mimeData()->urls();

        if (urls.isEmpty())
        {
            return;
        }

        m_dirPath = urls.first().toLocalFile();
        this->createModel();
    }
}


