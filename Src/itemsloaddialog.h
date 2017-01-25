#ifndef ITEMSLOADDIALOG_H
#define ITEMSLOADDIALOG_H

#include <QDialog>

namespace Ui {
class ItemsLoadDialog;
}

class ItemsLoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemsLoadDialog(QWidget *parent = 0);
    ~ItemsLoadDialog();
    void SetItems(QString, QString, QString);
signals:
    void SendBill(QString, QString, QString);

private slots:
    void on_pushButton_clicked();

    void on_tableWidget_itemSelectionChanged();

private:
    Ui::ItemsLoadDialog *ui;
};

#endif // ITEMSLOADDIALOG_H
