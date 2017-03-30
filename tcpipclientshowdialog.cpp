#include "tcpipclientshowdialog.h"
#include "ui_tcpipclientshowdialog.h"

TcpIpClientShowDialog::TcpIpClientShowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpIpClientShowDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    this->setParent(parent);
}

TcpIpClientShowDialog::~TcpIpClientShowDialog()
{
    delete ui;
}
