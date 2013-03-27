#include "qcatchbindialog.h"
#include "ui_qcatchbindialog.h"
#include "qcatchbin.h"

QCatchBinDialog::QCatchBinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCatchBinDialog)
{
    ui->setupUi(this);

    connect(ui->catchButton, SIGNAL(clicked()), this, SLOT(clickCatch()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->browse, SIGNAL(clicked()), this, SLOT(clickBrowse()));
}

QCatchBinDialog::~QCatchBinDialog()
{
    delete ui;
}

void QCatchBinDialog::clickBrowse()
{
    QString filePath = QFileDialog::getOpenFileName(this);
    ui->filePath->setText(filePath);
}

void QCatchBinDialog::clickCatch()
{
    QCatchBin catchBin;
    catchBin.setStartAndEnd(ui->start->text().toInt(0, 16), ui->end->text().toInt(0, 16));
    catchBin.setOutPutFileName(ui->outPutFileName->text());
    if( catchBin.catchBin(ui->filePath->text()) == false)
    {
        ui->textBrowser->append(catchBin.m_lastError);
    }
    else
    {
        ui->textBrowser->append(catchBin.m_lastError);
    }
}
