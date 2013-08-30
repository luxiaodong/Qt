#ifndef QTCPTESTDIALOG_H
#define QTCPTESTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QByteArray>
#include <QDataStream>
#include <QTcpSocket>
#include <QFileDialog>
#include "qtcpstruct.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include "qttreepropertybrowser.h"
#include "qtvariantproperty.h"

namespace Ui {
class QTcpTestDialog;
}

class QTcpTestDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QTcpTestDialog(QWidget *parent = 0);
    ~QTcpTestDialog();

public slots:
    void load();
    void add();
    void clear();
    void send();
    void tryToConnect();
    void connected();
    void disconnected();
    void error();
    void readyRead();

public:
    void addSingleStruct(QString);

private:
    void createTreePropertyBrowser();
    void createTreePropertyBrowser(QList<QSingleStruct*> *list);
    void createTreePropertyBrowser(QSingleStruct* single, QtVariantProperty* parentProperty = 0);
    void bindData(QtVariantProperty* property, QSingleStruct* singleStruct, int arrayLength = 1);
    void bindData(QtVariantProperty* property, QSingleVariable* singleVariable);

    void ergodicTreeToTest();
    void ergodicTreeToTest(QtProperty* parentProperty);

    void ergodicTreeForSend();
    void ergodicTreeForSend(QtProperty* parentProperty);

public:
    QtVariantPropertyManager *m_variantManager;
    QtTreePropertyBrowser*  m_browser;
    QList<QSingleStruct*> m_currentList;

    QTcpSocket m_tcpSocket;
    QByteArray m_sendBuffer;
    QDataStream* m_dataStream;
    
private:
    Ui::QTcpTestDialog *ui;
};

#endif // QTCPTESTDIALOG_H
