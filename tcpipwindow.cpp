#include "tcpipwindow.h"
#include "ui_tcpipwindow.h"
#include "language.h"

TcpIpWindow::TcpIpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpIpWindow)
{
    ui->setupUi(this);
    ui->modeTreeWidget->setColumnCount(1);  //设置列数
    ui->modeTreeWidget->setHeaderLabel(tr("")); //设置头的标题
    clientMode = new QTreeWidgetItem(ui->modeTreeWidget,QStringList(QString("客户端模式")));
    serverMode = new QTreeWidgetItem(ui->modeTreeWidget,QStringList(QString("服务器模式")));
    ui->modeTreeWidget->expandAll(); //结点全部展开

    server = new TcpIpServer(this);
    connect(server,SIGNAL(serverReadData(QString,int,QString)),this,SLOT(showServerReadData(QString,int,QString)));
}

TcpIpWindow::~TcpIpWindow()
{
    delete ui;
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
