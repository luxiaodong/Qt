#include "deskwidget.h"
#include "ui_deskwidget.h"

DeskWidget::DeskWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeskWidget)
{
    ui->setupUi(this);
}

DeskWidget::~DeskWidget()
{
    delete ui;
}
