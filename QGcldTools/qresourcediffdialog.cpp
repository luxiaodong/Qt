#include "qresourcediffdialog.h"
#include "ui_qresourcediffdialog.h"
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>

QResourceDiffDialog::QResourceDiffDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QResourceDiffDialog)
{
    ui->setupUi(this);

    connect(ui->browse1, SIGNAL(clicked()), this, SLOT(clickBrowse1()));
    connect(ui->browse2, SIGNAL(clicked()), this, SLOT(clickBrowse2()));
    connect(ui->start, SIGNAL(clicked()), this, SLOT(clickStart()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(clickQuit()));

    QSettings settings("QResourceDiffDialog");
    m_oneDir = settings.value("oneDir",QString("")).toString();
    m_otherDir = settings.value("otherDir",QString("")).toString();

    ui->oneDir->setText( m_oneDir );
    ui->otherDir->setText( m_otherDir );
}

QResourceDiffDialog::~QResourceDiffDialog()
{
    delete ui;
}

void QResourceDiffDialog::clickBrowse1()
{
    QString path = QFileDialog::getExistingDirectory(this, "browse", m_oneDir);
    if(path.isEmpty() == true)
    {
        return ;
    }

    m_oneDir = path;
    ui->oneDir->setText( m_oneDir );
    QSettings settings("QResourceDiffDialog");
    settings.setValue("oneDir",m_oneDir);
}

void QResourceDiffDialog::clickBrowse2()
{
    QString path = QFileDialog::getExistingDirectory(this, "browse", m_otherDir);
    if(path.isEmpty() == true)
    {
        return ;
    }

    m_otherDir = path;
    ui->otherDir->setText( m_otherDir );
    QSettings settings("QResourceDiffDialog");
    settings.setValue("otherDir", m_otherDir);
}

void QResourceDiffDialog::clickStart()
{
    if( this->loadOneRes2() == false || this->loadOtherRes2() == false )
    {
        QMessageBox::information(this,"information","miss res.lua");
        return ;
    }

    QStringList oneList = m_oneMap.keys();
    QStringList otherList = m_otherMap.keys();

    ui->textBrowser->clear();
    QString title = QString("%1 vs %2").arg(oneList.size()).arg(otherList.size());
    ui->textBrowser->append(title);

    int indexOne = 0;
    int indexOther = 0;
    int sizeOne = oneList.size();
    int sizeOther = otherList.size();
    int addNumber = 0;
    int modNumber = 0;
    int delNumber = 0;
    m_diffMap.clear();

    while(1)
    {
        if(indexOne == sizeOne && indexOther == sizeOther)
        {
            break;
        }

        if(indexOne == sizeOne)
        {
            QString otherStr = otherList.at(indexOther);
            m_diffMap.insert(otherStr, "A");
            ui->textBrowser->append(QString("%1 %2").arg("A    ", otherStr));
            indexOther++;
            addNumber++;
            continue;
        }

        if(indexOther == sizeOther)
        {
            QString oneStr = oneList.at(indexOne);
            m_diffMap.insert(oneStr, "D");
            ui->textBrowser->append(QString("%1 %2").arg("D    ", oneStr));
            indexOne++;
            delNumber++;
            continue;
        }

        QString oneStr = oneList.at(indexOne);
        QString otherStr = otherList.at(indexOther);

        int rtn = oneStr.compare(otherStr);
        if(rtn > 0)
        {
            m_diffMap.insert(otherStr, "A");
            ui->textBrowser->append(QString("%1 %2").arg("A    ", otherStr));
            indexOther++;
            addNumber++;
        }
        else if(rtn < 0)
        {
            m_diffMap.insert(oneStr, "D");
            indexOne++;
            delNumber++;
            ui->textBrowser->append(QString("%1 %2").arg("D    ", oneStr));
        }
        else
        {
            QString oneStrMd5 = m_oneMap.value(oneStr);
            QString otherStrMd5 = m_otherMap.value(otherStr);
            indexOne++;
            indexOther++;

            if(oneStrMd5 != otherStrMd5)
            {
                m_diffMap.insert(oneStr, "M");
                ui->textBrowser->append(QString("%1 %2").arg("M    ", oneStr));
                modNumber++;
            }
        }
    }

    ui->textBrowser->append( QString("--------------------------------------") );
    ui->textBrowser->append( QString("Add number:%1").arg(addNumber) );
    ui->textBrowser->append( QString("Del number:%1").arg(delNumber) );
    ui->textBrowser->append( QString("Mod number:%1").arg(modNumber) );

    qDebug()<<"love is over";
}

bool QResourceDiffDialog::loadOneRes2()
{
    m_oneMap.clear();
    QString filePath = QString("%1/res.lua").arg(m_oneDir);
    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QTextStream stream(&file);
    while(stream.atEnd() == false)
    {
        QString str = stream.readLine();
        if(str.contains("md5") == true)
        {
            QStringList tempList = str.split("\"");
            m_oneMap.insert(tempList.at(1), tempList.at(3) );
        }
    }

    file.close();
    return true;
}

bool QResourceDiffDialog::loadOtherRes2()
{
    m_otherMap.clear();
    QString filePath = QString("%1/res.lua").arg(m_otherDir);
    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QTextStream stream(&file);
    while(stream.atEnd() == false)
    {
        QString str = stream.readLine();
        if(str.contains("md5") == true)
        {
            QStringList tempList = str.split("\"");
            m_otherMap.insert(tempList.at(1), tempList.at(3) );
        }
    }

    file.close();
    return true;
}

bool QResourceDiffDialog::loadOneRes()
{
    m_oneList.clear();
    QDir dir(m_oneDir);
    QFileInfoList list = dir.entryInfoList();
    foreach(QFileInfo single, list)
    {
        QString name = single.baseName();
        if(name.isEmpty() == true || name == "version.lua" || name == "res.lua")
        {
            continue;
        }

        m_oneList.append(name);
    }

    m_oneList.sort();
    return true;
}

bool QResourceDiffDialog::loadOtherRes()
{
    m_otherList.clear();
    QDir dir(m_otherDir);
    QFileInfoList list = dir.entryInfoList();
    foreach(QFileInfo single, list)
    {
        QString name = single.baseName();
        if(name.isEmpty() == true || name == "version.lua" || name == "res.lua")
        {
            continue;
        }

        m_otherList.append(name);
    }

    m_otherList.sort();
    return true;
}

bool QResourceDiffDialog::compareTwoMap()
{
    return false;
}

void QResourceDiffDialog::clickQuit()
{
    this->reject();
}

