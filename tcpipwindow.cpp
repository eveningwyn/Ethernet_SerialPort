#include "tcpipwindow.h"
#include "ui_tcpipwindow.h"

TcpIpWindow::TcpIpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpIpWindow)
{
    ui->setupUi(this);
}

TcpIpWindow::~TcpIpWindow()
{
    delete ui;
}
