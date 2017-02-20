#include "commtooldlg.h"
#include "ui_commtooldlg.h"

CommToolDlg::CommToolDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommToolDlg)
{
    ui->setupUi(this);
    btnIndex = 0;
}

CommToolDlg::~CommToolDlg()
{
    delete ui;
}

void CommToolDlg::on_serialPortBtn_clicked()
{
    btnIndex = 1;   //表示选择串口调试工具
    accept();
}

void CommToolDlg::on_tcpIpBtn_clicked()
{
    btnIndex = 2;   //表示选择TCP/IP调试工具
    accept();
}
