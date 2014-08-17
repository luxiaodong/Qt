#include "qbuildresourcedialog.h"
#include "ui_qbuildresourcedialog.h"
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QSettings>
#include <QMimeData>
#include <QRegExp>
#include <QDebug>
#include <QProcess>
#include <QMessageBox>
#include <QThread>
#include <QFile>
#include <QTextStream>

QBuildResourceDialog::QBuildResourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QBuildResourceDialog)
{
    ui->setupUi(this);
    connect(ui->browse, SIGNAL(pressed()), this, SLOT(click_browse()));
    connect(ui->start, SIGNAL(pressed()), this, SLOT(click_start()));
    connect(ui->quit, SIGNAL(pressed()), this, SLOT(click_quit()));
    connect(ui->selectAll, SIGNAL(clicked()), this, SLOT(click_selectAll()));
    connect(ui->disableAll, SIGNAL(clicked()), this, SLOT(click_disableAll()));
    connect(ui->filter, SIGNAL(textChanged(QString)), this, SLOT(click_filter(QString)));
    connect(ui->listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*)));
    connect(ui->path, SIGNAL(returnPressed()), this, SLOT(click_returnPressed()));

    this->setAcceptDrops(true);

    QSettings settings("QBuildResourceDialog");
    m_resourceDir = settings.value("resourceDir",QString("")).toString();
    this->createModel();
}

QBuildResourceDialog::~QBuildResourceDialog()
{
    delete ui;
}

void QBuildResourceDialog::click_browse()
{
    QString path = QFileDialog::getExistingDirectory(this, "browse", m_resourceDir);
    if(path.isEmpty() == true)
    {
        return ;
    }

    m_resourceDir = path;
    this->createModel();
}

void QBuildResourceDialog::click_returnPressed()
{
    m_resourceDir = ui->path->text();
    this->createModel();
}

void QBuildResourceDialog::click_start()
{
    int size = m_filterFileListMap.size();
    if(size < 1000)
    {
        QString dirPath = QCoreApplication::applicationDirPath();
        QString name = QCoreApplication::applicationName();
        QString path = dirPath.remove( QString("/%1.app/Contents/MacOS").arg(name) );

        QString resourceDir = m_resourceDir;
        if(resourceDir.right(1) == "/")
        {
            resourceDir = resourceDir.left( m_resourceDir.size() - 1 );
        }

        QStringList keys = m_filterFileListMap.keys();
        foreach(QString key, keys)
        {
            bool isChecked = m_filterFileListMap.value(key, false);
            if( isChecked == true )
            {
                QStringList args;
                args.append(QString("%1/packageTools/build_single_file.sh").arg(path));
                args.append( resourceDir );
                args.append( QString("%1/%2").arg(resourceDir, key));
                QProcess::execute("/bin/bash", args);
            }
        }
    }
    else
    {
        QString resourceDir = m_resourceDir;
        if(resourceDir.right(1) == "/")
        {
            resourceDir = resourceDir.left( m_resourceDir.size() - 1 );
        }

        int index = resourceDir.lastIndexOf('/');
        if(index == -1)
        {
            QMessageBox::warning(this, "build", "cant't find version.lua");
            return ;
        }

        QString versionFilePath = QString("%1/version.lua").arg(resourceDir.left(index) );
        qDebug()<<versionFilePath;
        QFile file(versionFilePath);
        if(file.open(QIODevice::ReadOnly) == false)
        {
            QMessageBox::warning(this, "build", "cant't open version.lua");
            return ;
        }

        QTextStream stream(&file);
        QStringList text = stream.readAll().split("\n");

        QMessageBox msgBox;
        msgBox.setText("请注意,整包和动更包版本不一样");
        msgBox.setInformativeText(QString("%1\n%2").arg(text.at(1), text.at(2)));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Cancel)
        {
            return ;
        }

        index = ui->country->currentIndex();
        QStringList countryList;
        countryList<<""<<"kr"<<"tw"<<"vi"<<"th";
        QString country = countryList[index];

        QString dirPath = QCoreApplication::applicationDirPath();
        QString name = QCoreApplication::applicationName();
        QString path = dirPath.remove( QString("/%1.app/Contents/MacOS").arg(name) );

        QStringList args;
        args.append(QString("%1/packageTools/build_resources_zip.sh").arg(path));
        args.append( resourceDir );
        args.append( country );
        QProcess::execute("/bin/bash", args);
    }
}

bool QBuildResourceDialog::isExistVersion()
{
    QString dirPath = QCoreApplication::applicationDirPath();
    QString name = QCoreApplication::applicationName();
    QString path = dirPath.remove( QString("/%1.app/Contents/MacOS").arg(name) );

    QStringList args;
    args.append(QString("%1/packageTools/checkVersion.sh").arg(path));
    args.append(QString("%1../version.lua").arg(m_resourceDir));

    if (QProcess::execute("/bin/bash", args) == 0)
    {
        return true;
    }

    return false;
}

void QBuildResourceDialog::click_quit()
{
    this->reject();
}

void QBuildResourceDialog::click_selectAll()
{
    QStringList fileList = m_filterFileListMap.keys();
    foreach(QString single, fileList)
    {
        m_filterFileListMap.insert(single, true);
    }

    QString filterString = ui->filter->text();
    this->click_filter(filterString);
}

void QBuildResourceDialog::click_disableAll()
{
    QStringList fileList = m_filterFileListMap.keys();
    foreach(QString single, fileList)
    {
        m_filterFileListMap.insert(single, false);
    }

    QString filterString = ui->filter->text();
    this->click_filter(filterString);
}

void QBuildResourceDialog::itemChanged(QListWidgetItem* item)
{
    QString key = item->text();
    Qt::CheckState state = item->checkState();
    m_fileListMap.insert(key, state);
    m_filterFileListMap.insert(key, state);
}

void QBuildResourceDialog::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void QBuildResourceDialog::dropEvent(QDropEvent* event)
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
            m_resourceDir = dirPath;
            this->createModel();
        }
    }
}

void QBuildResourceDialog::click_filter(QString filterString)
{
    if(filterString.isEmpty() == false)
    {
        QStringList fileList = m_fileListMap.keys();
        QStringList fileListAfterFilter = fileList.filter(filterString);

        m_filterFileListMap.clear();
        foreach (QString key, fileListAfterFilter)
        {
            m_filterFileListMap.insert( key,  m_fileListMap.value(key) );
        }
    }
    else
    {
        m_filterFileListMap = m_fileListMap;
    }

    ui->totalNumber->setText( QString("%1").arg(m_filterFileListMap.size()));
    ui->listWidget->clear();

    QStringList fileList = m_filterFileListMap.keys();
    foreach(QString single, fileList)
    {
        QListWidgetItem *item = new QListWidgetItem(single);
        bool isChecked = m_filterFileListMap.value(single, false);
        if (isChecked == true)
        {
            item->setCheckState( Qt::Checked );
        }
        else
        {
            item->setCheckState( Qt::Unchecked );
        }

        ui->listWidget->addItem(item);
    }
}

void QBuildResourceDialog::createModel()
{
    m_fileListMap.clear();
    this->traverseFiles(m_resourceDir);

    QString filterString = ui->filter->text();
    this->click_filter(filterString);

    ui->path->setText(m_resourceDir);

    QSettings settings("QBuildResourceDialog");
    settings.setValue("resourceDir", m_resourceDir);
}

void QBuildResourceDialog::traverseFiles(QString dirPath)
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
                m_fileListMap.insert( filePath.remove(m_resourceDir), true );
            }
        }
    }
}
