#ifndef TCPIPCLIENTSHOWDIALOG_H
#define TCPIPCLIENTSHOWDIALOG_H

#include <QDialog>

namespace Ui {
class TcpIpClientShowDialog;
}

class TcpIpClientShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TcpIpClientShowDialog(QWidget *parent = 0);
    ~TcpIpClientShowDialog();

private:
    Ui::TcpIpClientShowDialog *ui;
};

#endif // TCPIPCLIENTSHOWDIALOG_H
