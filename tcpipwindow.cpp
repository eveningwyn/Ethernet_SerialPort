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

    server = new TcpIpServer(this);
    tcpip_thread = new Tcpip_Thread(this);


    connect(server,SIGNAL(serverReadData(QString,int,QString)),this,SLOT(showServerReadData(QString,int,QString)));
    connect(server,SIGNAL(clientConnect(QString,int)),this,SLOT(updateClientConnect(QString,int)));
    connect(server,SIGNAL(clientDisconnected(QString,int)),this,SLOT(updateClientDisconnected(QString,int)));

    tcpip_thread->start();
}

TcpIpWindow::~TcpIpWindow()
{
    if(tcpip_thread->isRunning())
        tcpip_thread->stop();
    QThread::msleep(500);
    delete tcpip_thread;
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

void TcpIpWindow::on_pushButtonListen_clicked()
{
    if(!server->isListening())
    {
        if(!server->stratListen(ui->lineEditListenIP->text(),(quint16)ui->lineEditListenPort->text().toInt()))
        {
            QMessageBox::warning(this,tr("Error"),tr("聆听失败！\n"),QMessageBox::Ok);
            return;
        }

        QString prefix = ui->comboBoxServerPrefix->currentText();
        QString suffix = ui->comboBoxServerSuffix->currentText();
        suffix.replace("\\r","\r");
        suffix.replace("\\n","\n");
        server->prefix = prefix;
        server->suffix = suffix;

        ui->pushButtonListen->setText(tr("倾听中..."));
        ui->lineEditListenIP->setDisabled(true);
        ui->lineEditListenPort->setDisabled(true);
        QDateTime time = QDateTime::currentDateTime();
        QString strTemp = time.toString("yyyy-MM-dd hh:mm:ss.zzz") + "\n"
                + ui->lineEditListenIP->text() + " "
                + ui->lineEditListenPort->text() + " Listen...\n";
        ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
        ui->textBrowserServerReceiveArea->insertPlainText(strTemp);
        ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
    }
    else
    {
        server->closeServerListen();
        ui->pushButtonListen->setText(tr("开始倾听"));
        ui->lineEditListenIP->setDisabled(false);
        ui->lineEditListenPort->setDisabled(false);
        QDateTime time = QDateTime::currentDateTime();
        QString strTemp = time.toString("yyyy-MM-dd hh:mm:ss.zzz") + "\n"
                + ui->lineEditListenIP->text() + " "
                + ui->lineEditListenPort->text() + " Stop listen.\n";
        ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
        ui->textBrowserServerReceiveArea->insertPlainText(strTemp);
        ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
    }
}

void TcpIpWindow::showErrorMsg(QString errorMsg)
{
    QMessageBox::warning(this,tr("Error"),QString("%1\n").arg(errorMsg),QMessageBox::Ok);
}

void TcpIpWindow::updateClientConnect(QString IP, int Port)
{
    ui->comboBoxServerClientIP->addItem(IP);
    ui->comboBoxServerClientPort->addItem(QString("%1").arg(Port));
    QDateTime time = QDateTime::currentDateTime();
    QString strTemp = time.toString("yyyy-MM-dd hh:mm:ss.zzz")+"\n"+IP+QString(" %1").arg(Port)+" Connected.\n";
    ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
    ui->textBrowserServerReceiveArea->insertPlainText(strTemp);
    ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
}

void TcpIpWindow::updateClientDisconnected(QString IP, int Port)
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
    QString strTemp = time.toString("yyyy-MM-dd hh:mm:ss.zzz")+"\n"+IP+QString(" %1").arg(Port)+" Disconnected.\n";
    ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
    ui->textBrowserServerReceiveArea->insertPlainText(strTemp);
    ui->textBrowserServerReceiveArea->moveCursor(QTextCursor::End);
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
    QString strTemp = arg1;

    server->prefix = strTemp;
    int count = ui->comboBoxServerClientPort->count();
    QString ipTemp;
    QString portTemp;
    for(int i=0;i<count;i++)
    {
        ipTemp = ui->comboBoxServerClientIP->itemText(i);
        portTemp = ui->comboBoxServerClientPort->itemText(i);
        TcpIpClient *client = server->getSocket(ipTemp,(quint16)portTemp.toInt());
        if(NULL==client)
        {
            QMessageBox::warning(this,tr("Error"),tr("设置数据规则时，客户端不存在！\n"),QMessageBox::Ok);
            return;
        }
        client->prefix = strTemp;
    }
}

void TcpIpWindow::on_comboBoxServerSuffix_currentTextChanged(const QString &arg1)
{
    QString strTemp = arg1;
    strTemp.replace("\\r","\r");
    strTemp.replace("\\n","\n");
    server->suffix = strTemp;
    int count = ui->comboBoxServerClientPort->count();
    QString ipTemp;
    QString portTemp;
    for(int i=0;i<count;i++)
    {
        ipTemp = ui->comboBoxServerClientIP->itemText(i);
        portTemp = ui->comboBoxServerClientPort->itemText(i);
        TcpIpClient *client = server->getSocket(ipTemp,(quint16)portTemp.toInt());
        if(NULL==client)
        {
            QMessageBox::warning(this,tr("Error"),tr("设置数据规则时，客户端不存在！\n"),QMessageBox::Ok);
            return;
        }
        client->suffix = strTemp;
    }
}
