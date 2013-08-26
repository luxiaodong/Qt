#include "qencryptiondialog.h"
#include "ui_qencryptiondialog.h"
#include "qencryption.h"

#include <QUrl>
#include <QList>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QMimeData>

QEncryptionDialog::QEncryptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QEncryptionDialog)
{
    ui->setupUi(this);
    ui->selectAll->setChecked(true);

    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOK()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clickClear()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(clickQuit()));
    connect(ui->selectAll,SIGNAL(stateChanged(int)),this,SLOT(selectAll(int)));

    this->setAcceptDrops(true);
}

QEncryptionDialog::~QEncryptionDialog()
{
    delete ui;
}

void QEncryptionDialog::clickOK()
{
    int size = m_fileList.size();
    QEncryption encryption;
    for(int i = 0; i < size; ++i)
    {
        if(ui->listWidget->item(i)->checkState() == Qt::Checked)
        {
            if(encryption.codeFile(m_fileList.at(i)) == true)
            {
                ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
            }
            else
            {
                break;
            }
        }
    }

    QMessageBox::information(this,"EncryptionDialog", encryption.m_lastError);
}

void QEncryptionDialog::clickQuit()
{
    this->reject();
}

void QEncryptionDialog::clickClear()
{
    ui->listWidget->clear();
    m_fileList.clear();
}

void QEncryptionDialog::selectAll(int state)
{
    int size = ui->listWidget->count();
    for(int i = 0; i < size; ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState((Qt::CheckState)state);
    }
}

void QEncryptionDialog::createModel()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(this->m_fileList);
    this->selectAll(ui->selectAll->checkState());
}

void QEncryptionDialog::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void QEncryptionDialog::dropEvent(QDropEvent* event)
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
            this->traverseFiles(dirPath);
        }
        else
        {
            for(int i = 0; i < urls.size(); ++i)
            {
                QString localFile = urls.at(i).toLocalFile();
                if(localFile.contains(".plist"))
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

void QEncryptionDialog::traverseFiles(QString dirPath)
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
                if( (filePath.contains(".plist") == true) )
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

