#ifndef TCPIPWINDOW_H
#define TCPIPWINDOW_H

#include <QMainWindow>
#include "tcpipserver.h"
#include "serverobj.h"
#include <QThread>

namespace Ui {
class TcpIpWindow;
}

class TcpIpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpIpWindow(QWidget *parent = 0);
    ~TcpIpWindow();

private slots:

    void on_pushButtonListen_clicked();//监听
    void showErrorMsg(QString errorMsg);//错误信息
    void set_Listen_UI(bool status,QString btnText);
    void show_Msg(QString Msg);



    void setClientConnect(QString IP,int Port);

    void setClientDisconnected(QString IP,int Port);

    void on_comboBoxServerClientIP_currentIndexChanged(int index);

    void on_comboBoxServerClientPort_currentIndexChanged(int index);

    void on_comboBoxServerPrefix_currentTextChanged(const QString &arg1);

    void on_comboBoxServerSuffix_currentTextChanged(const QString &arg1);

    void on_pushButtonServerSend_clicked();

private:
    Ui::TcpIpWindow *ui;
    ServerObj *serverObj;
    QThread *thread1;


    TcpIpClient *client;//客户端对象
    int serverReadlength;//服务器读取数据长度
    int serverWritelength;//服务器写入数据长度


signals:
    void beginListen(QString ip,QString port,QString prefix,QString suffix);//服务器开始监听
    void set_Server_Prefix_Suffix(QString prefix,QString suffix);
    void serverSend(quint16 port,QString msg);
};

#endif // TCPIPWINDOW_H
