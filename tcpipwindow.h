#ifndef TCPIPWINDOW_H
#define TCPIPWINDOW_H

#include <QMainWindow>
#include "tcpipserver.h"
#include <QTreeWidgetItem>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QTextBrowser>
#include <QTextEdit>

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
    void showServerReadData(QString IP,int Port,QString readMsg);

    void showClientReadData(int clientID,QString IP,int Port,QString msg);

    void clientDisconnect(int clientID,QString IP,int Port);

private:
    Ui::TcpIpWindow *ui;
    QTreeWidgetItem *clientMode;
    QTreeWidgetItem *serverMode;
    TcpIpServer *server;//服务器对象
    TcpIpClient *client;//客户端对象
    int serverReadlength;//服务器读取数据长度
    int serverWritelength;//服务器写入数据长度
    int clientReadlength;//客户端读取数据长度
    int clientWritelength;//客户端写入数据长度

    /*****界面布局对象*****/
    void createWidget();
    QGridLayout *mainLayout;
//IP、端口号设置
    QGroupBox *ip_port_GroupBox;
    QGridLayout *ip_port_SetGridLayout;
    QLabel *ipLabel;
    QLineEdit *ipLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QCheckBox *portSetCheckBoxBtn;
    QLineEdit *portSetLineEdit;
    QPushButton *clientConnectBtn;
//发送设置
    QGroupBox *receiveSetGroupBox;
    QGridLayout *receiveSetGridLayout;

//接收设置
    QGroupBox *sendSetGroupBox;
    QGridLayout *sendSetGridLayout;

//发送区域
    QGroupBox *SendGroupBox;
    QGridLayout *SendgridLayout;
    QTextEdit *sendTextEdit;
    QPushButton *sendBtn;
//接收区域
    QGroupBox *ReceiveGroupBox;
    QGridLayout *ReceivegridLayout;
    QTextBrowser *receiveShow;
};

#endif // TCPIPWINDOW_H
