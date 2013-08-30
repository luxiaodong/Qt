#include "qtcptestdialog.h"
#include "ui_qtcptestdialog.h"

#include <QDebug>
#include <QSettings>

QTcpTestDialog::QTcpTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTcpTestDialog)
{
    ui->setupUi(this);

    connect(ui->load, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui->add, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->send, SIGNAL(clicked()), this, SLOT(send()));
    connect(ui->connect, SIGNAL(clicked()), this, SLOT(tryToConnect()));

    connect(&m_tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(&m_tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(&m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    QVBoxLayout* gridLayer = new QVBoxLayout();
    gridLayer->setSizeConstraint(QLayout::SetDefaultConstraint);
    ui->tree->setLayout(gridLayer);

    m_variantManager = new QtVariantPropertyManager();
    QtVariantEditorFactory *variantFactory = new VariantFactory();
    m_browser = new QtTreePropertyBrowser();
    m_browser->setFactoryForManager(m_variantManager, variantFactory);
    gridLayer->addWidget(m_browser, 1);

    QSettings settings("yuguosoft.com");
    QString ip = settings.value("mmDouDiZhu_ip", QString("127.0.0.1")).toString();
    QString port = settings.value("mmDouDiZhu_port", "10501").toString();
    ui->ip->setText(ip);
    ui->port->setText(port);

    //QTcpSocket m_tcpSocket;
    //m_dataStream.setDevice(&m_tcpSocket);

    //test
//    this->addSingleStruct("testA");
//    this->addSingleStruct("testB");
//    this->addSingleStruct("testC");
//    this->createTreePropertyBrowser();
}

QTcpTestDialog::~QTcpTestDialog()
{
    delete ui;
}

void QTcpTestDialog::createTreePropertyBrowser()
{
    this->createTreePropertyBrowser(&m_currentList);
}

void QTcpTestDialog::createTreePropertyBrowser(QList<QSingleStruct*> *list)
{
    if(list == 0 || list->size() == 0)
    {
        return ;
    }

    int size = list->size();

    for(int i = 0; i < size; ++i)
    {
        QSingleStruct* single = list->at(i);
        QtVariantProperty* currentStruct = m_variantManager->addProperty(QVariant::String, single->m_name);
        //currentStruct->setEnabled(false);
        this->bindData(currentStruct, single);
        m_browser->addProperty(currentStruct);
        this->createTreePropertyBrowser(single, currentStruct);
    }
}

void QTcpTestDialog::createTreePropertyBrowser(QSingleStruct* single, QtVariantProperty* parentProperty)
{
    int size = single->m_list.size();

    for(int i = 0; i < size; ++i)
    {
        QSingleVariable* variable = single->m_list.at(i);
        QtVariantProperty* currentVariable = m_variantManager->addProperty(QVariant::String, variable->m_name);
        currentVariable->setValue(QString(""));
        parentProperty->addSubProperty(currentVariable);

        if(variable->m_isSimple == false)
        {
            QSingleStruct* subStruct = variable->m_singleStruct;
            this->bindData(currentVariable, subStruct, variable->m_arrayLength);
            if(variable->m_arrayLength == 1)
            {
                this->createTreePropertyBrowser(subStruct, currentVariable);
            }
            else
            {
                for(int j = 0; j < variable->m_arrayLength; ++j)
                {
                    QSingleStruct* subStruct = variable->m_singleStruct;
                    QtVariantProperty* subVariable = m_variantManager->addProperty(QVariant::String, QString("%1[%2]").arg(subStruct->m_name).arg(j));
                    currentVariable->addSubProperty(subVariable);
                    this->bindData(subVariable, subStruct);
                    this->createTreePropertyBrowser(subStruct, subVariable);
                }
            }
        }
        else
        {
            this->bindData(currentVariable, variable);
        }
    }
}

void QTcpTestDialog::bindData(QtVariantProperty* property, QSingleStruct* singleStruct, int arrayLength)
{
    property->setData("isStruct", "1");
    property->setData("name", singleStruct->m_name);
    property->setData("totalLength", QString("%1").arg(singleStruct->m_length));
    property->setData("arrayLength", QString("%1").arg(arrayLength));
}

void QTcpTestDialog::bindData(QtVariantProperty* property, QSingleVariable* singleVariable)
{
    property->setData("isStruct", "0");
    property->setData("name", singleVariable->m_name);
    property->setData("totalLength", QString("%1").arg(singleVariable->m_totalLength));//总长度
    property->setData("type",  QString("%1").arg(singleVariable->m_type));//类型
    property->setData("arrayLength", QString("%1").arg(singleVariable->m_arrayLength));//数组的长度
    property->setData("typeLength", QString("%1").arg(singleVariable->m_length));  //类型的长度
}

void QTcpTestDialog::ergodicTreeToTest()
{
    QList<QtProperty*> list = m_browser->properties();
    foreach(QtProperty* property, list)
    {
        QString name = property->data("name").toString();
        if(name.isEmpty() == false)
        {
            qDebug()<<name<<property->data("isStruct")<<" "<<property->data("arrayLength");
        }

        this->ergodicTreeToTest(property);
    }
}

void QTcpTestDialog::ergodicTreeToTest(QtProperty* parentProperty)
{
    QList<QtProperty *> list = parentProperty->subProperties();
    foreach(QtProperty* property, list)
    {
        QString name = property->data("name").toString();
        if(name.isEmpty() == false)
        {
            qDebug()<<name<<property->data("isStruct")<<" "<<property->data("arrayLength");
        }

        this->ergodicTreeToTest(property);
    }
}

void QTcpTestDialog::ergodicTreeForSend()
{
    //m_dataStream.setDevice(0);
    //m_dataStream.setDevice(&m_tcpSocket);

    m_sendBuffer.clear();
    QDataStream stream(&m_sendBuffer, QIODevice::ReadWrite);
    m_dataStream = &stream;
    m_dataStream->setByteOrder(QDataStream::LittleEndian);

    QList<QtProperty*> list = m_browser->properties();
    foreach(QtProperty* property, list)
    {
        QString name = property->data("name").toString();
        if(name.isEmpty() == false)
        {
            qDebug()<<name;
        }

        this->ergodicTreeForSend(property);
    }

    int pos2 = m_sendBuffer.size();

    while(pos2%4 != 0)
    {
        qint8 t = 0;
        (*m_dataStream)<<t;
        pos2++;
    }

    //打印下内存对不对
    qDebug()<<"-------"<<m_sendBuffer.size();
    for(int i = 0; i < m_sendBuffer.size(); ++i)
    {
        qint8 c = m_sendBuffer.at(i);
        qDebug()<<c;
    }
    qDebug()<<"-------";
}

void QTcpTestDialog::ergodicTreeForSend(QtProperty* parentProperty)
{
    int isStruct = parentProperty->data("isStruct").toInt();

    if(isStruct == 0)
    {
        QString type = parentProperty->data("type").toString();
        int arrayLength = parentProperty->data("arrayLength").toInt();
        int typeLength = parentProperty->data("typeLength").toInt();
        QtVariantProperty* variantProperty = (QtVariantProperty*)parentProperty;
        QString value = variantProperty->value().toString();

        if(typeLength == 1)
        {
            if(type == QString("TCHAR") || type == QString("char"))
            {
                QByteArray arr = value.toAscii();
                int size = arr.size();
                for(int i = 0; i < arrayLength; ++i)
                {
                    qint8 a = 0;
                    if(i < size)
                    {
                        a = arr.at(i);
                    }

                    (*m_dataStream)<<a;
                }
            }
            else if(type == QString("BYTE"))
            {
                QList<qint8> list1;
                QStringList list2 = value.split(",");
                int size = list2.size();

                for(int i = 0; i < arrayLength; ++i)
                {
                    if(i < size)
                    {
                        list1.append(list2.at(i).toInt());
                    }
                    else
                    {
                        list1.append(0);
                    }
                }

                foreach(qint8 v, list1)
                {
                    (*m_dataStream)<<v;
                }
            }
        }
        else if(typeLength == 2)
        {
            QList<qint16> list1;
            QStringList list2 = value.split(",");
            int size = list2.size();

            for(int i = 0; i < arrayLength; ++i)
            {
                if(i < size)
                {
                    list1.append(list2.at(i).toInt());
                }
                else
                {
                    list1.append(0);
                }
            }

            foreach(qint16 v, list1)
            {
                (*m_dataStream)<<v;
            }
        }
        else if(typeLength == 4)
        {
            QList<qint32> list1;
            QStringList list2 = value.split(",");
            int size = list2.size();

            for(int i = 0; i < arrayLength; ++i)
            {
                if(i < size)
                {
                    list1.append(list2.at(i).toInt());
                }
                else
                {
                    list1.append(0);
                }
            }

            foreach(qint32 v, list1)
            {
                (*m_dataStream)<<v;
            }
        }
        else if(typeLength == 8)
        {
            QList<qint64> list1;
            QStringList list2 = value.split(",");
            int size = list2.size();

            for(int i = 0; i < arrayLength; ++i)
            {
                if(i < size)
                {
                    list1.append(list2.at(i).toInt());
                }
                else
                {
                    list1.append(0);
                }
            }

            foreach(qint64 v, list1)
            {
                (*m_dataStream)<<v;
            }
        }
    }
    else
    {
        int pos0 = m_sendBuffer.size();

        while(pos0%4 != 0)
        {
            qint8 t = 0;
            (*m_dataStream)<<t;
            pos0++;
        }

        int arrayLength = parentProperty->data("arrayLength").toInt();
        if(arrayLength == 1)
        {
            int pos1 = m_sendBuffer.size();
            QList<QtProperty *> list = parentProperty->subProperties();
            foreach(QtProperty* property, list)
            {
                QString name = property->data("name").toString();
                if(name.isEmpty() == false)
                {
                    qDebug()<<name;
                }

                this->ergodicTreeForSend(property);
            }
            int pos2 = m_sendBuffer.size();

            while((pos2 - pos1)%4 != 0)
            {
                qint8 t = 0;
                (*m_dataStream)<<t;
                pos2++;
            }
        }
        else
        {
            QList<QtProperty *> list = parentProperty->subProperties();
            foreach(QtProperty* property, list)
            {
                QString name = property->data("name").toString();
                if(name.isEmpty() == false)
                {
                    qDebug()<<name;
                }

                this->ergodicTreeForSend(property);
            }
        }
    }
}
void QTcpTestDialog::addSingleStruct(QString name)
{
    QSingleStruct* ss = QTcpStruct::getInstance()->m_map.value(name, 0);

    if(ss == 0)
    {
        qDebug()<<"addSingleStruct "<<name;
        return ;
    }

    //m_currentList.append(ss);
    QList<QSingleStruct*> list;
    list.append(ss);
    this->createTreePropertyBrowser(&list);
}

void QTcpTestDialog::connected()
{
    ui->output->append("connected.");
}

void QTcpTestDialog::disconnected()
{
    ui->output->append("disConnected");
}

void QTcpTestDialog::error()
{
    ui->output->append("error");
}

void QTcpTestDialog::readyRead()
{
    ui->output->append("receive");
}

void QTcpTestDialog::add()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    if(items.size() > 0)
    {
        QListWidgetItem* item = items.first();
        this->addSingleStruct(item->text());
    }
}

void QTcpTestDialog::clear()
{
    m_browser->clear();
}

void QTcpTestDialog::send()
{
    this->ergodicTreeForSend();
    m_tcpSocket.write(m_sendBuffer);
}

void QTcpTestDialog::load()
{
    QStringList list = QFileDialog::getOpenFileNames(this);
    QTcpStruct::getInstance()->readHeaderFromPath(list);
    ui->listWidget->clear();
    ui->listWidget->addItems(QTcpStruct::getInstance()->m_list);
}

void QTcpTestDialog::tryToConnect()
{
    QString ip = ui->ip->text();
    int port = ui->port->text().toInt();
    m_tcpSocket.connectToHost(ip, port);
    ui->output->append("try To Connect ...");
    QSettings settings("yuguosoft.com");
    settings.setValue("mmDouDiZhu_ip", ip);
    settings.setValue("mmDouDiZhu_port", ui->port->text());
}

