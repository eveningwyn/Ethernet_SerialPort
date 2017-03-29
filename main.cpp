#include "serialportwindow.h"
#include "commtooldlg.h"
#include "tcpipwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CommToolDlg commtooldlg;
    if (commtooldlg.exec() == CommToolDlg::Accepted)
    {
        if(1 == commtooldlg.btnIndex)
        {
            SerialPortWindow serialPort_window;
            serialPort_window.show();
            return a.exec();
        }
        else
        {
            if(2 == commtooldlg.btnIndex)
            {
                TcpIpWindow tcpIp_window;
                tcpIp_window.show();
                return a.exec();
            }
        }
    }
    else
        return 0;
}
