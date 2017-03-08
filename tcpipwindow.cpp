#include "tcpipwindow.h"
#include "ui_tcpipwindow.h"
#include "language.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

TcpIpWindow::TcpIpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpIpWindow)
{
    ui->setupUi(this);

    ui->lineEditListenIP->setText("192.168.1.108");
    ui->lineEditListenPort->setText("4096");

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
    ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
    ui->textBrowserServerReceiveArea->insertPlainText(Msg);
    ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
}

void TcpIpWindow::setClientConnect(QString IP, int Port)
{
    ui->comboBoxServerClientIP->addItem(IP);
    ui->comboBoxServerClientPort->addItem(QString("%1").arg(Port));
    QDateTime time = QDateTime::currentDateTime();
    QString strTemp = time.toString("yyyy-MM-dd hh:mm:ss.zzz")+"\n"+IP+QString(" %1").arg(Port)+" Connected\n";
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
    QDateTime time = QDateTime::currentDateTime();
    QString strTemp = time.toString("yyyy-MM-dd hh:mm:ss.zzz")+"\n"+IP+QString(" %1").arg(Port)+" Disconnected\n";
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
