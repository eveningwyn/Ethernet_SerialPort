#ifndef TCPIP_THREAD_H
#define TCPIP_THREAD_H

#include <QObject>
#include <QThread>

class Tcpip_Thread : public QThread
{
    Q_OBJECT

public:
    explicit Tcpip_Thread(QObject *parent = 0);
    void stop();

private:
    volatile bool stopped;

protected:
    void run();
};

#endif // TCPIP_THREAD_H
