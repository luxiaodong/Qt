#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->browse_1, SIGNAL(clicked()), this, SLOT(clickBrowse1()));
    connect(ui->browse_2, SIGNAL(clicked()), this, SLOT(clickBrowse2()));
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOK()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickOK()
{
    ui->textEdit->clear();
    ui->textEdit->append(tr("右面"));

    if(this->readSalary() == false)
    {
        qDebug()<<"read failed";
        return ;
    }

    if(this->writeSalary() == false)
    {
        qDebug()<<"write failed";
    }

    qDebug()<<"ok";
}

bool MainWindow::readSalary()
{
    m_map.clear();
    m_mapHelp.clear();
    QString srcFilePath = ui->lineEdit_1->text();

    if(srcFilePath.isEmpty() == true)
    {
        ui->textEdit->append(tr("1 zhaobudao"));
        return false;
    }

    //Excel应用程序包括一系列的workbooks，每个workbook又由多个sheets组成。

    QString nameStart = ui->nameEdit1->text();
    QString salaryStart = ui->salaryEdit1->text();

    if(nameStart.right(1) != salaryStart.right(1))
    {
        ui->textEdit->append(tr("qishi weizhi buyiyang"));
        return false;
    }

    QString nameChar = nameStart.left(1);
    QString salaryChar = salaryStart.left(1);
    bool isOk = false;
    int index = nameStart.right(1).toInt(&isOk);

    if(isOk == false)
    {
        ui->textEdit->append(tr("you wenti"));
        return false;
    }

    QAxObject* excel = new QAxObject( "Excel.Application" );
    excel->dynamicCall( "SetVisible(bool)", false );
    QAxObject *workbooks = excel->querySubObject( "Workbooks" ); //得到Workbooks集合的指针
    //QAxObject *workbookSrc = workbooks->querySubObject( "Open(const QString&)", "F:\\QExcelConvert\\test.xls" );
    QAxObject *workbookSrc = workbooks->querySubObject( "Open(const QString&)", srcFilePath );
    QAxObject *worksheet = workbookSrc->querySubObject("Worksheets(int)", ui->spinBox->value());

    bool result = true;
    while(1)
    {
        QString namePos = QString("Range(%1%2)").arg(nameChar).arg(index);
        QString salaryPos = QString("Range(%1%2)").arg(salaryChar).arg(index);

        QAxObject* nameAx = worksheet->querySubObject(namePos.toStdString().c_str());
        QAxObject* salaryAx = worksheet->querySubObject(salaryPos.toStdString().c_str());

        QString name = nameAx->property("Value").toString().trimmed();
        QString salary = salaryAx->property("Value").toString();

        if(name.isEmpty() && salary.isEmpty() )
        {
            break;
        }

        ui->textEdit->append(QString("%1 %2 %3").arg(index).arg(name).arg(salary));

        if(m_map.contains(name) == true)
        {
            ui->textEdit->append(QString("same name with line %1.").arg(m_mapHelp.value(name)));
            result = false;
            break;
        }

        m_map.insert(name, salary);
        m_mapHelp.insert(name, index);
        index++;
    }

    workbookSrc->dynamicCall("Close (Boolean)", false);
    excel->dynamicCall("Quit (void)");
    delete excel;
    return result;
}

bool MainWindow::writeSalary()
{
    QString tagFilePath = ui->lineEdit_3->text();

    if(tagFilePath.isEmpty() == true)
    {
        ui->textEdit->append(tr("2 zhaobudao"));
        return false;
    }

    //Excel应用程序包括一系列的workbooks，每个workbook又由多个sheets组成。

    QString nameStart = ui->nameEdit2->text();
    QString salaryStart = ui->salaryEdit2->text();

    if(nameStart.right(1) != salaryStart.right(1))
    {
        ui->textEdit->append(tr("qishi weizhi buyiyang"));
        return false;
    }

    QString nameChar = nameStart.left(1);
    QString salaryChar = salaryStart.left(1);
    bool isOk = false;
    int index = nameStart.right(1).toInt(&isOk);

    if(isOk == false)
    {
        ui->textEdit->append(tr("youwenti"));
        return false;
    }

    QAxObject* excel = new QAxObject( "Excel.Application" );
    excel->dynamicCall( "SetVisible(bool)", false );
    QAxObject *workbooks = excel->querySubObject( "Workbooks" ); //得到Workbooks集合的指针
    //QAxObject *workbookSrc = workbooks->querySubObject( "Open(const QString&)", "F:\\QExcelConvert\\test.xls" );
    QAxObject *workbookSrc = workbooks->querySubObject( "Open(const QString&)", tagFilePath );
    QAxObject *worksheet = workbookSrc->querySubObject("Worksheets(int)", ui->spinBox2->value());

    QMap<QString, int> mapHelp;

    int totalCount = m_map.count();
    bool result = true;
    while(1)
    {
        QString namePos = QString("Range(%1%2)").arg(nameChar).arg(index);
        QAxObject* nameAx = worksheet->querySubObject(namePos.toStdString().c_str());
        QString name = nameAx->property("Value").toString().trimmed();

        QString salaryPos = QString("Range(%1%2)").arg(salaryChar).arg(index);
        QAxObject* salaryAx = worksheet->querySubObject(salaryPos.toStdString().c_str());

        if(name.isEmpty() == true)
        {
            break;
        }

        if(mapHelp.contains(name) == true)
        {
            result = false;
            ui->textEdit->append(QString("2 mizichongfu %1").arg(mapHelp.value(name)));
            break;
        }

        if( m_map.contains(name) )
        {
            salaryAx->setProperty("Value", m_map.value(name));
            m_map.remove(name);
            ui->textEdit->append(QString("%1 %2 %3 OK").arg(index).arg(name).arg(m_map.value(name)));
        }
        else
        {
            salaryAx->clear();
        }

        mapHelp.insert(name, index);
        index++;
    }

    if(result == true)
    {
        int nowCount = m_map.count();
        ui->textEdit->append(QString("wancheng %1/%2").arg(totalCount - nowCount).arg(totalCount));

        if(nowCount > 0)
        {
            QList<QString> keys = m_map.keys();
            for(int i = 0; i < nowCount; ++i)
            {
                int j = index + 5 + i;
                QString namePos = QString("Range(%1%2)").arg(nameChar).arg(j);
                QAxObject* nameAx = worksheet->querySubObject(namePos.toStdString().c_str());
                nameAx->setProperty("Value", keys.at(i));

                QString salaryPos = QString("Range(%1%2)").arg(salaryChar).arg(j);
                QAxObject* salaryAx = worksheet->querySubObject(salaryPos.toStdString().c_str());
                salaryAx->setProperty("Value", m_map.value(keys.at(i)));
            }
        }
    }

    workbookSrc->dynamicCall("Save (void)");
    workbookSrc->dynamicCall("Close (Boolean)", false);
    excel->dynamicCall("Quit (void)");
    delete excel;
    return result;
}

void MainWindow::clickBrowse1()
{
    QString file = QFileDialog::getOpenFileName(this);
    ui->lineEdit_1->setText(file);
}

void MainWindow::clickBrowse2()
{
    QString file = QFileDialog::getOpenFileName(this);
    ui->lineEdit_3->setText(file);
}

void MainWindow::test()
{
    QAxObject* excel = new QAxObject( "Excel.Application" );
    excel->dynamicCall( "SetVisible(bool)", false );
    QAxObject *workbooks = excel->querySubObject( "Workbooks" ); //得到Workbooks集合的指针
    //QAxObject *workbookSrc = workbooks->querySubObject( "Open(const QString&)", "F:\\QExcelConvert\\test.xls" );
    QAxObject *workbookSrc = workbooks->querySubObject( "Open(const QString&)", "F:\\QExcelConvert\\test.xls" );
    QAxObject *worksheet = workbookSrc->querySubObject("Worksheets(int)", 1);
    QAxObject * b5 = worksheet->querySubObject("Range(B7)");
    QString str = b5->property("Value").toString();
    qDebug()<<str;

    QAxObject * a5 = worksheet->querySubObject("Range(A5)");
    a5->setProperty("Value", QVariant(str));

    //excel.setProperty("DisplayAlerts", 0);
    //workbookSrc->dynamicCall("SaveAs (const QString&)", "F:\\QExcelConvert\\test2.xls");
    workbookSrc->dynamicCall("Save (void)");
    //excel->setProperty("DisplayAlerts", 1);
    workbookSrc->dynamicCall("Close (Boolean)", false);
    excel->dynamicCall("Quit (void)");
    delete excel;
}
