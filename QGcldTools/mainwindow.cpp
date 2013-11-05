#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qconvertdialogure.h"
#include "qbezierpoly.h"
#include "qcityroadmap.h"
#include "qcityposition.h"
#include "qcubepoly.h"
#include "qpolytime.h"
#include "qcollectxiaoqiandialogure.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->convertDialogue,SIGNAL(clicked()),this,SLOT(clickConvertDialogue()));
    connect(ui->bezierPoly, SIGNAL(clicked()),this, SLOT(clickConvertBezierPoly()));
    connect(ui->cityRoadMap, SIGNAL(clicked()), this, SLOT(clickCityRoadMap()));
    connect(ui->cityPosition, SIGNAL(clicked()), this, SLOT(clickCityPosition()));
    connect(ui->cubePoly, SIGNAL(clicked()), this, SLOT(clickCubePoly()));
    connect(ui->tracePoints, SIGNAL(clicked()), this ,SLOT(clickTracePoint()));
    connect(ui->xiaoqianCollect, SIGNAL(clicked()), this, SLOT(clickXiaoqianCollect()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickConvertDialogue()
{
    QString filePath = QString("/home/luxiaodong/Dropbox/dialogue.xml");
    //QString filePath = QString("/Users/ruizhan/Dropbox/work/dialogue.xml");
    QConvertDialogure convert;
    convert.parse(filePath);
}

void MainWindow::clickConvertBezierPoly()
{
    QString filePath = QString("/home/luxiaodong/Dropbox/work/bezierpoly.txt");
    //QString filePath = QString("/Users/ruizhan/Dropbox/work/bezierpoly.txt");
    QBezierPoly bezierPoly;
    bezierPoly.parse(filePath);
}

void MainWindow::clickCityRoadMap()
{
    //QString filePath = QString("/home/luxiaodong/Dropbox/work/cityRoadMap.txt");
    QString filePath = QString("/Users/ruizhan/Dropbox/work/cityRoadMap.txt");
    QCityRoadMap cityMap;
    cityMap.parse(filePath);
}

void MainWindow::clickCityPosition()
{
    //QString filePath = QString("/home/luxiaodong/Dropbox/work/cityRoadMap.txt");
    QString filePath = QString("/Users/ruizhan/Dropbox/work/cityPosition.lua");
    QCityPosition city;
    city.parse(filePath);
}

void MainWindow::clickCubePoly()
{
    QString filePath = QString("/Users/ruizhan/Dropbox/work/polyTable.txt");
    QCubePoly poly;
    poly.parse(filePath);
}

void MainWindow::clickTracePoint()
{
    QString filePath = QString("/Users/ruizhan/Dropbox/work/timeTable.txt");
    QPolyTime poly;
    poly.parse(filePath);
}

void MainWindow::clickXiaoqianCollect()
{
    QString filePath = QString("/Users/ruizhan/Dropbox/work/constant2.lua");
    QCollectXiaoqianDialogure collect;
    collect.parse(filePath);
}


