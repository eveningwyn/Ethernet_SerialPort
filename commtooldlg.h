#ifndef COMMTOOLDLG_H
#define COMMTOOLDLG_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class CommToolDlg;
}

class CommToolDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CommToolDlg(QWidget *parent = 0);
    ~CommToolDlg();
    int btnIndex;

private slots:
    void on_serialPortBtn_clicked();

    void on_tcpIpBtn_clicked();

private:
    Ui::CommToolDlg *ui;
};

#endif // COMMTOOLDLG_H
