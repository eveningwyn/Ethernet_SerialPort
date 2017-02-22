#ifndef TCPIPSOCKET_H
#define TCPIPSOCKET_H

#include <QObject>
#include <QtNetwork>

class TcpIpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpIpSocket(QObject *parent = 0);
    ~TcpIpSocket();

    /*Server*/
    bool serverListen(QString address,quint16 port);          //Server: Listen connection
    void closeServerConnection();                             //Server: Close connection
    void serverSendMessage(QString writeString);              //Server: Send Message

    /*Client*/
    QString clientReadString;                                                   //Client: Receive string from server
    quint16 readDataBlockSize;                                                  //Client: Data block size
    void newConnect_hostName(const QString hostName, quint16 port);             //Client: Request connection to the server
    void newConnect_address(const QString address, quint16 port);               //Client: Request connection to the server
    void clientReadMessage(QString &readString);                                //Client: Read Message

private:
    /*Server*/
    QTcpServer *tcpServer;              //Server
    QTcpSocket *clientConnection;       //Client connection
    void displayServerError();

    /*Client*/
    QTcpSocket *tcpSocket;              //Client


signals:
    /*Server*/
    void serverErrorMessage(QString errorString);

    /*Client*/
    void readyReadData();               //Read data ready signal
    void clientErrorMessage(QString errorString);

public slots:

private slots:
    /*Server*/
    void saveConnection();              //Save client connection

    /*Client*/
    void displayClientError(QAbstractSocket::SocketError);

};

#endif // TCPIPSOCKET_H
