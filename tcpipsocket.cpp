#include "tcpipsocket.h"
#include <QMessageBox>
#include <QtCore/QDebug>

TcpIpSocket::TcpIpSocket(QObject *parent) : QObject(parent)
{
    /*Sercer*/
    tcpServer = new QTcpServer(this);
    clientConnection = new QTcpSocket(this);

    /*Client*/
    clientReadString = "";
    readDataBlockSize =(quint16) 0;
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(readyRead()),this,SIGNAL(readyReadData()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayClientError(QAbstractSocket::SocketError)));
}
TcpIpSocket::~TcpIpSocket()
{

}

bool TcpIpSocket::serverListen(QString address,quint16 port)
{   //address (e.g., "127.0.0.1").
    if(!tcpServer->listen(QHostAddress(address),port))
    {
        displayServerError();
        tcpServer->close();
        return false;
    }
    //Connect signal and slot
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(saveConnection()));
    return true;
}

//Get already connected socket
void TcpIpSocket::saveConnection()
{
    clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection,SIGNAL(disconnected()),clientConnection,SLOT(deleteLater()));
}

//Server: socket disconnect
void TcpIpSocket::closeServerConnection()
{
    clientConnection->disconnectFromHost();
}

//Server: send message
void TcpIpSocket::serverSendMessage(QString writeString)
{
    QByteArray writeDataBlock = writeString.toLatin1();

    //Temporarily save the send data
    QByteArray blockTemp;

    //Using the data flow to write data
    QDataStream outWrite(&blockTemp,QIODevice::WriteOnly);

    //Set data flow version, server and client are the same
//    outWrite.setVersion(QDataStream::Qt_4_6);
    int version = outWrite.version();
    outWrite.setVersion(version);
    qDebug()<<"Server_QDataStream_version: "<<version;

//    outWrite<<(quint16) 0;
//    outWrite<<tr("hello Tcp!!!");
//    outWrite.device()->seek(0);
//    outWrite<<(quint16) (blockTemp.size() - sizeof(quint16));

    outWrite<<(quint16) writeDataBlock.length();
    outWrite<<tr(writeDataBlock.data());

    writeDataBlock = blockTemp;
    qDebug()<<"Server_data: "<<writeDataBlock;
    clientConnection->write(writeDataBlock);
}

//Server error message: send signal
void TcpIpSocket::displayServerError()
{
    QString errorString = tcpServer->errorString();
    emit serverErrorMessage(errorString);
}

//Request connection to the server_1
void TcpIpSocket::newConnect_hostName(const QString hostName, quint16 port)
{
    readDataBlockSize =(quint16) 0;
    tcpSocket->abort();//Abort old connection
    tcpSocket->connectToHost(hostName,port);
}
//Request connection to the server_2
void TcpIpSocket::newConnect_address(const QString address, quint16 port)
{
    readDataBlockSize =(quint16) 0;
    tcpSocket->abort();//Abort old connection
    tcpSocket->connectToHost(QHostAddress(address),port);
}

//Client receive server data
void TcpIpSocket::clientReadMessage(QString &readString)
{
    QDataStream inRead(tcpSocket);

    //Set data flow version, server and client are the same
//    inRead.setVersion(QDataStream::Qt_4_6);
    int version = inRead.version();
    inRead.setVersion(version);

    //Start receive data
    if(0 == readDataBlockSize)
    {
       //判断接收的数据是否有两字节，也就是文件的大小信息
       //如果有则保存到readDataBlockSize变量中，没有则返回，继续接收数据
       if(tcpSocket->bytesAvailable() < (int)sizeof(quint16))
           return;
       inRead >> readDataBlockSize;     //save data block size
    }
    //If not all data, return continue to receive
    if(tcpSocket->bytesAvailable() < readDataBlockSize)
        return;
    inRead >> clientReadString;

    readString = clientReadString;
    clientReadString = "";
}

//Client error message: send signal
void TcpIpSocket::displayClientError(QAbstractSocket::SocketError)
{
    QString errorString = tcpSocket->errorString();
    emit clientErrorMessage(errorString);
}
