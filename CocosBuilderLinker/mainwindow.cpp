#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qparseccbfile.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->test, SIGNAL(clicked()), this, SLOT(clickTest()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::clickTest()
{
    QString filePath = QFileDialog::getOpenFileName(this);
    if(filePath.isEmpty() == true)
    {
        return ;
    }

    QParseCCBFile parser;
    parser.parseCCBFile(filePath);
}
