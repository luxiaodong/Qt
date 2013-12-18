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
//    QString filePath = QString("/Users/ruizhan/Dropbox/work/polyTable.txt");
//    QCubePoly poly;
//    poly.parse(filePath);

    QStringList list;
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_1.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_2.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_3.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_4.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_5.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_6.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_7.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_8.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_9.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_11.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10001.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10002.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10003.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10004.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10005.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10006.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10007.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10008.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10009.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/polyTable_10010.txt");

    foreach(QString single, list)
    {
        QCubePoly poly;
        poly.parse2(single);
    }
}

void MainWindow::clickTracePoint()
{
//    QString filePath = QString("/Users/ruizhan/Dropbox/work/timeTable.txt");
//    QPolyTime poly;
//    poly.parse(filePath);

    QStringList list;
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_1.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_2.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_3.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_4.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_5.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_6.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_7.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_8.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_9.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_11.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10001.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10002.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10003.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10004.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10005.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10006.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10007.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10008.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10009.txt");
    list.append("/Users/ruizhan/Documents/MATLAB/gcld/juben/timeTable_10010.txt");

    foreach(QString single, list)
    {
        QPolyTime poly;
        poly.parse2(single);
    }
}

void MainWindow::clickXiaoqianCollect()
{
    QString filePath = QString("/Users/ruizhan/Dropbox/work/constant2.lua");
    QCollectXiaoqianDialogure collect;
    collect.parse(filePath);
}


