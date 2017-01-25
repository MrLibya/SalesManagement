#ifndef CONSUMERITEMDIALOG_H
#define CONSUMERITEMDIALOG_H

#include <QDialog>

namespace Ui {
class ConsumerItemDialog;
}

class ConsumerItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConsumerItemDialog(QWidget *parent = 0);
    ~ConsumerItemDialog();
private slots:
    void on_Cart_itemSelectionChanged();

    void on_delete_item_clicked();
private:
    Ui::ConsumerItemDialog *ui;
};

#endif // CONSUMERITEMDIALOG_H
