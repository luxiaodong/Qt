#ifndef QCLINET_H
#define QCLINET_H

#include <QObject>
#include <QVariant>
#include <QTcpSocket>
#include <QHostInfo>
#include <QProcess>
#include <QDataStream>
#include "deskwidget.h"
#include "../protocol.h"
#include "string.h"

class QClient : public QObject
{
    Q_OBJECT
public:
    QClient(QObject* parent = 0);
public:
    QString localHostUserName();
    void connectToServer(QString serverIp);
    void sendMessage(QString msg);

private:
    QString strings(int type);
    void receiveProtocol(int protocol, QList<QVariant> &data);
    void sendProtocol(int protocol, QList<QVariant> &data);

signals:
    void displayMessage(QString);
    void showMessage(QString);

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void error(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket* m_socket;
    DeskWidget* m_desk;
};

#endif // QCLINET_H
