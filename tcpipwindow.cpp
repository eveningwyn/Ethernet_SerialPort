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
