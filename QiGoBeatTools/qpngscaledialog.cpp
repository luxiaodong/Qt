#include "qpngscaledialog.h"
#include "ui_qpngscaledialog.h"

#include <QUrl>
#include <QList>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include "qpngscale.h"
#include <QMimeData>

QPngScaleDialog::QPngScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPngScaleDialog)
{
    ui->setupUi(this);
    ui->selectAll->setChecked(true);

    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOK()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clickClear()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(clickQuit()));
    connect(ui->selectAll,SIGNAL(stateChanged(int)),this,SLOT(selectAll(int)));

    this->setAcceptDrops(true);
}

QPngScaleDialog::~QPngScaleDialog()
{
    delete ui;
}

void QPngScaleDialog::clickOK()
{
    int size = m_fileList.size();
    QPngScale pngScale;
    pngScale.m_scale = ui->doubleSpinBox->value();
    for(int i = 0; i < size; ++i)
    {
        if(ui->listWidget->item(i)->checkState() == Qt::Checked)
        {
            if(pngScale.scale(m_fileList.at(i)) == true)
            {
                ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
            }
            else
            {
                break;
            }
        }
    }

    QMessageBox::information(this,"pngSacleDialog", pngScale.m_lastError);
}

void QPngScaleDialog::clickQuit()
{
    this->reject();
}

void QPngScaleDialog::clickClear()
{
    ui->listWidget->clear();
    m_fileList.clear();
}

void QPngScaleDialog::selectAll(int state)
{
    int size = ui->listWidget->count();
    for(int i = 0; i < size; ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState((Qt::CheckState)state);
    }
}

void QPngScaleDialog::createModel()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(this->m_fileList);
    this->selectAll(ui->selectAll->checkState());
}

void QPngScaleDialog::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void QPngScaleDialog::dropEvent(QDropEvent* event)
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
                if(localFile.contains(".plist") ||
                   localFile.contains(".png") )
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

void QPngScaleDialog::traverseFiles(QString dirPath)
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

                if((filePath.contains(".plist") == true) ||
                  (filePath.contains(".png") == true))
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

