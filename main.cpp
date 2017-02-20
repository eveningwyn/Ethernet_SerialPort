#include "serialportwindow.h"
#include "commtooldlg.h"
#include "tcpipwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialPortWindow spw;
    TcpIpWindow tiw;
    CommToolDlg commtooldlg;
    if (commtooldlg.exec() == CommToolDlg::Accepted)
    {
        if(1 == commtooldlg.btnIndex)
        {
            spw.show();
            return a.exec();
        }
        else
            if(2 == commtooldlg.btnIndex)
            {
                tiw.show();
                return a.exec();
            }
    }
    else
        return 0;
}
