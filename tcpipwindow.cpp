#include "tcpipwindow.h"
#include "ui_tcpipwindow.h"
#include "language.h"
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>

TcpIpWindow::TcpIpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpIpWindow)
{
    ui->setupUi(this);

    ui->lineEditListenIP->setText("192.168.1.108");
    ui->lineEditListenPort->setText("4096");
     ui->lineEditServerIntervalTime->setText("1000");

    serverIsLoopSend = false;

    serverObj = new ServerObj;
    thread1 = new QThread;
    serverObj->moveToThread(thread1);

    connect(thread1,&QThread::started,serverObj,&ServerObj::init);

    connect(serverObj,&ServerObj::server_Error_Msg,this,&TcpIpWindow::showErrorMsg);
    connect(serverObj,&ServerObj::update_Listen_UI,this,&TcpIpWindow::set_Listen_UI);
    connect(serverObj,&ServerObj::server_Show_Msg,this,&TcpIpWindow::show_Msg);
    connect(serverObj,&ServerObj::updateClientConnect,this,&TcpIpWindow::setClientConnect);
    connect(serverObj,&ServerObj::updateClientDisconnected,this,&TcpIpWindow::setClientDisconnected);

    connect(this,&TcpIpWindow::beginListen,serverObj,&ServerObj::beginListening);
    connect(this,&TcpIpWindow::set_Server_Prefix_Suffix,serverObj,&ServerObj::update_Server_Prefix_Suffix);
    connect(this,&TcpIpWindow::serverSend,serverObj,&ServerObj::serverSendMsg);
    connect(this,&TcpIpWindow::serverLoopSend,serverObj,&ServerObj::serverLoopSendMsg);
    connect(this,&TcpIpWindow::serverStopLoopSend,serverObj,&ServerObj::stopLoopSend);

    thread1->start();
}

TcpIpWindow::~TcpIpWindow()
{
    delete ui;
}

void TcpIpWindow::on_pushButtonListen_clicked()
{
    QString ip = ui->lineEditListenIP->text();
    QString port = ui->lineEditListenPort->text();
    QString prefix = ui->comboBoxServerPrefix->currentText();
    QString suffix = ui->comboBoxServerSuffix->currentText();
    suffix.replace("\\r","\r");
    suffix.replace("\\n","\n");

    emit beginListen(ip, port, prefix, suffix);
}

void TcpIpWindow::showErrorMsg(QString errorMsg)
{
    QMessageBox::warning(this,tr("Error"),QString("%1\n").arg(errorMsg),QMessageBox::Ok);
}

void TcpIpWindow::set_Listen_UI(bool status, QString btnText)
{
    ui->pushButtonListen->setText(btnText);
    ui->lineEditListenIP->setDisabled(status);
    ui->lineEditListenPort->setDisabled(status);
}

void TcpIpWindow::show_Msg(QString Msg)
{
    if(ui->checkBoxServerNextLineShow->isChecked())
    {
        Msg += "\n";
    }
    ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
    ui->textBrowserServerReceiveArea->insertPlainText(Msg);
    ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
}

void TcpIpWindow::setClientConnect(QString IP, int Port)
{
    ui->comboBoxServerClientIP->addItem(IP);
    ui->comboBoxServerClientPort->addItem(QString("%1").arg(Port));
    QString time = QDateTime::currentDateTime().toString("yyyyMMdd_hh:mm:ss.zzz");
    QString strTemp =QString("%1\n%2 %3 Connected\n").arg(time).arg(IP).arg(Port);
    show_Msg(strTemp);
}

void TcpIpWindow::setClientDisconnected(QString IP, int Port)
{
    int count = ui->comboBoxServerClientPort->count();
    QString ipTemp;
    QString portTemp;
    for(int i=0;i<count;i++)
    {
        ipTemp = ui->comboBoxServerClientIP->itemText(i);
        portTemp = ui->comboBoxServerClientPort->itemText(i);
        if(IP==ipTemp && QString("%1").arg(Port)==portTemp)
        {
            ui->comboBoxServerClientPort->removeItem(i);
            ui->comboBoxServerClientIP->removeItem(i);
        }
    }
    QString time = QDateTime::currentDateTime().toString("yyyyMMdd_hh:mm:ss.zzz");
    QString strTemp =QString("%1\n%2 %3 Disconnected\n").arg(time).arg(IP).arg(Port);
    show_Msg(strTemp);
}

void TcpIpWindow::on_comboBoxServerClientIP_currentIndexChanged(int index)
{
    ui->comboBoxServerClientPort->setCurrentIndex(index);
}

void TcpIpWindow::on_comboBoxServerClientPort_currentIndexChanged(int index)
{
    ui->comboBoxServerClientIP->setCurrentIndex(index);
}

void TcpIpWindow::on_comboBoxServerPrefix_currentTextChanged(const QString &arg1)
{
    emit set_Server_Prefix_Suffix(arg1,ui->comboBoxServerSuffix->currentText());
}

void TcpIpWindow::on_comboBoxServerSuffix_currentTextChanged(const QString &arg1)
{
    emit set_Server_Prefix_Suffix(ui->comboBoxServerPrefix->currentText(),arg1);
}

void TcpIpWindow::on_pushButtonServerSend_clicked()
{
    if(""==ui->comboBoxServerClientPort->currentText())
    {
        QMessageBox::warning(this,tr("提示"),tr("客户端不存在!\n"),QMessageBox::Ok);
        return;
    }
    QString sendStr = ui->textEditServerSendInput->toPlainText();
    if(""==sendStr)
    {
        return;
    }
    qint16 port =ui->comboBoxServerClientPort->currentText().toInt();
    if(!ui->checkBoxServerLoopSend->isChecked())
    {
        emit serverSend(port,sendStr);
    }
    else
    {
        if(false==serverIsLoopSend)
        {
            ui->checkBoxServerAutoClear->setChecked(false);
            int loopTime = ui->lineEditServerIntervalTime->text().toInt();
            emit serverLoopSend(port,sendStr,loopTime);
            serverIsLoopSend = true;
            ui->pushButtonServerSend->setText(tr("停止"));
            ui->checkBoxServerAutoClear->setDisabled(true);
            ui->checkBoxServerHexShow_Send->setDisabled(true);
            ui->checkBoxServerLoopSend->setDisabled(true);
            ui->lineEditServerIntervalTime->setDisabled(true);
            ui->pushButtonServerClear->setDisabled(true);
            ui->textEditServerSendInput->setDisabled(true);
            ui->comboBoxServerClientIP->setDisabled(true);
            ui->comboBoxServerClientPort->setDisabled(true);
            ui->comboBoxServerPrefix->setDisabled(true);
            ui->comboBoxServerSuffix->setDisabled(true);
            ui->pushButtonListen->setDisabled(true);
        }
        else
        {
            emit serverStopLoopSend();
            serverIsLoopSend = false;
            ui->checkBoxServerLoopSend->setChecked(false);
            ui->pushButtonServerSend->setText(tr("发送"));
            ui->checkBoxServerAutoClear->setDisabled(false);
            ui->checkBoxServerHexShow_Send->setDisabled(false);
            ui->checkBoxServerLoopSend->setDisabled(false);
            ui->lineEditServerIntervalTime->setDisabled(false);
            ui->pushButtonServerClear->setDisabled(false);
            ui->textEditServerSendInput->setDisabled(false);
            ui->comboBoxServerClientIP->setDisabled(false);
            ui->comboBoxServerClientPort->setDisabled(false);
            ui->comboBoxServerPrefix->setDisabled(false);
            ui->comboBoxServerSuffix->setDisabled(false);
            ui->pushButtonListen->setDisabled(false);
        }
    }
    if(ui->checkBoxServerAutoClear->isChecked())
    {
        ui->textEditServerSendInput->clear();
    }

}

void TcpIpWindow::on_checkBoxServerHexShow_Send_clicked()
{
    //待完善
}

void TcpIpWindow::on_pushButtonServerClear_clicked()
{
    ui->textEditServerSendInput->clear();
}

void TcpIpWindow::on_pushButtonServerClearReceive_clicked()
{
    ui->textBrowserServerReceiveArea->clear();
}

void TcpIpWindow::on_pushButtonServerSaveToFile_clicked()
{
    server_fileName = QFileDialog::getSaveFileName(this,tr("另存为"),"fileName.txt");
    if(!server_fileName.isEmpty())
    {
        server_saveData(ui->textBrowserServerReceiveArea->toPlainText());
        return;
    }
}

void TcpIpWindow::server_saveData(QString msg)
{
    QFile file(server_fileName);
    file.open(QFile::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);    // 鼠标指针变为等待状态
    out << msg;
    QApplication::restoreOverrideCursor();              // 鼠标指针恢复原来的状态
    file.close();
}

void TcpIpWindow::on_checkBoxServerSaveToFile_clicked()
{
    if(!(ui->checkBoxServerSaveToFile->isChecked()))
    {
        ui->pushButtonServerSaveToFile->setEnabled(true);
        ui->pushButtonServerClearReceive->setEnabled(true);
    }
    else
    {
        server_fileName = QFileDialog::getSaveFileName(this,tr("另存为"),"fileName.txt");
        if(server_fileName.isEmpty())
        {
            ui->checkBoxServerSaveToFile->setChecked(false);
            return;
        }
        ui->pushButtonServerSaveToFile->setEnabled(false);
        ui->pushButtonServerClearReceive->setEnabled(false);
    }
}

void TcpIpWindow::on_checkBoxServerHexShow_Receive_clicked()
{
    //待完善
}
