#include "tcpip_thread.h"

Tcpip_Thread::Tcpip_Thread(QObject *parent) :
    QThread(parent)
{    
    stopped = false;
}

void Tcpip_Thread::run()
{
    while (!stopped)
    {
        msleep(1);
    }
//    stopped = false;
}

void Tcpip_Thread::stop()
{
    stopped = true;
}
