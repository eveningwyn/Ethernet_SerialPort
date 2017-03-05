#ifndef TCPIPWINDOW_H
#define TCPIPWINDOW_H

#include <QMainWindow>
#include "tcpipserver.h"
#include "tcpip_thread.h"

namespace Ui {
class TcpIpWindow;
}

class TcpIpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpIpWindow(QWidget *parent = 0);
    ~TcpIpWindow();
    Tcpip_Thread *tcpip_thread;

private slots:
    void showServerReadData(QString IP,int Port,QString readMsg);

    void showClientReadData(int clientID,QString IP,int Port,QString msg);

    void clientDisconnect(int clientID,QString IP,int Port);

    void on_pushButtonListen_clicked();

    void showErrorMsg(QString errorMsg);

    void updateClientConnect(QString IP,int Port);

    void updateClientDisconnected(QString IP,int Port);

    void on_comboBoxServerClientIP_currentIndexChanged(int index);

    void on_comboBoxServerClientPort_currentIndexChanged(int index);

    void on_comboBoxServerPrefix_currentTextChanged(const QString &arg1);

    void on_comboBoxServerSuffix_currentTextChanged(const QString &arg1);

private:
    Ui::TcpIpWindow *ui;
    TcpIpServer *server;//服务器对象
    TcpIpClient *client;//客户端对象
    int serverReadlength;//服务器读取数据长度
    int serverWritelength;//服务器写入数据长度
    int clientReadlength;//客户端读取数据长度
    int clientWritelength;//客户端写入数据长度
};

#endif // TCPIPWINDOW_H
