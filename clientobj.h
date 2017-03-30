#ifndef CLIENTOBJ_H
#define CLIENTOBJ_H

#include <QObject>
#include "tcpipclient.h"

class ClientObj : public QObject
{
    Q_OBJECT
public:
    explicit ClientObj(QObject *parent = 0);

signals:
    void newClientPass();
    void newClientFail();

public slots:
    void init();
    void saveClientSocket(QString server_ip, int server_port, int client_id, int client_port);
    void deleteClientSocket(QString server_ip, int server_port, int client_id);
    void clientConnect();
    void clientConnectTimeout();

private:
    TcpIpClient* clientSocketTemp;
    QList<TcpIpClient *> clientSocketList;  //保存客户端对象集合
    bool clientIsConnect;

};

#endif // CLIENTOBJ_H
