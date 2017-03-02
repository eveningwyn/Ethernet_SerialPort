#include "tcpipwindow.h"
#include "ui_tcpipwindow.h"
#include "language.h"

TcpIpWindow::TcpIpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpIpWindow)
{
    ui->setupUi(this);

    server = new TcpIpServer(this);
    connect(server,SIGNAL(serverReadData(QString,int,QString)),this,SLOT(showServerReadData(QString,int,QString)));

    createWidget();
}

TcpIpWindow::~TcpIpWindow()
{
    delete ui;
}

void TcpIpWindow::createWidget()
{
    mainLayout = new QGridLayout(this);
//IP、端口号设置
    ip_port_GroupBox = new QGroupBox(tr("配置"));
    ip_port_SetGridLayout = new QGridLayout(ip_port_GroupBox);
    ipLabel = new QLabel(tr("目标IP："),ip_port_GroupBox);
    ipLineEdit = new QLineEdit(ip_port_GroupBox);
    portLabel = new QLabel(tr("目标端口："),ip_port_GroupBox);
    portLineEdit = new QLineEdit(ip_port_GroupBox);
    portSetCheckBoxBtn = new QCheckBox(tr("指定本机端口："),ip_port_GroupBox);
    portSetLineEdit = new QLineEdit(ip_port_GroupBox);
    clientConnectBtn = new QPushButton(tr("连接"),ip_port_GroupBox);
    //    connect()
    ip_port_SetGridLayout->addWidget(ipLabel,0,0);
    ip_port_SetGridLayout->addWidget(ipLineEdit,1,0);
    ip_port_SetGridLayout->addWidget(portLabel,2,0);
    ip_port_SetGridLayout->addWidget(portLineEdit,3,0);
    ip_port_SetGridLayout->addWidget(portSetCheckBoxBtn,4,0);
    ip_port_SetGridLayout->addWidget(portSetLineEdit,5,0);
    ip_port_SetGridLayout->addWidget(clientConnectBtn,6,0);

    ip_port_GroupBox->setLayout(ip_port_SetGridLayout);
    ip_port_GroupBox->setFixedSize(120,200);

//发送设置
    sendSetGroupBox = new QGroupBox(tr("发送区配置"));
    sendSetGridLayout = new QGridLayout(sendSetGroupBox);
    //待设计

//接收设置
    receiveSetGroupBox = new QGroupBox(tr("接收区配置"));
    receiveSetGridLayout = new QGridLayout(receiveSetGroupBox);
    //待设计

//发送区域
    SendGroupBox = new QGroupBox(tr("发送区"));
    SendgridLayout = new QGridLayout(SendGroupBox);
    sendTextEdit = new QTextEdit(SendGroupBox);
    sendBtn = new QPushButton(tr("发送"),SendGroupBox);
    //    connect()
    SendgridLayout->addWidget(sendTextEdit,0,0,2,1);
    SendgridLayout->addWidget(sendBtn,2,0,1,1);

    SendGroupBox->setLayout(SendgridLayout);

//接收区域
    ReceiveGroupBox = new QGroupBox(tr("接收区"));
    ReceivegridLayout = new QGridLayout(ReceiveGroupBox);
    receiveShow = new QTextBrowser(ReceiveGroupBox);
    ReceivegridLayout->addWidget(receiveShow);

    ReceiveGroupBox->setLayout(ReceivegridLayout);

//主布局
    mainLayout->addWidget(ip_port_GroupBox,0,0,1,1);
    mainLayout->addWidget(receiveSetGroupBox,2,0,1,1);
    mainLayout->addWidget(sendSetGroupBox,3,0,1,1);
    mainLayout->addWidget(ReceiveGroupBox,0,1,5,1);
    mainLayout->addWidget(SendGroupBox,5,1,2,1);
//    ui->mainCentralwidget->setLayout(mainLayout);
    ui->tabServer->setLayout(mainLayout);
}

void TcpIpWindow::showServerReadData(QString IP,int Port,QString readMsg)
{
}

void TcpIpWindow::showClientReadData(int clientID,QString IP,int Port,QString msg)
{
}

void TcpIpWindow::clientDisconnect(int clientID, QString IP, int Port)
{
}
