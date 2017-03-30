#ifndef TCPIPSERVERSHOWDIALOG_H
#define TCPIPSERVERSHOWDIALOG_H

#include <QDialog>

namespace Ui {
class TcpIpServerShowDialog;
}

class TcpIpServerShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TcpIpServerShowDialog(QWidget *parent = 0);
    ~TcpIpServerShowDialog();

public slots:
    void show_Msg(QString Msg);

private slots:
    void on_pushButtonServerClearReceive_clicked();

    void on_pushButtonServerSaveToFile_clicked();

    void on_checkBoxServerHexShow_Receive_clicked();

    void on_checkBoxServerSaveToFile_clicked();

private:
    Ui::TcpIpServerShowDialog *ui;
    QString server_fileName;
    void server_saveData(QString msg);
};

#endif // TCPIPSERVERSHOWDIALOG_H
