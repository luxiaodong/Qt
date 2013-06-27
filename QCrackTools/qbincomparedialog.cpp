#include "qbincomparedialog.h"
#include "ui_qbincomparedialog.h"

#include <QFile>
#include <QString>
#include <QDataStream>
#include <QFileDialog>

QBinCompareDialog::QBinCompareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QBinCompareDialog)
{
    ui->setupUi(this);
    connect(ui->browse1, SIGNAL(clicked()), this, SLOT(clickBrowse1()));
    connect(ui->browse2, SIGNAL(clicked()), this, SLOT(clickBrowse2()));
    connect(ui->compare, SIGNAL(clicked()), this, SLOT(clickCompare()));
}

QBinCompareDialog::~QBinCompareDialog()
{
    delete ui;
}

void QBinCompareDialog::clickBrowse1()
{
    QString filePath = QFileDialog::getOpenFileName(this);
    ui->file1Path->setText(filePath);
}

void QBinCompareDialog::clickBrowse2()
{
    QString filePath = QFileDialog::getOpenFileName(this);
    ui->file2Path->setText(filePath);
}

void QBinCompareDialog::clickCompare()
{
    QString filePath1 = ui->file1Path->text();
    QString filePath2 = ui->file2Path->text();

    if(filePath1.isEmpty() || filePath2.isEmpty())
    {
        return ;
    }

    QFile file1(filePath1);
    QFile file2(filePath2);

    if( file1.open(QIODevice::ReadOnly) == false)
    {
        return ;
    }

    if( file2.open(QIODevice::ReadOnly) == false)
    {
        file1.close();
        return ;
    }

    QDataStream stream1(&file1);
    QDataStream stream2(&file2);

    int offset = 0;
    while(stream1.atEnd() == false)
    {
        qint8 byte1;
        qint8 byte2;

        stream1>>byte1;
        stream2>>byte2;

        if(byte1 != byte2)
        {
            QString error = QString("address %1, %2 - %3").arg(offset, 0, 16).arg(byte1, 0, 16).arg(byte2, 0, 16);
            ui->output->append(error);
        }

        offset++;
    }

    file1.close();
    file2.close();
    ui->output->append("it's over");
}
