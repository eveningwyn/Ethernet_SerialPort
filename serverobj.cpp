#include "serverobj.h"
#include "language.h"
#include <QDateTime>
#include <QThread>

ServerObj::ServerObj(QObject *parent) : QObject(parent)
{

}

void ServerObj::init()
{
    server = new TcpIpServer(this);
    loopSendTimer = new QTimer(this);
    connect(server,&TcpIpServer::clientConnect,this,&ServerObj::updateClientConnect);
    connect(server,&TcpIpServer::clientDisconnected,this,&ServerObj::updateClientDisconnected);
    connect(server,&TcpIpServer::errorMessage,this,&ServerObj::server_Error_Msg);
    connect(server,&TcpIpServer::serverShowMsg,this,&ServerObj::server_Show_Msg);

    connect(loopSendTimer,&QTimer::timeout,this,&ServerObj::loopSendTimeout);

    connect(this,&ServerObj::serverSendMsg,server,&TcpIpServer::sendData);
}

void ServerObj::beginListening(QString ip, QString port, QString prefix, QString suffix)
{
    bool status;
    QString btnText = "";
    QString time = QDateTime::currentDateTime().toString("yyyyMMdd_hh:mm:ss.zzz");
    QString strTemp = QString("%1\n%2 %3 ").arg(time).arg(ip).arg(port);

    if(!server->isListening())
    {
        if(!server->stratListen(ip,(quint16)port.toInt()))
        {
            emit server_Error_Msg(tr("The server create failure!\n"));
            return;
        }
        server->prefix = prefix;
        server->suffix = suffix;
        status = true;
        btnText = tr("停止服务器");
        strTemp += tr("Listening\n");
    }
    else
    {
        server->closeServerListen();
        status = false;
        btnText = tr("启动服务器");
        strTemp += tr("Stop listen\n");
    }

    emit update_Listen_UI(status,btnText);
    emit server_Show_Msg(strTemp);
}

void ServerObj::update_Server_Prefix_Suffix(QString prefix, QString suffix)
{
    server->set_prefix_suffix(prefix,suffix);
}

void ServerObj::serverLoopSendMsg(quint16 port, QString msg, int loopTime)
{
    port_loopSend = port;
    msg_loopSend = msg;
    time_loopSend = loopTime;
    if(!loopSendTimer->isActive())
        loopSendTimer->start(time_loopSend);
    server->sendData(port_loopSend,msg_loopSend);
}

void ServerObj::loopSendTimeout()
{
    server->sendData(port_loopSend,msg_loopSend);
}

void ServerObj::stopLoopSend()
{
    if(loopSendTimer->isActive())
        loopSendTimer->stop();
}
