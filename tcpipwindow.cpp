#include "tcpipwindow.h"
#include "ui_tcpipwindow.h"

TcpIpWindow::TcpIpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpIpWindow)
{
    ui->setupUi(this);
//    ui->connectModeDockWidget->setGeometry(0,33,80,395);
////    ui->connectModeDockWidgetContents->setGeometry(0,0,10,424);
//    ui->showFrame->setGeometry(3,0,466,395);
}

TcpIpWindow::~TcpIpWindow()
{
    delete ui;
}
