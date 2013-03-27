#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qpointtranslatedialog.h"
#include "qrenamedialog.h"
#include "qiphonetoitouchdialog.h"
#include "qiphonetoipaddialog.h"
#include "qtcptestdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionPointTranslate, SIGNAL(triggered()), this, SLOT(clickPointTranslate()));
    connect(ui->actionRename, SIGNAL(triggered()), this, SLOT(clickRename()));
    connect(ui->actionTcpTest, SIGNAL(triggered()), this, SLOT(clickTcpTest()));
    connect(ui->actionIphoneToiTouch, SIGNAL(triggered()), this, SLOT(clickConvertIPhoneToITouch()));
    connect(ui->actionIphoneToiPad, SIGNAL(triggered()), this, SLOT(clickConvertIPhoneToIPad()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickPointTranslate()
{
    this->hide();
    QPointTranslateDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}

void MainWindow::clickRename()
{
    this->hide();
    QRenameDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
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

void MainWindow::clickConvertIPhoneToITouch()
{
    this->hide();
    QIPhoneToITouchDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}

void MainWindow::clickConvertIPhoneToIPad()
{
    this->hide();
    QIPhoneToIPadDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}


