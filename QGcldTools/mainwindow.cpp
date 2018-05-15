#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qconvertdialogure.h"
#include "qbezierpoly.h"
#include "qcityroadmap.h"
#include "qcityposition.h"
#include "qcubepoly.h"
#include "qpolytime.h"
#include "qcollectxiaoqiandialogure.h"
#include "qbuildresourcedialog.h"
#include "qresourcediffdialog.h"
#include "qutf8convert.h"
#include "qgoldscatter.h"

#include <QDebug>
#include <QMediaPlayer>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect(ui->convertDialogue,SIGNAL(clicked()),this,SLOT(clickConvertDialogue()));
    //connect(ui->bezierPoly, SIGNAL(clicked()),this, SLOT(clickConvertBezierPoly()));
    connect(ui->cityRoadMap, SIGNAL(clicked()), this, SLOT(clickCityRoadMap()));
    connect(ui->cityPosition, SIGNAL(clicked()), this, SLOT(clickCityPosition()));
    connect(ui->cubePoly, SIGNAL(clicked()), this, SLOT(clickCubePoly()));
    connect(ui->tracePoints, SIGNAL(clicked()), this ,SLOT(clickTracePoint()));
    //connect(ui->xiaoqianCollect, SIGNAL(clicked()), this, SLOT(clickXiaoqianCollect()));
    //connect(ui->resourceDiff, SIGNAL(clicked()), this, SLOT(clickResourceDiff()));
    connect(ui->utf8Bom, SIGNAL(clicked()), this, SLOT(clickUtf8Convert()));
    //connect(ui->decode, SIGNAL(clicked()),this,SLOT(clickDecode()));
    //connect(ui->rc4, SIGNAL(clicked()), this, SLOT(clickRc4()));
    //connect(ui->tqcfConvertDialog, SIGNAL(clicked()), this, SLOT(clickDialogConvert()));
    //clickBuildResource();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickCityRoadMap()
{
    QString filePath = QString("/Users/luxiaodong/Project/Git/luxiaodong/Matlab/gcld/data/qxgj2/Road.xml");
    QCityRoadMap cityMap;
    cityMap.parse(filePath);
}

void MainWindow::clickCityPosition()
{
    QString filePath = QString("/Users/luxiaodong/Project/Git/luxiaodong/Matlab/gcld/data/qxgj2/CityInfo.xml");
    QCityPosition city;
    city.parse(filePath);
}

void MainWindow::clickCubePoly()
{
//    QString filePath = QString("/Users/ruizhan/Dropbox/work/polyTable.txt");
//    QString filePath = QString("/home/luxiaodong/Project/Git/Matlab/gcld/data/world/polyTable.txt");
//    QCubePoly poly;
//    poly.parse(filePath);

//    QList<int> list;
//    for(int i=0; i<11; ++i)
//    {
//        list.append(i+1);
//    }

//    for(int i=10000; i < 10; ++i)
//    {
//        list.append(i+1);
//    }

//    foreach(int index, list)
//    {
//        QString filePath = QString("/home/luxiaodong/Project/Git/Matlab/gcld/data/juben/polyTable_%1.txt").arg(index);
//        QCubePoly poly;
//        poly.parse2(filePath);
//    }

//    for(int i=10000; i < 10010; ++i)
//    {
//        //list.append(i+1);
//        QString filePath = QString("/Users/luxiaodong/Project/Git/luxiaodong/Matlab/nhly/data/juben/polyTable_%1.txt").arg(i+1);
//        //qDebug()<<filePath;
//        QCubePoly poly;
//        poly.parse2(filePath);
//    }

//    for(int i=0; i<1; ++i)
//    {
//        QString filePath = QString("/Users/luxiaodong/Project/Git/luxiaodong/Matlab/gcld/data/liuqiu/polyTable_%1.txt").arg(i + 1);
//        QCubePoly poly;
//        poly.parse2(filePath);
//    }

    QString filePath = QString("/Users/luxiaodong/Project/Git/luxiaodong/Matlab/gcld/data/qxgj/polyTable.txt");
    QCubePoly poly;
    poly.parse2(filePath);

}

void MainWindow::clickTracePoint()
{
//    QString filePath = QString("/Users/ruizhan/Dropbox/work/timeTable.txt");
//    QString filePath = QString("/home/luxiaodong/Project/Git/Matlab/gcld/data/world/timeTable.txt");
//    QPolyTime poly;
//    poly.parse(filePath);


//    QList<int> list;
//    for(int i=0; i<11; ++i)
//    {
//        list.append(i+1);
//    }

//    for(int i=10000; i < 10; ++i)
//    {
//        list.append(i+1);
//    }

//    foreach(int index, list)
//    {
//        QString filePath = QString("/home/luxiaodong/Project/Git/Matlab/gcld/data/juben/timeTable_%1.txt").arg(index);
//        QPolyTime poly;
//        poly.parse2(filePath);
//    }


//    for(int i=0; i<1; ++i)
//    {
//        QString filePath = QString("/Users/luxiaodong/Project/Git/luxiaodong/Matlab/gcld/data/liuqiu/timeTable_%1.txt").arg(i + 1);
//        QPolyTime poly;
//        poly.parse2(filePath);
//    }

//    for(int i=10000; i < 10010; ++i)
//    {
//        QString filePath = QString("/Users/luxiaodong/Project/Git/luxiaodong/Matlab/nhly/data/juben/timeTable_%1.txt").arg(i+1);
//        //qDebug()<<filePath;
//        QPolyTime poly;
//        poly.parse2(filePath);
//    }

    QString filePath = QString("/Users/luxiaodong/Project/Git/luxiaodong/Matlab/gcld/data/qxgj/timeTable.txt");
    QPolyTime poly;
    poly.parse2(filePath);
}

void MainWindow::clickUtf8Convert()
{
    //QString filePath = QFileDialog::getExistingDirectory(this, QString("Open Directory"), "", QFileDialog::ShowDirsOnly);
    QString filePath = QFileDialog::getOpenFileName(this, QString("Open Directory"), "");
    if( filePath.isEmpty() == true)
    {
        return ;
    }

    if( QFile::exists(filePath) == true)
    {
        QUtf8Convert convert;
        //convert.convert(filePath);
        convert.checkBOM(filePath);
    }
}

