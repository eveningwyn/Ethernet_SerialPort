#ifndef TCPIPWINDOW_H
#define TCPIPWINDOW_H

#include <QMainWindow>

namespace Ui {
class TcpIpWindow;
}

class TcpIpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpIpWindow(QWidget *parent = 0);
    ~TcpIpWindow();

private:
    Ui::TcpIpWindow *ui;
};

#endif // TCPIPWINDOW_H
