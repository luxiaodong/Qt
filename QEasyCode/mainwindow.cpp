#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lineedit.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionCode_path, SIGNAL(triggered()), this, SLOT(setCodePath()));
    connect(ui->actionCocos2dx, SIGNAL(triggered()), this, SLOT(createCocos2dxCode()));
    connect(ui->actionUnity, SIGNAL(triggered()), this, SLOT(createUnityCode()));
    connect(ui->actionPhp, SIGNAL(triggered()), this, SLOT(createPHPCode()));

    connect(ui->test, SIGNAL(clicked()), this, SLOT(test()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createCocos2dxCode()
{
    if(m_hdWikiRead.tryToConnectDataBase() == true)
    {
        ui->textBrowser->append("connect wiki database ok!");

        QStringList list = m_hdWikiRead.tryToCatchProtocolText();
        if(list.size() > 0)
        {
            ui->textBrowser->append("catch wiki text ok!");
            QList<QProtocolData> datas;
            foreach(QString str, list)
            {
                QProtocolData data(str);
                datas.append(data);
            }

            if(datas.size() > 0)
            {
                ui->textBrowser->append("prase protocol ok!");
                QCocos2dxCode code;
                if(code.createProtocolEnumFile(datas) == true)
                {
                    ui->textBrowser->append("create protocol.h ok!");
                }

                if(code.createProtocolVariableInServiceDataHFile(datas) == true)
                {
                    ui->textBrowser->append("create GServiceData.h ok!");
                }

                if(code.createProtocolVariableInServiceDataCppFile(datas) == true)
                {
                    ui->textBrowser->append("create GServiceData.cpp! ok");
                }

                if(code.createProtocolFunctionInServiceHFile(datas) == true)
                {
                    ui->textBrowser->append("create GService.h! ok");
                }

                if(code.createProtocolFunctionInServiceCppFile(datas) == true)
                {
                    ui->textBrowser->append("create GService.cpp! ok");
                }
            }
        }
    }

    ui->textBrowser->append("love is over!");
}

void MainWindow::createPHPCode()
{}

void MainWindow::createUnityCode()
{}

void MainWindow::test()
{
    if(m_hdWikiRead.tryToConnectDataBase() == true)
    {
        QStringList list = m_hdWikiRead.tryToCatchProtocolText();
        QList<QProtocolData> datas;

        foreach(QString str, list)
        {
            QProtocolData data(str);
            datas.append(data);
            //qDebug()<<data.protocolId()<<data.args()<<data.des()<<data.functionName();
        }

        if(datas.size() > 0)
        {
            QCocos2dxCode code;
            if(code.createProtocolFunctionInServiceCppFile(datas) == true)
            {
                qDebug()<<"xxxxxx";
            }
            else
            {
                qDebug()<<"yyyyyy";
            }
        }

        ui->statusBar->showMessage("connect wiki ok!");
    }
    else
    {
        ui->statusBar->showMessage("connect wiki failed!");
    }
}

void MainWindow::setCodePath()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    LineEdit dialog(this);
    dialog.setTitle("set source code path:");
    dialog.setText(path);
    if(dialog.exec() == QDialog::Accepted)
    {
        QSettings settings("yuguosoft");
        settings.setValue("easyCode_codePath", dialog.lineText());
        ui->statusBar->showMessage(QString("path: %1").arg(dialog.lineText()));
    }
}
