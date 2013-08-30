#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GNumberTheory numberTheory;
    qDebug()<<numberTheory.gcd(32,18);
}

MainWindow::~MainWindow()
{
    delete ui;
}
