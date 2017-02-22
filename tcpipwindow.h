#ifndef TCPIPWINDOW_H
#define TCPIPWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

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
    QTreeWidgetItem *clientMode;
    QTreeWidgetItem *serverMode;
};

#endif // TCPIPWINDOW_H
