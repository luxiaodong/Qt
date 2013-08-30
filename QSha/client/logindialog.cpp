#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QString userName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->nameEdit->setText(userName);
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->ipEdit,SIGNAL(returnPressed()),this,SLOT(accept()));
    connect(ui->ok, SIGNAL(clicked()),this,SLOT(accept()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::userName()
{
    return ui->nameEdit->text();
}

QString LoginDialog::serverIP()
{
    return ui->ipEdit->text();
}

void LoginDialog::accept()
{
    QString ip = this->serverIP();
    QStringList list = ip.split(".");

    if(ip.isEmpty() || (list.size() != 4))
    {
        return ;
    }

    QDialog::accept();
}
