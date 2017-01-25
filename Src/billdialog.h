#ifndef BILLDIALOG_H
#define BILLDIALOG_H

#include <QDialog>
#include "typedef.h"

namespace Ui {
class BillDialog;
}

class BillDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BillDialog(QWidget *parent = 0);
    ~BillDialog();

    void UpdateItemInfo();
    bool BillDialog::AlreadyExist(qint64, unsigned int, unsigned int, QString);

private slots:
    void GetItemInfo();

    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();
signals:
    void UpdateMainItemQuery();
private:
    Ui::BillDialog *ui;
    LPITEM current_item;
    bool is_exist = false;
};

#endif // BILLDIALOG_H
