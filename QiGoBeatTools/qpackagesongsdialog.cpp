#include "qpackagesongsdialog.h"
#include "ui_qpackagesongsdialog.h"

#include <QList>
#include <QUrl>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <qDebug>
#include "qpackagesongs.h"
#include <QDropEvent>
#include <QMimeData>

QPackageSongsDialog::QPackageSongsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPackageSongsDialog)
{
    ui->setupUi(this);
    ui->selectAll->setChecked(true);

    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOK()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clickClear()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(clickQuit()));
    connect(ui->selectAll,SIGNAL(stateChanged(int)),this,SLOT(selectAll(int)));

    this->setAcceptDrops(true);
}

QPackageSongsDialog::~QPackageSongsDialog()
{
    delete ui;
}

void QPackageSongsDialog::clickOK()
{
    int size = m_dirPathlist.size();
    QPackageSongs packageSongs;
    for(int i = 0; i < size; ++i)
    {
        if(ui->listWidget->item(i)->checkState() == Qt::Checked)
        {
            if(packageSongs.packageSong(m_dirPathlist.at(i)) == true)
            {
                ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
                //package zip.
                QString path = m_dirPathlist.at(i);
                path.remove(path.size() - 1, 1);
                QString name = path.split("/").last();
                QString workPath = path;
                int index1 = path.lastIndexOf("/");
                QProcess proc;
                proc.setWorkingDirectory(workPath.left(index1));
                QStringList arguments;
                arguments << "-r" << name + "_Full.zip" << name;
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
            }
            else
            {
                break;
            }
        }
    }

    QMessageBox::information(this,"packageSongsDialog", packageSongs.m_lastError);
}

void QPackageSongsDialog::clickQuit()
{
    this->reject();
}

void QPackageSongsDialog::clickClear()
{
    ui->listWidget->clear();
    m_dirPathlist.clear();
}

void QPackageSongsDialog::selectAll(int state)
{
    int size = ui->listWidget->count();
    for(int i = 0; i < size; ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState((Qt::CheckState)state);
    }
}

void QPackageSongsDialog::createModel()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(this->m_dirPathlist);
    this->selectAll(ui->selectAll->checkState());
}

void QPackageSongsDialog::dropEvent( QDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void QPackageSongsDialog::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls() == true)
    {
        QList<QUrl> urls = event->mimeData()->urls();

        if (urls.isEmpty())
        {
            return;
        }

        foreach(QUrl url, urls)
        {
            QString dirPath = url.toLocalFile();

            QDir dir(dirPath);
            if(dir.exists() == true)
            {
                if(m_dirPathlist.indexOf(dirPath) == -1)
                {
                    m_dirPathlist.append(dirPath);
                }
            }
        }

        this->createModel();
    }
}

