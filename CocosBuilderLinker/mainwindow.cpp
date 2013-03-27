#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qparseccbfile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QParseCCBFile parser;
    parser.parseCCBFile(QString("/home/luxiaodong/Desk/StatusLayer.ccb"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
