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
}

TcpIpWindow::~TcpIpWindow()
{
    delete ui;
}
