#include "lineedit.h"
#include "ui_lineedit.h"

LineEdit::LineEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LineEdit)
{
    ui->setupUi(this);
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancel, SIGNAL(clicked()),this, SLOT(reject()));
}

LineEdit::~LineEdit()
{
    delete ui;
}

void LineEdit::setTitle(QString str)
{
    ui->title->setText(str);
}

void LineEdit::setText(QString str)
{
    ui->lineEdit->setText(str);
}

QString LineEdit::lineText()
{
    return ui->lineEdit->text();
}
