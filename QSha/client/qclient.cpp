#include "qclient.h"

QClient::QClient(QObject* parent)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
}

QString QClient::strings(int type)
{
    return tr(client_strings[type]);
}

QString QClient::localHostUserName()
{
    QString userName;
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
              << "HOSTNAME.*" << "DOMAINNAME.*";

    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
            {
                userName = stringList.at(1).toUtf8();
                break;
            }
        }
    }

    if(userName.isEmpty())
    {
        userName = QHostInfo::localHostName();
    }

    return userName;
}

void QClient::connectToServer(QString serverIp)
{
    m_socket->connectToHost(serverIp,17951);
}

void QClient::connected()
{
    QList<QVariant> data;
    data.append(QVariant(localHostUserName()));
    this->sendProtocol(c2s_request_login,data);
    emit this->showMessage(strings(1));
}

void QClient::sendMessage(QString msg)
{
    if( msg.isEmpty() )
    {
        return ;
    }

    QList<QVariant> data;
    data.append(QVariant(msg));
    this->sendProtocol(c2s_request_talking,data);
}

void QClient::disconnected()
{
    emit this->showMessage(strings(2));
}

void QClient::readyRead()
{
    if ((m_socket == 0) ||
        (m_socket->state() != QAbstractSocket::ConnectedState))
    {
        return;
    }

    QDataStream stream(m_socket);
    int protocol;
    QList<QVariant> data;
    stream>>protocol;
    stream>>data;
    this->receiveProtocol(protocol,data);
}

void QClient::error(QAbstractSocket::SocketError socketError)
{
    emit this->showMessage(strings(0));
}

void QClient::receiveProtocol(int protocol, QList<QVariant> &data)
{
    if(protocol <= s2c_start ||
       protocol >= s2c_end)
    {
        return;
    }

    switch(protocol)
    {
    case s2c_send_welcome:
        {
            QString name = data.first().toString();
            emit displayMessage(tr("%1 is connected!").arg(name));
        }
        break;

    case s2c_send_talking:
        {
            QString name = data.at(0).toString();
            QString msg = data.at(1).toString();
            emit displayMessage(QString("%1: %2").arg(name).arg(msg));
        }
        break;

    default:
        break;
    }
}

void QClient::sendProtocol(int protocol, QList<QVariant> &data)
{
    if(protocol <= c2s_start ||
       protocol >= c2s_end)
    {
        return;
    }

    QDataStream out(m_socket);
    out<<protocol<<data;
}
