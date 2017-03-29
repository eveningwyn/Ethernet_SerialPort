#include "serialportwindow.h"
#include "ui_serialportwindow.h"
#include <QSerialPortInfo>
#include "language.h"
#include <QDateTime>
#include <QByteArray>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include "str_hex.h"
//#include <QDebug>

SerialPortWindow::SerialPortWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SerialPortWindow)
{
    ui->setupUi(this);
    serialPort = new SerialPortObj(this);
    serialPortIsOpen = false;
    repeatSendFlag = false;
    getPortName();
    showStatusBar();

    timer = new QTimer(this);
    //关联定时器溢出信号和相应的槽函数
    connect(timer, SIGNAL(timeout()), this, SLOT(repeatSend()));
    connect(serialPort,SIGNAL(serialReadReady()),this,SLOT(ShowDataReceiveArea()));
}

SerialPortWindow::~SerialPortWindow()
{
    serialPort->closeSerialPort();
    delete ui;
}
void SerialPortWindow::getPortName()
{
    QList<QSerialPortInfo> portInfoList = QSerialPortInfo::availablePorts();
    if(portInfoList.isEmpty())
    {
        ui->portNameComboBox->addItem("None");
        return;
    }
    foreach (QSerialPortInfo info, portInfoList)
    {
        ui->portNameComboBox->addItem(info.portName());
    }
    ui->portNameComboBox->addItem("None");
}

void SerialPortWindow::on_portNameRefreshBtn_clicked()
{
    ui->portNameComboBox->clear();
    getPortName();
}
bool SerialPortWindow::initSerialPort()
{
    bool ok;
    QString portName = ui->portNameComboBox->currentText();             //端口号
    int baudRate =ui->baudRateComboBox->currentText().toInt(&ok,10);    //波特率
    int dataBit = ui->dataBitComboBox->currentText().toInt(&ok,10);     //数据位
    QString parityBit = ui->parityBitComboBox->currentText();           //校验位
    QString stopBit = ui->stopBitComboBox->currentText();               //停止位
    bool setDTR = ui->DTRcheckBox->isChecked();                         //DTR
    bool setRTS = ui->RTScheckBox->isChecked();                         //RTS

    /*参数：端口号、波特率、数据位、校验位、停止位、DTR和RTS；打开串口，并配置参数*/
    if(serialPort->openSerialPort(portName,baudRate,dataBit,
                                  parityBit,stopBit,setDTR,setRTS))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SerialPortWindow::on_openSerialPortBtn_clicked()
{   //如果串口没有打开，则打开串口；否则关闭串口
    if(false == serialPortIsOpen)
    {
        if(initSerialPort())
        {
            //禁用界面串口设置
            ui->openSerialPortBtn->setText(tr("关闭串口"));
            const QIcon icon(":/res/aladdin_lamp_green.png");
            ui->openSerialPortBtn->setIcon(icon);
            ui->portNameComboBox->setEnabled(false);
            ui->portNameRefreshBtn->setEnabled(false);
            ui->baudRateComboBox->setEnabled(false);
            ui->dataBitComboBox->setEnabled(false);
            ui->parityBitComboBox->setEnabled(false);
            ui->stopBitComboBox->setEnabled(false);
            //设置串口打开标志true
            serialPortIsOpen = true;
        }
        else
        {
            return;
        }
    }
    else
    {
        //启用界面串口设置
        ui->openSerialPortBtn->setText(tr("打开串口"));
        QIcon icon(":/res/Aladdin_lamp_red.png");
        ui->openSerialPortBtn->setIcon(icon);
        ui->portNameComboBox->setEnabled(true);
        ui->portNameRefreshBtn->setEnabled(true);
        ui->baudRateComboBox->setEnabled(true);
        ui->dataBitComboBox->setEnabled(true);
        ui->parityBitComboBox->setEnabled(true);
        ui->stopBitComboBox->setEnabled(true);
        //设置串口打开标志false,关闭串口
        serialPortIsOpen = false;
        serialPort->closeSerialPort();
    }
}
void SerialPortWindow::ShowDataReceiveArea()
{
    QString readString;
    QString prefix;
    QString suffix;
    get_prefix_suffix(prefix,suffix);

    serialPort->serialPortRead(readString,prefix,suffix); //向串口接收数据
    if(readString.isEmpty())
    {
        return;
    }
    receiveDataLength += readString.length();
    //更新状态栏
    statusBarLabel1->setText(tr("发送：%1").arg(sendDataLength));
    statusBarLabel2->setText(tr("接收：%1").arg(receiveDataLength));

    if(ui->hexShowCheckBox->isChecked())    //十六进制显示
    {
        Hex_To_String(readString);
    }
    if(ui->showReceiveTimeCheckBox->isChecked())    //显示时间
    {
        QDateTime time = QDateTime::currentDateTime();
        readString = time.toString("【yyyy-MM-dd hh:mm:ss.zzz】") + readString;
    }
    if(ui->nextLineShowCheckBox->isChecked())   //换行显示
    {
        readString += "\r\n";
    }
    if(ui->stopReceiveCheckBox->isChecked())    //停止显示
    {
        return;
    }

    ui->dataReceiveAreaTextBrowser->moveCursor(QTextCursor::End);
    ui->dataReceiveAreaTextBrowser->insertPlainText("【Receiver】" + readString);
    ui->dataReceiveAreaTextBrowser->moveCursor(QTextCursor::End);
}

void SerialPortWindow::on_showReceiveTimeCheckBox_clicked()
{
    if(ui->showReceiveTimeCheckBox->isChecked())
    {
        ui->nextLineShowCheckBox->setChecked(true);
        ui->nextLineShowCheckBox->setEnabled(false);
    }
    else
    {
        ui->nextLineShowCheckBox->setEnabled(true);
    }
}

void SerialPortWindow::on_clearReceiveBtn_clicked()
{
    ui->dataReceiveAreaTextBrowser->clear();
}

void SerialPortWindow::on_saveDataBtn_clicked()
{
    fileName = QFileDialog::getSaveFileName(this,tr("另存为"),"fileName.txt");
    if(!fileName.isEmpty())
    {
        saveData();
        return;
    }
}

void SerialPortWindow::on_dataSaveToFileCheckBox_clicked()
{
    if(!(ui->dataSaveToFileCheckBox->isChecked()))
    {
        ui->saveDataBtn->setEnabled(true);
        ui->clearReceiveBtn->setEnabled(true);
    }
    else
    {
        fileName = QFileDialog::getSaveFileName(this,tr("另存为"),"fileName.txt");
        if(fileName.isEmpty())
        {
            ui->dataSaveToFileCheckBox->setChecked(false);
            return;
        }
        ui->saveDataBtn->setEnabled(false);
        ui->clearReceiveBtn->setEnabled(false);
    }
}

void SerialPortWindow::on_sendBtn_clicked()
{
    if(true == serialPortIsOpen)
    {
        QString writeString = ui->dataSendAreaTextEdit->toPlainText();
        if(!writeString.isEmpty())
        {
            if(!(ui->repeatSendCheckBox->isChecked()))      //判断是单次发送还是连续发送
            {
                sendData();
            }
            else
            {
                if(false == repeatSendFlag)     //判断是否已经开始循环发送
                {
                    repeatSendFlag = true;
                    ui->sendGroupBox->setEnabled(false);
                    ui->sendBtn->setText(tr("停止"));
                    ui->dataSendAreaTextEdit->setEnabled(false);
                    bool ok;
                    int time = ui->intervalTimeLineEdit->text().toInt(&ok,10);
                    timer->start(time);
                }
                else
                {
                    repeatSendFlag = false;
                    ui->sendGroupBox->setEnabled(true);
                    ui->sendBtn->setText(tr("发送"));
                    ui->dataSendAreaTextEdit->setEnabled(true);
                    timer->stop();
                }
            }
        }
    }
    else
    QMessageBox::warning(this,tr("SerialPort"),tr("串口未打开！"),QMessageBox::Ok);
}

void SerialPortWindow::on_clearSendAreaBtn_clicked()
{
    ui->dataSendAreaTextEdit->clear();
}

void SerialPortWindow::on_hexSendAreaCheckBox_clicked()
{
    QString writeString = ui->dataSendAreaTextEdit->toPlainText();

    if(ui->hexSendAreaCheckBox->isChecked())
    {
        Hex_To_String(writeString);
        ui->dataSendAreaTextEdit->installEventFilter(this);//加入TextEdit控件的事件过滤器
    }
    else
    {
        String_To_Hex(writeString);
        ui->dataSendAreaTextEdit->removeEventFilter(this);//移除TextEdit控件的事件过滤器
    }
    ui->dataSendAreaTextEdit->clear();
    ui->dataSendAreaTextEdit->setText(writeString);
}

void SerialPortWindow::sendData()
{
    QString writeString = ui->dataSendAreaTextEdit->toPlainText();
    if(!writeString.isEmpty())
    {
        if(ui->hexSendAreaCheckBox->isChecked())
        {
            String_To_Hex(writeString);
        }
        QString prefix;
        QString suffix;
        get_prefix_suffix(prefix,suffix);
        if("" != prefix)
        {//如果前缀不为空，则添加前缀，注意添加顺序
            writeString = prefix + writeString;
        }
        if("" != suffix)
        {//如果后缀不为空，则添加后缀，注意添加顺序
            writeString = writeString + suffix;
        }

        serialPort->serialPortWrite(writeString);       //向串口发送数据

        sendDataLength += writeString.length();
        //更新状态栏
        statusBarLabel1->setText(tr("发送：%1").arg(sendDataLength));
        statusBarLabel2->setText(tr("接收：%1").arg(receiveDataLength));

        if(ui->clearSendAreaCheckBox->isChecked())
        {   //发送完清空输入
            ui->dataSendAreaTextEdit->clear();
        }
        QDateTime time = QDateTime::currentDateTime();
        QString strTime = time.toString("【yyyy-MM-dd hh:mm:ss.zzz】");
        ui->dataReceiveAreaTextBrowser->moveCursor(QTextCursor::End);
        ui->dataReceiveAreaTextBrowser->insertPlainText("【Sender】" + strTime + writeString + "\n");
        ui->dataReceiveAreaTextBrowser->moveCursor(QTextCursor::End);
    }
}

void SerialPortWindow::on_repeatSendCheckBox_clicked()
{
    if(ui->repeatSendCheckBox->isChecked())
    {
        ui->clearSendAreaCheckBox->setChecked(false);
        ui->clearSendAreaCheckBox->setEnabled(false);
    }
    else
    {
        ui->clearSendAreaCheckBox->setEnabled(true);
    }
}

void SerialPortWindow::saveData()
{
    QFile file(fileName);
    file.open(QFile::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);    // 鼠标指针变为等待状态
    out << ui->dataReceiveAreaTextBrowser->toPlainText();
    QApplication::restoreOverrideCursor();              // 鼠标指针恢复原来的状态
    file.close();
}
void SerialPortWindow::on_dataReceiveAreaTextBrowser_textChanged()
{
    if(ui->dataSaveToFileCheckBox->isChecked())
    {
        saveData();
    }
}
void SerialPortWindow::repeatSend()
{
    sendData();
}

void SerialPortWindow::Hex_To_String(QString &str)
{
    if(!str.isEmpty())
    {
        QByteArray byte = str.toLatin1();
        char *buf = byte.data();            //转换前的数据
        int index = 0;                      //转换的初始位
        char *ptr = new char;               //转换后的数据
        int len = str.length();             //数据转换前的长度
        //执行转换：例如"123"->"31 32 33 "
        hex_to_str(ptr,index,buf,len);

        str = QString(ptr);
    }
}

void SerialPortWindow::String_To_Hex(QString &str)
{
    QByteArray byte = str.toLatin1();
    char *ch = byte.data();             //转换前的数据
    char *cbuf = new char;              //转换后的数据
    int len = str.length();             //数据转换前的字符串长度
    //执行转换：例如"31 32 33 "->"123"
    str_to_hex(ch,cbuf,len);

    str = QString(cbuf);
}

void SerialPortWindow::on_loadFileBtn_clicked()
{
    QString fileNameTemp = QFileDialog::getOpenFileName(this);
    if (!fileNameTemp.isEmpty())
    {
        QFile file(fileNameTemp); // 新建QFile对象
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("Load File"),
                                  tr("无法读取文件 %1:\n%2.")
                                  .arg(fileNameTemp).arg(file.errorString()));
            return;
        }
        QTextStream in(&file); // 新建文本流对象
        QApplication::setOverrideCursor(Qt::WaitCursor);    //鼠标光标设置等待
        // 读取文件的全部文本内容，并添加到编辑器中
        ui->dataSendAreaTextEdit->setPlainText(in.readAll());
        QApplication::restoreOverrideCursor();              //恢复鼠标光标
    }
}

void SerialPortWindow::showStatusBar()
{
    sendDataLength = 0;
    receiveDataLength = 0;
    statusBarLabel1 = new QLabel(this);
    statusBarLabel2 = new QLabel(this);
    statusBarBtn = new QPushButton(tr("复位计数"),this);

    statusBarLabel1->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
    statusBarLabel1->setTextFormat(Qt::RichText);
    ui->statusBar->addPermanentWidget(statusBarLabel1);
    //更新状态栏
    statusBarLabel1->setText(tr("发送：%1").arg(sendDataLength));

    statusBarLabel2->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
    statusBarLabel2->setTextFormat(Qt::RichText);
    ui->statusBar->addPermanentWidget(statusBarLabel2);
    //更新状态栏
    statusBarLabel2->setText(tr("接收：%1").arg(receiveDataLength));

    ui->statusBar->addPermanentWidget(statusBarBtn);
    connect(statusBarBtn,SIGNAL(clicked()),this,SLOT(statusBarBtnClicked()));
}
void SerialPortWindow::statusBarBtnClicked()
{
    sendDataLength = 0;
    receiveDataLength = 0;
    //更新状态栏
    statusBarLabel1->setText(tr("发送：%1").arg(sendDataLength));
    statusBarLabel2->setText(tr("接收：%1").arg(receiveDataLength));
}

/*重载事件过滤器*/
bool SerialPortWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->dataSendAreaTextEdit)
    {
        if (event->type() == QEvent::KeyPress)
        {
            if(ui->hexSendAreaCheckBox->isChecked())
            {
                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
                if(keyEvent->key()>=Qt::Key_0 && keyEvent->key()<=Qt::Key_9)
                    return false;
                else if(keyEvent->key()>=Qt::Key_A && keyEvent->key()<=Qt::Key_F)
                    return false;
                else if(keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Backspace
                        || keyEvent->key()==Qt::Key_CapsLock || keyEvent->key()==Qt::Key_Space
                        || keyEvent->key()==Qt::Key_Up || keyEvent->key()==Qt::Key_Down
                        || keyEvent->key()==Qt::Key_Left || keyEvent->key()==Qt::Key_Right)
                    return false;
                else
                    return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, event);
    }
}

void SerialPortWindow::on_DTRcheckBox_clicked()
{
    bool setDTR = ui->DTRcheckBox->isChecked();                         //DTR
    bool setRTS = ui->RTScheckBox->isChecked();                         //RTS
    serialPort->setDTR_RTS(setDTR,setRTS);
}

void SerialPortWindow::on_RTScheckBox_clicked()
{
    bool setDTR = ui->DTRcheckBox->isChecked();                         //DTR
    bool setRTS = ui->RTScheckBox->isChecked();                         //RTS
    serialPort->setDTR_RTS(setDTR,setRTS);
}

void SerialPortWindow::get_prefix_suffix(QString &prefix,QString &suffix)
{
    prefix = ui->dataPrefixComboBox->currentText();
    suffix = ui->dataSuffixComboBox->currentText();
    prefix = prefix.toLower();
    suffix = suffix.toLower();
    if("none" != prefix)
    {
        prefix.replace("\\r","\r");
        prefix.replace("\\n","\n");
    }
    else
        prefix ="";

    if("none" != suffix)
    {
        suffix.replace("\\r","\r");
        suffix.replace("\\n","\n");
    }
    else
        suffix ="";
}
