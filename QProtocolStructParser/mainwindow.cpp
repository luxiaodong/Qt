#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qtcptestdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->tcpTest, SIGNAL(clicked()), this, SLOT(clickTcpTest()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickTcpTest()
{
    this->hide();
    QTcpTestDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}
