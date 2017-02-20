#include "serialportwindow.h"
#include "commtooldlg.h"
#include "tcpipwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialPortWindow w;
    TcpIpWindow t;
    CommToolDlg commtooldlg;
    if (commtooldlg.exec()==CommToolDlg::Accepted)
    {
        if(1==commtooldlg.btnIndex)
        {
            w.show();
            return a.exec();
        }
        else
            if(2==commtooldlg.btnIndex)
            {
                t.show();
                return a.exec();
            }
    }
    else
        return 0;
}
