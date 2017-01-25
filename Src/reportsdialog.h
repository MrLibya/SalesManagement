#ifndef REPORTSDIALOG_H
#define REPORTSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class ReportsDialog;
}

class ReportsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportsDialog(QWidget *parent = 0);
    ~ReportsDialog();

private slots:
    void on_pushButton_clicked();

    void on_cash_sell_type_clicked();

    void on_later_sell_type_clicked();

    void on_refund_cash_sell_type_clicked();

    void on_refund_later_sell_type_clicked();

    void on_consumer_items_clicked();

    void on_retention_items_clicked();

    void on_client_later_sell_type_clicked();

    void on_client_debt_clicked();

    void on_client_retention_clicked();

    void on_client_name_currentIndexChanged(int index);

    void on_pushButton_3_clicked();

private:
    Ui::ReportsDialog *ui;
    QStringList *HeadsTableView;
    QSqlQueryModel *CartItems;
};

#endif // REPORTSDIALOG_H
