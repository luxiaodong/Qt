#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
//    QSqlDatabase db = QSqlDatabase(QSqlDatabase::addDatabase("QODBC"));
//    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ= train.mdb");

//    if(!db.open())
//    {
//        qDebug()<<"open failed!";
//        return ;
//    }
//    else
//    {
//        qDebug()<<"open ok!"<<QSqlDatabase::drivers();
//        qDebug()<<db.tables();
//    }

//    m_model = new QSqlTableModel(this,db);
//    m_model->setTable(db.tables().first());
//    m_model->select();
//    ui->tableView->setModel(m_model);
//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    qDebug()<<e->pos();
}

void MainWindow::updateTrace(int x, int y)
{
    const int blockNumber = 8;
    int index = y*blockNumber + x;
    int count = m_moveTrace.count();
    int find = m_moveTrace.indexOf(index);

    if( find == -1 )
    {
        //要判断方向才可以加
        if(count == 0)
        {
            m_moveTrace.append(index);
        }
        else
        {
            int last = m_moveTrace.at(count - 1);
        }
        return ;
    }
    else
    {
        if(find == m_moveTrace.at(count - 1))
        {
            return ;
        }
        else if(find == m_moveTrace.at(count - 2))
        {
            m_moveTrace.removeAt(count - 1);
            return ;
        }
    }
}

int MainWindow::direction(int x1, int y1, int x2, int y2)
{
    if(x1 = x2 + 1)
    {
        if(y1 = y2 + 1)
        {
            return 7;
        }
        else if(y1 == y2)
        {
            return 6;
        }
        else if(y1 + 1== y2)
        {
            return 5;
        }
    }
    else if(x1 == x2)
    {
        if(y1 = y2 + 1)
        {
            return 0;
        }

        if(y1 + 1 = y2)
        {
            return 4;
        }
    }
    else if(x1 + 1 = x2)
    {
        if(y1 = y2 + 1)
        {
            return 1;
        }
        else if(y1 == y2)
        {
            return 2;
        }
        else if(y1 + 1== y2)
        {
            return 3;
        }
    }

    return -1;
}

