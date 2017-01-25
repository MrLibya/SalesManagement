#ifndef EXPENSES_H
#define EXPENSES_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class ExpensesDialog;
}

class ExpensesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExpensesDialog(QWidget *parent = 0);
    ~ExpensesDialog();

private slots:

    void on_pushButton_clicked();

private:
    Ui::ExpensesDialog *ui;
    QSqlTableModel *model;
};

#endif // EXPENSES_H
