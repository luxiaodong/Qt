#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client/logindialog.h"

#include <QHostInfo>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_server(0),
    m_client(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->talkEidt,SIGNAL(returnPressed()),this,SLOT(talking()));
}

MainWindow::~MainWindow()
{
    delete ui;

    if(m_server)
    {
        delete m_server;
    }

    if(m_client)
    {
        delete m_client;
    }
}

QString MainWindow::strings(int type)
{
    return tr(mainwindow_strings[type]);
}

void MainWindow::on_actionServer_triggered()
{
    if(m_server)
    {
        if(QMessageBox::No == QMessageBox::information(this,
                                 strings(1),
                                 strings(0),
                                 QMessageBox::Yes,
                                 QMessageBox::No) )
        {
            return ;
        }

        delete m_server;
        m_server = 0;
    }

    m_server = new QServer(this);
    if(m_server->canBeService())
    {
        m_server->startupService();
        on_actionClient_triggered();
        return ;
    }

    if(QMessageBox::Yes == QMessageBox::information(this,
                             strings(3),
                             strings(4),
                             QMessageBox::Yes,
                             QMessageBox::No) )
    {
        m_server->startupService();
        on_actionClient_triggered();
        return ;
    }

    delete m_server;
    m_server = 0;
}

void MainWindow::on_actionStart_triggered()
{
    if(m_server == 0)
    {
        return ;
    }

    m_server->startGame();
}

void MainWindow::on_actionClient_triggered()
{
    if(m_client)
    {
        if(QMessageBox::No == QMessageBox::information(this,
                                 strings(2),
                                 strings(0),
                                 QMessageBox::Yes,
                                 QMessageBox::No) )
        {
            return ;
        }

        delete m_client;
        m_client = 0;
    }

    m_client = new QClient(this);
    connect(m_client,SIGNAL(displayMessage(QString)),this,SLOT(displayMessage(QString)));
    connect(m_client,SIGNAL(showMessage(QString)),ui->statusBar,SLOT(showMessage(QString)));
    QString serverIP;
    QString userName = m_client->localHostUserName();

    if(m_server)
    {
        serverIP = m_server->m_hostIp;
    }
    else
    {
        LoginDialog dialog(userName,this);
        if(dialog.exec() == QDialog::Accepted)
        {
            serverIP = dialog.serverIP();
            userName = dialog.userName();
        }
        else
        {
            delete m_client;
            m_client = 0;
            return ;
        }
    }

    m_client->connectToServer(serverIP);
    ui->statusBar->showMessage(strings(5));
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::talking()
{
    QString str = ui->talkEidt->text();

    if(str.isEmpty())
    {
        return;
    }

    if(m_client)
    {
        m_client->sendMessage(str);
    }
    else
    {
        displayMessage(str);
    }

    ui->talkEidt->setText(QString());
}

void MainWindow::displayMessage(QString msg)
{
    if(msg.isEmpty() == false)
    {
        ui->talkBrowser->append(msg);
    }
}
