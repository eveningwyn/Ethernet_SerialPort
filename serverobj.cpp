#include "serverobj.h"
#include "language.h"
#include <QDateTime>
#include <QThread>
#include <QDebug>

ServerObj::ServerObj(QObject *parent) : QObject(parent)
{

}

void ServerObj::init()
{
    server = new TcpIpServer(this);
    connect(server,&TcpIpServer::clientConnect,this,&ServerObj::updateClientConnect);
    connect(server,&TcpIpServer::clientDisconnected,this,&ServerObj::updateClientDisconnected);
}

void ServerObj::beginListening(QString ip, QString port, QString prefix, QString suffix)
{
    bool status;
    QString btnText = "";
    QDateTime time = QDateTime::currentDateTime();
    QString strTemp = time.toString("yyyy-MM-dd hh:mm:ss.zzz") + "\n" + ip + " " + port + " ";

    if(!server->isListening())
    {
        if(!server->stratListen(ip,(quint16)port.toInt()))
        {
            emit server_Error_Msg(tr("Listen fail!\n"));
            return;
        }
        server->prefix = prefix;
        server->suffix = suffix;
        status = true;
        btnText = tr("Listenning...");
        strTemp += "Listenning...\n";
    }
    else
    {
        server->closeServerListen();
        status = false;
        btnText = tr("Open");
        strTemp += "Stop listen.\n";
    }

    emit update_Listen_UI(status,btnText);
    emit server_Show_Msg(strTemp);
}

void ServerObj::update_Server_Prefix_Suffix(QString prefix, QString suffix)
{
    server->set_prefix_suffix(prefix,suffix);
}
