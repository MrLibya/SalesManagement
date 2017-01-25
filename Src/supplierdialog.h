#ifndef SUPPLIERDIALOG_H
#define SUPPLIERDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QCompleter>
#include <QSqlQueryModel>

namespace Ui {
class SupplierDialog;
}

class SupplierDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SupplierDialog(QWidget *parent = 0);
    ~SupplierDialog();
private slots:
    void GetClientInfo();
    void on_cart_id_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_amount_payment_clicked();

    void on_bill_payment_clicked();
private:
    Ui::SupplierDialog *ui;
    QSqlQueryModel *CartItems;
    unsigned int supplierid;
    QStringList *HeadsTableView;
};

#endif // SUPPLIERDIALOG_H
