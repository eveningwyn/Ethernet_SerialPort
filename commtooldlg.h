#ifndef COMMTOOLDLG_H
#define COMMTOOLDLG_H

#include <QDialog>

namespace Ui {
class CommToolDlg;
}

class CommToolDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CommToolDlg(QWidget *parent = 0);
    ~CommToolDlg();
    int btnIndex;   //用于记录用户工具的选择

private slots:
    void on_serialPortBtn_clicked();

    void on_tcpIpBtn_clicked();

private:
    Ui::CommToolDlg *ui;
};

#endif // COMMTOOLDLG_H
