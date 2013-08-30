#include "qserver.h"

//QServer
QServer::QServer(QObject *parent)
    : QTcpServer(parent),m_game(0)
{
    m_sockets.clear();
    m_names.clear();
    //≤ª‘⁄’‚±ﬂ
    //m_game = new QGame(this);
}

bool QServer::canBeService()
{
    m_hostIp.clear();
    QHostInfo localInfo = QHostInfo::fromName(QHostInfo::localHostName());
    QList<QHostAddress> hostAddresses = localInfo.addresses();
    if (hostAddresses.isEmpty() == false)
    {
        m_hostIp = hostAddresses.first().toString();
    }

    if(m_hostIp.isEmpty())
    {
        return false;
    }

    return true;
}

void QServer::startupService()
{
    if(m_hostIp.isEmpty() == false)
    {
        listen(QHostAddress(m_hostIp),17951);
        return ;
    }

    listen(QHostAddress::LocalHost,17951);
}

void QServer::startGame()
{
    m_game = new QGame(this);
}

void QServer::incomingConnection(int socketDescriptor)
{
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    emit newConnection(socket);
    m_sockets.append(socket);
    m_names.append(QString());
}

int QServer::indexOf(QTcpSocket* socket)
{
    return m_sockets.indexOf(socket);
}

void QServer::connectionError(QAbstractSocket::SocketError socketError)
{
    qDebug("QAbstractSocket::SocketError %d",(int)socketError);
}

void QServer::disconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if ((socket == 0) ||
        (socket->state() != QAbstractSocket::ConnectedState))
    {
        return;
    }

    int index = this->indexOf(socket);
    if(index != -1)
    {
        delete socket;
        socket = 0;
        m_names.replace(index,QString());
    }
}

void QServer::readyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if ((socket == 0) ||
        (socket->state() != QAbstractSocket::ConnectedState))
    {
        return;
    }

    int index = this->indexOf(socket);
    if(index == -1)
    {
        return ;
    }

    QDataStream stream(socket);

    int protocol;
    QList<QVariant> data;
    stream>>protocol;
    stream>>data;

    this->receiveProtocol(index, protocol,data);
}

void QServer::receiveProtocol(int index, int protocol, QList<QVariant> &data)
{
    if(protocol <= c2s_start ||
       protocol >= c2s_end)
    {
        return;
    }

    switch(protocol)
    {
    case c2s_request_login:
        {
            QString name = data.first().toString();
            m_names.replace(index,name);
            data.append(QVariant(name));
            this->sendProtocol(s2c_send_welcome,data);
        }
        break;
    case c2s_request_talking:
        {
            QString name = m_names.at(index);
            data.prepend(QVariant(name));
            this->sendProtocol(s2c_send_talking,data);
        }
        break;
    default:
        break;
    }
}

void QServer::sendProtocol(int protocol, QList<QVariant> &data)
{
    if(protocol <= s2c_start ||
       protocol >= s2c_end)
    {
        return;
    }

    for(int i = 0; i < m_sockets.size(); ++i)
    {
        QTcpSocket* socket = m_sockets.at(i);
        if ((socket != 0) &&
            (socket->state() == QAbstractSocket::ConnectedState) )
        {
            QDataStream stream(socket);
            stream<<protocol<<data;
        }
    }
}
