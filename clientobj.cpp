#include "clientobj.h"

ClientObj::ClientObj(QObject *parent) : QObject(parent)
{

}

void ClientObj::init()
{
    clientSocketTemp = new TcpIpClient(this);
    connect(clientSocketTemp,&TcpIpClient::clientConnected,this,&ClientObj::clientConnect);
    connect(clientSocketTemp,&TcpIpClient::clientConnectTimeout,this,&ClientObj::clientConnectTimeout);
    clientIsConnect = false;
}

void ClientObj::saveClientSocket(QString server_ip, int server_port, int client_id, int client_port)
{
    if(false==clientIsConnect)
    {
        clientSocketTemp->bindClient_port(client_port);
        clientSocketTemp->clientID = client_id;
        clientSocketTemp->newConnect(server_ip,server_port);
        clientIsConnect = true;
    }
}

void ClientObj::deleteClientSocket(QString server_ip, int server_port, int client_id)
{

}

void ClientObj::clientConnect()
{
    clientIsConnect = false;
    clientSocketList.append(clientSocketTemp);//将新的客户端连接对象添加到列表
    clientSocketTemp = NULL;
    emit newClientPass();
}

void ClientObj::clientConnectTimeout()
{
    clientIsConnect = false;
    clientSocketTemp = NULL;
    emit newClientFail();
}
