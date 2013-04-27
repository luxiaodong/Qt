#include "qrenamedialog.h"
#include "ui_qrenamedialog.h"

#include <QUrl>
#include <QList>
#include <QString>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QMimeData>

QRenameDialog::QRenameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRenameDialog)
{
    ui->setupUi(this);
    ui->selectAll->setChecked(true);

    m_fileSuffix = ui->suffix->text();

    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOK()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clickClear()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(clickQuit()));
    connect(ui->selectAll,SIGNAL(stateChanged(int)),this,SLOT(selectAll(int)));
    connect(ui->browse, SIGNAL(clicked()), this, SLOT(clickBrowse()));

    this->setAcceptDrops(true);
}

QRenameDialog::~QRenameDialog()
{
    delete ui;
}

void QRenameDialog::clickBrowse()
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

void QRenameDialog::clickOK()
{
    int size = m_fileList.size();
    for(int i = 0; i < size; ++i)
    {
        if(ui->listWidget->item(i)->checkState() == Qt::Checked)
        {
            if(this->rename( m_fileList.at(i) ) == true)
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

bool QRenameDialog::rename(QString fileName)
{
    bool add = ui->add->isChecked();
    bool cut = ui->cut->isChecked();
    bool head = ui->head->isChecked();
    bool nail = ui->nail->isChecked();

    if((add ^ cut) == false)
    {
        return false;
    }

    if( (head ^ nail) == false)
    {
        return false;
    }

    int lastIndex = fileName.lastIndexOf("/");
    QString filePath = fileName.left(lastIndex);
    QString fileShortName = fileName;
    fileShortName.remove(filePath);

    fileShortName.remove(m_fileSuffix);
    if(add == true)
    {
        if(nail == true)
        {
            fileShortName += ui->key->text();
        }
        else
        {
            fileShortName = ui->key->text() + fileShortName;
        }
    }
    else
    {
        int size = ui->key->text().size();
        if(nail == true)
        {
            if(fileShortName.right(size) == ui->key->text())
            {
                fileShortName.remove(ui->key->text());
            }
        }
        else
        {
            if(fileShortName.left(size) == ui->key->text())
            {
                fileShortName.remove(ui->key->text());
            }
        }
    }

    QString copyFileName = filePath + QString("/") + fileShortName + m_fileSuffix;
    QFile::rename(fileName,copyFileName);
    return true;
}

void QRenameDialog::clickQuit()
{
    this->reject();
}

void QRenameDialog::clickClear()
{
    ui->listWidget->clear();
    m_fileList.clear();
}

void QRenameDialog::selectAll(int state)
{
    int size = ui->listWidget->count();
    for(int i = 0; i < size; ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState((Qt::CheckState)state);
    }
}

void QRenameDialog::createModel()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(this->m_fileList);
    this->selectAll(ui->selectAll->checkState());
}

void QRenameDialog::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void QRenameDialog::dropEvent(QDropEvent* event)
{
    m_fileSuffix = ui->suffix->text();

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
                if(localFile.contains(m_fileSuffix) )
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

void QRenameDialog::traverseFiles(QString dirPath)
{
    m_fileSuffix = ui->suffix->text();

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

                if( (filePath.contains(m_fileSuffix) == true) )
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

