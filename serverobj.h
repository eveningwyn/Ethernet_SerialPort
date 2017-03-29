#ifndef SERVEROBJ_H
#define SERVEROBJ_H

#include <QObject>
#include "tcpipserver.h"
#include <QTimer>

class ServerObj : public QObject
{
    Q_OBJECT
public:
    explicit ServerObj(QObject *parent = 0);

private:
    TcpIpServer *server;
    quint16 port_loopSend;
    QString msg_loopSend;
    int time_loopSend;
    QTimer *loopSendTimer;

signals:
    void server_Error_Msg(QString errorMsg);
    void update_Listen_UI(bool status,QString btnText);
    void server_Show_Msg(QString Msg);
    void updateClientConnect(QString IP,int Port);
    void updateClientDisconnected(QString IP,int Port);
    void serverSendMsg(quint16 port,QString msg);

public slots:
    void init();
    void beginListening(QString ip,QString port,QString prefix,QString suffix);
    void update_Server_Prefix_Suffix(QString prefix,QString suffix);
    void serverLoopSendMsg(quint16 port,QString msg,int loopTime);
    void loopSendTimeout();
    void stopLoopSend();

private slots:
};

#endif // SERVEROBJ_H
