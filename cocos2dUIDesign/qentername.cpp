#include "qentername.h"
#include "ui_qentername.h"

QEnterName::QEnterName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QEnterName)
{
    ui->setupUi(this);
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancel, SIGNAL(clicked()), this, SLOT(reject()));
    this->setNodeName("");
}

QEnterName::~QEnterName()
{
    delete ui;
}

void QEnterName::setNodeName(const QString &name)
{
    ui->lineEdit->setText(name);
}

QString QEnterName::nodeName()
{
    return ui->lineEdit->text();
}
