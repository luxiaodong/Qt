#include "qbmsetoigobeatdialog.h"
#include "ui_qbmsetoigobeatdialog.h"

#include <QUrl>
#include <QList>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QMimeData>

#include "QParseBsm.h"

QBmseToIGoBeatDialog::QBmseToIGoBeatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QBmseToIGoBeatDialog)
{
    ui->setupUi(this);
    ui->selectAll->setChecked(true);

    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOK()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clickClear()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(clickQuit()));
    connect(ui->selectAll,SIGNAL(stateChanged(int)),this,SLOT(selectAll(int)));

    this->setAcceptDrops(true);
}

QBmseToIGoBeatDialog::~QBmseToIGoBeatDialog()
{
    delete ui;
}

void QBmseToIGoBeatDialog::clickOK()
{
    int size = m_fileList.size();
    for(int i = 0; i < size; ++i)
    {
        if(ui->listWidget->item(i)->checkState() == Qt::Checked)
        {
            QParseBsm bsm;
            bsm.praseBsmFile(m_fileList.at(i));
            bsm.createBsmNodeTree();
            bsm.createNodeFile(ui->spinBox->value());
            ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
        }
    }

    QMessageBox::information(this,"bmseToIGoBeatDialog", "love is over");
}

void QBmseToIGoBeatDialog::clickQuit()
{
    this->reject();
}

void QBmseToIGoBeatDialog::clickClear()
{
    ui->listWidget->clear();
    m_fileList.clear();
}

void QBmseToIGoBeatDialog::selectAll(int state)
{
    int size = ui->listWidget->count();
    for(int i = 0; i < size; ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState((Qt::CheckState)state);
    }
}

void QBmseToIGoBeatDialog::createModel()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(this->m_fileList);
    this->selectAll(ui->selectAll->checkState());
}

void QBmseToIGoBeatDialog::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void QBmseToIGoBeatDialog::dropEvent(QDropEvent* event)
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
                if(localFile.contains(".bms"))
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

void QBmseToIGoBeatDialog::traverseFiles(QString dirPath)
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

                if((filePath.contains(".bms") == true))
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

