#ifndef SERIALPORTWINDOW_H
#define SERIALPORTWINDOW_H

#include <QMainWindow>
#include "serialportobj.h"
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>

namespace Ui {
class SerialPortWindow;
}

class SerialPortWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SerialPortWindow(QWidget *parent = 0);
    ~SerialPortWindow();

    bool initSerialPort();
    void sendData();
    void saveData();
    //执行转换：例如"123"->"31 32 33 "
    void Hex_To_String(QString &str);
    //执行转换：例如"31 32 33 "->"123"
    void String_To_Hex(QString &str);

protected:
     bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void ShowDataReceiveArea();

    void on_portNameRefreshBtn_clicked();

    void on_openSerialPortBtn_clicked();

    void on_showReceiveTimeCheckBox_clicked();

    void on_clearReceiveBtn_clicked();

    void on_saveDataBtn_clicked();

    void on_dataSaveToFileCheckBox_clicked();

    void on_sendBtn_clicked();

    void on_clearSendAreaBtn_clicked();

    void on_hexSendAreaCheckBox_clicked();

    void on_repeatSendCheckBox_clicked();

    void on_dataReceiveAreaTextBrowser_textChanged();

    void repeatSend();

    void on_loadFileBtn_clicked();

    void statusBarBtnClicked();

    void on_DTRcheckBox_clicked();

    void on_RTScheckBox_clicked();

private:
    Ui::SerialPortWindow *ui;

    SerialPortObj *serialPort;
    bool serialPortIsOpen;
    void getPortName();
    bool repeatSendFlag;
    QString fileName;
    QTimer *timer;
    void showStatusBar();
    int receiveDataLength;
    int sendDataLength;
    QLabel *statusBarLabel1;
    QLabel *statusBarLabel2;
    QPushButton *statusBarBtn;
    void get_prefix_suffix(QString &prefix,QString &suffix);
};

#endif // SERIALPORTWINDOW_H
