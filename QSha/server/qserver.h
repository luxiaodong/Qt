#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <QStringList>
#include <QDataStream>
#include <QVariant>

#include "../protocol.h"
#include "qgame.h"

class QGame;
class QServer : public QTcpServer
{   
    Q_OBJECT
public:
    QServer(QObject *parent = 0);

public:
    bool canBeService();
    void startupService();
    void startGame();

protected:
    void incomingConnection(int socketDescriptor);

private:
    void receiveProtocol(int index, int protocol, QList<QVariant> &data);
    void sendProtocol(int protocol, QList<QVariant> &data);
    void boradMessage(const QString message);
    int  indexOf(QTcpSocket* socket);

signals:
    void newConnection(QTcpSocket *connection);

private slots:
    void connectionError(QAbstractSocket::SocketError socketError);
    void disconnected();
    void readyRead();
public:
    QString m_hostIp;
    QList<QTcpSocket*> m_sockets;
    QStringList m_names;
    QGame* m_game;
};

#endif // SERVER_H
