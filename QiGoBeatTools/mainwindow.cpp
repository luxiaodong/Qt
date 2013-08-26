#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qpointtranslatedialog.h"
#include "qpackagesongsdialog.h"
#include "qaddhddialog.h"
#include "qpngscaledialog.h"
#include "qsildenodeextranumberdialog.h"
#include "qbmsetoigobeatdialog.h"
#include "qencryptiondialog.h"
#include "qthreeinonedialog.h"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionPointTranslate, SIGNAL(triggered()), this, SLOT(clickPointTranslate()));
    connect(ui->actionPackageSongs, SIGNAL(triggered()), this, SLOT(clickPackageSongs()));
    connect(ui->actionAddHd, SIGNAL(triggered()), this, SLOT(clickAddHd()));
    connect(ui->actionScalePng, SIGNAL(triggered()), this, SLOT(clickScalePng()));
    connect(ui->actionSlideNode, SIGNAL(triggered()), this, SLOT(clickAddSlideNodeExtraNumber()));
    connect(ui->actionBmse_to_iGoBeat, SIGNAL(triggered()), this, SLOT(clickBsmToIGoBeat()));
    connect(ui->actionEncryption, SIGNAL(triggered()), this, SLOT(clickEncryption()));
    connect(ui->action3In1, SIGNAL(triggered()), this, SLOT(click3In1()));

    QSettings settings("YuGuoSoft", "iGoBeat");
    this->restoreGeometry(settings.value("geometry").toByteArray());
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

void MainWindow::clickPackageSongs()
{
    this->hide();
    QPackageSongsDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}

void MainWindow::clickAddHd()
{
    this->hide();
    QAddHdDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}

void MainWindow::clickScalePng()
{
    this->hide();
    QPngScaleDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}

void MainWindow::clickAddSlideNodeExtraNumber()
{
    this->hide();
    QSildeNodeExtraNumberDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}

void MainWindow::clickBsmToIGoBeat()
{
    this->hide();
    QBmseToIGoBeatDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}

void MainWindow::clickEncryption()
{
    this->hide();
    QEncryptionDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}

void MainWindow::click3In1()
{
    this->hide();
    QThreeInOneDialog dialog;
    dialog.setGeometry(this->geometry());
    dialog.exec();
    this->setGeometry(dialog.geometry());
    this->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("YuGuoSoft", "iGoBeat");
    settings.setValue("geometry", saveGeometry());
    QWidget::closeEvent(event);
}


