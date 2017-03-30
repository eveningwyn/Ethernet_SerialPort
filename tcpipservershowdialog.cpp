#include "tcpipservershowdialog.h"
#include "ui_tcpipservershowdialog.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

TcpIpServerShowDialog::TcpIpServerShowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpIpServerShowDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    this->setParent(parent);
}

TcpIpServerShowDialog::~TcpIpServerShowDialog()
{
    delete ui;
}

void TcpIpServerShowDialog::on_pushButtonServerClearReceive_clicked()
{
    ui->textBrowserServerReceiveArea->clear();
}

void TcpIpServerShowDialog::on_pushButtonServerSaveToFile_clicked()
{
    server_fileName = QFileDialog::getSaveFileName(this,tr("另存为"),"fileName.txt");
    if(!server_fileName.isEmpty())
    {
        server_saveData(ui->textBrowserServerReceiveArea->toPlainText());
        return;
    }
}

void TcpIpServerShowDialog::show_Msg(QString Msg)
{
    if(ui->checkBoxServerNextLineShow->isChecked())
    {
        Msg += "\n";
    }
    if(ui->checkBoxServerSaveToFile->isChecked())
    {
        server_saveData(Msg);
    }
    ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
    ui->textBrowserServerReceiveArea->insertPlainText(Msg);
    ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
}

void TcpIpServerShowDialog::on_checkBoxServerHexShow_Receive_clicked()
{
    //待完善
}

void TcpIpServerShowDialog::on_checkBoxServerSaveToFile_clicked()
{
    if(!(ui->checkBoxServerSaveToFile->isChecked()))
    {
        server_fileName = "";
        ui->pushButtonServerClearReceive->setEnabled(true);
        ui->pushButtonServerSaveToFile->setEnabled(true);
    }
    else
    {
        server_fileName = QFileDialog::getSaveFileName(this,tr("另存为"),"fileName.txt");
        if(server_fileName.isEmpty())
        {
            ui->checkBoxServerSaveToFile->setChecked(false);
            return;
        }
        ui->pushButtonServerClearReceive->setEnabled(false);
        ui->pushButtonServerSaveToFile->setEnabled(false);
    }
}

void TcpIpServerShowDialog::server_saveData(QString msg)
{
    QFile file(server_fileName);
    file.open(QFile::WriteOnly | QIODevice::Text | QIODevice::Append);
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);    // 鼠标指针变为等待状态
    out << msg;
    QApplication::restoreOverrideCursor();              // 鼠标指针恢复原来的状态
    file.close();
}
