#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GAStar.h"
#include "QDebug"
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(test()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test()
{
    int map[64] =
    {
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,0,1,1,
        1,1,1,1,0,1,1,1,
        1,1,1,0,1,1,1,1,
        1,0,0,1,1,0,1,1,
        1,1,1,0,0,1,1,1,
        1,1,1,1,1,1,1,1,
    };

    GAStar aStar;
    aStar.setMapInformation(map, 8, 8);
    if(aStar.searchPath(14,57) == true)
    {
        QList<int> list = QList<int>::fromStdList(aStar.m_best);
        qDebug()<<"find.\n"<<list<<"\n";
    }
    else
    {
        qDebug()<<"sorry, not find!";
    }
}
