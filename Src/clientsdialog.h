#ifndef CLIENTSDIALOG_H
#define CLIENTSDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QCompleter>
#include <QSqlQueryModel>

namespace Ui {
class ClientsDialog;
}

class ClientsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientsDialog(QWidget *parent = 0,bool check = false);
    ~ClientsDialog();
private slots:
    void on_cart_id_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_amount_payment_clicked();

    void on_bill_payment_clicked();

    void on_name_currentIndexChanged(const QString &arg1);

private:
    Ui::ClientsDialog *ui;
    QSqlQueryModel *CartItems;
    unsigned int clientid;
    bool reservation;
};

#endif // CLIENTSDIALOG_H
